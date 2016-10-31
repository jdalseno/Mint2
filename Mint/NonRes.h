#ifndef NONRES_LINESHAPE_HH
#define NONRES_LINESHAPE_HH
// author: Nunya Business (ya.mum@bristol.ac.uk)
// status:  Mon 22 Jul 2014 19:17:59 GMT

#include <complex>

#include "Mint/ILineshape.h"
#include "Mint/BW_BW.h"
#include "Mint/NamedParameter.h"
#include "Mint/FitParDependent.h"

class NonRes : public BW_BW, virtual public ILineshape{
 public:
  /**
     Constructor
   */
  NonRes( const AssociatedDecayTree& tree
	  , const std::string& type=""
	  , const std::string& namePrefix=""
	  );

  /**
     Evaluate non-res lineshape
   */
  virtual std::complex<double> getVal( IDalitzEvent& evt );

  /**
     Print decay
   */
  virtual void print( std::ostream& out = std::cout ) const;

  /**
     Print decay
   */
  virtual void print( IDalitzEvent& evt, std::ostream& out = std::cout );

  /**
     Decay Name
   */
  virtual std::string name() const{
    return "NonRes("+_theDecay.oneLiner() +")";
  }

  /**
     Destructor
   */
  virtual ~NonRes(){}

 protected:
  /**
     Return shape parameter alpha
     Not possible to vary this in the fit in the current implementation
   */
  double GetAlpha() const;
  double c1() const {
        return _RPL->get(mumsPID())->c1();
  }
  double c2() const {
        return _RPL->get(mumsPID())->c2();
  }
  double c3() const {
        return _RPL->get(mumsPID())->c3();
  }
  double c4() const {
        return _RPL->get(mumsPID())->c4();
  }
  double c5() const {
        return _RPL->get(mumsPID())->c5();
  }
  const std::string _type;
  //const MINT::NamedParameter<double> _alpha;
};

std::ostream& operator<<( std::ostream& out, const NonRes& amp );

#endif
//
