#ifndef MINT_POLYMORPH_VECTOR
#define MINT_POLYMORPH_VECTOR

#include <vector>

namespace MINT{
template<typename T>
class PolymorphVector{
  
 protected:
  std::vector<T> _vec;
  
 public:
 PolymorphVector() : _vec(){}
 PolymorphVector(unsigned int N) : _vec(N){}
 PolymorphVector(unsigned int N, const T& c) : _vec(N, c){}
 PolymorphVector(const PolymorphVector& other) : _vec(other._vec){}  
 PolymorphVector(const typename std::vector<T>& other) : _vec(other){}

  virtual ~PolymorphVector(){}

  std::vector<T>& theVector(){return _vec;}
  const std::vector<T>& theVector()const {return _vec;}


  T& operator[](unsigned int i){return _vec[i];}
  const T& operator[](unsigned int i) const{return _vec[i];}
    
  T& at(unsigned int i){return _vec.at(i);}
  const T& at(unsigned int i) const{return _vec.at(i);}
    
  typename std::vector<T>::iterator begin(){return _vec.begin();}
  typename std::vector<T>::const_iterator begin() const{return _vec.begin();}

  typename std::vector<T>::iterator end(){return _vec.end();}
  typename std::vector<T>::const_iterator end() const{return _vec.end();}

  typename std::vector<T>::iterator find(const T& c){return _vec.find(c);}
  typename std::vector<T>::const_iterator find(const T& c) const{return _vec.find(c);}

  T& front() {return _vec.front();}
  const T& front()const {return _vec.front();}
  T& back() {return _vec.back();}
  const T& back()const {return _vec.back();}
 
  unsigned int size() const{return _vec.size();}
  bool empty() const{return _vec.empty();}


  void push_back(const T& c){_vec.push_back(c);}
  void pop_back(){_vec.pop_back();}

  void erase(typename std::vector<T>::iterator pos){_vec.erase(pos);}
  void erase(typename std::vector<T>::iterator first,
	     typename std::vector<T>::iterator last){
    _vec.erase(first, last);
  }
  
  PolymorphVector<T>& operator=(const PolymorphVector<T>& other){
    _vec = other._vec;
    return *this;
  }

  void clear(){_vec.clear();}
  void resize(unsigned int N){_vec.resize(N);}
  void resize(unsigned int N, const T& c){_vec.resize(N, c);}
 
  operator const typename  std::vector<T>& ()const{
    return this->theVector();
  }
  operator typename std::vector<T>& (){
    return this->theVector();
  }

  bool operator==(const MINT::PolymorphVector<T>& v2) const{
    return _vec == v2.theVector();
  }
  bool operator!=(const MINT::PolymorphVector<T>& v2) const{
    return !(_vec == v2.theVector());
  }
  bool operator<(const MINT::PolymorphVector<T>& v2) const{
    return _vec < v2.theVector();
  }
  bool operator>(const MINT::PolymorphVector<T>& v2) const{
    return v2 < _vec.theVector();
  }
  
 };


}// namespace MINT

#endif
