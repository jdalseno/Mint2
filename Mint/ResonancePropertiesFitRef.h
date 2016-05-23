#ifndef RESONANCEPROPERTIES_FITREF_HH
#define RESONANCEPROPERTIES_FITREF_HH
// author: Philippe d'Argent (p.dargent@cern.ch) & Jonas Rademacker

#include "Mint/FitParRef.h"
#include "Mint/FitParDependent.h"

class ResonanceProperties;

class ResonancePropertiesFitRef : public MINT::FitParDependent{
 protected:
    int _pid;
    MINT::FitParRef _fitMass;
    MINT::FitParRef _fitWidth;
    MINT::FitParRef _fitRadius;
    MINT::FitParRef _fitAlpha;
    MINT::FitParRef _fitc1;
    MINT::FitParRef _fitc2;
    MINT::FitParRef _fitc3;
    MINT::FitParRef _fitc4;
    MINT::FitParRef _fitc5;

    MINT::FitParRef _fitFermiLambda;
    MINT::FitParRef _fitFermiS0;
    MINT::FitParRef _fitRhoOmegaDelta_Re;
    MINT::FitParRef _fitRhoOmegaDelta_Im;
    MINT::FitParRef _fitFlatte_gPi;
    MINT::FitParRef _fitFlatte_gK_by_gPi;
    MINT::FitParRef _fitBugg_M;
    MINT::FitParRef _fitBugg_b1;
    MINT::FitParRef _fitBugg_b2;
    MINT::FitParRef _fitBugg_A;
    MINT::FitParRef _fitBugg_g_4pi;
    MINT::FitParRef _fitBugg_g_KK;
    MINT::FitParRef _fitBugg_g_etaeta;
    MINT::FitParRef _fitBugg_alpha;
    MINT::FitParRef _fitBugg_sA;
    MINT::FitParRef _fitBugg_4pi_lambda;
    MINT::FitParRef _fitBugg_4pi_s0;
    MINT::FitParRef _fitModel_independent_Bin_1_Re;
    MINT::FitParRef _fitModel_independent_Bin_1_Im;
    MINT::FitParRef _fitModel_independent_Bin_2_Re;
    MINT::FitParRef _fitModel_independent_Bin_2_Im;
    MINT::FitParRef _fitModel_independent_Bin_3_Re;
    MINT::FitParRef _fitModel_independent_Bin_3_Im;
    MINT::FitParRef _fitModel_independent_Bin_4_Re;
    MINT::FitParRef _fitModel_independent_Bin_4_Im;
    MINT::FitParRef _fitModel_independent_Bin_5_Re;
    MINT::FitParRef _fitModel_independent_Bin_5_Im;
    MINT::FitParRef _fitModel_independent_Bin_6_Re;
    MINT::FitParRef _fitModel_independent_Bin_6_Im;
    MINT::FitParRef _fitModel_independent_Bin_7_Re;
    MINT::FitParRef _fitModel_independent_Bin_7_Im;
    MINT::FitParRef _fitModel_independent_Bin_8_Re;
    MINT::FitParRef _fitModel_independent_Bin_8_Im;
    MINT::FitParRef _fitModel_independent_Bin_9_Re;
    MINT::FitParRef _fitModel_independent_Bin_9_Im;
    MINT::FitParRef _fitModel_independent_Bin_10_Re;
    MINT::FitParRef _fitModel_independent_Bin_10_Im;

 public:
    ResonancePropertiesFitRef(const ResonanceProperties& repo
			      , IFitParRegister* daddy=0);

    double mass()   const{return _fitMass;}
    double width()  const{return _fitWidth;}
    double radius() const{return _fitRadius;}
    double alpha()  const{return _fitAlpha;}
    double c1()  const{return _fitc1;}
    double c2()  const{return _fitc2;}
    double c3()  const{return _fitc3;}
    double c4()  const{return _fitc4;}
    double c5()  const{return _fitc5;}

    double fermiLambda() const{return _fitFermiLambda;}
    double fermiS0() const{return _fitFermiS0;}
    double rhoOmegaDelta_Re() const{return _fitRhoOmegaDelta_Re;}
    double rhoOmegaDelta_Im() const{return _fitRhoOmegaDelta_Im;}
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

    int pid()       const{return _pid;}

    virtual ~ResonancePropertiesFitRef(){}
};

#endif
//
