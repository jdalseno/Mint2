// author: Jonas Rademacker (Jonas.Rademacker@bristol.ac.uk)
// status:  Mon 9 Feb 2009 19:17:57 GMT
#include "Mint/Utils.h"

using namespace MINT;

bool MINT::A_is_in_B( const std::string& a, const std::string& b )
{
  unsigned int pos = b.find(a);

  return pos < b.size();
}

int MINT::LeviCita( const std::vector<int>& v )
{
  if(v.size() <=1) return 1;

  std::vector<int> checkMultiples(v.size(), 0);
  int p=1;
  for( unsigned int i=0; i<v.size(); ++i){
    if( v[i] < 0 || v[i] >= (int) v.size() )
      return 0;

    if( ++(checkMultiples[i]) > 1 )
      return 0;
  }

  for( unsigned int i=0; i<v.size()-1; ++i)
    for( unsigned int j=i+1; j<v.size(); ++j)
      if(v[i] > v[j])
	p*= -1;

  return p;
}

std::ostream& operator<<( std::ostream& out, const TLorentzVector& v )
{
  out << "(";
  for( int i=0; i<3; ++i )
    out << v[i] << ", ";

  out << v[3] << ")";

  return out;
}
