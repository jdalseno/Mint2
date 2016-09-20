#ifndef HISTO_BW_LINESHAPE_HH
#define HISTO_BW_LINESHAPE_HH
// author: Philippe d'Argent (p.dargent@cern.ch)
// status: 19 March 2015 GMT
#include "TH1D.h"
#include "TFile.h"
#include <complex>

#include "Mint/ILineshape.h"
#include "Mint/BW_BW.h"
#include "Mint/NamedParameter.h"
#include "Mint/NamedParameterBase.h"
#include "Mint/ResonancePropertiesList.h"
#include "Mint/FitParDependent.h"
#include "ResonancePropertiesFitRef.h"
#include "ResonanceProperties.h"

using namespace MINT;
using namespace std;

class Histo_BW  : public BW_BW, virtual public ILineshape{
 public:
  
 Histo_BW( const AssociatedDecayTree& tree, const std::string& namePrefix): BW_BW(tree, namePrefix), _runningWidthHist(0), _runningMassHist(0)
  {
      NamedParameter<string> fileNameWidth(("RunningWidth_"+ namePrefix + (BW_BW::resonanceProperties()->nameFromPid(abs(mumsPID())))),("RunningWidth_"+ (BW_BW::resonanceProperties()->nameFromPid(abs(mumsPID()))) + ".root"));
      
      NamedParameter<string> fileNameMass(("RunningMass_"+ namePrefix+ (BW_BW::resonanceProperties()->nameFromPid(abs(mumsPID())))),("RunningMass_"+ (BW_BW::resonanceProperties()->nameFromPid(abs(mumsPID()))) + ".root"));
      
      TFile* f_w = TFile::Open(((string)fileNameWidth).c_str());
      _runningWidthHist = get_width_histo(f_w, "RunningWidth");
      TFile* f_m = TFile::Open(((string)fileNameMass).c_str());
      _runningMassHist = get_mass_histo(f_m, "RunningMass");
  }

  virtual std::string name() const{
    return "Histo_BW("+prefix()+_theDecay.oneLiner() +")";
  }

  virtual ~Histo_BW(){}

 protected:
    TH1D* _runningWidthHist;
    TH1D* _runningMassHist;
    double runningMass2();
    virtual double GofM(); 
    virtual std::complex<double> BreitWigner();
    TH1D* get_width_histo(TFile* f, const std::string& hname); 
    TH1D* get_mass_histo(TFile* f, const std::string& hname); 

    //Double_t phaseSpace(Double_t *x, Double_t *par);
    TH1D* producePhaseSpaceHist();
};

#endif
//
