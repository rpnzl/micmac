/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr

   
    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in 
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte 
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/


  // Test de Greg

#include "StdAfx.h"

/*
void XXXXX(FILE * aF)
{
  int x;
  int TOTO;
  TOTO = fscanf(aF,"%d",&x);
}
*/

class cParamMap2DRobustInit
{
      public :
         cParamMap2DRobustInit(eTypeMap2D aType,int aNbTirRans);

         eTypeMap2D mType;
         int        mNbTirRans;
         int        mNbMaxPtsRansac;
         int        mNbTestFor1P;
         double     mPropRan;
         int        mNbIterL2;
         cElMap2D*  mRes;
};

void  Map2DRobustInit(const ElPackHomologue & aPackFull,cParamMap2DRobustInit & aParam);

//=====================================================================================


ElAffin2D::ElAffin2D
(
     Pt2dr im00,  // partie affine
     Pt2dr im10,  // partie vecto
     Pt2dr im01
) :
    mI00 (im00),
    mI10 (im10),
    mI01 (im01)
{
}


ElAffin2D::ElAffin2D() :
    mI00 (0,0),
    mI10 (1,0),
    mI01 (0,1)
{
}

bool ElAffin2D::IsId() const
{
   return 
           (mI00==Pt2dr(0,0))
        && (mI10==Pt2dr(1,0))
        && (mI01==Pt2dr(0,1)) ;
}

ElAffin2D ElAffin2D::Id()
{
   return ElAffin2D();
}

ElAffin2D ElAffin2D::trans(Pt2dr aTr)
{
   return ElAffin2D(aTr,Pt2dr(1,0),Pt2dr(0,1));
}





ElAffin2D::ElAffin2D (const ElSimilitude & aSim) :
    mI00 (aSim(Pt2dr(0,0))),
    mI10 (aSim(Pt2dr(1,0)) -mI00),
    mI01 (aSim(Pt2dr(0,1)) -mI00)
{
}

ElAffin2D ElAffin2D::operator * (const ElAffin2D & sim2) const 
{
    return ElAffin2D
           (
              (*this)(sim2(Pt2dr(0,0))),
              IVect(sim2.IVect(Pt2dr(1,0))),
              IVect(sim2.IVect(Pt2dr(0,1)))
           );

}
ElAffin2D ElAffin2D::operator + (const ElAffin2D & sim2) const 
{
    return ElAffin2D
           (
               mI00 + sim2.mI00,
               mI10 + sim2.mI10,
               mI01 + sim2.mI01
           );

}

ElAffin2D ElAffin2D::CorrectWithMatch(Pt2dr aPt,Pt2dr aRes) const
{
    Pt2dr aGot = (*this) (aPt);

    return ElAffin2D
           (
               mI00 + aRes-aGot,
               mI10,
               mI01
           );
}


ElAffin2D ElAffin2D::inv () const
{
    REAL delta = mI10 ^ mI01;

    Pt2dr  Inv10 = Pt2dr(mI01.y,-mI10.y) /delta;
    Pt2dr  Inv01 = Pt2dr(-mI01.x,mI10.x) /delta;

    return  ElAffin2D
            (
                 -(Inv10*mI00.x+Inv01*mI00.y),
                 Inv10,
                 Inv01
            );
}

ElAffin2D ElAffin2D::TransfoImCropAndSousEch(Pt2dr aTr,Pt2dr aResol,Pt2dr * aSzInOut)
{
   ElAffin2D aRes
             (
                   -Pt2dr(aTr.x/aResol.x,aTr.y/aResol.y),
                   Pt2dr(1.0/aResol.x,0.0),
                   Pt2dr(0.0,1.0/aResol.y)
             );

   if (aSzInOut)
   {
      Box2dr aBoxIn(aTr, aTr+*aSzInOut);
      Box2dr aBoxOut  = aBoxIn.BoxImage(aRes);

      *aSzInOut = aBoxOut.sz();
       aRes = trans(-aBoxOut._p0) * aRes;
   }

   return aRes;
}

ElAffin2D  ElAffin2D::TransfoImCropAndSousEch(Pt2dr aTr,double aResol,Pt2dr * aSzInOut)
{
   return TransfoImCropAndSousEch(aTr,Pt2dr(aResol,aResol),aSzInOut);
}


ElAffin2D  ElAffin2D::L2Fit(const  ElPackHomologue & aPack,double *aResidu)
{
   ELISE_ASSERT(aPack.size()>=3,"Less than 3 point in ElAffin2D::L2Fit");

   static L2SysSurResol aSys(6);
   aSys.GSSR_Reset(false);


   //   C0 X1 + C1 Y1 +C2 =  X2     (C0 C1)  (X1)   C2
   //                               (     )  (  ) +
   //   C3 X1 + C4 Y1 +C5 =  Y2     (C3 C4)  (Y1)   C5

  double aCoeffX[6]={1,1,1,0,0,0};
  double aCoeffY[6]={0,0,0,1,1,1};


   for 
   (
        ElPackHomologue::const_iterator it=aPack.begin();
        it!=aPack.end();
        it++
   )
   {
       aCoeffX[0] = it->P1().x;
       aCoeffX[1] = it->P1().y;

       aCoeffY[3] = it->P1().x;
       aCoeffY[4] = it->P1().y;

       aSys.AddEquation(1,aCoeffX, it->P2().x);
       aSys.AddEquation(1,aCoeffY, it->P2().y);
   }

   Im1D_REAL8 aSol = aSys.Solve(0);
   double * aDS = aSol.data();

   Pt2dr aIm00(aDS[2],aDS[5]);
   Pt2dr aIm10(aDS[0],aDS[3]);
   Pt2dr aIm01(aDS[1],aDS[4]);


   ElAffin2D aRes(aIm00,aIm10,aIm01);

   if (aResidu)
   {
      *aResidu = 0;
      for 
      (
           ElPackHomologue::const_iterator it=aPack.begin();
           it!=aPack.end();
           it++
      )
      {
          *aResidu +=  euclid(aRes(it->P1()),it->P2()) ;
      }
      int aNbPt = aPack.size();
      if (aNbPt>3)
          *aResidu /= (aNbPt-3);
   }
   return aRes;
}


ElAffin2D ElAffin2D::FromTri2Tri
          (
               const Pt2dr & a0, const Pt2dr & a1, const Pt2dr & a2,
               const Pt2dr & b0, const Pt2dr & b1, const Pt2dr & b2
          )
{
     ElAffin2D aA(a0,a1-a0,a2-a0);
     ElAffin2D aB(b0,b1-b0,b2-b0);

     return aB * aA.inv();
}

cElHomographie ElAffin2D::ToHomographie() const
{
    cElComposHomographie aHX(mI10.x,mI01.x,mI00.x);
    cElComposHomographie aHY(mI10.y,mI01.y,mI00.y);
    cElComposHomographie aHZ(     0,     0,     1);

    return  cElHomographie(aHX,aHY,aHZ);
}

// -------------------- :: -------------------
cXml_Map2D MapFromElem(const cXml_Map2DElem & aMapE)
{
    cXml_Map2D aRes;
    aRes.Maps().push_back(aMapE);
    return aRes;
}

//--------------------------------------------


cElMap2D *  ElAffin2D::Map2DInverse() const
{
   return  new ElAffin2D(inv());
}

cXml_Map2D ElAffin2D::ToXmlGen()
{
   cXml_Map2DElem anElem;
   anElem.Aff().SetVal(El2Xml(*this));
   return cXml_Map2D(MapFromElem(anElem));
}

int   ElAffin2D::NbUnknown() const
{
    return 6;
}

std::vector<double> ElAffin2D::Params() const
{
   std::vector<double> aRes;

   aRes.push_back(mI10.x);
   aRes.push_back(mI01.x);
   aRes.push_back(mI00.x);
   aRes.push_back(mI10.y);
   aRes.push_back(mI01.y);
   aRes.push_back(mI00.y);

   return aRes;
}

void  ElAffin2D::InitFromParams(const std::vector<double> &aSol)
{
   mI10 = Pt2dr(aSol[0],aSol[3]);
   mI01 = Pt2dr(aSol[1],aSol[4]);
   mI00 = Pt2dr(aSol[2],aSol[5]);
}
//    A B  X  +  C
//    D E  Y     F
//   A C = P10 , CD = P01  EF = P00
void  ElAffin2D::AddEq
      (
           Pt2dr & aCste,
           std::vector<double> & anEqX,
           std::vector<double> & anEqY,
           const Pt2dr & aP1,
           const Pt2dr & aP2 
       ) const
{
    aCste.x  = aP2.x;
    anEqX[0] = aP1.x;
    anEqX[1] = aP1.y;
    anEqX[2] = 1;
    anEqX[3] = 0;
    anEqX[4] = 0;
    anEqX[5] = 0;


    aCste.y  = aP2.y;
    anEqY[0] = 0;
    anEqY[1] = 0;
    anEqY[2] = 0;
    anEqY[3] = aP1.x;
    anEqY[4] = aP1.y;
    anEqY[5] = 1;
}

cElMap2D * ElAffin2D::Duplicate() 
{
   return new ElAffin2D(*this);
}

cElMap2D * ElAffin2D::Identity() 
{
   return new ElAffin2D(ElAffin2D::Id());
}

int  ElAffin2D::Type()  const {return eTM2_Affine;}


/*****************************************************/
/*                                                   */
/*            ElSimilitude                           */
/*                                                   */
/*****************************************************/

cElMap2D * ElSimilitude::Map2DInverse() const
{
    return new ElSimilitude(inv());
}

cXml_Map2D  ElSimilitude::ToXmlGen()
{
   cXml_Map2DElem anElem;
   anElem.Sim().SetVal(El2Xml(*this));
   return cXml_Map2D(MapFromElem(anElem));
}


int   ElSimilitude::NbUnknown() const
{
    return 4;
}

//   _sc * aP = (s.x+i s.y) * (p.x + i p.y) = (s.x*p.x - s.y p.y) + i (s.y *p.x + s.x * p.y)
//    A -B  X  +  C
//     B A  Y     D
//    A = v1 B=v2 C=v3 D=v4
void  ElSimilitude::AddEq
      (
           Pt2dr & aCste,
           std::vector<double> & anEqX,
           std::vector<double> & anEqY,
           const Pt2dr & aP1,
           const Pt2dr & aP2 
       ) const
{
    aCste.x  = aP2.x;
    anEqX[0] = aP1.x;
    anEqX[1] = -aP1.y;
    anEqX[2] = 1;
    anEqX[3] = 0;


    aCste.y  = aP2.y;
    anEqY[0] = aP1.y;
    anEqY[1] = aP1.x;
    anEqY[2] = 0;
    anEqY[3] = 1;
}

std::vector<double> ElSimilitude::Params() const
{
   std::vector<double> aRes;

   aRes.push_back(_sc.x);
   aRes.push_back(_sc.y);
   aRes.push_back(_tr.x);
   aRes.push_back(_tr.y);

   return aRes;
}

void  ElSimilitude::InitFromParams(const std::vector<double> &aSol)
{
   _sc = Pt2dr(aSol[0],aSol[1]);
   _tr = Pt2dr(aSol[2],aSol[3]);
}

cElMap2D * ElSimilitude::Duplicate() 
{
   return new ElSimilitude(*this);
}

cElMap2D * ElSimilitude::Identity() 
{
   return new ElSimilitude();
}

int ElSimilitude::Type() const { return eTM2_Simil; }


/*****************************************************/
/*                                                   */
/*            cCamAsMap                              */
/*                                                   */
/*****************************************************/

cCamAsMap::cCamAsMap(CamStenope * aCam,bool aDirect)  :
     mCam   (aCam),
     mDirect (aDirect)
{
}

Pt2dr cCamAsMap::operator () (const Pt2dr & p) const
{
   return  mDirect  ? 
           mCam->DistDirecte(p) :  
           mCam->DistInverse(p);
}

cElMap2D * cCamAsMap::Map2DInverse() const
{
   return new cCamAsMap(mCam,!mDirect);
}

cXml_Map2D    cCamAsMap::ToXmlGen()
{
   cXml_MapCam aXmlCam;

   aXmlCam.Directe() = mDirect;
   aXmlCam.PartieCam() = mCam->ExportCalibInterne2XmlStruct(mCam->Sz());

   cXml_Map2DElem anElem;
   anElem.Cam().SetVal(aXmlCam);
   return cXml_Map2D(MapFromElem(anElem));
}

int cCamAsMap::Type() const { return eTM2_Cam; }


/*****************************************************/
/*                                                   */
/*            cElHomographie                         */
/*                                                   */
/*****************************************************/


cElMap2D * cElHomographie::Map2DInverse() const
{
    return new cElHomographie(Inverse());
}

cXml_Map2D   cElHomographie::ToXmlGen()
{
   cXml_Map2DElem anElem;
   anElem.Homog().SetVal(ToXml());
   return cXml_Map2D(MapFromElem(anElem));
}

Pt2dr  cElHomographie::operator() (const Pt2dr & aP) const
{
   return Direct(aP);
}

int   cElHomographie::NbUnknown() const
{
    return 8;
}

std::vector<double> cElHomographie::Params() const
{
   std::vector<double> aRes;

   aRes.push_back(mHX.CoeffX());
   aRes.push_back(mHX.CoeffY());
   aRes.push_back(mHX.Coeff1());

   aRes.push_back(mHY.CoeffX());
   aRes.push_back(mHY.CoeffY());
   aRes.push_back(mHY.Coeff1());

   aRes.push_back(mHZ.CoeffX());
   aRes.push_back(mHZ.CoeffY());


   return aRes;
}

void  cElHomographie::InitFromParams(const std::vector<double> &aSol)
{
   mHX = cElComposHomographie(aSol[0],aSol[1],aSol[2]);
   mHY = cElComposHomographie(aSol[3],aSol[4],aSol[5]);
   mHZ = cElComposHomographie(aSol[6],aSol[7],1.0);
}
//    A B  X1 +  C  ~   X2      A X1 + BY1 +C - X2 (GX1 + HY1 ) = X2
//    D E  Y1    F  ~   Y2      DX1  + EY1 +F - Y2 (GX1 + HY1 ) = Y2
//    G H  1  +  I  ~   1
//   A  B C=Hx ,  DEF=Hy  GHI=Hz
void  cElHomographie::AddEq
      (
           Pt2dr & aCste,
           std::vector<double> & anEqX,
           std::vector<double> & anEqY,
           const Pt2dr & aP1,
           const Pt2dr & aP2 
       ) const
{
    aCste.x  = aP2.x;
      anEqX[0] = aP1.x;
      anEqX[1] = aP1.y;
      anEqX[2] = 1;
      anEqX[3] = 0;
      anEqX[4] = 0;
      anEqX[5] = 0;
      anEqX[6] =  -aP2.x * aP1.x;
      anEqX[7] =  -aP2.x * aP1.y;

    aCste.y  = aP2.y;
      anEqY[0] = 0;
      anEqY[1] = 0;
      anEqY[2] = 0;
      anEqY[3] = aP1.x;
      anEqY[4] = aP1.y;
      anEqY[5] = 1;
      anEqY[6] =  -aP2.y * aP1.x;
      anEqY[7] =  -aP2.y * aP1.y;
}

cElMap2D * cElHomographie::Duplicate() 
{
   return new cElHomographie(*this);
}

cElMap2D * cElHomographie::Identity() 
{
   return new cElHomographie(cElHomographie::Id());
}

int cElHomographie::Type() const { return eTM2_Homogr; }

/*****************************************************/
/*                                                   */
/*            ElHomot                                */
/*                                                   */
/*****************************************************/
ElHomot::ElHomot(Pt2dr aTrans, double aScale) :
   mTr (aTrans),
   mSc (aScale)
{
}

ElHomot ElHomot::operator * (const ElHomot & aHom2) const
{
  return ElHomot ( mTr+aHom2.mTr*mSc   ,    mSc*aHom2.mSc );
}
int ElHomot::Type() const { return int(eTM2_Homot); }
cElMap2D * ElHomot::Map2DInverse() const 
{
   return new ElHomot(inv());
}
cElMap2D *  ElHomot::Duplicate() 
{
    return new ElHomot(mTr,mSc);
}

cElMap2D  * ElHomot::Identity() {return new ElHomot;}

cXml_Map2D  ElHomot::ToXmlGen()
{
   cXml_Map2DElem anElem;
   anElem.Homot().SetVal(EL2Xml(*this));
   return MapFromElem(anElem);
}

int  ElHomot::NbUnknown() const {return 3;}

//  A  X1  + B  = X2
//     Y1    C    Y2

void  ElHomot::AddEq
      (
           Pt2dr & aCste,
           std::vector<double> & anEqX,
           std::vector<double> & anEqY,
           const Pt2dr & aP1,
           const Pt2dr & aP2 
      ) const
{
    aCste.x  = aP2.x;
    anEqX[0] = aP1.x;
    anEqX[1] = 1;
    anEqX[2] = 0;


    aCste.y  = aP2.y;
    anEqY[0] = aP1.y;
    anEqY[1] = 0;
    anEqY[2] = 1;
}

std::vector<double> ElHomot::Params() const
{
   std::vector<double> aRes;

   aRes.push_back(mSc);
   aRes.push_back(mTr.x);
   aRes.push_back(mTr.y);

   return aRes;
}

void  ElHomot::InitFromParams(const std::vector<double> &aSol)
{
   mSc = aSol[0];
   mTr = Pt2dr(aSol[1],aSol[2]);
}

ElHomot ElHomot::inv () const
{
   return ElHomot ( (-mTr)/mSc, 1/mSc);
}

cXml_Homot   EL2Xml(const ElHomot & aHom)
{
   cXml_Homot aXml;
   aXml.Scale() =  aHom.Sc();
   aXml.Tr() =  aHom.Tr();

   return aXml;
}

ElHomot      Xml2EL(const cXml_Homot & aXml)
{
   return ElHomot(aXml.Tr(),aXml.Scale());
}




/*****************************************************/
/*                                                   */
/*            cElMap2D                               */
/*                                                   */
/*****************************************************/

cElMap2D * cElMap2D::IdentFromType(int aType)
{
    if (aType == int(eTM2_Homot))    return new ElHomot;
    if (aType == int(eTM2_Simil))    return new ElSimilitude;
    if (aType == int(eTM2_Affine))   return new ElAffin2D(ElAffin2D::Id());
    if (aType == int(eTM2_Homogr))   return new cElHomographie(cElHomographie::Id());

    ELISE_ASSERT(false,"cElMap2D::IdentFromType");
    return 0;
}

std::vector<double> cElMap2D::Params() const
{
    ELISE_ASSERT(false,"cElMap2D::Params");
    return std::vector<double>();
}

void cElMap2D::Affect(const cElMap2D & aMap)
{
    ELISE_ASSERT(Type()==aMap.Type(),"Different type in cElMap2D::Affect");
    std::vector<double>  aVP = aMap.Params();
    InitFromParams(aVP);
}

cElMap2D * cElMap2D::Map2DInverse() const
{
   ELISE_ASSERT(false,"No def cElMap2D::Map2DInverse");
   return 0;
}

cElMap2D * cElMap2D::Simplify() 
{
   return this;
}

cXml_Map2D      cElMap2D::ToXmlGen()
{
   ELISE_ASSERT(false,"No def cElMap2D::ToXmlGen");
   return cXml_Map2D();
}

void   cElMap2D::SaveInFile(const std::string & aName)
{
    cXml_Map2D aXml = ToXmlGen();
    MakeFileXML(aXml,aName);
}

cElMap2D *  Map2DFromElem(const cXml_Map2DElem & aXml)
{
   if (aXml.Homog().IsInit()) return new cElHomographie(aXml.Homog().Val());
   if (aXml.Sim().IsInit()) return new ElSimilitude(Xml2EL(aXml.Sim().Val()));
   if (aXml.Aff().IsInit()) return new ElAffin2D(Xml2EL(aXml.Aff().Val()));
   if (aXml.Cam().IsInit())
   {
       CamStenope* aCS = Std_Cal_From_CIC(aXml.Cam().Val().PartieCam());
       return new cCamAsMap(aCS,aXml.Cam().Val().Directe());
   }


   ELISE_ASSERT(false,"Map2DFromElem");
   return 0;
}

cElMap2D *  cElMap2D::FromFile(const std::string & aName)
{
   cXml_Map2D aXml = StdGetFromSI(aName,Xml_Map2D);
   std::vector<cElMap2D *> aVMap;

   for (std::list<cXml_Map2DElem>::const_iterator itM=aXml.Maps().begin() ; itM!=aXml.Maps().end() ; itM++)
   {
      aVMap.push_back(Map2DFromElem(*itM));
   }


   return new cComposElMap2D(aVMap);
}

int   cElMap2D::NbUnknown() const
{
   ELISE_ASSERT(false,"cElMap2D::NbUnknown");
   return -1;
}
void  cElMap2D::AddEq(Pt2dr & ,std::vector<double> & ,std::vector<double> & ,const Pt2dr & aP1,const Pt2dr & aP2 ) const
{
   ELISE_ASSERT(false,"cElMap2D::AddEq");
}

void  cElMap2D::InitFromParams(const std::vector<double> &aSol)
{
   ELISE_ASSERT(false,"cElMap2D::InitFromParams");
}

cElMap2D * cElMap2D::Duplicate() 
{
   ELISE_ASSERT(false,"cElMap2D::AddEq");
   return 0;
}

cElMap2D * cElMap2D::Identity() 
{
   ELISE_ASSERT(false,"cElMap2D::AddEq");
   return 0;
}

/*****************************************************/
/*                                                   */
/*            cComposElMap2D                         */
/*                                                   */
/*****************************************************/

cComposElMap2D::cComposElMap2D(const std::vector<cElMap2D *>  & aVMap) :
   mVMap (aVMap)
{
}

Pt2dr cComposElMap2D::operator () (const Pt2dr & aP)  const
{
   Pt2dr aRes = aP;
   for (int aK=0 ; aK<int(mVMap.size()) ; aK++)
       aRes = (*(mVMap[aK]))(aRes);
   return aRes;
}

cElMap2D *  cComposElMap2D::Map2DInverse() const
{
   std::vector<cElMap2D *> aVInv;
   for (int aK=int(mVMap.size()-1) ; aK>=0 ; aK--)
      aVInv.push_back(mVMap[aK]->Map2DInverse());

   return new cComposElMap2D(aVInv);
}

cElMap2D * cComposElMap2D::Simplify() 
{
   if (mVMap.size()==1) 
      return mVMap[0];

   return this;
}


cXml_Map2D    cComposElMap2D::ToXmlGen()
{
   cXml_Map2D aRes;

   for (int aK=0 ; aK<int(mVMap.size()) ; aK++)
   {
        cXml_Map2D aXml = mVMap[aK]->ToXmlGen();
        for (std::list<cXml_Map2DElem>::const_iterator itM2=aXml.Maps().begin() ; itM2!=aXml.Maps().end() ; itM2++)
        {
            aRes.Maps().push_back(*itM2);
        }
   }
   return aRes;
}
int  cComposElMap2D::Type()  const {return eTM2_Compos;}

/*****************************************************/
/*                                                   */
/*                 ::                                */
/*                                                   */
/*****************************************************/

void  L2EstimMapHom(cElMap2D * aRes,const ElPackHomologue & aPack)
{
    // cElMap2D & aMapInit = *(cElMap2D::IdentFromType(aType));


    int aNbUk = aRes->NbUnknown();
    std::vector<double> aVCoefX(aNbUk);
    std::vector<double> aVCoefY(aNbUk);
    L2SysSurResol aSys(aNbUk);

    for (ElPackHomologue::const_iterator itCpl=aPack.begin();itCpl!=aPack.end() ; itCpl++)
    {
        Pt2dr aQ;
        aRes->AddEq(aQ,aVCoefX,aVCoefY,itCpl->P1(),itCpl->P2());
        aSys.AddEquation(itCpl->Pds(),VData(aVCoefX),aQ.x);
        aSys.AddEquation(itCpl->Pds(),VData(aVCoefY),aQ.y);
    }
    Im1D_REAL8  aSol = aSys.Solve(0);
    aRes->InitFromParams(std::vector<double>(aSol.data(),aSol.data()+aNbUk));
}

cElMap2D * L2EstimMapHom(eTypeMap2D aType,const ElPackHomologue & aPack)
{
    cElMap2D * aRes = cElMap2D::IdentFromType(aType);
    L2EstimMapHom(aRes,aPack);
    return aRes;
}


int CPP_CalcMapAnalitik(int argc,char** argv)
{
    std::string aName1,aName2,aNameOut,aSH,anExt="dat";
    std::string anOri;
    std::string aNameType;
    Pt2dr       aPerResidu(100,100);
    std::vector<double> aVRE; // Robust Estim

    // int NbTest =50;
    // double  Perc = 80.0;
    // int     NbMaxPts= 10000;
    eTypeMap2D aType;
    bool aModeHelp;

    if ((argc>=2) && (std::string(argv[1]) == "-help"))
    {
        StdReadEnum(aModeHelp,aType,"-help",eTypeMap2D(eTM2_Homogr+1));
    }

    ElInitArgMain
    (
        argc,argv,
        LArgMain()  <<  EAMC(aName1,"Name Im1")
                    <<  EAMC(aName2,"Name Im2")
                    <<  EAMC(aNameType,"Model in [Homot,Simil,Affine,Homogr]")
                    <<  EAMC(aNameOut,"Name Out"),
        LArgMain()  <<  EAM(aSH,"SH",true,"Set of homologue")
                    <<  EAM(anOri,"Ori",true,"Directory to read distorsion")
                    <<  EAM(aPerResidu,"PerResidu",true,"Period for computing residual")
                    <<  EAM(aVRE,"PRE",true,"Param for robust estimation [PropInlayer,NbRan(500),NbPtsRan(+inf)]")
    );

    StdReadEnum(aModeHelp,aType,std::string("TM2_")+aNameType,eTM2_NbVals);


    cElemAppliSetFile anEASF(aName1);
    cInterfChantierNameManipulateur * anICNM = anEASF.mICNM;
    std::string aDir = anEASF.mDir;

    std::string aDirResidu = aDir+ "ResiduImDir/";
   


    CamStenope * aCS1=0,*aCS2=0;
    if (EAMIsInit(&anOri))
    {
         StdCorrecNameOrient(anOri,aDir);
         aCS1 = anICNM->GlobCalibOfName(aName1,anOri,false);
         aCS2 = anICNM->GlobCalibOfName(aName2,anOri,false);

         aCS1->Get_dist().SetCameraOwner(aCS1);
         aCS2->Get_dist().SetCameraOwner(aCS2);
    }

    std::string aKHIn =   std::string("NKS-Assoc-CplIm2Hom@")
                       +  std::string(aSH)
                       +  std::string("@")
                       +  std::string(anExt);


    std::string aNameIn = aDir + anICNM->Assoc1To2(aKHIn,aName1,aName2,true);
    ElPackHomologue aPackIn =  ElPackHomologue::FromFile(aNameIn);
    ElPackHomologue aPackInitial = aPackIn; 

    Pt2dr aP1Max(-1e20,-1e20);
    Pt2dr aP1Min( 1e20, 1e20);
    for (ElPackHomologue::iterator itCpl=aPackIn.begin();itCpl!=aPackIn.end() ; itCpl++)
    {
        if (aCS1)
        {
            itCpl->P1() = aCS1->DistInverse(itCpl->P1());
            itCpl->P2() = aCS2->DistInverse(itCpl->P2());
        }
        aP1Max = Sup(aP1Max, itCpl->P1());
        aP1Min = Inf(aP1Min, itCpl->P1());
    }

    std::cout << "P1Max " << aP1Max  << " " << aP1Min << "\n";

    Pt2di aSzResidu = round_up(aP1Max.dcbyc(aPerResidu));
    Im2D_REAL8 aImResX(aSzResidu.x,aSzResidu.y,0.0);
    Im2D_REAL8 aImResY(aSzResidu.x,aSzResidu.y,0.0);


    // double anEcart,aQuality;
    // bool Ok;

    cElMap2D * aMapCor = 0;
  
    if (EAMIsInit(&aVRE))
    {
       // <<  EAM(aVRE,"PRE",true,"Param for robust estimation [PercInlayer,NbRan(500),NbPtsRan(+inf)]")
       double aProp  =  aVRE[0];
       int aNbRan    = (aVRE.size()>1) ? aVRE[1] : 500;
       int aNbPtsRan = (aVRE.size()>2) ? aVRE[2] : 2e9;
       cParamMap2DRobustInit aParam(aType,aNbRan);
       aParam.mPropRan = aProp;
       aParam.mNbMaxPtsRansac = aNbPtsRan;
       Map2DRobustInit(aPackIn,aParam);
       aMapCor = aParam.mRes;
    }
    else
    {
       aMapCor  =  L2EstimMapHom(aType,aPackIn);
    }


    std::vector<cElMap2D *> aVMap;
    if (aCS1)
    {
       aVMap.push_back(new cCamAsMap(aCS1,false));
    }
    aVMap.push_back(aMapCor);
    if (aCS2)
    {
       aVMap.push_back(new cCamAsMap(aCS2,true));
    }

    cComposElMap2D aComp(aVMap);

    std::vector<double> aVDist;
    double aSomD2X=0.0;
    double aSomD2Y=0.0;
    for (ElPackHomologue::iterator itCpl=aPackInitial.begin();itCpl!=aPackInitial.end() ; itCpl++)
    {
        Pt2dr aRes = aComp(itCpl->P1())-itCpl->P2();
        double aD = euclid(aRes);
        aVDist.push_back(aD);
        aSomD2X += ElSquare(aRes.x);
        aSomD2Y += ElSquare(aRes.y);
        Pt2di aPInd = round_ni(itCpl->P1().dcbyc(aPerResidu));
        aImResX.SetR_SVP(aPInd,aRes.x);
        aImResY.SetR_SVP(aPInd,aRes.y);
        // if (BadNumber(aD)) std::cout << "KKKK " << aD << "\n";
    }
    MakeFileXML(aComp.ToXmlGen(),aNameOut);

    if (EAMIsInit(&aPerResidu))
    {
       ELISE_fp::MkDirSvp(aDirResidu);
       std::string aPref = "Res-" +  aNameType + "-" + aName1 + "-" + aName2 ;
       Tiff_Im::CreateFromIm(aImResX,aDirResidu+aPref+"-X.tif");
       Tiff_Im::CreateFromIm(aImResY,aDirResidu+aPref+"-Y.tif");
    }
    

    int aNbDist=10;
    for (int aK=0; aK<=aNbDist ; aK++)
    {
        double aProp = aK/double(aNbDist);
        std::cout << "  Residu at " << aProp*100 << " percentil = " << KthValProp(aVDist,aProp) << "\n";
    }
    aSomD2X /= aPackIn.size();
    aSomD2Y /= aPackIn.size();

    std::cout << "MoyD2 = " << sqrt(aSomD2X)  << " " << sqrt(aSomD2Y) << "\n";



    return EXIT_SUCCESS;

}


int CPP_ReechImMap(int argc,char** argv)
{
    std::string aNameIm,aNameMap;
    Pt2di aSzOut;
    std::string aNameOut;

    ElInitArgMain
    (
        argc,argv,
        LArgMain()  <<  EAMC(aNameIm,"Name Im")
                    <<  EAMC(aNameMap,"Name map"),
        LArgMain()  
    );

    if (!EAMIsInit(&aNameOut))
       aNameOut = DirOfFile(aNameIm) + "Reech_" + NameWithoutDir(StdPrefix(aNameIm)) +".tif";

    cElMap2D * aMap = cElMap2D::FromFile(aNameMap);

    Tiff_Im aTifIn = Tiff_Im::StdConvGen(aNameIm,-1,true);


    std::vector<Im2DGen *>  aVecImIn =  aTifIn.ReadVecOfIm();
    int aNbC = aVecImIn.size();
    Pt2di aSzIn = aVecImIn[0]->sz();
    if (! EAMIsInit(&aSzOut))
       aSzOut = aSzIn;

    std::vector<Im2DGen *> aVecImOut =  aTifIn.VecOfIm(aSzOut);

    std::vector<cIm2DInter*> aVInter;
    for (int aK=0 ; aK<aNbC ; aK++)
    {
        aVInter.push_back(aVecImIn[aK]->SinusCard(5,5));
    }


    Pt2di aP;
    for (aP.x =0 ; aP.x<aSzOut.x ; aP.x++)
    {
        for (aP.y =0 ; aP.y<aSzOut.y ; aP.y++)
        {
            Pt2dr aQ = (*aMap)(Pt2dr(aP));
            for (int aK=0 ; aK<aNbC ; aK++)
            {
                double aV = aVInter[aK]->GetDef(aQ,0);
                aVecImOut[aK]->SetR(aP,aV);
            }
        }
    }

    Tiff_Im aTifOut
            (
                aNameOut.c_str(),
                aSzOut,
                aTifIn.type_el(),
                Tiff_Im::No_Compr,
                aTifIn.phot_interp()
            );

    ELISE_COPY(aTifOut.all_pts(),StdInPut(aVecImOut),aTifOut.out());

    return EXIT_SUCCESS;
}

class cAppli_CPP_DenseMapToHom
{
   public :

    std::string mName1;
    std::string mName2;
    std::string mNamePx1;
    std::string mNamePx2;
    std::string mSH;
    std::string mExt;
    Pt2di       mSzIm;
    Tiff_Im *   mTifX;
    Tiff_Im *   mTifY;
    Fonc_Num    mFPond;
    double      mNbTile;
    Pt2di       mSz;
    Pt2di       mSzRed;

    Im2D_REAL8  mImX1;
    Im2D_REAL8  mImY1;
    Im2D_REAL8  mImX2;
    Im2D_REAL8  mImY2;
    Im2D_REAL8  mImP;
    double      mOverlap; // Si 1 les dalles se recouvrent juste

    cAppli_CPP_DenseMapToHom(int argc,char** argv);
};


cAppli_CPP_DenseMapToHom::cAppli_CPP_DenseMapToHom(int argc,char** argv) :
    mSH      ("DM"),
    mExt     ("dat"),
    mFPond   (1.0),
    mNbTile  (30),
    mImX1    (1,1),
    mImY1    (1,1),
    mImX2    (1,1),
    mImY2    (1,1),
    mImP     (1,1),
    mOverlap (1.0)
{

    ElInitArgMain
    (
        argc,argv,
        LArgMain()  <<  EAMC(mName1,"Name Im1")
                    <<  EAMC(mName2,"Name Im2")
                    <<  EAMC(mNamePx1,"Name Px1 (dx)")
                    <<  EAMC(mNamePx2,"Name Px2 (dy)"),
        LArgMain()  <<  EAM(mSH,"SH",true,"Set of homologue, def=DM")
                    <<  EAM(mNbTile,"NbTiles",true,"Number of tile / side (will be slightly changed), Def=30")
    );

    cElemAppliSetFile anEASF(mName1);
    cInterfChantierNameManipulateur * anICNM = anEASF.mICNM;
    std::string aDir = anEASF.mDir;

    std::string aKHOut =   std::string("NKS-Assoc-CplIm2Hom@")
                       +  std::string(mSH)
                       +  std::string("@")
                       +  std::string(mExt);

    std::string aNameOut = aDir + anICNM->Assoc1To2(aKHOut,mName1,mName2,true);

    mTifX =   new Tiff_Im(Tiff_Im::StdConvGen(mNamePx1,-1,true));
    mTifY =   new Tiff_Im(Tiff_Im::StdConvGen(mNamePx2,-1,true));

    mSz = mTifX->sz();

    double aLargTile = sqrt((mSz.x * double(mSz.y) ) / ElSquare(mNbTile));
    mSzRed = round_up(Pt2dr(mSz)/aLargTile);
    aLargTile = ElMax(mSz.x/double(mSzRed.x),mSz.y/double(mSzRed.y)) * mOverlap;
    Pt2dr mDemiTile(aLargTile/2.0,aLargTile/2.0);

    mImX1 =  Im2D_REAL8(mSzRed.x,mSzRed.y);
    mImY1 =  Im2D_REAL8(mSzRed.x,mSzRed.y);
    mImX2 =  Im2D_REAL8(mSzRed.x,mSzRed.y);
    mImY2 =  Im2D_REAL8(mSzRed.x,mSzRed.y);
    mImP  =  Im2D_REAL8(mSzRed.x,mSzRed.y,0.0);

    Pt2di aP;
    double aMaxP=0.0;
    for (aP.x=0 ; aP.x< mSzRed.x ; aP.x++)
    {
       for (aP.y=0 ; aP.y< mSzRed.y ; aP.y++)
       {
           Pt2dr aPC = Pt2dr(aP) + Pt2dr(0.5,0.5); // au cente de la dalle reduite
           aPC = aPC.dcbyc(Pt2dr(mSzRed)); // Entre 0 et 1
           aPC = aPC.mcbyc(Pt2dr(mSz));    // au centre des coordonnees pleines

           Pt2di aPIm0 = Sup(Pt2di(0,0),round_down(aPC-mDemiTile));
           Pt2di aPIm1 = Inf(mSz,round_down(aPC+mDemiTile));
           double aSom[5];
           ELISE_COPY
           (
               rectangle(aPIm0,aPIm1),
               Virgule(FX*mFPond,FY*mFPond,mTifX->in()*mFPond,mTifY->in()*mFPond,mFPond),
               sigma(aSom,5)
           );
           if (aSom[4] > mImP.GetR(aP)) 
           {
               mImX1.SetR(aP,aSom[0]/aSom[4]);
               mImY1.SetR(aP,aSom[1]/aSom[4]);
               mImX2.SetR(aP,aSom[2]/aSom[4]);
               mImY2.SetR(aP,aSom[3]/aSom[4]);
               mImP.SetR(aP,aSom[4]);
               ElSetMax(aMaxP,aSom[4]);
           }
       }
    }
    std::cout << "PerResidu=" << Pt2dr(mSz).dcbyc(Pt2dr(mSzRed)) << "\n";

    ElPackHomologue aPack;
    for (aP.x=0 ; aP.x< mSzRed.x ; aP.x++)
    {
       for (aP.y=0 ; aP.y< mSzRed.y ; aP.y++)
       {
           Pt2dr aPIm1(mImX1.GetR(aP),mImY1.GetR(aP));
           Pt2dr aPIm2(mImX2.GetR(aP),mImY2.GetR(aP));
           aPIm2  = aPIm1 + aPIm2;
           aPack.Cple_Add(ElCplePtsHomologues(aPIm1,aPIm2,mImP.GetR(aP)));
       }
    }
    aPack.StdPutInFile(aNameOut);
}


int CPP_DenseMapToHom(int argc,char** argv)
{
    cAppli_CPP_DenseMapToHom anAppli(argc,argv);

    return EXIT_SUCCESS;
}

int CPP_CmpDenseMap(int argc,char** argv)
{
    std::string mNameXI1,mNameYI1,mNameXI2,mNameYI2;
    ElInitArgMain
    (
        argc,argv,
        LArgMain()  <<  EAMC(mNameXI1,"Im1 Px1 (dx)")
                    <<  EAMC(mNameYI1,"Im1 Px2 (dy)")
                    <<  EAMC(mNameXI2,"Im2 Px1 (dx)")
                    <<  EAMC(mNameYI2,"Im2 Px2 (dy)"),
        LArgMain()  
    );

    Tiff_Im aX1(mNameXI1.c_str());
    Tiff_Im aY1(mNameYI1.c_str());
    Tiff_Im aX2(mNameXI2.c_str());
    Tiff_Im aY2(mNameYI2.c_str());

    Pt2di aSz = aX1.sz();

    double aS1,aS2;

    ELISE_COPY(aX1.all_pts(),Abs(aX1.in())+Abs(aY1.in()),sigma(aS1));
    ELISE_COPY(aX1.all_pts(),Abs(aX2.in())+Abs(aY2.in()),sigma(aS2));

    double R12 = aS1/aS2;
    double aDif;

    ELISE_COPY
    (
         aX1.all_pts(),
         Abs(aX2.in()*R12-aX1.in())+Abs(aY2.in()*R12-aY1.in()),
         sigma(aDif)
    );

    std::cout << "R12 = " << R12  << " DIF=" << (aDif / (double (aSz.x) * aSz.y))<< "\n";

    return EXIT_SUCCESS;
}



cParamMap2DRobustInit::cParamMap2DRobustInit(eTypeMap2D aType,int aNbTirRans) :
    mType           (aType),
    mNbTirRans      (aNbTirRans),
    mNbMaxPtsRansac (2e9),
    mNbTestFor1P    (4),
    mPropRan        (0.8),
    mNbIterL2       (4)
{
}


typedef  std::pair<Pt2dr,Pt2dr>  tPairP;
void  Map2DRobustInit(const ElPackHomologue & aPackFull,cParamMap2DRobustInit & aParam)
{
   aParam.mRes = 0;
   std::vector<tPairP> aVRansac;
   int aNbPtsTot = aPackFull.size();

   // Selection de mNbMaxPtsRansac points repartis regulierement
   {
       int aCpt = 0;
       double aPropCons = ElMin(aParam.mNbMaxPtsRansac/double(aNbPtsTot),1.0);

       for (ElPackHomologue::tCstIter itH=aPackFull.begin() ; itH!=aPackFull.end() ; itH++)
       {
            Pt2dr aP1 = itH->P1();
            Pt2dr aP2 = itH->P2();
            tPairP aPair(aP1,aP2);

            if  ( round_ni((aCpt-1)*aPropCons) != round_ni(aCpt*aPropCons) )
            {
                aVRansac.push_back(aPair);
            }
       }
   }
   int aNbPRan = aVRansac.size();

   // Calcul du Ransac
   cElMap2D * aTestMap = cElMap2D::IdentFromType(aParam.mType);
   cElMap2D * aBestSol = cElMap2D::IdentFromType(aParam.mType);
   double aBestScRan = 1e60;
   {
      int aNbUk = aBestSol->NbUnknown();
      int aNbPtsReq = (aNbUk+1)/2;
      if (aNbPtsTot < aNbPtsReq)
        return;

      for (int aKTirage = 0 ; aKTirage<aParam.mNbTirRans ; aKTirage++)
      {
           // aPackEstim must contain point that are relatively far from each others
           ElPackHomologue aPackEstim;
           for (int aKP=0 ; aKP<aNbPtsReq ; aKP++)
           {
                int aNbTest = 0;
                double aDMaxMin = -1;
                int aKMM=-1; // KMaxMin
                while (aNbTest <  aParam.mNbTestFor1P)
                {
                     int aKP = NRrandom3(aNbPRan);
                     Pt2dr aP1 = aVRansac[aKP].first;
                     double aDMin = 1e60;
                     for (ElPackHomologue::const_iterator itP = aPackEstim.begin(); itP!=aPackEstim.end() ; itP++)
                     {
                         ElSetMin(aDMin,euclid(itP->P1(),aP1));
                     }
                     if (aDMin>0)
                     {
                         aNbTest++;
                         aDMin = NRrandom3() * aDMin;
                         if (aDMin>aDMaxMin)
                         {
                            aDMaxMin = aDMin;
                            aKMM = aKP;
                         }
                     }
                }
                ELISE_ASSERT(aKMM>=0,"Incoh in Map2DRobustInit");
                aPackEstim.Cple_Add(ElCplePtsHomologues(aVRansac[aKMM].first,aVRansac[aKMM].second));
           }
  
           // Une fois connu le pack on estime la map, puis le score
           L2EstimMapHom(aTestMap,aPackEstim);
           std::vector<double> aVDist;
           for (int aKP=0 ; aKP< aNbPRan ; aKP++)
           {
               const tPairP & aPair = aVRansac[aKP];
               aVDist.push_back(euclid((*aTestMap)(aPair.first) - aPair.second));
           }
           double aScore = KthValProp(aVDist,aParam.mPropRan);
           if (aScore< aBestScRan)
           {
              aBestScRan = aScore;
              aBestSol->Affect(*aTestMap);
           }
      }
   }
   double aD2Std  = ElMax(1e-60,ElSquare(aBestScRan));
   
   for (int aKItL2=0 ; aKItL2<aParam.mNbIterL2; aKItL2++)
   {
       ElPackHomologue aPackEstim;
       std::vector<double> aVD2;
       for (ElPackHomologue::tCstIter itH=aPackFull.begin() ; itH!=aPackFull.end() ; itH++)
       {
            Pt2dr aP1 = itH->P1();
            Pt2dr aP2 = itH->P2();
            double aD2 = square_euclid((*aBestSol)(aP1)-aP2);
            double aPds   = 1/ (1+ (4.0*aD2)/aD2Std);
            aPackEstim.Cple_Add(ElCplePtsHomologues(aP1,aP2,aPds));
            aVD2.push_back(aD2);
       }
       aD2Std  = KthValProp(aVD2,aParam.mPropRan);
       L2EstimMapHom(aBestSol,aPackEstim);
   }
   delete aTestMap;
   aParam.mRes = aBestSol;
}


/**********************************************************************/
/*                                                                    */
/*                     TESTS                                          */
/*                                                                    */
/**********************************************************************/

Pt2dr PRanCInSquare(double aScale) {return Pt2dr(aScale*NRrandC(),aScale*NRrandC());}

double TestMap2D(cElMap2D & aMapInit,const ElPackHomologue & aPackInit,bool WithRand)
{
    ElPackHomologue  aPack = aPackInit;
    for (ElPackHomologue::iterator itCpl=aPack.begin();itCpl!=aPack.end() ; itCpl++)
    {
         itCpl->P2() = (aMapInit)(itCpl->P1());
    }

    // On test aussi la fonction d'affectation
    cElMap2D * aMap = L2EstimMapHom(eTypeMap2D(aMapInit.Type()),aPack);
    cElMap2D * aMap2 =  cElMap2D::IdentFromType(aMapInit.Type());

    aMap2->Affect(*aMap);

    double aSomD = 0;
    for (ElPackHomologue::iterator itCpl=aPack.begin();itCpl!=aPack.end() ; itCpl++)
    {
         aSomD += euclid(aMapInit(itCpl->P1()) - (*aMap2)(itCpl->P1()));
    }

    // Randomization
    if (WithRand)
    {
       int aCpt = 0;
       for (ElPackHomologue::iterator itCpl=aPack.begin();itCpl!=aPack.end() ; itCpl++)
       {
           if ((aCpt%8)==0) // 12.5 % d'erreur
               itCpl->P2() =  itCpl->P2() +  PRanCInSquare(100.0);
            else 
               itCpl->P2() =  itCpl->P2() +  PRanCInSquare(0.5);
           aCpt ++;
       }
       cParamMap2DRobustInit aParam(eTypeMap2D(aMapInit.Type()),200);
       Map2DRobustInit(aPack,aParam);
       cElMap2D & aMRob = *(aParam.mRes);

       std::vector<double> aVDistTh;
       std::vector<double> aVDistEmp;
       for (ElPackHomologue::iterator itCpl=aPack.begin();itCpl!=aPack.end() ; itCpl++)
       {
            aVDistTh.push_back(euclid(aMapInit(itCpl->P1()) - (aMRob)(itCpl->P1())));
            aVDistEmp.push_back(euclid( aMRob(itCpl->P1()) - itCpl->P2()));
       }
       std::cout << "MED " << KthValProp(aVDistTh,0.5) << " " <<  KthValProp(aVDistEmp,0.5) << "\n";
       std::cout << " 75 " << KthValProp(aVDistTh,0.75) << " " <<  KthValProp(aVDistEmp,0.75) << "\n";
       std::cout << " 90 " << KthValProp(aVDistTh,0.9) << " " <<  KthValProp(aVDistEmp,0.9) << "\n\n";
    }


    return aSomD;
}

cElComposHomographie RanCH(double aScaleXY,double Cste){return cElComposHomographie(NRrandC()*aScaleXY,NRrandC()*aScaleXY,1+Cste*NRrandC());}

void TestMap2D()
{
    ElPackHomologue aPackInit;
    for (int aK=0 ; aK< 1000 ; aK++)
        aPackInit.Cple_Add(ElCplePtsHomologues(PRanCInSquare(1000),PRanCInSquare(1000),1.0));

    for (int aK=0 ; aK< 10 ; aK++)
    {
         ElSimilitude aS(PRanCInSquare(100),PRanCInSquare(1));
         ElAffin2D anAff(PRanCInSquare(100),PRanCInSquare(1),PRanCInSquare(1));
         cElHomographie aHom(RanCH(1,1000),RanCH(1,1000),RanCH(1e-5,1e-5));

         ElHomot  aHomot(PRanCInSquare(100), NRrandC());


         if (aK==5)
            std::cout << "=============================================\n";
         if (aK< 5)
         {
            std::cout << " # "
                      << " SIM " << TestMap2D(aS   ,aPackInit,false) 
                      << " AFF " << TestMap2D(anAff,aPackInit,false) 
                      << " Hom " << TestMap2D(aHom,aPackInit,false) 
                      << " Homot " << TestMap2D(aHomot,aPackInit,false) 
                      << "\n";
          }
          else
          {
             TestMap2D(aS,aPackInit,true);
             TestMap2D(anAff,aPackInit,true);
             TestMap2D(aHom,aPackInit,true);
             TestMap2D(aHomot,aPackInit,true);
             std::cout << "  -  -  -  -  -  -  -  -  -\n";
          }

    }
    exit(EXIT_SUCCESS);
}





  
         // static cElMap2D * FromFile(const std::string &);
         // virtual cXml_Map2D *     ToXmlGen() ; // P

/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant à la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
