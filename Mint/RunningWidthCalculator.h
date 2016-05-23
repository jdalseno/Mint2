#ifndef RUNNINGWIDTHCALCULATOR_HH
#define RUNNINGWIDTHCALCULATOR_HH
// author: Philippe d'Argent (p.dargent@cern.ch)
// created: 21 Oct 2015

#include "Mint/IFastAmplitudeIntegrable.h"
#include "Mint/DalitzEventPattern.h"
#include "TH1D.h"
#include "TF1.h"
#include "Mint/ResonanceProperties.h"
#include "Mint/ResonancePropertiesList.h"

class RunningWidthCalculator{
    
  DalitzEventPattern _pat;  
  double _min_s_inGeV2;  
  void set_min_s_inGeV2();  
    
public:

  RunningWidthCalculator(const DalitzEventPattern& pat);
  
  TH1D* makeHisto_phaseSpace(int nBins,double max_m_inMeV,std::string OutputFileName = "");
  TH1D* makeHisto_2body(int nBins,double max_m_inMeV, const DecayTree& thisDcy, std::string OutputFileName = "");
  TH1D* makeHisto_3body(int nBins,double max_m_inMeV, const DecayTree& thisDcy, std::string OutputFileName = "");
  TH1D* makeHisto_dalitz(int nBins,double max_m_inMeV,int nIntegrationEvents,IFastAmplitudeIntegrable* amps = 0,std::string OutputFileName = "");

  TF1* getRunningWidthFunction_phaseSpace(double max_s_inGeV2);  
  TF1* getRunningWidthFunction_2body(double max_s_inGeV2, const DecayTree& thisDcy);  
  TF1* getRunningWidthFunction_3body(double max_s_inGeV2, const DecayTree& thisDcy);  
  TF1* getRunningWidthFunction_Fermi(double max_s_inGeV2, double lambda, double s0);  
    
  std::vector<double> getPartialWidthCouplingsFromBF(std::vector<double> BF , std::vector<TF1*> partialWidths, double m0, double gamma0);
    
  TH1D* makeRunningMassHisto_3body(int nBins, double max_s_inGeV2, std::vector<TF1*> gamma, std::vector<double> g, std::string OutputFileName = "");
    
  void setDalitzEventPattern(const DalitzEventPattern& pat);
    
};


#endif
//
