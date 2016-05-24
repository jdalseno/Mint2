#include <cmath>

#include "Mint/CLHEPPhysicalConstants.h"
#include "Mint/GaussFct.h"

using namespace MINT;

GaussFct::GaussFct()
  : _x(0.0)
  , _height(1./sqrt(twopi))
  , _mean(0.0)
  , _sigma(1.0)
  , _sqrt2pi(sqrt(twopi))
{}

GaussFct::GaussFct( const double& x_in, const double& h,
		    const double& m, const double& s )
  : _x(x_in)
  , _height(h)
  , _mean(m)
  , _sigma(s)
  , _sqrt2pi(sqrt(twopi))
{}

double GaussFct::getVal() const
{
  const double dxs = (x()-mean())/sigma();

  return height() * exp(-0.5*dxs*dxs);
}
