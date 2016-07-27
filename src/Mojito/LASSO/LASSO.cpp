// author: Philippe d'Argent (p.dargent@cern.ch)
#include "Mint/Neg2LLSum.h"
#include "Mint/IMinimisable.h"
#include "Mint/LASSO.h"
#include "Mint/NamedParameter.h"


using namespace std;
using namespace MINT;


double LASSO::getVal(){
    if(_lassoMod) return _lambda * (_pdf->sumOfSqrtFitFractions() + _pdf->absSumOfSqrtInterferenceFractions());
    else return _lambda * _pdf->sumOfSqrtFitFractions();
}

int LASSO::numberOfFitFractionsLargerThanThreshold(double threshold){
    return _pdf->numberOfFitFractionsLargerThanThreshold(threshold);
}

//void LASSO::Gradient(Double_t* grad){
void LASSO::Gradient(vector<double>& grad){
  std::vector<double> grad_pdf(this->getParSet()->size());  
    

  /*
  for(unsigned int j=0; j < this->getParSet()->size(); j++){
    grad_pdf.at(j) = 0.;   
    
  _pdf->GradientForLasso(&grad_pdf[0]);
  }
  */

  for(unsigned int j=0; j < grad.size(); j++) grad[j]=0;
    _pdf->GradientForLasso(grad);

  /*
  for(unsigned int j=0; j < this->getParSet()->size(); j++){
    grad.at(j)= _lambda * grad_pdf.at(;   
  }
  */
    
}


//
