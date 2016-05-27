#ifndef IMINIMISABLE_HH
#define IMINIMISABLE_HH

#include <iostream>
#include "Mint/MinuitParameterSet.h"

namespace MINT
{
  class IMinimisable
  {
   public:
    virtual ~IMinimisable()
    {}

    virtual MinuitParameterSet* getParSet() = 0;

    virtual void beginFit()=0;

    virtual void parametersChanged()=0;

    virtual void endFit()=0;

    virtual double getVal()=0;

    virtual double getNewVal()=0;

    // we should make the following three abstract (=0)
    // and move their default definition to Minimisable.
    //move to vector argument
    virtual std::vector<double> Gradient( const std::vector<double>& par )
    {
      std::cout << "Gradient of pdf is not implemented. Please implement me or set useAnalyticGradient to 0 in your options file. I'll crash now. " << std::endl;
      throw "crash";
      (void)par;
    }

    virtual bool useAnalyticGradient()
    { return false; }

    virtual void setUseAnalyticGradient( const bool& useAnalyticGradient )
    { (void)useAnalyticGradient; }
  };
} //namespace MINT

#endif //IMINIMISABLE_HH
