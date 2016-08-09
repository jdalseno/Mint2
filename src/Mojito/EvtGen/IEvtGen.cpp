#include "Mint/IEvtGen.h"

using namespace MINT;

IEvtGen::IEvtGen( const std::string& inputFileName,
		  const std::vector<int>& pattern,
		  TRandom* rand, const double& unitConversion )
  : m_unitConversion(unitConversion)
{
  NamedParameterBase::setDefaultInputFile(inputFileName);

  DalitzEventPattern pdg(pattern);
  if( pattern[0] < 0 )
    pdg = pdg.makeCPConjugate();

  std::cout << "Mother Particle " << pattern[0] << std::endl
	    << " got event pattern: " << pdg << std::endl;

  m_sg = new SignalGenerator(pdg, rand);
}

std::vector<std::vector<double>> IEvtGen::GenerateEvent()
{
  MINT::counted_ptr<IDalitzEvent> newEvt = m_sg->newEvent();
  const IDalitzEvent* const event = newEvt.get();
  //const IDalitzEvent* const event = m_sg->newEvent().get();

  std::vector<std::vector<double>> daughters;

  //0th entry is the mother particle
  for( unsigned int i=1;
       i<static_cast<unsigned int>(event->eventPattern().numDaughters()+1);
       ++i ){
    const std::vector<double> daughter = GetDaughter(event, i);
    daughters.push_back(daughter);
  }

  return daughters;
}

std::vector<double> IEvtGen::GetDaughter( const IDalitzEvent* const event,
					  const unsigned int& daughter_id )
{
  std::vector<double> daughter;
  daughter.push_back(event->p(daughter_id).T()/m_unitConversion);
  daughter.push_back(event->p(daughter_id).X()/m_unitConversion);
  daughter.push_back(event->p(daughter_id).Y()/m_unitConversion);
  daughter.push_back(event->p(daughter_id).Z()/m_unitConversion);

  return daughter;
}
