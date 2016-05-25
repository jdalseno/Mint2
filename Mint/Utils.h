#ifndef UHTILS_HH
#define UHTILS_HH

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "TLorentzVector.h"
#include "TMatrixT.h"
#include "TMatrixTSym.h"

namespace MINT
{
  enum OWNERSHIP{CALLER_PASSES_OWNERSHIP, CALLER_KEEPS_OWNERSHIP};

  template <typename Key, typename Val>
  const Val& keyFinder( const Key& k, const std::map<Key, Val>& m,
			const Val& dummy, bool& successFlag )
  {
    typename std::map<Key, Val>::const_iterator it = m.find(k);

    if(it == m.end()){
      successFlag = false;
      return dummy;
    }else{
      successFlag = true;
      return it->second;
    }
  }

  template <typename Key, typename Val>
  const Val& keyFinder( const Key& k, const std::map<Key, Val>& m,
			const Val& dummy )
  {
    bool successFlag;
    return keyFinder<Key, Val>(k, m, dummy, successFlag);
  }

  template<typename T>
  TMatrixTSym<T> makeTMatrixTSym( const TMatrixT<T>& m )
  {
    TMatrixTSym<T> ms(m.GetNcols());
    for( unsigned int i=0; i<static_cast<unsigned int>(ms.GetNcols()); ++i)
      for( unsigned int j=i; j<static_cast<unsigned int>(ms.GetNcols()); ++j)
	ms(i,j) = ms(j,i) = m(i,j);

    return ms;
  }

  template<typename T>
  void stringToAnything( const std::string& str, T& anything )
  {
    std::stringstream strm(str);
    strm >> anything;
  }

  template<typename T>
  std::string anythingToString( const T& anything )
  {
    std::stringstream strm;
    strm << anything;
    std::string ing;
    strm >> ing;

    return ing;
  }

  template<typename T>
  std::string anyVectorToString( const std::vector<T>& anyV )
  {
    std::stringstream strm;
    for( unsigned int i=0; i<anyV.size(); ++i)
      strm << anyV[i];

    std::string ing;
    strm >> ing;

    return ing;
  }

  std::string stringtime( const double& dt );

  //flagged for modification with internal C++ functions
  int nearestInt(double f);

  bool A_is_in_B( const std::string& a, const std::string& b );

  // need to move these to Dalitz Fitter
  template<typename... INTS>
  int LeviCita( const INTS&... ints )
  {
    const std::vector<int> v({ints...});

    return MINT::LeviCita(v);
  }

  int LeviCita( const std::vector<int>& v );
} //namespace MINT

std::ostream& operator<<( std::ostream& os, const TLorentzVector& v );

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& v )
{
  os << "(";
  for( unsigned int i=0; i<v.size(); ++i ){
    os << v[i];
    if( i + 1 < v.size() )
      os << ", ";
  }

  os << ")";

  return os;
}

#endif //UHTILS_HH
