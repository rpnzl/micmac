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


#ifndef _TiePTri_
#define _TiePTri_

#include "StdAfx.h"
#include "../../TpMMPD/TiePByMesh/Fast.h"
#include "MultTieP.h"
// Header du header
class cHomolPackTiepTri;
class cParamAppliTieTri;
class cAppliTieTri;
class cImTieTri;
class cImMasterTieTri;
class cImSecTieTri;
template<class Type> class cResulRechCorrel;
template<class Type> class cResulMultiImRechCorrel;
class cOneTriMultiImRechCorrel;
class cIntTieTriInterest;
class cLinkImTT;


#define TT_DefCorrel -2.0
#define TT_MaxCorrel 1.0
#define TT_DIST_RECH_HOM 12.0  // Seuil de recherche des homologues
#define TT_DIST_EXTREMA  3.0   // calcul des extrema locaux
#define TT_DIST_FAST  4.0   // Critere type Fast calcul des extrema locaux

#define TT_SEUIL_CORREL_1PIXSUR2  0.7   // calcul des extrema locaux
#define TT_DefSeuilDensiteResul   50   // conserve 1 point / disque de rayon TT_DefSeuilDensiteResul
#define TT_RatioFastFiltrSpatial  4     // Ratio par rapport a TT_DefSeuilDensiteResul     
#define TT_SEUIL_SURF_TRI_PIXEL   100.0 //  Supprime les triangles trop petits

#define TT_SEUIL_AutoCorrel  0.85          // Seuil d'elimination par auto-correlation
#define TT_SEUIL_CutAutoCorrel_INT 0.65    // Seuil d'acceptation rapide par auto correl entiere
#define TT_SEUIL_CutAutoCorrel_REEL 0.75   // Seuil d'acceptation rapide par auto correl reelle

#define TT_SEUIl_DIST_Extrema_Entier  1.5  // Distance entre l'extrema init et le max de correl trouve

#define TT_DemiFenetreCorrel 6

#define TT_PropFastStd 0.75
#define TT_PropFastConsec 0.6

#define TT_SeuilFastStd  5
#define TT_SeuilFastCons 3

#define TT_SZ_AUTO_COR 3

// #define  TT_SEUIL


extern bool BugAC;
extern bool USE_SCOR_CORREL;




//  =====================================

//  =====================================

typedef double                          tElTiepTri ;
typedef Im2D<tElTiepTri,tElTiepTri>     tImTiepTri;
typedef TIm2D<tElTiepTri,tElTiepTri>    tTImTiepTri;
typedef cInterpolateurIm2D<tElTiepTri>  tInterpolTiepTri;

// Prop.x => standard , Prop.y => contingu
// extern Pt2dr   TestFastQuality(TIm2D<double,double> anIm,Pt2di aP,double aRay,bool IsMax,Pt2dr aProp);
extern void TestcAutoCorrelDir(TIm2D<double,double> aTIm,const Pt2di & aP0);

// Pour initialiser les parametres avec EAM en ayant un constructeur trivial
class cParamAppliTieTri
{
    public :
        cParamAppliTieTri() ;

        double   mDistFiltr; 
        int      mNumInterpolDense; 
        bool     mDoRaffImInit;
        int      mNbByPix;
        int      mSzWEnd;
        int      mNivLSQM;
        double   mRandomize;
        bool     mNoTif;
};



class cAppliTieTri : public cParamAppliTieTri
{
      public :

           cAppliTieTri
           (
              const cParamAppliTieTri &,
              cInterfChantierNameManipulateur *,
              const std::string & aDir,  
              const std::string & anOri,  
              const cXml_TriAngulationImMaster &
           );

           void SetSzW(Pt2di , int);


           cInterfChantierNameManipulateur * ICNM();
           const std::string &               Ori() const;
           const std::string &               Dir() const;
           void DoAllTri              (const cXml_TriAngulationImMaster &);

           bool  WithW() const;
           Pt2di  SzW() const;
           int    ZoomW() const;
           cImMasterTieTri * Master();
           const std::vector<Pt2di> &   VoisExtr() const;
           const std::vector<Pt2di> &   VoisHom() const;
           bool  & Debug() ;
           const double & DistRechHom() const;
           int & NivInterac();
           const cElPlan3D & CurPlan() const;


           tInterpolTiepTri * Interpol();

           void FiltrageSpatialRMIRC(const double & aDist);
           void  RechHomPtsDense(cResulMultiImRechCorrel<double> &);
           void SetPtsSelect(const Pt2dr & aP);
           void SetNumSelectImage(const std::vector<int> & aNum);
           bool HasPtSelecTri() const;
           const Pt2dr & PtsSelectTri() const;
           bool NumImageIsSelect(const int aNum) const;

           void PutInGlobCoord(cResulMultiImRechCorrel<double> & aRMIRC);

            const std::string &  KeyMasqIm() const;
            void SetMasqIm(const  std::string  & aKeyMasqIm);

      private  :
         cAppliTieTri(const cAppliTieTri &); // N.I.
         void DoOneTri  (const cXml_Triangle3DForTieP & ,int aKT);


         cInterfChantierNameManipulateur * mICNM;
         std::string                       mDir;
         std::string                       mOri;
         cImMasterTieTri *                 mMasIm;
         std::vector<cImSecTieTri *>       mImSec;
         std::vector<cImSecTieTri *>       mImSecLoaded;
         Pt2di                             mSzW;
         int                               mZoomW;
         bool                              mWithW;

         double                            mDisExtrema;
         double                            mDistRechHom;

         // Les voisins pour savoir si un point est un extrema local, ne contient
         // pas le point central (0,0)
         std::vector<Pt2di>                mVoisExtr;
         // Les voisins pour rechercher les homologues une certaine distance
         std::vector<Pt2di>                mVoisHom;
         bool                              mDebug;
         int                               mNivInterac;
         // Le plan du triangle courant
         cElPlan3D                         mCurPlan;
         // Les interpolateurs
         tInterpolTiepTri *                mInterpolSinC;
         tInterpolTiepTri *                mInterpolBicub;
         tInterpolTiepTri *                mInterpolBilin;

         std::vector<cResulMultiImRechCorrel<double>*> mVCurMIRMC;
         std::vector<cOneTriMultiImRechCorrel>         mVGlobMIRMC;

         int       mNbTriLoaded;
         int       mNbPts;
         double    mTimeCorInit;
         double    mTimeCorDense;



         bool               mHasPtSelecTri;
         Pt2dr              mPtsSelectTri;
         bool               mHasNumSelectImage;
         std::vector<int>   mNumSelectImage;

         std::string        mKeyMasqIm;
};

/*
   cIntTieTriInterest : point d'interet = Local + Type (Max,Min ...) + Qualite de contraste (Fast)
   

*/
 
typedef enum eTypeTieTri
{
    eTTTNoLabel = 0,
    eTTTMax = 1,
    eTTTMin = 2
}  eTypeTieTri;



class cIntTieTriInterest
{
    public :
       cIntTieTriInterest(const Pt2di & aP,eTypeTieTri aType,const double & aFastQual);

       Pt2di        mPt;
       eTypeTieTri  mType;
       double       mFastQual;
       bool         mSelected;
       
};

/*
class cLinkImTT
{
      public :
         cImTieTri * mIm1;
         cImTieTri * mIm2;
         bool        mLnkActif;
      private :
};
*/


class cImTieTri
{
      public :
            friend class cImMasterTieTri;
            friend class cImSecTieTri;

           cImTieTri(cAppliTieTri & ,const std::string& aNameIm,int aNum);
           Video_Win *        W();
           virtual bool IsMaster() const = 0;
           virtual tTImTiepTri & ImRedr() = 0; // C'est l'image init pour Mastre et Redr sinon

           const Pt2di  &   Decal() const;
           const int & Num() const;
           string NameIm() {return mNameIm;}
           bool AutoCorrel(Pt2di aP);
           
      protected :
           cImTieTri(const cImTieTri &) ; // N.I.
           int  IsExtrema(const TIm2D<tElTiepTri,tElTiepTri> &,Pt2di aP);
           void MakeInterestPoint
                (
                     std::list<cIntTieTriInterest> *,
                     TIm2D<U_INT1,INT>  *,
                     const TIm2DBits<1> & aMasq,const TIm2D<tElTiepTri,tElTiepTri> &
                );
           void  MakeInterestPointFAST
                 (
                      std::list<cIntTieTriInterest> *,
                      TIm2D<U_INT1,INT>  *,
                      const TIm2DBits<1> & aMasq,const TIm2D<tElTiepTri,tElTiepTri> &
                 );

           bool LoadTri(const cXml_Triangle3DForTieP & );

           Col_Pal  ColOfType(eTypeTieTri);

           cAppliTieTri & mAppli;
           std::string    mNameIm;
           Tiff_Im        mTif;
           cBasicGeomCap3D *   mCamGen;
           CamStenope *        mCamS;
           Pt2dr          mP1Glob;
           Pt2dr          mP2Glob;
           Pt2dr          mP3Glob;
           std::vector<Pt2dr> mVTriGlob;

           Pt2dr          mP1Loc;
           Pt2dr          mP2Loc;
           Pt2dr          mP3Loc;
 
           Pt2di          mDecal;
           Pt2di          mSzIm;

           tImTiepTri                    mImInit;
           tTImTiepTri                   mTImInit;

           Im2D_Bits<1>                  mMasqTri;
           TIm2DBits<1>                  mTMasqTri;
           Im2D_Bits<1>                  mMasqIm;
           TIm2DBits<1>                  mTMasqIm;

           int                           mRab;
           Video_Win *                   mW;
           int                           mNum;
           cFastCriterCompute *          mFastCC;
           cCutAutoCorrelDir<tTImTiepTri> mCutACD;
           bool mLoaded;
};

class cImMasterTieTri : public cImTieTri
{
    public :
           cImMasterTieTri(cAppliTieTri & ,const std::string& aNameIm);
           bool LoadTri(const cXml_Triangle3DForTieP & );

           cIntTieTriInterest  GetPtsInteret();
           virtual bool IsMaster() const ;
           virtual tTImTiepTri & ImRedr();
           const std::list<cIntTieTriInterest> & LIP() const;


    private :
           cImMasterTieTri(const cImMasterTieTri&) ; // N.I.
           std::list<cIntTieTriInterest> mLIP;
           
};

class cImSecTieTri : public cImTieTri
{
    public :
           cImSecTieTri(cAppliTieTri & ,const std::string& aNameIm,int aNum);
           bool LoadTri(const cXml_Triangle3DForTieP & );

            cResulRechCorrel<double>  RechHomPtsInteretBilin(const cIntTieTriInterest & aP,int aNivInterac);
            cResulRechCorrel<double>  RechHomPtsDense(const Pt2di & aP0,const cResulRechCorrel<double> & aPIn);

           virtual bool IsMaster() const ;
           virtual tTImTiepTri & ImRedr();
           ElPackHomologue & PackH() ;
    private :
           bool InMasqReech(const Pt2dr &) const;
           bool InMasqReech(const Pt2di &) const;

           cImSecTieTri(const cImSecTieTri&); // N.I.
           void  DecomposeVecHom(const Pt2dr & aPSH1,const Pt2dr & aPSH2,Pt2dr & aDirProf,Pt2dr & aNewCoord);

           tImTiepTri                    mImReech;
           tTImTiepTri                   mTImReech;
           Im2D<U_INT1,INT>              mImLabelPC;
           TIm2D<U_INT1,INT>             mTImLabelPC;


           Im2D_Bits<1>                  mMasqReech;
           TIm2DBits<1>                  mTMasqReech;

           Pt2di                         mSzReech;
           ElAffin2D                     mAffMas2Sec;
           ElAffin2D                     mAffSec2Mas;
           cImMasterTieTri *             mMaster;
           ElPackHomologue               mPackH;
};

//  ====================================  Correlation ==========================

class cLSQAffineMatch
{
    public :
        cLSQAffineMatch
        (
            Pt2dr              aPC1,
            const tImTiepTri & aI1,
            const tImTiepTri & aI2,
            ElAffin2D          anAf1To2
        );

        bool OneIter(tInterpolTiepTri *,int aNbW,double aStep,bool AffineGeom,bool AffineRadiom);
        const ElAffin2D &    Af1To2() const;

    private :
        void CalcRect(tInterpolTiepTri *,double aStepTop);
        void AddEqq(L2SysSurResol & aSys,const Pt2dr &PIm1,const Pt2dr & aPC1);


        Pt2dr         mPC1;
        Pt2dr         mPInfIm1;
        Pt2dr         mPSupIm1;
        Pt2dr         mPInfIm2;
        Pt2dr         mPSupIm2;
        tTImTiepTri   mTI1;
        tElTiepTri**  mData1;
        tTImTiepTri   mTI2;
        tElTiepTri**  mData2;
        ElAffin2D     mAf1To2;
        double        mA;
        double        mB;
        bool          mAffineGeom;
        bool          mAffineRadiom;
        double        mCoeff[10];
        int           NumAB;
        int           NumTr;
        int           NumAffGeom;
        int           NumAfRad;
        tInterpolTiepTri * mInterp;
        double        mSomDiff;

};


// inline const double & MyDeCorrel() {static double aR=-2.0; return aR;}


template<class Type> class cResulRechCorrel
{
     public :
          cResulRechCorrel(const Pt2d<Type>& aPt,double aCorrel)  :
              mPt     (aPt),
              mCorrel (aCorrel)
          {
          }
          bool IsInit() const {return mCorrel > TT_DefCorrel;}

          cResulRechCorrel() :
              mPt     (0,0),
              mCorrel (TT_DefCorrel)
          {
          }

          void Merge(const cResulRechCorrel & aRRC)
          {
              if (aRRC.mCorrel > mCorrel)
              {
                    // mCorrel = aRRC.mCorrel;
                    // mPt     =  aRRC.mPt;
                  *this = aRRC;
              }
          }

          Pt2d<Type>  mPt;
          double      mCorrel;

};

template<class Type> class cResulMultiImRechCorrel
{
    public :
         cResulMultiImRechCorrel(const cIntTieTriInterest & aPMaster) :
                mPMaster (aPMaster),
                mScore   (TT_MaxCorrel),
                mAllInit  (true)
          {
          }

          double square_dist(const cResulMultiImRechCorrel & aR2) const
          {
               return square_euclid(mPMaster.mPt,aR2.mPMaster.mPt);
          }
          void AddResul(const cResulRechCorrel<double> aRRC,int aNumIm)
          {
              if (aRRC.IsInit())
              {
                  mScore = ElMin(mScore,aRRC.mCorrel);
                  mVRRC.push_back(aRRC);
                  mVIndex.push_back(aNumIm);
              }
              else
              {
                   mAllInit = false;
              }
          }
          bool AllInit() const  {return mAllInit ;}
          bool IsInit() const  {return mAllInit && (mVRRC.size() !=0) ;}
          double Score() const {return mScore;}
          const std::vector<cResulRechCorrel<double> > & VRRC() const {return mVRRC;}
          std::vector<cResulRechCorrel<double> > & VRRC() {return mVRRC;}
          const cIntTieTriInterest & PMaster() const {return  mPMaster;}
          cIntTieTriInterest & PMaster() {return  mPMaster;}
          const std::vector<int> &                              VIndex()   const {return  mVIndex;}
    private :

         cResulMultiImRechCorrel(const cResulMultiImRechCorrel<Type> & ) ; // N.I.
        
         cIntTieTriInterest                     mPMaster;
         double                                 mScore;
         bool                                   mAllInit;
         std::vector<cResulRechCorrel<double> > mVRRC;
         std::vector<int>                       mVIndex;
};


class cOneTriMultiImRechCorrel
{
    public :
       cOneTriMultiImRechCorrel(int aKT,const std::vector<cResulMultiImRechCorrel<double>*> & aVMultiC) :
           mKT      (aKT),
           mVMultiC (aVMultiC)
       {
       }
       const std::vector<cResulMultiImRechCorrel<double>*>&  VMultiC() const {return  mVMultiC;}
       const int  &  KT()   const {return  mKT;}
    private :

         // cOneTriMultiImRechCorrel(const cOneTriMultiImRechCorrel &); // N.I.
        
        int mKT;
        std::vector<cResulMultiImRechCorrel<double>*>  mVMultiC;
};



Pt2dr TT_CorrelBasique
                             (
                                const tTImTiepTri & Im1,
                                const Pt2di & aP1,
                                const tTImTiepTri & Im2,
                                const Pt2di & aP2,
                                const int   aSzW,
                                const int   aStep
                             );

cResulRechCorrel<int> TT_RechMaxCorrelBasique
                      (
                             const tTImTiepTri & Im1,
                             const Pt2di & aP1,
                             const tTImTiepTri & Im2,
                             const Pt2di & aP2,
                             const int   aSzW,
                             const int   aStep,
                             const int   aSzRech
                      );


Pt2dr TT_CorrelBilin
       (
               const tTImTiepTri & Im1,
               const Pt2di & aP1,
               const tTImTiepTri & Im2,
               const Pt2dr & aP2,
               const int   aSzW
       );

cResulRechCorrel<int> TT_RechMaxCorrelLocale
                      (
                             const tTImTiepTri & aIm1,
                             const Pt2di & aP1,
                             const tTImTiepTri & aIm2,
                             const Pt2di & aP2,
                             const int   aSzW,
                             const int   aStep,
                             const int   aSzRechMax
                      );

cResulRechCorrel<double> TT_RechMaxCorrelMultiScaleBilin
                      (
                             const tTImTiepTri & aIm1,
                             const Pt2dr & aP1,
                             const tTImTiepTri & aIm2,
                             const Pt2dr & aP2,
                             const int   aSzW
                      );

cResulRechCorrel<double> TT_MaxLocCorrelDS1R
                         (
                              tInterpolTiepTri *  anInterpol,
                              cElMap2D *          aMap,
                              const tTImTiepTri & aIm1,
                              Pt2dr               aPC1,
                              const tTImTiepTri & aIm2,
                              Pt2dr               aPC2,
                              const int           aSzW,
                              const int           aNbByPix,
                              double              aStep0,
                              double              aStepEnd
                         );

//  ====================================  cHomolPackTiepTri ==========================

class cHomolPackTiepTri
{
    public:
        cHomolPackTiepTri (std::string img1, std::string img2, int index, cInterfChantierNameManipulateur * aICNM, bool skipPackVide);
        void writeToDisk(std::string aHomolOut);
        ElPackHomologue & Pack() {return mPack;}
        std::string & Img1() {return mImg1;}
        std::string & Img2() {return mImg2;}
    private:
        std::string mImg1;
        std::string mImg2;
        int mIndex;
        cInterfChantierNameManipulateur * mICNM;
        ElPackHomologue mPack;
        bool mSkipVide;
};


class cCmpInterOnFast
{
    public :
       bool operator () (const cIntTieTriInterest & aI1,const cIntTieTriInterest &aI2)
       {
             return aI1.mFastQual > aI2.mFastQual;
       }
};





#endif //  _TiePTri_


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
aooter-MicMac-eLiSe-25/06/2007*/
