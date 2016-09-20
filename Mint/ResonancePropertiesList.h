#ifndef RESONANCEPROPERTIESLIST_HH
#define RESONANCEPROPERTIESLIST_HH
// author: Philippe d'Argent (p.dargent@cern.ch)

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <utility>
	
#include "Mint/MinuitParameterSet.h"
#include "Mint/ParticleProperties.h"
#include "Mint/ResonanceProperties.h"
#include "Mint/FitParameter.h"

class ResonancePropertiesList{ // NOT a singleton - but for each prefix (to be used to pre-fix the parameter name) it ensures everyone using it
  // uses the same particle properties, which is initialised 
  // only once.  Saves space and time
  // and (maybe) increases consistency.
  // To get access do:
  // ParticlePropertiesList* PPL = ParticlePropertiesList::getMe();
  // and then:
  // ParticleProperties* pionProps = PPL->get("pion");
  // ParticleProperties* pionProps = PPL->get(221);
  // 

  //  static ResonancePropertiesList* ptr;
  static std::map<std::pair<std::string, const MINT::MinuitParameterSet*>, ResonancePropertiesList*> _mapOfLists;
  ResonancePropertiesList(const std::string& namePrefix="", MINT::MinuitParameterSet* mps=0);

 protected:

  MINT::MinuitParameterSet* _mps;
  std::string _prefix;
  
  MINT::FitParameter _radius;
  //ResonanceProperties _rp;  
  std::vector<ResonanceProperties*> theList;
  //std::map<std::string, std::list<ResonanceProperties>::iterator > byName;
  std::map<int, ResonanceProperties* > byID;
  const ResonanceProperties* AddToList(ResonanceProperties* rp);

  MINT::MinuitParameterSet* getMinuitParameterSet();
  const MINT::MinuitParameterSet* getMinuitParameterSet()const;

 public:
  static ResonancePropertiesList* getMe(const std::string& prefix="", MINT::MinuitParameterSet* mps=0);
  const ResonanceProperties* AddToListIfMissing(int pdg);

  const std::string& prefix()const {return _prefix;}
  //const ResonanceProperties* get(const std::string& name) const;
  const ResonanceProperties* get(int i) const;
  const MINT::FitParameter& radius() const {return _radius;}

  //void print(std::ostream& out=std::cout) const;

    /*
  // fast fuss-free access:
  static double mass(const std::string& name);
  static double mass(int PDG);
  static double width(const std::string& name);
  static double width(int PDG);
  */
};


//std::ostream& operator<<(std::ostream& out, const ParticlePropertiesList& ppl);

#endif
//
