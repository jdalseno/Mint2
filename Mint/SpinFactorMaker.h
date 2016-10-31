// author: Jonas Rademacker (Jonas.Rademacker@bristol.ac.uk)
// status:  Mon 9 Feb 2009 19:18:13 GMT
#ifndef SPINFACTOR_MAKER
#define SPINFACTOR_MAKER

#include <iostream>
//#include "Mint/DecayTree.h"
#include "Mint/AssociatedDecayTree.h"
#include "Mint/ISpinFactor.h"

void PrintAllSpinFactors(std::ostream& out = std::cout);

ISpinFactor* SpinFactorMaker(const AssociatedDecayTree& thisDcy
			     , char SPD_Wave='?'
			     , const std::string& lopt=""
			    );

ISpinFactor* SpinFactorMaker3Body(const AssociatedDecayTree& thisDcy
                                  , char SPD_Wave='?'
                                  , const std::string& lopt=""
                                  );

ISpinFactor* SpinFactorMaker4Body(const AssociatedDecayTree& thisDcy
				  , char SPD_Wave='?'
				  , const std::string& lopt=""
				  );
#endif
//
