#ifndef RESONANCEPROPERTIES_HH
#define RESONANCEPROPERTIES_HH
// author: Philippe d'Argent (p.dargent@cern.ch)

#include <string>
#include <iostream>
#include "Mint/ParticleProperties.h"
#include "Mint/FitParameter.h"
#include "Mint/NamedParameter.h"
#include "Mint/Utils.h"
#include "Mint/MinuitParameterSet.h"

class ResonancePropertiesFitRef;

class ResonanceProperties{
  friend class ResonancePropertiesFitRef;
 protected:
  static double massInit(int pdg_id);
  static double widthInit(int pdg_id);
  static double radiusInit(int pdg_id);

  std::string _prefix;

  int _pid;
  mutable MINT::FitParameter _fitMass;
  MINT::FitParameter _fitWidth;
  MINT::FitParameter _fitRadius;
  MINT::FitParameter _fitAlpha;
  MINT::FitParameter _fitc1;
  MINT::FitParameter _fitc2;
  MINT::FitParameter _fitc3;
  MINT::FitParameter _fitc4; 
  MINT::FitParameter _fitc5;
  MINT::FitParameter _fitFermiLambda;
  MINT::FitParameter _fitFermiS0;
  MINT::FitParameter _fitRhoOmegaDelta_Re;
  MINT::FitParameter _fitRhoOmegaDelta_Im;
  MINT::FitParameter _fitFlatte_gPi;
  MINT::FitParameter _fitFlatte_gK_by_gPi;
  MINT::FitParameter _fitBugg_M;
  MINT::FitParameter _fitBugg_b1;
  MINT::FitParameter _fitBugg_b2;
  MINT::FitParameter _fitBugg_A;
  MINT::FitParameter _fitBugg_g_4pi;
  MINT::FitParameter _fitBugg_g_KK;
  MINT::FitParameter _fitBugg_g_etaeta;
  MINT::FitParameter _fitBugg_alpha;
  MINT::FitParameter _fitBugg_sA;
  MINT::FitParameter _fitBugg_4pi_s0;
  MINT::FitParameter _fitBugg_4pi_lambda;

  MINT::FitParameter _fitModel_independent_Bin_1_Re;
  MINT::FitParameter _fitModel_independent_Bin_1_Im;
  MINT::FitParameter _fitModel_independent_Bin_2_Re;
  MINT::FitParameter _fitModel_independent_Bin_2_Im;    
  MINT::FitParameter _fitModel_independent_Bin_3_Re;
  MINT::FitParameter _fitModel_independent_Bin_3_Im;
  MINT::FitParameter _fitModel_independent_Bin_4_Re;
  MINT::FitParameter _fitModel_independent_Bin_4_Im;
  MINT::FitParameter _fitModel_independent_Bin_5_Re;
  MINT::FitParameter _fitModel_independent_Bin_5_Im;
  MINT::FitParameter _fitModel_independent_Bin_6_Re;
  MINT::FitParameter _fitModel_independent_Bin_6_Im;
  MINT::FitParameter _fitModel_independent_Bin_7_Re;
  MINT::FitParameter _fitModel_independent_Bin_7_Im;    
  MINT::FitParameter _fitModel_independent_Bin_8_Re;
  MINT::FitParameter _fitModel_independent_Bin_8_Im;
  MINT::FitParameter _fitModel_independent_Bin_9_Re;
  MINT::FitParameter _fitModel_independent_Bin_9_Im;
  MINT::FitParameter _fitModel_independent_Bin_10_Re;
  MINT::FitParameter _fitModel_independent_Bin_10_Im;
   
  const std::string& prefix()const{return _prefix;}
  std::string makeFitParName(const std::string& propertyName, int pdg_id) const;
 public:
  static std::string nameFromPid(int pdg_id);
  static int pidFromName(const std::string& name);
  static std::string nameFromName(const std::string& name);
  ResonanceProperties(int pid, MINT::MinuitParameterSet* mps=0, const std::string& namePrefix="");
  ResonanceProperties(const std::string& name, MINT::MinuitParameterSet* mps=0, const std::string& namePrefix="");
  double mass() const{return _fitMass;}
  double width() const{return _fitWidth;}
  double radius() const{return _fitRadius;}
  double alpha() const{return _fitAlpha;}
  double c1() const{return _fitc1;}
  double c2() const{return _fitc2;}
  double c3() const{return _fitc3;}
  double c4() const{return _fitc4;}
  double c5() const{return _fitc5;}

  double fermiLambda() const{return _fitFermiLambda;}
  double fermiS0() const{return _fitFermiS0;}
  double fitFlatte_gPi() const{return _fitFlatte_gPi;}  
  double fitFlatte_gK_by_gPi() const{return _fitFlatte_gK_by_gPi;}  
  double fitBugg_M() const{return _fitBugg_M;}
  double fitBugg_b1() const{return _fitBugg_b1;}
  double fitBugg_b2() const{return _fitBugg_b2;}
  double fitBugg_A() const{return _fitBugg_A;}
  double fitBugg_g_4pi() const{return _fitBugg_g_4pi;}
  double fitBugg_g_KK() const{return _fitBugg_g_KK;}
  double fitBugg_g_etaeta() const{return _fitBugg_g_etaeta;}
  double fitBugg_alpha() const{return _fitBugg_alpha;}
  double fitBugg_sA() const{return _fitBugg_sA;}
  double fitBugg_4pi_lambda() const{return _fitBugg_4pi_lambda;}
  double fitBugg_4pi_s0() const{return _fitBugg_4pi_s0;}
  double fitModel_independent_Bin_1_Re() const{return _fitModel_independent_Bin_1_Re;}
  double fitModel_independent_Bin_1_Im() const{return _fitModel_independent_Bin_1_Im;}
  double fitModel_independent_Bin_2_Re() const{return _fitModel_independent_Bin_2_Re;}
  double fitModel_independent_Bin_2_Im() const{return _fitModel_independent_Bin_2_Im;}
  double fitModel_independent_Bin_3_Re() const{return _fitModel_independent_Bin_3_Re;}
  double fitModel_independent_Bin_3_Im() const{return _fitModel_independent_Bin_3_Im;}
  double fitModel_independent_Bin_4_Re() const{return _fitModel_independent_Bin_4_Re;}
  double fitModel_independent_Bin_4_Im() const{return _fitModel_independent_Bin_4_Im;}
  double fitModel_independent_Bin_5_Re() const{return _fitModel_independent_Bin_5_Re;}
  double fitModel_independent_Bin_5_Im() const{return _fitModel_independent_Bin_5_Im;} 
  double fitModel_independent_Bin_6_Re() const{return _fitModel_independent_Bin_6_Re;}
  double fitModel_independent_Bin_6_Im() const{return _fitModel_independent_Bin_6_Im;}
  double fitModel_independent_Bin_7_Re() const{return _fitModel_independent_Bin_7_Re;}
  double fitModel_independent_Bin_7_Im() const{return _fitModel_independent_Bin_7_Im;}
  double fitModel_independent_Bin_8_Re() const{return _fitModel_independent_Bin_8_Re;}
  double fitModel_independent_Bin_8_Im() const{return _fitModel_independent_Bin_8_Im;}
  double fitModel_independent_Bin_9_Re() const{return _fitModel_independent_Bin_9_Re;}
  double fitModel_independent_Bin_9_Im() const{return _fitModel_independent_Bin_9_Im;}
  double fitModel_independent_Bin_10_Re() const{return _fitModel_independent_Bin_10_Re;}
  double fitModel_independent_Bin_10_Im() const{return _fitModel_independent_Bin_10_Im;}
    
  int pid() const{return _pid;}

  void changeMassForDebug(double newVal) const{
    _fitMass = newVal;
  }
  
};

#endif
//
