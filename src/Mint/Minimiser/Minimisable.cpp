#include "Mint/Minimisable.h"

using namespace MINT;

Minimisable::Minimisable( MinuitParameterSet* mps )
  : _pset(mps)
{}

Minimisable::Minimisable( const Minimisable& other )
  : IMinimisable()
  , _pset(other._pset)
{}

void Minimisable::setPset( MinuitParameterSet* mps )
{ _pset = mps; }

MinuitParameterSet* Minimisable::getParSet()
{
  if( 0 != _pset ){
    return _pset;
  }else{
    return MinuitParameterSet::getDefaultSet();
  }
}

double Minimisable::getNewVal()
{
  parametersChanged();
  return getVal();
}
