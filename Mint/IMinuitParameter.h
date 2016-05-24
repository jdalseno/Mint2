#ifndef IMINUIT_PARAMETER_HH
#define IMINUIT_PARAMETER_HH

#include <iostream>

namespace MINT
{
  class MinuitParameterSet;

  class IMinuitParameter
  {
   public:
    virtual ~IMinuitParameter(){}

    virtual int iFixInit() const=0;

    virtual bool hidden() const=0;

    virtual const std::string& name() const=0;

    virtual double meanInit() const=0;

    virtual double stepInit() const=0;

    virtual double minInit() const=0;

    virtual double maxInit() const=0;

    virtual bool scan() const=0;

    virtual double scanMin() const=0;

    virtual double scanMax() const=0;

    virtual double mean() const=0;

    virtual double err() const=0;

    virtual const MinuitParameterSet* parSet() const=0;

    virtual int parSetIndex() const=0;

    virtual void setCurrentFitVal(double pval)=0;

    virtual void setResult( double fitMean, double fitErr,
			    double fitErrPos, double fitErrNeg )=0;

    virtual operator double() const=0;

    virtual void print( std::ostream& os = std::cout ) const=0;

    virtual void printResultVsInput( std::ostream& os = std::cout ) const=0;

    friend class MinuitParameterSet;

   protected:
    IMinuitParameter(){};

    virtual bool setParSet( MinuitParameterSet* ps )=0;

    virtual bool setParSetIndex( int psetIndex )=0;
  };
} //namespace MINT

#endif //IMINUIT_PARAMETER_HH
