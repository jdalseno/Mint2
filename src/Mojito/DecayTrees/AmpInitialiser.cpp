// author: Jonas Rademacker (Jonas.Rademacker@bristol.ac.uk)
// status:  Mon 9 Feb 2009 19:18:02 GMT

#include <iostream>
#include <algorithm>
#include "Mint/AmpInitialiser.h"
#include "Mint/DecayTreeComparisons.h"
#include "Mint/Utils.h"
#include "Mint/NamedDecayTreeList.h"

using namespace std;
using namespace MINT;

AmpInitialiser::AmpInitialiser()
  : _SPD('?')
  , _valid(false)
  , _lopt("")
  , _numOpts()
  , _autoSwap(true)
  , _index(-1)
{}
AmpInitialiser::AmpInitialiser(const std::string& StandardisedDecayTreeName
			       , const std::string& lopt_in
			       , const std::string& namePrefix
			       , const std::string& lineshapePrefix
			       , const std::vector<double>& numOpts_in
			       , bool autoSwap_in
             , int index_in
			       )
  : _valid(false)
  , _prefix(namePrefix)
  , _lsPrefix(lineshapePrefix)
  , _lopt(lopt_in)
  , _numOpts(numOpts_in)
  , _autoSwap(autoSwap_in)
  , _index(index_in)
{
  setSPD('?');
  const NamedDecayTreeList* ndl = NamedDecayTreeList::getMe();
  if(! ndl){
    cout << "ERROR in constructor of AmpInitialiser from string: "
	 << StandardisedDecayTreeName << endl;
    cout << "Can't find NamedDecayTreeList!" << endl;
  }else{
    bool success=false;
    (*this) = ndl->find(StandardisedDecayTreeName, success);
    if(! success){
      cout << "ERROR in constructor of AmpInitialiser from string: "
	   << StandardisedDecayTreeName << endl;
      cout << "Can't find this decay in List" << endl;
    }
  }
}
AmpInitialiser::AmpInitialiser(const DecayTree& dt_in
			       , char SPD_in
			       , const std::string& lopt_in
			       , const std::string& namePrefix
			       , const std::string& lineshapePrefix
			       , const std::vector<double>& numOpts_in
			       , bool autoSwap_in
             , int index_in
             )
  : _prefix(namePrefix)
  , _lsPrefix(lineshapePrefix)
  , _lopt(lopt_in)
  , _numOpts(numOpts_in)
  , _autoSwap(autoSwap_in)
  , _index(index_in)
{
  setTree(dt_in);
  setSPD(SPD_in);
}
AmpInitialiser::AmpInitialiser(const DecayTree& dt_in
			       , const std::string& lopt_in
			       , const std::string& namePrefix
			       , const std::string& lineshapePrefix
			       , const std::vector<double>& numOpts_in
			       , bool autoSwap_in
             , int index_in
			       )
  : _prefix(namePrefix)
  , _lsPrefix(lineshapePrefix)
  , _lopt(lopt_in)
  , _numOpts(numOpts_in)
  , _autoSwap(autoSwap_in)
  , _index(index_in)
{
  setTree(dt_in);
  setSPD('?');
}

AmpInitialiser::AmpInitialiser(const AmpInitialiser& other, const std::string& addedPrefix, const std::string& addedLineshapePrefix)
  : _SPD(other._SPD)
  , _valid(other._valid)
  , _prefix(addedPrefix + other._prefix)
  , _lsPrefix(addedLineshapePrefix + other._lsPrefix)
  , _lopt(other._lopt)
  , _numOpts(other._numOpts)
  , _autoSwap(other._autoSwap)
  , _index(other._index)
{
  setTree(other._dt);
}
AmpInitialiser& AmpInitialiser::operator=(const AmpInitialiser& rhs){
  setTree(rhs._dt);
  _SPD   = rhs._SPD;
  _valid = rhs._valid;
  _prefix = rhs._prefix;
  _lsPrefix = rhs._lsPrefix;
  _lopt  = rhs._lopt;
  _numOpts = rhs._numOpts;
  _autoSwap = rhs._autoSwap;
  _index = rhs._index;
  return *this;
}

const DecayTree& AmpInitialiser::tree() const{
  return _dt;
}

DalitzEventPattern AmpInitialiser::getEventPattern()const{
  std::vector<const DecayTreeItem*> fs = tree().finalState();

  std::vector<int> fs_int;
  fs_int.push_back(tree().getVal().pdg()); // mum;
  for(unsigned int i=0; i< fs.size(); i++){
    fs_int.push_back(fs[i]->pdg());
  }
  return DalitzEventPattern(fs_int);
}

void AmpInitialiser::setTree(const DecayTree& dt_in){
  _dt = dt_in;
  standardSort(_dt);
  // sorting makes the name derived 
  // from tree unique.
  _valid=true;
}
char AmpInitialiser::SPD() const{
  return _SPD;
}
void AmpInitialiser::setSPD(char SPD_in){
  _SPD = SPD_in;
}

const std::string& AmpInitialiser::lopt() const{
  return _lopt;
}
void AmpInitialiser::setLopt(const std::string& lopt_in){
  _lopt = lopt_in;
}
void AmpInitialiser::addLopt(const std::string& lopt_in){
  if(! A_is_in_B(lopt_in, _lopt)){ 
    _lopt += lopt_in;
  }
}

void AmpInitialiser::setNumOpts(const std::vector<double>& numOpts_in){
  _numOpts = numOpts_in;
}

void AmpInitialiser::setNumOpts(double n1){
  _numOpts.push_back(n1);
}
void AmpInitialiser::setNumOpts(double n1, double n2){
  _numOpts.push_back(n1); _numOpts.push_back(n2);
}

void AmpInitialiser::setNumOpts(double n1, double n2, double n3){
  _numOpts.push_back(n1); _numOpts.push_back(n2); _numOpts.push_back(n3);
}


const std::vector<double>& AmpInitialiser::numOpts() const{
  return _numOpts;
}

bool AmpInitialiser::autoSwap() const{
  return _autoSwap;
}
bool AmpInitialiser::setAutoSwap(bool autoSwap_in) {
  return _autoSwap=autoSwap_in;
}

int AmpInitialiser::index() const{
  return _index;
}

void AmpInitialiser::setIndex(int index_in){
  _index = index_in;
}

std::string AmpInitialiser::indexString()const{
  if(index() < 0){
    return "";
  }else{
    return "_" + anythingToString(index());
  }
}

const std::string&  AmpInitialiser::prefix() const{
  return _prefix;
}
void AmpInitialiser::addPrefix(const std::string& namePrefix){
  _prefix = namePrefix + _prefix;
}

const std::string&  AmpInitialiser::lsPrefix() const{
  return _lsPrefix;
}
void AmpInitialiser::addLsPrefix(const std::string& lineshapePrefix){
  _lsPrefix = lineshapePrefix + _lsPrefix;
}

std::string AmpInitialiser::uniqueName() const{
  if(_valid){
    std::string s = prefix() + _lopt + _dt.oneLiner() + indexString();
    //    if(_SPD != '?') s = s + "_" + _SPD;
    return s;
  }else{
    return "Tree not set - so no name";
  }
}
std::string AmpInitialiser::name() const{
  return uniqueName();
}

