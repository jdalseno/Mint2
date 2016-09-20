// author: Nunya Business (ya.mum@bristol.ac.uk)
// status:  Mon 22 Jul 2014 19:17:59 GMT
#include <iostream>

#include "Mint/CLHEPPhysicalConstants.h"
#include "Mint/ParticlePropertiesList.h"
#include "Mint/ParticleProperties.h"

#include "Mint/NonRes.h"

using namespace std;
using namespace MINT;

NonRes::NonRes( const AssociatedDecayTree& tree,
		const std::string& type, const std::string& namePrefix)
  : BW_BW(tree, namePrefix)
  , _type(type)
  //, _alpha("NonRes::alpha", 1.0)
{
    //cout << _alpha << endl;
}

std::complex<double> NonRes::getVal( IDalitzEvent& evt ) {
  setEventPtr(evt);
  resetInternals();

  const double m2hh = mumsRecoMass2()/GeV/GeV;
    
  std::vector<int> asi = _theDecay.getVal().asi();
  double min = getEvent()->eventPattern().sijMin(asi)/(GeV*GeV);
  double max = getEvent()->eventPattern().sijMax(asi)/(GeV*GeV);
    
  double x = 2.* (m2hh - min)/(max - min) - 1.;  
    
  double non_res;
  if( _type == "Exp" )
    non_res = exp(-GetAlpha()*x);
  else if( _type == "Pow" ){
      non_res = 1. 
      + c1()*x 
      + c2()*(2.* pow(x,2) - 1.) 
      + c3()*(4.* pow(x,3) - 3.*x) 
      + c4()*(8.* pow(x,4) - 8.*pow(x,2)+1.)
      + c5()*(16.* pow(x,5) - 20.*pow(x,3)+5.*x);
  }
  else
    non_res = 1.0;

  double formFactor= 1.;
  if( _normBF == 1 ) formFactor = Fr();
  else if( _normBF == 0 ) formFactor = Fr_PDG_BL();  
  else if(_normBF == 2 ) formFactor = Fr_BELLE(0.);   
    
  return formFactor*non_res;
}


void NonRes::print( std::ostream& out ) const {
  out << name();
}

void NonRes::print( IDalitzEvent& evt, std::ostream& out ) {
  setEventPtr(evt);
  resetInternals();
  out << name()
      << "\n\t> co-ordinate: " << getDalitzCoordinate()
      << "\n\t> This is the decay I'm looking at:"
      << "\n" << _theDecay
      << "\n\t> These are a few values: "
      << " startOfDecayChain? " << startOfDecayChain()
      << ", BreitWigner = " 
      << BreitWigner()
      << ", Blatt-Weisskopf penetration factor: "
      << Fr()
      << ", total BW_BW: " 
      << getVal(evt);
}
    
double NonRes::GetAlpha() const {
  return _RPL->get(mumsPID())->alpha();
}

std::ostream& operator<<( std::ostream& out, const NonRes& amp ) {
  amp.print(out);

  return out;
}
