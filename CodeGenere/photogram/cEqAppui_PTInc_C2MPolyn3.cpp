// File Automatically generated by eLiSe
#include "general/all.h"
#include "private/all.h"
#include "cEqAppui_PTInc_C2MPolyn3.h"


cEqAppui_PTInc_C2MPolyn3::cEqAppui_PTInc_C2MPolyn3():
    cElCompiledFonc(2)
{
   AddIntRef (cIncIntervale("Intr",0,17));
   AddIntRef (cIncIntervale("Orient",17,23));
   AddIntRef (cIncIntervale("Tmp_PTer",23,26));
   Close(false);
}



void cEqAppui_PTInc_C2MPolyn3::ComputeVal()
{
   double tmp0_ = mCompCoord[17];
   double tmp1_ = mCompCoord[18];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = sin(tmp0_);
   double tmp4_ = cos(tmp0_);
   double tmp5_ = sin(tmp1_);
   double tmp6_ = mCompCoord[19];
   double tmp7_ = mCompCoord[23];
   double tmp8_ = mCompCoord[20];
   double tmp9_ = tmp7_-tmp8_;
   double tmp10_ = sin(tmp6_);
   double tmp11_ = -(tmp10_);
   double tmp12_ = -(tmp5_);
   double tmp13_ = cos(tmp6_);
   double tmp14_ = mCompCoord[24];
   double tmp15_ = mCompCoord[21];
   double tmp16_ = tmp14_-tmp15_;
   double tmp17_ = mCompCoord[25];
   double tmp18_ = mCompCoord[22];
   double tmp19_ = tmp17_-tmp18_;
   double tmp20_ = mLocXIm/mLocPolyn3_State_0_0;
   double tmp21_ = mLocYIm/mLocPolyn3_State_0_0;
   double tmp22_ = (tmp20_)*(tmp20_);
   double tmp23_ = (tmp21_)*(tmp21_);
   double tmp24_ = mCompCoord[0];
   double tmp25_ = -(tmp3_);
   double tmp26_ = tmp4_*tmp12_;
   double tmp27_ = tmp3_*tmp12_;
   double tmp28_ = tmp25_*tmp11_;
   double tmp29_ = tmp26_*tmp13_;
   double tmp30_ = tmp28_+tmp29_;
   double tmp31_ = (tmp30_)*(tmp9_);
   double tmp32_ = tmp4_*tmp11_;
   double tmp33_ = tmp27_*tmp13_;
   double tmp34_ = tmp32_+tmp33_;
   double tmp35_ = (tmp34_)*(tmp16_);
   double tmp36_ = tmp31_+tmp35_;
   double tmp37_ = tmp2_*tmp13_;
   double tmp38_ = tmp37_*(tmp19_);
   double tmp39_ = tmp36_+tmp38_;
   double tmp40_ = mCompCoord[3];
   double tmp41_ = mCompCoord[4];
   double tmp42_ = mCompCoord[5];
   double tmp43_ = (tmp20_)*(tmp21_);
   double tmp44_ = mCompCoord[6];
   double tmp45_ = tmp22_*(tmp20_);
   double tmp46_ = (tmp21_)*(tmp20_);
   double tmp47_ = tmp46_*(tmp20_);
   double tmp48_ = tmp23_*(tmp20_);
   double tmp49_ = (tmp21_)*tmp23_;

  mVal[0] = (mCompCoord[1]+tmp24_*((tmp4_*tmp2_*(tmp9_)+tmp3_*tmp2_*(tmp16_)+tmp5_*(tmp19_))/(tmp39_)))-((((1+tmp40_)*(tmp20_)+tmp41_*(tmp21_))-tmp42_*2*tmp22_+tmp44_*tmp43_+mCompCoord[7]*tmp23_)*mLocPolyn3_State_0_0+(mCompCoord[9]*tmp45_+mCompCoord[10]*tmp47_+mCompCoord[11]*tmp48_+mCompCoord[12]*tmp49_)*mLocPolyn3_State_0_0);

  mVal[1] = (mCompCoord[2]+tmp24_*(((tmp25_*tmp13_+tmp26_*tmp10_)*(tmp9_)+(tmp4_*tmp13_+tmp27_*tmp10_)*(tmp16_)+tmp2_*tmp10_*(tmp19_))/(tmp39_)))-((((1-tmp40_)*(tmp21_)+tmp41_*(tmp20_)+tmp42_*tmp43_)-tmp44_*2*tmp23_+mCompCoord[8]*tmp22_)*mLocPolyn3_State_0_0+(mCompCoord[13]*tmp45_+mCompCoord[14]*tmp47_+mCompCoord[15]*tmp48_+mCompCoord[16]*tmp49_)*mLocPolyn3_State_0_0);

}


void cEqAppui_PTInc_C2MPolyn3::ComputeValDeriv()
{
   double tmp0_ = mCompCoord[17];
   double tmp1_ = mCompCoord[18];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = sin(tmp0_);
   double tmp4_ = cos(tmp0_);
   double tmp5_ = sin(tmp1_);
   double tmp6_ = mCompCoord[19];
   double tmp7_ = mCompCoord[23];
   double tmp8_ = mCompCoord[20];
   double tmp9_ = tmp7_-tmp8_;
   double tmp10_ = sin(tmp6_);
   double tmp11_ = -(tmp10_);
   double tmp12_ = -(tmp5_);
   double tmp13_ = cos(tmp6_);
   double tmp14_ = mCompCoord[24];
   double tmp15_ = mCompCoord[21];
   double tmp16_ = tmp14_-tmp15_;
   double tmp17_ = mCompCoord[25];
   double tmp18_ = mCompCoord[22];
   double tmp19_ = tmp17_-tmp18_;
   double tmp20_ = mLocXIm/mLocPolyn3_State_0_0;
   double tmp21_ = mLocYIm/mLocPolyn3_State_0_0;
   double tmp22_ = (tmp20_)*(tmp20_);
   double tmp23_ = (tmp21_)*(tmp21_);
   double tmp24_ = tmp4_*tmp2_;
   double tmp25_ = tmp24_*(tmp9_);
   double tmp26_ = tmp3_*tmp2_;
   double tmp27_ = tmp26_*(tmp16_);
   double tmp28_ = tmp25_+tmp27_;
   double tmp29_ = tmp5_*(tmp19_);
   double tmp30_ = tmp28_+tmp29_;
   double tmp31_ = -(tmp3_);
   double tmp32_ = tmp31_*tmp11_;
   double tmp33_ = tmp4_*tmp12_;
   double tmp34_ = tmp33_*tmp13_;
   double tmp35_ = tmp32_+tmp34_;
   double tmp36_ = (tmp35_)*(tmp9_);
   double tmp37_ = tmp4_*tmp11_;
   double tmp38_ = tmp3_*tmp12_;
   double tmp39_ = tmp38_*tmp13_;
   double tmp40_ = tmp37_+tmp39_;
   double tmp41_ = (tmp40_)*(tmp16_);
   double tmp42_ = tmp36_+tmp41_;
   double tmp43_ = tmp2_*tmp13_;
   double tmp44_ = tmp43_*(tmp19_);
   double tmp45_ = tmp42_+tmp44_;
   double tmp46_ = (tmp30_)/(tmp45_);
   double tmp47_ = (tmp20_)*(tmp21_);
   double tmp48_ = tmp22_*(tmp20_);
   double tmp49_ = (tmp21_)*(tmp20_);
   double tmp50_ = tmp49_*(tmp20_);
   double tmp51_ = tmp23_*(tmp20_);
   double tmp52_ = (tmp21_)*tmp23_;
   double tmp53_ = -(1);
   double tmp54_ = tmp53_*tmp3_;
   double tmp55_ = mCompCoord[0];
   double tmp56_ = tmp53_*tmp5_;
   double tmp57_ = -(tmp2_);
   double tmp58_ = ElSquare(tmp45_);
   double tmp59_ = -(tmp13_);
   double tmp60_ = tmp53_*tmp10_;
   double tmp61_ = mCompCoord[3];
   double tmp62_ = mCompCoord[4];
   double tmp63_ = mCompCoord[5];
   double tmp64_ = mCompCoord[6];
   double tmp65_ = tmp31_*tmp13_;
   double tmp66_ = tmp33_*tmp10_;
   double tmp67_ = tmp65_+tmp66_;
   double tmp68_ = (tmp67_)*(tmp9_);
   double tmp69_ = tmp4_*tmp13_;
   double tmp70_ = tmp38_*tmp10_;
   double tmp71_ = tmp69_+tmp70_;
   double tmp72_ = (tmp71_)*(tmp16_);
   double tmp73_ = tmp68_+tmp72_;
   double tmp74_ = tmp2_*tmp10_;
   double tmp75_ = tmp74_*(tmp19_);
   double tmp76_ = tmp73_+tmp75_;
   double tmp77_ = (tmp76_)/(tmp45_);
   double tmp78_ = (tmp20_)*mLocPolyn3_State_0_0;
   double tmp79_ = -(tmp78_);
   double tmp80_ = tmp47_*mLocPolyn3_State_0_0;
   double tmp81_ = -(tmp80_);
   double tmp82_ = tmp48_*mLocPolyn3_State_0_0;
   double tmp83_ = -(tmp82_);
   double tmp84_ = tmp50_*mLocPolyn3_State_0_0;
   double tmp85_ = -(tmp84_);
   double tmp86_ = tmp51_*mLocPolyn3_State_0_0;
   double tmp87_ = -(tmp86_);
   double tmp88_ = tmp52_*mLocPolyn3_State_0_0;
   double tmp89_ = -(tmp88_);
   double tmp90_ = -(tmp4_);
   double tmp91_ = tmp54_*tmp12_;
   double tmp92_ = tmp90_*tmp11_;
   double tmp93_ = tmp91_*tmp13_;
   double tmp94_ = tmp92_+tmp93_;
   double tmp95_ = (tmp94_)*(tmp9_);
   double tmp96_ = tmp54_*tmp11_;
   double tmp97_ = tmp96_+tmp34_;
   double tmp98_ = (tmp97_)*(tmp16_);
   double tmp99_ = tmp95_+tmp98_;
   double tmp100_ = tmp57_*tmp4_;
   double tmp101_ = tmp57_*tmp3_;
   double tmp102_ = tmp100_*tmp13_;
   double tmp103_ = tmp102_*(tmp9_);
   double tmp104_ = tmp101_*tmp13_;
   double tmp105_ = tmp104_*(tmp16_);
   double tmp106_ = tmp103_+tmp105_;
   double tmp107_ = tmp56_*tmp13_;
   double tmp108_ = tmp107_*(tmp19_);
   double tmp109_ = tmp106_+tmp108_;
   double tmp110_ = tmp59_*tmp31_;
   double tmp111_ = tmp60_*tmp33_;
   double tmp112_ = tmp110_+tmp111_;
   double tmp113_ = (tmp112_)*(tmp9_);
   double tmp114_ = tmp59_*tmp4_;
   double tmp115_ = tmp60_*tmp38_;
   double tmp116_ = tmp114_+tmp115_;
   double tmp117_ = (tmp116_)*(tmp16_);
   double tmp118_ = tmp113_+tmp117_;
   double tmp119_ = tmp60_*tmp2_;
   double tmp120_ = tmp119_*(tmp19_);
   double tmp121_ = tmp118_+tmp120_;
   double tmp122_ = tmp53_*(tmp35_);
   double tmp123_ = tmp53_*(tmp40_);
   double tmp124_ = tmp53_*tmp43_;

  mVal[0] = (mCompCoord[1]+tmp55_*(tmp46_))-((((1+tmp61_)*(tmp20_)+tmp62_*(tmp21_))-tmp63_*2*tmp22_+tmp64_*tmp47_+mCompCoord[7]*tmp23_)*mLocPolyn3_State_0_0+(mCompCoord[9]*tmp48_+mCompCoord[10]*tmp50_+mCompCoord[11]*tmp51_+mCompCoord[12]*tmp52_)*mLocPolyn3_State_0_0);

  mCompDer[0][0] = tmp46_;
  mCompDer[0][1] = 1;
  mCompDer[0][2] = 0;
  mCompDer[0][3] = tmp79_;
  mCompDer[0][4] = -((tmp21_)*mLocPolyn3_State_0_0);
  mCompDer[0][5] = -(-(2*tmp22_)*mLocPolyn3_State_0_0);
  mCompDer[0][6] = tmp81_;
  mCompDer[0][7] = -(tmp23_*mLocPolyn3_State_0_0);
  mCompDer[0][8] = 0;
  mCompDer[0][9] = tmp83_;
  mCompDer[0][10] = tmp85_;
  mCompDer[0][11] = tmp87_;
  mCompDer[0][12] = tmp89_;
  mCompDer[0][13] = 0;
  mCompDer[0][14] = 0;
  mCompDer[0][15] = 0;
  mCompDer[0][16] = 0;
  mCompDer[0][17] = (((tmp54_*tmp2_*(tmp9_)+tmp24_*(tmp16_))*(tmp45_)-(tmp30_)*(tmp99_))/tmp58_)*tmp55_;
  mCompDer[0][18] = (((tmp56_*tmp4_*(tmp9_)+tmp56_*tmp3_*(tmp16_)+tmp2_*(tmp19_))*(tmp45_)-(tmp30_)*(tmp109_))/tmp58_)*tmp55_;
  mCompDer[0][19] = (-((tmp30_)*(tmp121_))/tmp58_)*tmp55_;
  mCompDer[0][20] = ((tmp53_*tmp24_*(tmp45_)-(tmp30_)*tmp122_)/tmp58_)*tmp55_;
  mCompDer[0][21] = ((tmp53_*tmp26_*(tmp45_)-(tmp30_)*tmp123_)/tmp58_)*tmp55_;
  mCompDer[0][22] = ((tmp56_*(tmp45_)-(tmp30_)*tmp124_)/tmp58_)*tmp55_;
  mCompDer[0][23] = ((tmp24_*(tmp45_)-(tmp30_)*(tmp35_))/tmp58_)*tmp55_;
  mCompDer[0][24] = ((tmp26_*(tmp45_)-(tmp30_)*(tmp40_))/tmp58_)*tmp55_;
  mCompDer[0][25] = ((tmp5_*(tmp45_)-(tmp30_)*tmp43_)/tmp58_)*tmp55_;
  mVal[1] = (mCompCoord[2]+tmp55_*(tmp77_))-((((1-tmp61_)*(tmp21_)+tmp62_*(tmp20_)+tmp63_*tmp47_)-tmp64_*2*tmp23_+mCompCoord[8]*tmp22_)*mLocPolyn3_State_0_0+(mCompCoord[13]*tmp48_+mCompCoord[14]*tmp50_+mCompCoord[15]*tmp51_+mCompCoord[16]*tmp52_)*mLocPolyn3_State_0_0);

  mCompDer[1][0] = tmp77_;
  mCompDer[1][1] = 0;
  mCompDer[1][2] = 1;
  mCompDer[1][3] = -(tmp53_*(tmp21_)*mLocPolyn3_State_0_0);
  mCompDer[1][4] = tmp79_;
  mCompDer[1][5] = tmp81_;
  mCompDer[1][6] = -(-(2*tmp23_)*mLocPolyn3_State_0_0);
  mCompDer[1][7] = 0;
  mCompDer[1][8] = -(tmp22_*mLocPolyn3_State_0_0);
  mCompDer[1][9] = 0;
  mCompDer[1][10] = 0;
  mCompDer[1][11] = 0;
  mCompDer[1][12] = 0;
  mCompDer[1][13] = tmp83_;
  mCompDer[1][14] = tmp85_;
  mCompDer[1][15] = tmp87_;
  mCompDer[1][16] = tmp89_;
  mCompDer[1][17] = ((((tmp90_*tmp13_+tmp91_*tmp10_)*(tmp9_)+(tmp54_*tmp13_+tmp66_)*(tmp16_))*(tmp45_)-(tmp76_)*(tmp99_))/tmp58_)*tmp55_;
  mCompDer[1][18] = (((tmp100_*tmp10_*(tmp9_)+tmp101_*tmp10_*(tmp16_)+tmp56_*tmp10_*(tmp19_))*(tmp45_)-(tmp76_)*(tmp109_))/tmp58_)*tmp55_;
  mCompDer[1][19] = ((((tmp60_*tmp31_+tmp13_*tmp33_)*(tmp9_)+(tmp60_*tmp4_+tmp13_*tmp38_)*(tmp16_)+tmp13_*tmp2_*(tmp19_))*(tmp45_)-(tmp76_)*(tmp121_))/tmp58_)*tmp55_;
  mCompDer[1][20] = ((tmp53_*(tmp67_)*(tmp45_)-(tmp76_)*tmp122_)/tmp58_)*tmp55_;
  mCompDer[1][21] = ((tmp53_*(tmp71_)*(tmp45_)-(tmp76_)*tmp123_)/tmp58_)*tmp55_;
  mCompDer[1][22] = ((tmp53_*tmp74_*(tmp45_)-(tmp76_)*tmp124_)/tmp58_)*tmp55_;
  mCompDer[1][23] = (((tmp67_)*(tmp45_)-(tmp76_)*(tmp35_))/tmp58_)*tmp55_;
  mCompDer[1][24] = (((tmp71_)*(tmp45_)-(tmp76_)*(tmp40_))/tmp58_)*tmp55_;
  mCompDer[1][25] = ((tmp74_*(tmp45_)-(tmp76_)*tmp43_)/tmp58_)*tmp55_;
}


void cEqAppui_PTInc_C2MPolyn3::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cEqAppui_PTInc_C2MPolyn3 Has no Der Sec");
}

void cEqAppui_PTInc_C2MPolyn3::SetPolyn3_State_0_0(double aVal){ mLocPolyn3_State_0_0 = aVal;}
void cEqAppui_PTInc_C2MPolyn3::SetXIm(double aVal){ mLocXIm = aVal;}
void cEqAppui_PTInc_C2MPolyn3::SetYIm(double aVal){ mLocYIm = aVal;}



double * cEqAppui_PTInc_C2MPolyn3::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "Polyn3_State_0_0") return & mLocPolyn3_State_0_0;
   if (aName == "XIm") return & mLocXIm;
   if (aName == "YIm") return & mLocYIm;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cEqAppui_PTInc_C2MPolyn3::mTheAuto("cEqAppui_PTInc_C2MPolyn3",cEqAppui_PTInc_C2MPolyn3::Alloc);


cElCompiledFonc *  cEqAppui_PTInc_C2MPolyn3::Alloc()
{  return new cEqAppui_PTInc_C2MPolyn3();
}


