#ifndef MINT_TWO_D_ARRAY_HH
#define MINT_TWO_D_ARRAY_HH

#include <iostream>

/*
MINT::TwoDArray is a simple template class that implements a
dynamically allocated array in such a way that it ensures that if you
pass it to any function that expects a simple C 2-D array, it will
work as such, i.e.

MINT::TwoDArray<double> A(N, N);
this->mnemat(&A[0][0], A) 

will work. Note this is not guaranteed with 
double A[N,N]  (when N is not a const)
and certainly does not work with
std::vector<std::vector<double>>
   A(N, std::vector<double>(M));

Use like this

MINT::TwoDArray<double> A(N, M);
or
MINT::TwoDArray<double> A(N, M, 0); 
to initialise with 0.
Then like a normal array:
A[i][j] = 17.0
etc.
*/

namespace MINT{

  template<typename T> 
    class TwoDArray{
    T* _m;
    unsigned int _Nrow;
    unsigned int _Ncol;
   public:
    unsigned int Nrow() const{return _Nrow;}
    unsigned int Ncol() const{return _Ncol;}
    unsigned int size() const{return _Nrow*_Ncol;}

  TwoDArray(unsigned int Nrow, unsigned int Ncol)
    : _m(0), _Nrow(Nrow), _Ncol(Ncol){
      _m = new T[size()];
    }
  TwoDArray(unsigned int Nrow, unsigned int Ncol, T initValue) 
    : _m(0), _Nrow(Nrow), _Ncol(Ncol){
      _m = new T[size()];
      for(unsigned int i=0; i < size(); i++) _m[i]=initValue;
    }
  TwoDArray(const TwoDArray& other) 
    : _m(0), _Nrow(other._Nrow), _Ncol(other._Ncol){
      _m = new T[size()];
      for(unsigned int i = 0; i < size(); i++){
	_m[i] = other._m[i];
      }
    }

    ~TwoDArray(){if(0 != _m) delete[] _m;}
    
    T* operator[](unsigned int row){
      unsigned int n = row * _Ncol;
      if(n >= size()){
	std::cout << "array size error in Mint::TwoDArray" 
		  << " row = " << row <<  ", n = " << n << std::endl;
	throw "crap";
      }
      return &(_m[n]);
    }
    const T* operator[](unsigned int row) const{
      unsigned int n = row * _Ncol;
      if(n >= size()){
	std::cout << "array size error in Mint::TwoDArray" 
		  << " row = " << row <<  ", n = " << n << std::endl;
	throw "crap";
      }
      return &(_m[n]);
    }

    T* c_array(){
      return _m;
    }
    const T* c_array() const{
      return _m;
    }
  };

} // namespace MINT

#endif
