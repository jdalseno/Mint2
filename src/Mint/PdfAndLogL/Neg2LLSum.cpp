#include "Mint/IMinimisable.h"
#include "Mint/NamedParameter.h"
#include "Mint/Neg2LLConstraint.h"
#include "Mint/Neg2LLSum.h"

using namespace std;
using namespace MINT;

Neg2LLSum::Neg2LLSum( MinuitParameterSet* mps )
  : Minimisable(mps)
{}

Neg2LLSum::Neg2LLSum( const std::vector<IMinimisable*>& likList_in,
		      MinuitParameterSet* mps )
  : Minimisable(mps)
  , _likList(likList_in)
{}

Neg2LLSum::Neg2LLSum( const Neg2LLSum& other )
  : IMinimisable()
  , Minimisable(other)
  , _likList(other._likList)
{}

bool Neg2LLSum::add( IMinimisable* llPtr )
{
  if( 0 == llPtr ){
    std::cout << "ERROR in Neg2LLSum::add: trying to add null pointer"
	      << std::endl;

    return false;
  }

  _likList.push_back(llPtr);

  return true;
}

bool Neg2LLSum::insert( IMinimisable* llPtr )
{
  if( 0 == llPtr ){
    std::cout << "ERROR in Neg2LLSum::insert: trying to insert null pointer"
	      << std::endl;

    return false;
  }

  _likList.insert(_likList.begin(), llPtr);

  return true;
}

void Neg2LLSum::beginFit()
{
  for( unsigned int i=0; i<_likList.size(); ++i )
    _likList[i]->beginFit();
}

void Neg2LLSum::parametersChanged()
{
  for( unsigned int i=0; i<_likList.size(); ++i )
    _likList[i]->parametersChanged();
}

void Neg2LLSum::endFit()
{
  for( unsigned int i=0; i<_likList.size(); ++i )
    _likList[i]->endFit();
}

double Neg2LLSum::getVal()
{
  double sum = 0.0;
  for( unsigned int i=0; i<_likList.size(); ++i )
    sum += _likList[i]->getVal();

  return sum;
}

bool Neg2LLSum::addConstraints()
{
  cout << "addConstraints for " << getParSet()->size()
       << " paramaters" <<  endl;

  for( unsigned int i=0; i<getParSet()->size(); ++i ){
    string name_i= getParSet()->getParPtr(i)->name();
    cout << "paramater " << i << " : " <<  name_i << endl;
    NamedParameter<double> constrain( ("Constrain_"+name_i).c_str(),-1,-1,
				      NamedParameterBase::QUIET );
    //cout << constrain << endl;
    if( constrain.getVal(0)>0 && constrain.getVal(1)>0 ){
      IMinimisable* gaussConstraint =
	new Neg2LLConstraint(getParSet()->getParPtr(i),constrain.getVal(0),
			     constrain.getVal(1),getParSet());
      add(gaussConstraint);
      cout << "Added gaussian constraint to parameter: " << name_i 
	   << " with initial value = " << (double) *(getParSet()->getParPtr(i))  
	   << " and mean = " << constrain.getVal(0)
	   << " and sigma = " << constrain.getVal(1) << endl  ;
      cout << "neg2LL value = " << gaussConstraint->getVal() << endl; 
    }
  }

  return true;
}

std::vector<double> Neg2LLSum::Gradient( const std::vector<double>& par )
{  
  std::vector<double> grad(this->getParSet()->size(), 0.0);

  for( unsigned int i=0; i<_likList.size(); ++i ){

    std::vector<double> grad_tmp = _likList[i]->Gradient(par);

    for( unsigned int j=0; j<this->getParSet()->size(); ++j )
      grad[j] += grad_tmp[j];
  }

  return grad;
}

bool Neg2LLSum::useAnalyticGradient()
{
  bool b = true;
  for( unsigned int i=0; i<_likList.size(); ++i )
    b = b && _likList[i]->useAnalyticGradient();

  return b;
}

void Neg2LLSum::setUseAnalyticGradient( bool useAnalyticGradient )
{
  for( unsigned int j=0; j<_likList.size(); ++j )
    _likList[j]->setUseAnalyticGradient(useAnalyticGradient);
}
