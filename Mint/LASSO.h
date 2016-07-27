#ifndef LASSO_HH
#define LASSO_HH
// author: Philippe d'Argent (p.dargent@cern.ch)

#include "TMath.h"
#include "Mint/Minimisable.h"
#include "Mint/DalitzPdfBaseFastInteg.h"
#include "Mint/NamedParameter.h"
#include <vector>

namespace MINT{

class LASSO: public Minimisable{
    protected:
    DalitzPdfBaseFastInteg* _pdf;
    double _lambda;
    NamedParameter<int> _lassoMod;
    
    public:
        LASSO(DalitzPdfBaseFastInteg* pdf, double lambda = 1.)
        : _pdf(pdf), _lambda(lambda), _lassoMod("LassoMod",0) {};
                
        virtual void beginFit(){};
        virtual void parametersChanged(){};
        virtual void endFit(){};
        
        virtual double getVal();
    
        virtual double getNewVal(){ 
            parametersChanged();
            return getVal();
        }
    
        //virtual void Gradient(Double_t* grad);
        virtual void Gradient(std::vector<double>& grad);
        virtual bool useAnalyticGradient() {return _pdf->useAnalyticGradient();}

        int numberOfFitFractionsLargerThanThreshold(double threshold);
        double absSumOfInterferenceFractions() {
             return _pdf->absSumOfInterferenceFractions();
        }
        double sumOfFitFractions(){
             return _pdf->sumOfFitFractions();
        }
        virtual ~LASSO(){}
        
};

}// namespace MINT
#endif
//
