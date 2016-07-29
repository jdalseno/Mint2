#ifndef MINTFCN_HH
#define MINTFCN_HH

#include "Minuit2/FCNBase.h"

#include "Mint/IMinimisable.h"

namespace MINT
{
  class MintFcn : public ROOT::Minuit2::FCNBase
  {
   public:
    /**
       Constructor
     */
    MintFcn( IMinimisable* theFunction )
      :_theFunction(theFunction)
    {}

    /**
       Calculate test-statistic from Minuit2 fit parameter guesses
     */
    virtual double operator()( const std::vector<double>& par ) const
    {
      for( unsigned int i=0; i<par.size(); ++i )
	_theFunction->getParSet()->getParPtr(i)->setCurrentFitVal(par[i]);

      std::cout << std::setprecision(std::numeric_limits<double>::digits10);
      std::cout << "-2logL = " << _theFunction->getVal()-_best_min << std::endl;

      return _theFunction->getVal()-_best_min;
    }

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

#endif //MINTFCN_HH
