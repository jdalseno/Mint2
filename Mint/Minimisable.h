#ifndef MINIMISABLE_HH
#define MINIMISABLE_HH

#include "Mint/IMinimisable.h"
#include "Mint/MinuitParameterSet.h"

namespace MINT
{
  class Minimisable : virtual public IMinimisable
  {
   public:
    Minimisable( MinuitParameterSet* mps=0 );

    Minimisable( const Minimisable& other );

    virtual ~Minimisable()
    {}

    void setPset(MinuitParameterSet* mps);

    MinuitParameterSet* getParSet();

    // you can choose to implement these:
    virtual void beginFit(){}

    virtual void parametersChanged(){}

    virtual void endFit(){}

    // you'll have to implement this:
    double getVal()=0;

    // useful:
    double getNewVal()
    {
      parametersChanged();

      return getVal();
    }

  private:
    MinuitParameterSet* _pset;
  };
} //namespace MINT

#endif //MINIMISABLE_HH
