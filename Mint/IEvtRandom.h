#ifndef IEVTRANDOM_HH 
#define IEVTRANDOM_HH

#include "TRandom.h"

namespace MINT
{
  class IEvtRandom
  {
   public:
    virtual ~IEvtRandom()
    {}

    /**
       For the sole purpose of binding to the EvtGen random number generator
       To be implemented in the EvtGen decay model
     */
    virtual double Rndm()=0;
  };

  class EvtTRandom : public TRandom
  {
   public:
    EvtTRandom( MINT::IEvtRandom* rand )
      : m_rand(rand)
    {
      SetName("EvtTRandom");
      SetTitle("Random number generator: EvtTRandom");
    }

    virtual ~EvtTRandom()
    { delete m_rand; }

    virtual Double_t Rndm( Int_t i=0 )
    {
      return m_rand->Rndm();
      (void)i;
    }

  private:
    MINT::IEvtRandom* m_rand;
  };
}

#endif //IEVTRANDOM_HH
