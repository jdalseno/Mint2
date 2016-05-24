#ifndef NEG_TWO_LL_Constraint_HH
#define NEG_TWO_LL_Constraint_HH

#include <vector>

#include "TMath.h"

#include "Mint/Minimisable.h"
#include "Mint/Neg2LLConstraint.h"

namespace MINT
{
  class Neg2LLConstraint : public Minimisable
  {
   public:
    Neg2LLConstraint( IMinuitParameter* mp, double mean, double sigma,
		      MinuitParameterSet* mps=0 )
      : Minimisable(mps), _mp(mp), _mean(mean), _sigma(sigma)
    {}

    virtual ~Neg2LLConstraint()
    {}

    virtual void beginFit()
    {}

    virtual void parametersChanged()
    {}

    virtual void endFit()
    {}
    
    virtual double getVal()
    {
      if( 0.0 == _sigma )
	return 0.0;

      const double d = ((double)*(_mp) -_mean)/_sigma;

      return d*d;
      //return -2.* log(gauss((double) *(_mp)) ); 
    }

    virtual double getNewVal()
    {
      parametersChanged();
      
      return getVal();
    }

    inline double gauss( const double& x )
    {
      if( 0.0 == _sigma )
	return 0;

      const double d = (x-_mean)/_sigma;
      const double n = 1./(sqrt(2.0*TMath::Pi())*_sigma);

      return n * exp(-0.5*d*d);
    }
    
   protected:
    IMinuitParameter* _mp;
    double _mean;
    double _sigma;
  };
} //namespace MINT
#endif //NEG_TWO_LL_Constraint_HH
