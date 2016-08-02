#include <iostream>

#include "TFile.h"
#include "TGraph.h"

#include "Minuit2/MinimumSeed.h"
#include "Minuit2/MinimumState.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MnScan.h"
#include "Minuit2/MnSimplex.h"
#include "Minuit2/MnUserTransformation.h"

#include "Mint/IMinuitParameter.h"
#include "Mint/Minimiser.h"

using namespace std;
using namespace MINT;

Minimiser* Minimiser::getDefaultMinimiser()
{
  if( 0 == _defaultMinimiser )
    _defaultMinimiser = new Minimiser;

  return _defaultMinimiser;
}

Minimiser::Minimiser( IMinimisable* fitFunction )
  : _theFunction(fitFunction)
  , _min(ROOT::Minuit2::MinimumSeed(ROOT::Minuit2::MinimumState(0),
    				    ROOT::Minuit2::MnUserTransformation()),
	 1.0)
{
  if( 0 != theFunction() )
    Init();
}

bool Minimiser::attachFunction( IMinimisable* fitFunction )
{
  if( 0 == fitFunction )
    return false;

  _theFunction = fitFunction;

  return Init();
}

unsigned int Minimiser::nPars() const
{
  if( 0 == _parSet )
    return 0;

  return _parSet->size();
}

IMinuitParameter* Minimiser::getParPtr( const unsigned int& i )
{
  if( 0 == _parSet )
    return 0;

  if( i >= nPars() )
    return 0;

  return _parSet->getParPtr(i);
}

const IMinuitParameter* Minimiser::getParPtr( const unsigned int& i ) const
{
  if( 0 == _parSet )
    return 0;

  if( i >= nPars() )
    return 0;

  return _parSet->getParPtr(i);
}

bool Minimiser::parsOK() const
{
  if( 0 == _parSet )
    return false;

  if( 0 == nPars() )
    return false;

  return true;
}

bool Minimiser::fcnOK() const
{
  if( 0 == theFunction() )
    return false;

  return true;
}

bool Minimiser::initialiseVariables()
{
  bool dbThis = false;

  if( !parsOK() )
    return false;

  bool success = true;
  if(dbThis){
    cout << "Minimiser::initialiseVariables() called. " << endl;
    cout << "\n\t(declaring them to MINUIT)" << endl;
  }

  //Temove hidden MinuitParameters
  for( unsigned int i=0; i < nPars(); i++ ){
    if( getParPtr(i)->hidden() ){
      if(dbThis)
	cout << i << ")" << getParPtr(i)->name() << endl;

      parSet()->unregister(getParPtr(i));
      i--;
    }
  }

  for( unsigned int i=0; i<nPars(); ++i ){
    int ierflag = 0;

    if(dbThis)
      cout << i << ")" << getParPtr(i)->name() << endl;

    const double step = getParPtr(i)->stepInit();
    _mn_param.Add( getParPtr(i)->name(), getParPtr(i)->meanInit(), step );
    //This logic may need checking and needs updating for single sided limits
    if( getParPtr(i)->maxInit() != 0.0 && getParPtr(i)->minInit() != 0.0 )
      _mn_param.SetLimits(i, getParPtr(i)->minInit(), getParPtr(i)->maxInit());

    if( getParPtr(i)->iFixInit() )
      _mn_param.Fix(i);

    success &= ! ierflag;
  }

  if(dbThis){
    cout << "Minimiser::initialiseVariables():"
	 << "done declaring variables, now:"
	 << "theFunction()->parametersChanged();"
	 << endl;
  }

  theFunction()->parametersChanged();
  if(dbThis)
    cout << "Minimiser::initialiseVariables():"
	 << " all done - returning " << success << endl;

  return success;
}

bool Minimiser::CallSimplex()
{
  //Strategy 2
  ROOT::Minuit2::MnSimplex simplex( *_fcn, _mn_param, 2 );
  _min = simplex();
  _cov = _min.UserCovariance();
  if( _useAnalyticGradient == false )
    static_cast<MintFcn*>(_fcn)->SetBestMin(_min.Fval());
  else
    static_cast<MintFcnGrad*>(_fcn)->SetBestMin(_min.Fval());

  return _min.IsValid();
}

bool Minimiser::CallMigrad()
{
  //Strategy 2
  ROOT::Minuit2::MnMigrad migrad( *_fcn, _mn_param, 2 );
  _min = migrad();
  _cov = _min.UserCovariance();
  if( _useAnalyticGradient == false )
    static_cast<MintFcn*>(_fcn)->SetBestMin(_min.Fval());
  else
    static_cast<MintFcnGrad*>(_fcn)->SetBestMin(_min.Fval());

  return _min.IsValid();
}

bool Minimiser::CallImprove( const unsigned int& searches )
{
  //Strategy 2
  ROOT::Minuit2::MnMigrad migrad( *_fcn, _min.UserParameters().Params(), _cov, 2 );
  for( unsigned int i=0; i<searches; ++i ){
    _min = migrad();
    _cov = _min.UserCovariance();
    if( _useAnalyticGradient == false )
      static_cast<MintFcn*>(_fcn)->SetBestMin(_min.Fval());
    else
      static_cast<MintFcnGrad*>(_fcn)->SetBestMin(_min.Fval());

    if( _min.IsValid() )
      break;
  }

  std::cout << _min << std::endl;

  return _min.IsValid();
}

bool Minimiser::CallMinos( const unsigned int& par_id )
{
  if( _min.IsValid() ){
    ROOT::Minuit2::MnMinos minos( *_fcn, _min, 2 );
    std::pair<double,double> minos_err = minos(par_id);
    std::cout << getParPtr(par_id)->name() << " = "
	      << _min.UserParameters().Params()[par_id]  
	      << " " << minos_err.first << " +" << minos_err.second << std::endl;      
  }else{
    std::cout << "ERROR: Minos cannot be called on an invalid minimum" << std::endl;
    exit(1);
  }

  return true;
}

bool Minimiser::CallScan( const unsigned int& par_id,
			  const double& low, const double& high )
{
  IMinuitParameter* p = getParPtr(par_id);
  if( 0 == p )
    return 0;

  const unsigned int points = 100;

  std::vector<std::pair<double,double>> scan_points;
  if( _min.IsValid() ){
    ROOT::Minuit2::MnScan scan(*_fcn, _min.UserParameters().Params(), _cov);
    scan_points = scan.Scan(points, low, high);
  }else{
    ROOT::Minuit2::MnScan scan(*_fcn, _mn_param, _cov);
    scan_points = scan.Scan(points, low, high);
  }

  std::vector<double> scan_x, scan_y;
  for( auto it = std::make_move_iterator(scan_points.begin()),
         end = std::make_move_iterator(scan_points.end()); it != end; ++it ){
    scan_x.push_back(std::move(it->first));
    scan_y.push_back(std::move(it->second));
  }

  TGraph* const graph = new TGraph(points, scan_x.data(), scan_y.data());

  const std::string fname = "scan_" + p->name() + ".root";

  TFile fscan(fname.c_str(), "RECREATE");
  fscan.cd();
  graph->Write();
  fscan.Close();

  delete graph;

  return true;
}

bool Minimiser::prepFit()
{
  bool dbThis=false;
  bool success=true;

  theFunction()->beginFit();

  if(dbThis)
    cout << "re-initialised variables" << endl;

  return success;
}

bool Minimiser::doFit()
{
  bool dbThis=false;
  bool success = true;
  if(dbThis)
    cout << "Minimiser::doFit() called" << endl;

  success &= prepFit();
  if(dbThis)
    cout << "... called prepFit " << success << endl;

  success &= CallMigrad();
  if(dbThis)
    cout << "... called MIGRAD" << success << endl;

  success &= this->endOfFit();
  if(dbThis)
    cout << "... called endOfFit" << success << endl;

  std::cout << _min << std::endl;

  return success;
}

bool Minimiser::endOfFit()
{
  if( !parsOK() )
    return false;

  setParametersToResult();
  theFunction()->endFit();

  return true;
}

/*
//might be useful for minos as well
TGraph* Minimiser::scan( IMinuitParameter& fp, double from, double to )
{
  if( fp.parSet() != this->parSet() ){
    cout << "ERROR Minimiser::scanParameter: "
	 << " You are trying to scan a parameter that is"
	 << " associated to a different fit (different fit parameter set)"
	 << " will not do it."
	 << " The parameter you tried was: " << endl;
    fp.print();
    cout <<  endl;
    return 0;
  }

  return scan(fp.parSetIndex(), from, to);
  }*/

void Minimiser::printResultVsInput( std::ostream& os ) const
{
  if( 0 == _parSet )
    return;

  _parSet->printResultVsInput(os);
}

Minimiser* Minimiser::_defaultMinimiser=0;

bool Minimiser::Init()
{
  bool dbThis = true;
  if(dbThis)
    cout << "Minimiser::init(): you called me" << endl;

  if( 0 == theFunction() ){
    std::cout << "ERROR: Minimiser::Init():"
	      << " the function-ptr is empty."
	      << " I won't do anything for now."
	      << std::endl;

    exit(1);
  }

  if(dbThis)
    cout << " ... calling theFunction()->beginFit()" << endl;

  theFunction()->beginFit();

  _parSet = theFunction()->getParSet();
  if( 0 == _parSet ){
    std::cout << "ERROR IN Minimiser::init():"
	      << " theFunction()->getParSet() returned empty pointer"
	      << " I won't do anything for now."
	      << std::endl;

    exit(1);
  }

  initialiseVariables();
  if(dbThis)
    cout << " initialised variables." << endl;

  if(dbThis)
    cout << "Minimiser::Init(): returning true" << endl;

  _useAnalyticGradient = theFunction()->useAnalyticGradient();  
  if( _useAnalyticGradient == false )
    _fcn = new MintFcn(theFunction());
  else
    _fcn = new MintFcnGrad(theFunction());

  std::cout << std::endl << "Behold the mighty Minuit2!" << std::endl
	    << std::endl << "Initial parameter state:"
	    << _mn_param << std::endl;

  return true;
}

bool Minimiser::setParametersToResult()
{
  if( !parsOK() )
    return false;

  const std::vector<double>& par = _min.UserParameters().Params();
  const std::vector<double>& err = _min.UserParameters().Errors();
  for( unsigned int i=0; i<nPars(); ++i)
    this->getParPtr(i)->setResult(par[i], err[i], 0.0, 0.0);

  theFunction()->parametersChanged();

  return true;
}

void Minimiser::PrintCovMatrix() const
{
  if( _min.IsValid() )
    std::cout << _cov << std::endl;
  else
    std::cout << "WARNING: Cannot print covariance matrix. Minimum was invalid"
	      << std::endl;
}

TMatrixTSym<double> Minimiser::covMatrixFull()
{
  TMatrixTSym<double> matrix(nPars());

  for( unsigned int i=0; i<_min.UserState().VariableParameters(); ++i )
    for( unsigned int j=i; j<_min.UserState().VariableParameters(); ++j ){
      const unsigned int ex_i = _min.UserState().ExtOfInt(i);
      const unsigned int ex_j = _min.UserState().ExtOfInt(j);

      matrix(ex_i,ex_j) = matrix(ex_j,ex_i) = _cov(i,j);
    }

  return matrix;
}
