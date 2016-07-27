#ifndef MINT_POLYMORPH_MAP
#define MINT_POLYMORPH_MAP

#include <map>

namespace MINT{
template<typename Key, typename Val>
class PolymorphMap{
  
 protected:
  std::map<Key, Val> _map;
  
 public:
 PolymorphMap() : _map(){}
 PolymorphMap(const PolymorphMap& other) : _map(other._map){}  
 PolymorphMap(const typename std::map<Key, Val>& other) : _map(other){}

  virtual ~PolymorphMap(){}

  std::map<Key, Val>& theMap(){return _map;}
  const std::map<Key, Val>& theMap()const {return _map;}

  Val& operator[](const Key& k){return _map[k];}
  const Val& operator[](const Key& k) const{return _map[k];}
    
  typename std::map<Key, Val>::iterator begin(){return _map.begin();}
  typename std::map<Key, Val>::const_iterator begin() const{return _map.begin();}

  typename std::map<Key, Val>::iterator end(){return _map.end();}
  typename std::map<Key, Val>::const_iterator end() const{return _map.end();}

  typename std::map<Key, Val>::iterator find(const Key& c){return _map.find(c);}
  typename std::map<Key, Val>::const_iterator find(const Key& c) const{return _map.find(c);}

  void insert(typename std::map<Key, Val>::const_iterator first
	      , typename std::map<Key, Val>::const_iterator last){
    _map.insert(first, last);
  }

  unsigned int size() const{return _map.size();}
  bool empty() const{return _map.empty();}

  PolymorphMap<Key, Val>& operator=(const PolymorphMap& other){
    _map = other._map;
    return *this;
  }

  void clear(){_map.clear();}
  void resize(unsigned int N){_map.resize(N);}
  void resize(unsigned int N, const Val& c){_map.resize(N, c);}
 
  operator const typename  std::map<Key, Val>& ()const{
    return this->theMap();
  }
  operator typename std::map<Key, Val>& (){
    return this->theMap();
  }

  bool operator==(const MINT::PolymorphMap<Key, Val>& v2) const{
    return _map == v2.theMap();
  }
  bool operator!=(const MINT::PolymorphMap<Key, Val>& v2) const{
    return !(_map == v2.theMap());
  }

  bool operator<(const MINT::PolymorphMap<Key, Val>& v2) const{
    return _map < v2.theMap();
  }
  bool operator>(const MINT::PolymorphMap<Key, Val>& v2) const{
    return v2 < _map.theMap();
  }


 };


}// namespace MINT

#endif
