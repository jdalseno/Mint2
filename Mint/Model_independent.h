#ifndef MODEL_INDEPENDENT_LINESHAPE_HH
#define MODEL_INDEPENDENT_LINESHAPE_HH
// author: Philippe d'Argent (p.dargent@cern.ch)
// status: 30.10.2015 

#include <complex>
#include <string>

#include "Mint/ILineshape.h"
#include "Mint/BW_BW.h"
#include "Mint/NamedParameter.h"
#include "Mint/NamedParameterBase.h"
#include "Mint/ResonancePropertiesList.h"
#include "Mint/FitParDependent.h"
#include "ResonancePropertiesFitRef.h"
#include "Math/Interpolator.h"
#include "Math/InterpolationTypes.h"

using namespace MINT;

class Model_independent : public BW_BW, virtual public ILineshape{
 public:
  
  Model_independent( const AssociatedDecayTree& tree, const std::string& namePrefix="");

  virtual std::string name() const{
    return "Model_independent("+_theDecay.oneLiner() +")";
  }

  virtual ~Model_independent(){ 
      plot();
      delete _interpolator_Re;
      delete _interpolator_Im;
  }
  virtual std::complex<double> getVal(IDalitzEvent& evt);
  std::complex<double> getInterpolatedVal(const double s_inGeV2);  
  void plot();  

 protected:
    
  int _nBins;  
  NamedParameter<double> _binCenters;
  std::vector<double> _binCenterVector;
    
  std::vector<double> getBinCenterValues_Re(); 
  std::vector<double> getBinCenterValues_Im();   
  
  NamedParameter<std::string> _interpolationTypeString;
  ROOT::Math::Interpolation::Type _interpolationType;  
    
  ROOT::Math::Interpolator* _interpolator_Re;
  ROOT::Math::Interpolator* _interpolator_Im;
    
  double Re_Bin(int i) const{
        if(i==0)return mumsFittableProperties().fitModel_independent_Bin_1_Re();
        if(i==1)return mumsFittableProperties().fitModel_independent_Bin_2_Re();
        if(i==2)return mumsFittableProperties().fitModel_independent_Bin_3_Re();
        if(i==3)return mumsFittableProperties().fitModel_independent_Bin_4_Re();
        if(i==4)return mumsFittableProperties().fitModel_independent_Bin_5_Re();
        if(i==5)return mumsFittableProperties().fitModel_independent_Bin_6_Re();
        if(i==6)return mumsFittableProperties().fitModel_independent_Bin_7_Re();
        if(i==7)return mumsFittableProperties().fitModel_independent_Bin_8_Re();
        if(i==8)return mumsFittableProperties().fitModel_independent_Bin_9_Re();
        if(i==9)return mumsFittableProperties().fitModel_independent_Bin_10_Re();
        else return 0.;
  }  
    
  double Im_Bin(int i) const{
        if(i==0)return mumsFittableProperties().fitModel_independent_Bin_1_Im();
        if(i==1)return mumsFittableProperties().fitModel_independent_Bin_2_Im();
        if(i==2)return mumsFittableProperties().fitModel_independent_Bin_3_Im();
        if(i==3)return mumsFittableProperties().fitModel_independent_Bin_4_Im();
        if(i==4)return mumsFittableProperties().fitModel_independent_Bin_5_Im();
        if(i==5)return mumsFittableProperties().fitModel_independent_Bin_6_Im();
        if(i==6)return mumsFittableProperties().fitModel_independent_Bin_7_Im();
        if(i==7)return mumsFittableProperties().fitModel_independent_Bin_8_Im();
        if(i==8)return mumsFittableProperties().fitModel_independent_Bin_9_Im();
        if(i==9)return mumsFittableProperties().fitModel_independent_Bin_10_Im();
        else return 0.;
  }    
    
  double Bin_1_Re() const{
        return mumsFittableProperties().fitModel_independent_Bin_1_Re();
  }
  double Bin_1_Im() const{
        return mumsFittableProperties().fitModel_independent_Bin_1_Im();
  }
  double Bin_2_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_2_Re();
  }
  double Bin_2_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_2_Im();
  }
  double Bin_3_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_3_Re();
  }
  double Bin_3_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_3_Im();
  }
  double Bin_4_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_4_Re();
  }
  double Bin_4_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_4_Im();
  }
  double Bin_5_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_5_Re();
  }
  double Bin_5_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_5_Im();
  }
  double Bin_6_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_6_Re();
  }
  double Bin_6_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_6_Im();
  }
  double Bin_7_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_7_Re();
  }
  double Bin_7_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_7_Im();
  }
  double Bin_8_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_8_Re();
  }
  double Bin_8_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_8_Im();
  }
  double Bin_9_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_9_Re();
  }
  double Bin_9_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_9_Im();
  }
  double Bin_10_Re() const{
      return mumsFittableProperties().fitModel_independent_Bin_10_Re();
  }
  double Bin_10_Im() const{
      return mumsFittableProperties().fitModel_independent_Bin_10_Im();
  }

};

#endif
//
