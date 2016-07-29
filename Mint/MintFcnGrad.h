#ifndef MINTFCNGRAD_HH
#define MINTFCNGRAD_HH

#include "Minuit2/FCNGradientBase.h"

#include "Mint/IMinimisable.h"

namespace MINT
{
  class MintFcnGrad : public ROOT::Minuit2::FCNGradientBase
  {
   public:
    /**
       Constructor
     */
    MintFcnGrad( IMinimisable* theFunction )
      :_theFunction(theFunction)
    {}

    /**
       Calculate test-statistic from Minuit2 fit parameter guesses
     */
    virtual double operator()( const std::vector<double>& par ) const
    {
      for( unsigned int i=0; i<par.size(); ++i )
	_theFunction->getParSet()->getParPtr(i)->setCurrentFitVal(par[i]);

      std::cout << "-2logL = " << _theFunction->getVal()-_best_min << std::endl;

      return _theFunction->getVal()-_best_min;
    }

    /**
       User-supplied gradient
     */
    virtual std::vector<double> Gradient( const std::vector<double>& par )
      const
    { return _theFunction->Gradient(par); }

    /**
       If minimisation fails due to edm being above tolerance,
       set the best known -2logL and reminimise
     */
    void SetBestMin( const double& best_min )
    { _best_min += best_min; }

  private:
    /**
       Change in test-statistic that defines the uncertainty of fit parameters
     */
    virtual double Up() const
    { return _error_def; }

    IMinimisable* _theFunction;

    const double _error_def = 1.0;

    double _best_min = 0.0;
  };
} //namespace MINT

#endif //MINTFCNGRAD_HH
