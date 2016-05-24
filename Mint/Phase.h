#ifndef PHASE_HH
#define PHASE_HH

#include <iostream>

#include "Mint/CLHEPPhysicalConstants.h"

namespace MINT
{
  class Phase
  {
   public:
    Phase( const double& ph_in=0 );

    Phase( const Phase& other );

    operator double() const
    { return _ph; }

    static double rangeMax()
    { return _minPh + twopi; }

    static double rangeMin()
    { return _minPh; }

    Phase& operator*=( const double& rhs );

    Phase& operator/=( const double& rhs );

    Phase& operator+=( const Phase& rhs );

    Phase operator+( const Phase& rhs ) const;

    const Phase& operator+ () const
    { return (*this); }

    Phase& operator-=( const Phase& rhs );

    Phase operator-( const Phase& rhs ) const;

    Phase  operator-() const;

    double inDegrees() const
    { return _ph * 180.0/pi; }

    void testPhase( std::ostream& os=std::cout ) const;

   protected:
    double _ph;
    const static double _minPh; // this makes it 0 to 2pi

    void toRange();
  };
} //namespace MINT

#endif //PHASE_HH
