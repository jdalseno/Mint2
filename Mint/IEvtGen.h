#ifndef IEVTGEN_HH
#define IEVTGEN_HH

#include <string>
#include <vector>

#include "TRandom.h"

#include "Mint/IDalitzEvent.h"
#include "Mint/SignalGenerator.h"

namespace MINT
{
  class IEvtGen
  {
   public:
    /**
       Interface to EvtGen
       Requires Mint steering file as argument
       The pattern should be deduced in the EvtGen model
       The random number generator should point to the EvtGen implementation
       Requires unit conversion rate between Mint and EvtGen as argument
       Mint 4-vectors will be divided by this factor and returned to EvtGen
     */
    IEvtGen( const std::string& inputFileName,
	     const std::vector<int>& pattern,
	     TRandom* rand, const double& unitConversion );

    virtual ~IEvtGen()
    { delete m_sg; }

    /**
       Generate single event in Mint
     */
    void GenerateEvent( std::vector<std::vector<double>>& daughters,
			double& weight_gen );

    /**
       Intermediary to go between Mint and EvtGen 4-vector types
     */
    std::vector<double> GetDaughter( const IDalitzEvent* const event,
				     const unsigned int& daughter_id );

   private:
    const double m_unitConversion;

    SignalGenerator* m_sg;
  };
} //namespace MINT

#endif //IEVTGEN_HH
