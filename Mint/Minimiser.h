#ifndef MINIMISER_HH
#define MINIMISER_HH

#include <iostream>

#include "TGraph.h"
#include "TMatrixTSym.h"//delete soon

#include "Minuit2/FunctionMinimum.h"

#include "Mint/IMinimisable.h"
#include "Mint/MintFcn.h"
#include "Mint/MintFcnGrad.h"
#include "Mint/NamedParameter.h"

namespace MINT
{
  class IMinuitParameter;

  class Minimiser
  {
   public:
    /**
       Ensure only one instance
     */
    static Minimiser* getDefaultMinimiser();

    /**
       Constructor
     */
    Minimiser( IMinimisable* fitFunction=0 );

    /**
       Destructor
     */
    virtual ~Minimiser()
    {}

    /**
       Replace stored Minimisable
     */
    bool attachFunction( IMinimisable* fcn );

    /**
       Return modifiable Minimisable
     */
    IMinimisable* theFunction()
    { return _theFunction; }

    /**
       Return non-modifiable Minimisable
     */
    const IMinimisable* theFunction() const
    { return _theFunction; }

    /**
       Return modifiable Mint parameters
     */
    MinuitParameterSet* parSet()
    { return _parSet; }

    /**
       Return non-modifiable Mint parameters
     */
    const MinuitParameterSet* parSet() const
    { return _parSet; }

    /**
       Number of Mint parameters
     */
    unsigned int nPars() const;

    /**
       Return modifiable Mint parameter
     */
    IMinuitParameter* getParPtr( const unsigned int& i );

    /**
       Return non-modifiable Mint parameter
     */
    const IMinuitParameter* getParPtr( const unsigned int& i ) const;

    /**
       Mint parameter and Minimisable status
     */
    bool OK() const
    { return parsOK() && fcnOK(); }

    /**
       Mint parameter status
     */
    bool parsOK() const;

    /**
       Minimisable status
     */
    bool fcnOK() const;

    /**
       Prepare Minimisable
       Initialise Minuit2 parameter set
     */
    //seems unnecessary to Prepare Minimisable again since Init does this already
    bool prepFit();

    /**
       Update Mint parameters with fit result
       Finalise Minimisable
       Print minimum
     */
    bool endOfFit();

    /**
       Initial call to Simplex
     */
    bool CallSimplex();

    /**
       Initial call to Migrad
       In most cases, this will likely fail due to high edm
       Use CallImprove to push down the edm
     */
    bool CallMigrad();

    /**
       Iteratively improve on the first attempt at Migrad
       Search will break once valid minimum is found
     */
    bool CallImprove( const unsigned int& searches=5 );

    /**
       Call Minos on a particular external parameter number
     */
    bool CallMinos( const unsigned int& par_id );

    /**
       Perform fast likelihood scan of given parameter
       Minimisation is not performed at each step in the scan
     */
    bool CallScan( const unsigned int& par_id,
		   const double& from=0.0, const double& to=0.0 );

    /**
       A typical minimsation script
       Call Migrad
     */
    bool doFit();

    /**
       Print fit pulls
     */
    void printResultVsInput( std::ostream& os = std::cout ) const;

    /**
       Print covariance matrix
     */
    void PrintCovMatrix() const;

    /**
       Return full covariance matrix including entries for fixed parameters
     */
    TMatrixTSym<double> covMatrixFull();

   protected:
    /**
       Prepare Minimisable
       Initialise Minuit2 parameter set
       Decide on FCN type
     */
    bool Init();

    /**
       Initialise Minuit2 parameter set
     */
    bool initialiseVariables();

    /**
       Update Mint parameters with fit result
     */
    bool setParametersToResult();

    bool _useAnalyticGradient = false;
    static Minimiser* _defaultMinimiser;

    MinuitParameterSet* _parSet;

    IMinimisable* _theFunction;

  private:
    ROOT::Minuit2::FCNBase* _fcn;
    ROOT::Minuit2::MnUserParameters _mn_param;
    ROOT::Minuit2::FunctionMinimum _min;
    ROOT::Minuit2::MnUserCovariance _cov;
  };
} //namespace MINT

#endif //MINIMISER_HH
