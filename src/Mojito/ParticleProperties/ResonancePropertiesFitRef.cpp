// author: Philippe d'Argent (p.dargent@cern.ch) & Jonas Rademacker
#include "Mint/ResonancePropertiesFitRef.h"
#include "Mint/ResonanceProperties.h"

using namespace std;
using namespace MINT;

ResonancePropertiesFitRef::ResonancePropertiesFitRef(const ResonanceProperties& repo
						     , IFitParRegister* daddy)
  : FitParDependent(daddy)
  , _fitMass(repo._fitMass, this)
  , _fitWidth(repo._fitWidth, this)
  , _fitRadius(repo._fitRadius, this)
  , _fitAlpha(repo._fitAlpha, this)
  , _fitc1(repo._fitc1, this)
  , _fitc2(repo._fitc2, this)
  , _fitc3(repo._fitc3, this)
  , _fitc4(repo._fitc4, this)
  , _fitc5(repo._fitc5, this)
  , _fitFermiLambda(repo._fitFermiLambda, this)
  , _fitFermiS0(repo._fitFermiS0, this)
  , _fitRhoOmegaDelta_Re(repo._fitRhoOmegaDelta_Re, this)
  , _fitRhoOmegaDelta_Im(repo._fitRhoOmegaDelta_Im, this)
  , _fitFlatte_gPi(repo._fitFlatte_gPi,this)
  , _fitFlatte_gK_by_gPi(repo._fitFlatte_gK_by_gPi,this)
  , _fitBugg_M(repo._fitBugg_M, this)
  , _fitBugg_b1(repo._fitBugg_b1, this)
  , _fitBugg_b2(repo._fitBugg_b2, this)
  , _fitBugg_A(repo._fitBugg_A, this)
  , _fitBugg_g_4pi(repo._fitBugg_g_4pi, this)
  , _fitBugg_g_KK(repo._fitBugg_g_KK, this)
  , _fitBugg_g_etaeta(repo._fitBugg_g_etaeta, this)
  , _fitBugg_alpha(repo._fitBugg_alpha, this)
  , _fitBugg_sA(repo._fitBugg_sA, this)
  , _fitBugg_4pi_lambda(repo._fitBugg_4pi_lambda, this)
  , _fitBugg_4pi_s0(repo._fitBugg_4pi_s0, this)
  , _fitModel_independent_Bin_1_Re(repo._fitModel_independent_Bin_1_Re, this)
  , _fitModel_independent_Bin_1_Im(repo._fitModel_independent_Bin_1_Im, this)
  , _fitModel_independent_Bin_2_Re(repo._fitModel_independent_Bin_2_Re, this)
  , _fitModel_independent_Bin_2_Im(repo._fitModel_independent_Bin_2_Im, this)
  , _fitModel_independent_Bin_3_Re(repo._fitModel_independent_Bin_3_Re, this)
  , _fitModel_independent_Bin_3_Im(repo._fitModel_independent_Bin_3_Im, this)
  , _fitModel_independent_Bin_4_Re(repo._fitModel_independent_Bin_4_Re, this)
  , _fitModel_independent_Bin_4_Im(repo._fitModel_independent_Bin_4_Im, this)
  , _fitModel_independent_Bin_5_Re(repo._fitModel_independent_Bin_5_Re, this)
  , _fitModel_independent_Bin_5_Im(repo._fitModel_independent_Bin_5_Im, this)
  , _fitModel_independent_Bin_6_Re(repo._fitModel_independent_Bin_6_Re, this)
  , _fitModel_independent_Bin_6_Im(repo._fitModel_independent_Bin_6_Im, this)
  , _fitModel_independent_Bin_7_Re(repo._fitModel_independent_Bin_7_Re, this)
  , _fitModel_independent_Bin_7_Im(repo._fitModel_independent_Bin_7_Im, this)
  , _fitModel_independent_Bin_8_Re(repo._fitModel_independent_Bin_8_Re, this)
  , _fitModel_independent_Bin_8_Im(repo._fitModel_independent_Bin_8_Im, this)
  , _fitModel_independent_Bin_9_Re(repo._fitModel_independent_Bin_9_Re, this)
  , _fitModel_independent_Bin_9_Im(repo._fitModel_independent_Bin_9_Im, this)
  , _fitModel_independent_Bin_10_Re(repo._fitModel_independent_Bin_10_Re, this)
  , _fitModel_independent_Bin_10_Im(repo._fitModel_independent_Bin_10_Im, this)
{
}

//



