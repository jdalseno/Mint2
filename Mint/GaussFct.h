#ifndef GAUSS_FCT_HH
#define GAUSS_FCT_HH

namespace MINT
{
  class GaussFct
  {
   public:
    GaussFct();

    GaussFct( const double& x_in, const double& h,
	      const double& m, const double& s );

    double x() const
    { return _x; }

    double height() const
    { return _height; }

    double mean() const
    { return _mean; }

    double sigma() const
    { return _sigma; }

    void set_x( const double& x_in )
    { _x = x_in; }

    void set_height( const double& h )
    { _height=h; }

    void set_mean( const double& m )
    { _mean = m; }

    void set_sigma( const double& s )
    { _sigma = s; }

    double getVal() const;

    double getIntegral() const
    { return height()*_sqrt2pi*sigma(); }

   protected:
    double _x;
    double _height;
    double _mean, _sigma;
    double _sqrt2pi;
  };
} //namespace MINT

#endif //GAUSS_FCT_HH
