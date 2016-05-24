#ifndef MINIMISER_HH
#define MINIMISER_HH

#include <iostream>

#include "TMatrixTSym.h"
#include "TMinuit.h"

#include "Mint/IMinimisable.h"
#include "Mint/NamedParameter.h"

class TGraph;

namespace MINT
{
  class IMinuitParameter;

  class Minimiser : public TMinuit
  {
   public:
    static Minimiser* getDefaultMinimiser();

    Minimiser( IMinimisable* fitFunction=0 );

    virtual ~Minimiser()
    {}

    bool attachFunction( IMinimisable* fcn );

    IMinimisable* theFunction()
    { return _theFunction; }

    const IMinimisable* theFunction() const
    { return _theFunction; }

    const MinuitParameterSet* parSet() const
    { return _parSet; }

    MinuitParameterSet* parSet()
    { return _parSet; }

    unsigned int nPars() const;

    IMinuitParameter* getParPtr( unsigned int i );

    const IMinuitParameter* getParPtr( unsigned int i ) const;

    bool OK() const
    { return parsOK() && fcnOK(); }

    bool parsOK() const;

    bool fcnOK() const;

    double getFCNVal();

    void FCNGradient( Double_t* grad );

    bool initialiseVariables();

    bool setPrintLevel( int level=-1 );

    bool temporarilyQuiet();

    bool resetPrintLevel()
    { return setPrintLevel(-9999); }

    bool SetSomeMinuitOptions();

    bool CallMigrad();

    bool CallMinos();

    bool CallSeek( int maxCalls=100, int devs=5 );

    bool CallSimplex( int maxCalls=300, double tolerance=1.0 );

    bool CallImprove( int maxCalls=1500, int searches=5 );

    bool prepFit();

    bool doFit();

    bool doMinosFit();

    bool doSeekFit( int maxCalls = 100, int devs = 5 );

    bool doSimplexFit( int maxCalls = 300, double tolerance = 1.0 );

    bool scanMarked();

    bool scanAll();

    TGraph* scan( int i, double from=0.0, double to=0.0 );

    // note that the index i is the one in the parameter list
    // which goes from 0 to n-1 (i.e. C-style).
    // This corresponds to Minuit's parameter number i+1.
    // (so if you want Minuit's fit parameter 1, pass it 1-1=0)
    TGraph* scan( IMinuitParameter& fp, double from=0.0, double to=0.0);

    void setMaxCalls( int maxCalls )
    { _maxCalls = maxCalls; }

    int getMaxCalls() const
    { return _maxCalls; }

    void printResultVsInput( std::ostream& os = std::cout ) const;

  // only entries for variable parameters.
  // if you want rows and columns to correspond
  // to parameter numbers in MinuitParameterSets
  // use covMatrixFull.
    TMatrixTSym<double> covMatrix();

  // rows and columns correspond to external parameter numbers
  // (if you have many fixed parameters, you'll get a lot of zeros)
    TMatrixTSym<double> covMatrixFull();

   protected:
    bool  _useAnalyticGradient;  
    static Minimiser* _defaultMinimiser;
    static int _defaultMaxCalls;

    MinuitParameterSet* _parSet;

    IMinimisable* _theFunction;
    int _maxCalls;

    int _printLevel;

    bool init();

    bool MakeSpace( int needSpace );

    void TMinInit();

    bool updateFitParameters( Double_t* p );

    bool setParametersToResult();

    bool endOfFit();

    Int_t Eval( Int_t npar, Double_t *grad, Double_t &fval,
		Double_t *par, Int_t flag );

  private:
    mutable Double_t arglist[10];
    mutable Int_t ierflg;
  };
} //namespace MINT

#endif //MINIMISER_HH
