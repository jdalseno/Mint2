// author: Philippe d'Argent (p.dargent@çern.ch)
#include "Mint/ResonancePropertiesList.h"
#include "Mint/ParticlePropertiesList.h"
#include "Mint/NamedParameter.h"
#include "Mint/CLHEPSystemOfUnits.h"

#include <string>
#include <fstream>
//#include <sstream>
#include <cstdio>
#include <stdlib.h>

using namespace std;
using namespace MINT;

//ResonancePropertiesList* ResonancePropertiesList::ptr=0;

std::map<std::pair<std::string, const MINT::MinuitParameterSet*>, ResonancePropertiesList*> ResonancePropertiesList::_mapOfLists;

ResonancePropertiesList* ResonancePropertiesList::getMe(const std::string& namePrefix, MinuitParameterSet* mps) {
  //cout << "Hello from  ResonancePropertiesList::getMe()" << endl;
  ResonancePropertiesList* ptr(0);
  if(0 == mps) mps = MinuitParameterSet::getDefaultSet();
  std::pair<std::string, const MINT::MinuitParameterSet*> nameAndSet(namePrefix, mps);
 
  //  auto it=_mapOfLists.find(nameAndSet);
  map<pair<string, const MinuitParameterSet*>, ResonancePropertiesList*>::iterator it=_mapOfLists.find(nameAndSet);

  if(_mapOfLists.end() != it) ptr=it->second;
  if(0 == ptr){
    //cout << "ResonancePropertiesList: making myself" << endl;
    ptr = new ResonancePropertiesList(namePrefix, mps);
  }
  if(0 == ptr){
    cout << "ERROR in ResonanceProperties::getMe( " << namePrefix << " )"
	 << " Couldn't get ResonancePropertiesList (i.e. myself)"
	 << "\n\t This is a serious problem. Will crash."
	 << endl;
    throw "No ResonancePropertiesList";
  }
  _mapOfLists[nameAndSet] = ptr;

  return ptr;
}

/*
double ParticlePropertiesList::mass(int pid) {
  const ResonanceProperties* props = getMe()->get(pid);
  if(0 == props){
    printCannotFindWarning("mass", pid);
    return -9999;
  }    
  return props->mass();
}


double ParticlePropertiesList::width(int pid) {
  const ParticleProperties* props = getMe()->get(pid);
  if(0 == props){
    printCannotFindWarning("width", pid);
    return -9999;
  }    
  return props->width();
}
*/


ResonancePropertiesList::ResonancePropertiesList(const std::string& namePrefix, MinuitParameterSet* mps)
  : _mps(mps)
  , _prefix(namePrefix)
  , _radius(namePrefix + "BW_radius", 1, 1.5/GeV, 0, 0, 0, mps,  NamedParameterBase::QUIET) //, _rp(10323)
{   

  /*
  cout << "Hello from ResonancePropertiesList::ResonancePropertiesList" << endl;
  cout << "I think I initialised _radius" << endl;
  cout << "here we go" << endl;
  cout << _radius << endl;
  */

    //fix BW radius to default value if not explicitly initiallised 
    //if(_radius.iFixInit() && _radius== 0) _radius = 1.5/GeV;

    /*
    const ParticlePropertiesList* PPL = ParticlePropertiesList::getMe();
    const std::list<ParticleProperties> theParticleList = PPL->getList();
    for(std::list<ParticleProperties>::const_iterator it= theParticleList.begin();it != theParticleList.end(); it++){
        if(it->pdg_id>0)
        ResonanceProperties* rp= new ResonanceProperties(it->pdg_id());
        theList.push_back(rp);
    }
    */
}
const MINT::MinuitParameterSet* ResonancePropertiesList::getMinuitParameterSet()const{
  return _mps;
}
MINT::MinuitParameterSet* ResonancePropertiesList::getMinuitParameterSet(){
  if(0 == _mps) _mps = MinuitParameterSet::getDefaultSet();
  return _mps;
}

const ResonanceProperties* ResonancePropertiesList::AddToList(ResonanceProperties* rp){
    //theList.push_back(rp);
  if(0 != rp) byID[rp->pid()] = rp;
  return rp;
}

const ResonanceProperties* ResonancePropertiesList::AddToListIfMissing(int pdg_id){
  const ResonanceProperties* rp = this->get(pdg_id);
  if(0 == rp){
    ResonanceProperties* rpnew= new ResonanceProperties(pdg_id, getMinuitParameterSet(), prefix());
    this->AddToList(rpnew);
    rp=rpnew;
  }
  return rp;
}


const ResonanceProperties* ResonancePropertiesList::get(int id_in) const{
  // assume CPT holds
  int id = abs(id_in);
  std::map<int, ResonanceProperties*>::const_iterator it = byID.find(id);
  if(it == byID.end()) return 0;
  return it->second;
}



/*
const ParticleProperties* ParticlePropertiesList::get(const std::string& name) const{
     std::map<std::string, std::list<ParticleProperties>::iterator >::const_iterator it 
       = byName.find(name);
     if(it == byName.end()) return 0;
     
     return &(*(it->second));
}
const ParticleProperties* ParticlePropertiesList::get(int id) const{
     std::map<int, std::list<ParticleProperties>::iterator >::const_iterator it 
       = byID.find(id);
     if(it == byID.end()) return 0;
     
     return &(*(it->second));
}
*/


//
