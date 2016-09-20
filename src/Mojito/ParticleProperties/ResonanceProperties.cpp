// author: Philippe d'Argent (p.dargent@cern.ch)
#include "Mint/ResonanceProperties.h"
#include "Mint/CLHEPPhysicalConstants.h"
#include "Mint/CLHEPSystemOfUnits.h"
#include "Mint/NamedParameter.h"
#include "Mint/Utils.h"
#include "Mint/FitParameter.h"
#include "Mint/ParticlePropertiesList.h"
#include "Mint/ParticleProperties.h"

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
using namespace MINT;

std::string ResonanceProperties::nameFromPid(int pdg_id){
  bool dbThis = false;  
  const ParticlePropertiesList* PPL = ParticlePropertiesList::getMe();
  const ParticleProperties* pp = PPL->get(pdg_id);
  if(0 == pp){
    cout << "ResonanceProperties::nameFromPid error!" << endl;
    cout << "Can't find ParticleProperties for pdg_id = " << pdg_id << endl;
    return "fuck!";
  }
  if(dbThis){
        cout << "got name from PID: " << endl;
        cout << PPL->get(pdg_id)->name() << endl;
  }
  return PPL->get(pdg_id)->name();
}

int ResonanceProperties::pidFromName(const std::string& name){
  const ParticlePropertiesList* PPL = ParticlePropertiesList::getMe();
  const ParticleProperties* pp = PPL->get(name);
  if(0 == pp){
    cout << "ResonanceProperties::nameFromPid error!" << endl;
    cout << "Can't find ParticleProperties for name = " << name << endl;
    throw "ResonanceProperties::pidFromName: cannot find name";
  }
  return pp->pdg_id();
}

std::string ResonanceProperties::nameFromName(const std::string& name){
  return nameFromPid(abs(pidFromName(name)));
}

double ResonanceProperties::massInit(int pdg_id){
  return ParticlePropertiesList::getMe()->get(pdg_id)->mass();
}
double ResonanceProperties::widthInit(int pdg_id){
  return ParticlePropertiesList::getMe()->get(pdg_id)->width();
}
double ResonanceProperties::radiusInit(int pdg_id){
  return ParticlePropertiesList::getMe()->get(pdg_id)->radius();
}

std::string ResonanceProperties::makeFitParName(const std::string& propertyName, int pdg_id) const{
  /*
  cout << "ResonanceProperties::makeFitParName "
       << "making name: "
       << prefix() + propertyName + nameFromPid(abs(pdg_id))
       << endl;
  */
  return prefix() + propertyName + nameFromPid(abs(pdg_id));
}

ResonanceProperties::ResonanceProperties(int pdg_id, MinuitParameterSet* mps, const std::string& namePrefix)
  : _prefix(namePrefix)
  , _pid(abs(pdg_id))
  , _fitMass(makeFitParName("mass_", pdg_id), 1, massInit(pdg_id)  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitWidth(makeFitParName("width_", pdg_id), 1, widthInit(pdg_id) , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRadius(makeFitParName("radius_", pdg_id), 1, radiusInit(pdg_id), 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitAlpha(makeFitParName("NonRes::alpha_", pdg_id), 1, 1.0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc1(makeFitParName("NonRes::c1_", pdg_id), 1, .0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc2(makeFitParName("NonRes::c2_", pdg_id), 1, .0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc3(makeFitParName("NonRes::c3_", pdg_id), 1, .0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc4(makeFitParName("NonRes::c4_", pdg_id), 1, .0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc5(makeFitParName("NonRes::c5_", pdg_id), 1, .0               , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFermiLambda(makeFitParName("FermiPS::lambda_", pdg_id), 1, 1.0  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFermiS0(makeFitParName("FermiPS::s0_", pdg_id), 1, 1.0          , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRhoOmegaDelta_Re(makeFitParName("RhoOmegaGS::Delta_Re_", pdg_id), 1, 0.00153  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRhoOmegaDelta_Im(makeFitParName("RhoOmegaGS::Delta_Im_", pdg_id ), 1, 0.00034  , 0, 0, 0, mps,  NamedParameterBase::QUIET)

  , _fitFlatte_gPi(makeFitParName("Flatte::gPi_", pdg_id), 1, 0.165*GeV , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFlatte_gK_by_gPi(makeFitParName("Flatte::gK_by_gPi_", pdg_id), 1, 4.21 , 0, 0, 0, mps,  NamedParameterBase::QUIET)

  , _fitBugg_M(makeFitParName("Bugg::M_", pdg_id), 1, 0.935  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_b1(makeFitParName("Bugg::b1_", pdg_id), 1, 1.302  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_b2(makeFitParName("Bugg::b2_", pdg_id), 1, 0.340  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_A(makeFitParName("Bugg::A_", pdg_id), 1, 2.426  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_4pi(makeFitParName("Bugg::g_4pi_", pdg_id), 1, 0.011  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_KK(makeFitParName("Bugg::g_KK_", pdg_id), 1, 0.6  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_etaeta(makeFitParName("Bugg::g_etaeta_", pdg_id), 1, 0.2  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_alpha(makeFitParName("Bugg::alpha_", pdg_id), 1, 1.3  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_sA(makeFitParName("Bugg::sA_", pdg_id), 1, 0.41 , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_4pi_s0(makeFitParName("Bugg::4pi_s0_", pdg_id), 1, 7.082/2.845  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_4pi_lambda(makeFitParName("Bugg::4pi_lambda_", pdg_id), 1, 2.845  , 0, 0, 0, mps,  NamedParameterBase::QUIET)

  , _fitModel_independent_Bin_1_Re(makeFitParName("MI::", pdg_id)+"_Bin_1_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_1_Im(makeFitParName("MI::", pdg_id)+"_Bin_1_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_2_Re(makeFitParName("MI::", pdg_id)+"_Bin_2_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_2_Im(makeFitParName("MI::", pdg_id)+"_Bin_2_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_3_Re(makeFitParName("MI::", pdg_id)+"_Bin_3_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_3_Im(makeFitParName("MI::", pdg_id)+"_Bin_3_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_4_Re(makeFitParName("MI::", pdg_id)+"_Bin_4_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_4_Im(makeFitParName("MI::", pdg_id)+"_Bin_4_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_5_Re(makeFitParName("MI::", pdg_id)+"_Bin_5_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_5_Im(makeFitParName("MI::", pdg_id)+"_Bin_5_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_6_Re(makeFitParName("MI::", pdg_id)+"_Bin_6_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_6_Im(makeFitParName("MI::", pdg_id)+"_Bin_6_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_7_Re(makeFitParName("MI::", pdg_id)+"_Bin_7_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_7_Im(makeFitParName("MI::", pdg_id)+"_Bin_7_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_8_Re(makeFitParName("MI::", pdg_id)+"_Bin_8_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_8_Im(makeFitParName("MI::", pdg_id)+"_Bin_8_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_9_Re(makeFitParName("MI::", pdg_id)+"_Bin_9_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_9_Im(makeFitParName("MI::", pdg_id)+"_Bin_9_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_10_Re(makeFitParName("MI::", pdg_id)+"_Bin_10_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_10_Im(makeFitParName("MI::", pdg_id)+"_Bin_10_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
{ 

        //fix to PDG value if not explicitly initiallised 
  //        const ParticlePropertiesList* PPL = ParticlePropertiesList::getMe();
  //      if(_fitMass.iFixInit() && _fitMass== 0)      _fitMass   = PPL->mass(pdg_id);
  //      if(_fitWidth.iFixInit() && _fitWidth== 0)    _fitWidth  = PPL->width(pdg_id);    
  //      if(_fitRadius.iFixInit() && _fitRadius== 0)  _fitRadius = PPL->get(pdg_id)->radius();   
        //std:: cout << _fitMass << endl;
        //std:: cout << _fitWidth << endl;
 }

ResonanceProperties::ResonanceProperties(const std::string& name, MinuitParameterSet* mps, const std::string& namePrefix)
  : _prefix(namePrefix)
  , _pid(abs(pidFromName(name)))
  , _fitMass(makeFitParName("mass_", _pid), 1, massInit(_pid)  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitWidth(makeFitParName("width_", _pid), 1, widthInit(_pid) , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRadius(makeFitParName("radius_", _pid), 1, radiusInit(_pid), 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitAlpha(makeFitParName("NonRes::alpha_", _pid), 1, 1.0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc1(makeFitParName("NonRes::c1_", _pid), 1, .0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc2(makeFitParName("NonRes::c2_", _pid), 1, .0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc3(makeFitParName("NonRes::c3_", _pid), 1, .0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc4(makeFitParName("NonRes::c4_", _pid), 1, .0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitc5(makeFitParName("NonRes::c5_" , _pid), 1, .0             , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFermiLambda(makeFitParName("FermiPS::lambda_", _pid), 1, 1.0  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFermiS0(makeFitParName("FermiPS::s0_", _pid), 1, 1.0          , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRhoOmegaDelta_Re(makeFitParName("RhoOmegaGS::Delta_Re_", _pid), 1, 0.00153  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitRhoOmegaDelta_Im(makeFitParName("RhoOmegaGS::Delta_Im_" , _pid), 1, 0.00034  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFlatte_gPi(makeFitParName("Flatte::gPi_", _pid), 1, 0.165*GeV , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitFlatte_gK_by_gPi(makeFitParName("Flatte::gK_by_gPi_", _pid), 1, 4.21 , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_M(makeFitParName("Bugg::M_", _pid), 1, 0.935  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_b1(makeFitParName("Bugg::b1_", _pid), 1, 1.302  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_b2(makeFitParName("Bugg::b2_", _pid), 1, 0.340  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_A(makeFitParName("Bugg::A_", _pid), 1, 2.426  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_4pi(makeFitParName("Bugg::g_4pi_", _pid), 1, 0.011  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_KK(makeFitParName("Bugg::g_KK_", _pid), 1, 0.6  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_g_etaeta(makeFitParName("Bugg::g_etaeta_", _pid), 1, 0.2  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_alpha(makeFitParName("Bugg::alpha_", _pid), 1, 1.3  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_sA(makeFitParName("Bugg::sA_", _pid), 1, 0.41 , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_4pi_s0(makeFitParName("Bugg::4pi_s0_", _pid), 1, 7.082/2.845  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitBugg_4pi_lambda(makeFitParName("Bugg::4pi_lambda_", _pid), 1, 2.845  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_1_Re(makeFitParName("MI::", _pid)+"_Bin_1_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_1_Im(makeFitParName("MI::", _pid)+"_Bin_1_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_2_Re(makeFitParName("MI::", _pid)+"_Bin_2_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_2_Im(makeFitParName("MI::", _pid)+"_Bin_2_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_3_Re(makeFitParName("MI::", _pid)+"_Bin_3_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_3_Im(makeFitParName("MI::", _pid)+"_Bin_3_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_4_Re(makeFitParName("MI::", _pid)+"_Bin_4_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_4_Im(makeFitParName("MI::", _pid)+"_Bin_4_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_5_Re(makeFitParName("MI::", _pid)+"_Bin_5_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_5_Im(makeFitParName("MI::", _pid)+"_Bin_5_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_6_Re(makeFitParName("MI::", _pid)+"_Bin_6_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_6_Im(makeFitParName("MI::", _pid)+"_Bin_6_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_7_Re(makeFitParName("MI::", _pid)+"_Bin_7_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_7_Im(makeFitParName("MI::", _pid)+"_Bin_7_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_8_Re(makeFitParName("MI::", _pid)+"_Bin_8_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_8_Im(makeFitParName("MI::", _pid)+"_Bin_8_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_9_Re(makeFitParName("MI::", _pid)+"_Bin_9_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_9_Im(makeFitParName("MI::", _pid)+"_Bin_9_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_10_Re(makeFitParName("MI::", _pid)+"_Bin_10_Re", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
  , _fitModel_independent_Bin_10_Im(makeFitParName("MI::", _pid)+"_Bin_10_Im", 1, 0.  , 0, 0, 0, mps,  NamedParameterBase::QUIET)
{ 
}



//
