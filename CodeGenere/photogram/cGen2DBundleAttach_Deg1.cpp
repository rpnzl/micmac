// File Automatically generated by eLiSe
#include "StdAfx.h"
#include "cGen2DBundleAttach_Deg1.h"


cGen2DBundleAttach_Deg1::cGen2DBundleAttach_Deg1():
    cElCompiledFonc(2)
{
   AddIntRef (cIncIntervale("CX",0,3));
   AddIntRef (cIncIntervale("CY",3,6));
   Close(false);
}



void cGen2DBundleAttach_Deg1::ComputeVal()
{
   double tmp0_ = mLocPFixV_y-mLocCentrFixV_y;
   double tmp1_ = (tmp0_)/mLocAmplFixV;
   double tmp2_ = mLocPFixV_x-mLocCentrFixV_x;
   double tmp3_ = (tmp2_)/mLocAmplFixV;

  mVal[0] = (mCompCoord[0]+mCompCoord[1]*(tmp1_)+mCompCoord[2]*(tmp3_))-mLocFixedV_x;

  mVal[1] = (mCompCoord[3]+mCompCoord[4]*(tmp1_)+mCompCoord[5]*(tmp3_))-mLocFixedV_y;

}


void cGen2DBundleAttach_Deg1::ComputeValDeriv()
{
   double tmp0_ = mLocPFixV_y-mLocCentrFixV_y;
   double tmp1_ = (tmp0_)/mLocAmplFixV;
   double tmp2_ = mLocPFixV_x-mLocCentrFixV_x;
   double tmp3_ = (tmp2_)/mLocAmplFixV;

  mVal[0] = (mCompCoord[0]+mCompCoord[1]*(tmp1_)+mCompCoord[2]*(tmp3_))-mLocFixedV_x;

  mCompDer[0][0] = 1;
  mCompDer[0][1] = tmp1_;
  mCompDer[0][2] = tmp3_;
  mCompDer[0][3] = 0;
  mCompDer[0][4] = 0;
  mCompDer[0][5] = 0;
  mVal[1] = (mCompCoord[3]+mCompCoord[4]*(tmp1_)+mCompCoord[5]*(tmp3_))-mLocFixedV_y;

  mCompDer[1][0] = 0;
  mCompDer[1][1] = 0;
  mCompDer[1][2] = 0;
  mCompDer[1][3] = 1;
  mCompDer[1][4] = tmp1_;
  mCompDer[1][5] = tmp3_;
}


void cGen2DBundleAttach_Deg1::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cGen2DBundleAttach_Deg1 Has no Der Sec");
}

void cGen2DBundleAttach_Deg1::SetAmplFixV(double aVal){ mLocAmplFixV = aVal;}
void cGen2DBundleAttach_Deg1::SetCentrFixV_x(double aVal){ mLocCentrFixV_x = aVal;}
void cGen2DBundleAttach_Deg1::SetCentrFixV_y(double aVal){ mLocCentrFixV_y = aVal;}
void cGen2DBundleAttach_Deg1::SetFixedV_x(double aVal){ mLocFixedV_x = aVal;}
void cGen2DBundleAttach_Deg1::SetFixedV_y(double aVal){ mLocFixedV_y = aVal;}
void cGen2DBundleAttach_Deg1::SetPFixV_x(double aVal){ mLocPFixV_x = aVal;}
void cGen2DBundleAttach_Deg1::SetPFixV_y(double aVal){ mLocPFixV_y = aVal;}



double * cGen2DBundleAttach_Deg1::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "AmplFixV") return & mLocAmplFixV;
   if (aName == "CentrFixV_x") return & mLocCentrFixV_x;
   if (aName == "CentrFixV_y") return & mLocCentrFixV_y;
   if (aName == "FixedV_x") return & mLocFixedV_x;
   if (aName == "FixedV_y") return & mLocFixedV_y;
   if (aName == "PFixV_x") return & mLocPFixV_x;
   if (aName == "PFixV_y") return & mLocPFixV_y;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cGen2DBundleAttach_Deg1::mTheAuto("cGen2DBundleAttach_Deg1",cGen2DBundleAttach_Deg1::Alloc);


cElCompiledFonc *  cGen2DBundleAttach_Deg1::Alloc()
{  return new cGen2DBundleAttach_Deg1();
}

