#ifndef SPINFACTORS_FOR_RUNNINGWIDTH_HH
#define SPINFACTORS_FOR_RUNNINGWIDTH_HH
// author: Philippe d'Argent (p.dargent@cern.ch)
// status:  Tu 6 May 2014 

#include "Mint/DalitzEventPattern.h"
#include "Mint/IDalitzEvent.h"
#include "Mint/AssociatedDecayTree.h"
#include "Mint/SpinFactor.h"
#include "Mint/counted_ptr.h"
#include <iostream>

using namespace std;

// ==========================================

class SF_AtoVP1_VtoP2P3_BASE : public SpinFactor{
protected:
    MINT::const_counted_ptr<AssociatedDecayTree> V;
    
    virtual bool parseTree(const DalitzEventPattern& pat);
    
public:
    virtual void printYourself(std::ostream& os=std::cout) const=0;
    SF_AtoVP1_VtoP2P3_BASE(const AssociatedDecayTree& theDecay) 
    : SpinFactor(theDecay, 3)
    , V(0)
    {}
    
    virtual ~SF_AtoVP1_VtoP2P3_BASE(){}
    virtual double getVal(IDalitzEvent& evt)=0;
    virtual std::complex<double> getNewVal(IDalitzEvent& evt)=0;
    
    virtual const DecayTree& exampleDecay()=0;
    virtual std::string name() const{
        return "SpinFactor3:SF_AtoVP1_VtoP2P3_BASE(" 
        + theDecay().oneLiner() + ")";
    }
    
};

class SF_AtoVP1_VtoP2P3 : public SF_AtoVP1_VtoP2P3_BASE{
protected:
  int _pol;
  static DecayTree* _exampleDecay;
 public:
  virtual void printYourself(std::ostream& os=std::cout) const;
  SF_AtoVP1_VtoP2P3(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_AtoVP1_VtoP2P3_BASE(theDecay), _pol(pol)
    {}

  virtual ~SF_AtoVP1_VtoP2P3(){}
  virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
  virtual std::complex<double> getNewVal(IDalitzEvent& evt);

  static const DecayTree& getExampleDecay();
  virtual const DecayTree& exampleDecay();
  virtual std::string name() const{
    return "SpinFactor3:SF_AtoVP1_VtoP2P3(" 
      + theDecay().oneLiner() + ")";
  }
  int getPolarisation() {return _pol;}
};

class SF_AtoVP1_VtoP2P3_D : public SF_AtoVP1_VtoP2P3_BASE{
protected:
    int _pol;
    static DecayTree* _exampleDecay;
public:
    virtual void printYourself(std::ostream& os=std::cout) const;
    SF_AtoVP1_VtoP2P3_D(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_AtoVP1_VtoP2P3_BASE(theDecay), _pol(pol)
    {}
    
    virtual ~SF_AtoVP1_VtoP2P3_D(){}
    virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
    virtual std::complex<double> getNewVal(IDalitzEvent& evt);
    
    static const DecayTree& getExampleDecay();
    virtual const DecayTree& exampleDecay();
    virtual std::string name() const{
        return "SpinFactor3:SF_AtoVP1_VtoP2P3_D(" 
        + theDecay().oneLiner() + ")";
    }
    int getPolarisation() {return _pol;}
};

// ==========================================

class SF_AtoSP1_StoP2P3_BASE : public SpinFactor{
protected:
    MINT::const_counted_ptr<AssociatedDecayTree> S;
    
    virtual bool parseTree(const DalitzEventPattern& pat);
    
public:
    virtual void printYourself(std::ostream& os=std::cout) const=0;
    SF_AtoSP1_StoP2P3_BASE(const DecayTree& theDecay) 
    : SpinFactor(theDecay, 3)
    , S(0)
    {}
    
    virtual ~SF_AtoSP1_StoP2P3_BASE(){}
    virtual double getVal(IDalitzEvent& evt)=0;
    virtual std::complex<double> getNewVal(IDalitzEvent& evt)=0;
    
    virtual const DecayTree& exampleDecay()=0;
    virtual std::string name() const{
        return "SpinFactor3:SF_AtoSP1_StoP2P3_BASE(" 
        + theDecay().oneLiner() + ")";
    }
    
};

class SF_AtoSP1_StoP2P3 : public SF_AtoSP1_StoP2P3_BASE{
protected:
    int _pol;
    static DecayTree* _exampleDecay;
public:
    virtual void printYourself(std::ostream& os=std::cout) const;
    SF_AtoSP1_StoP2P3(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_AtoSP1_StoP2P3_BASE(theDecay), _pol(pol)
    {}
    
    virtual ~SF_AtoSP1_StoP2P3(){}
    virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
    virtual std::complex<double> getNewVal(IDalitzEvent& evt);
    
    static const DecayTree& getExampleDecay();
    virtual const DecayTree& exampleDecay();
    virtual std::string name() const{
        return "SpinFactor3:SF_AtoSP1_StoP2P3(" 
        + theDecay().oneLiner() + ")";
    }
    int getPolarisation() {return _pol;}
};

// ==========================================

class SF_VtoV0P1_V0toP2P3_BASE : public SpinFactor{
protected:
    MINT::const_counted_ptr<AssociatedDecayTree> V0;
    
    virtual bool parseTree(const DalitzEventPattern& pat);
    
public:
    virtual void printYourself(std::ostream& os=std::cout) const=0;
    SF_VtoV0P1_V0toP2P3_BASE(const AssociatedDecayTree& theDecay) 
    : SpinFactor(theDecay, 3)
    , V0(0)
    {}
    
    virtual ~SF_VtoV0P1_V0toP2P3_BASE(){}
    virtual double getVal(IDalitzEvent& evt)=0;
    virtual std::complex<double> getNewVal(IDalitzEvent& evt)=0;
    
    virtual const DecayTree& exampleDecay()=0;
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoV0P1_V0toP2P3_BASE(" 
        + theDecay().oneLiner() + ")";
    }
    
};

class SF_VtoV0P1_V0toP2P3 : public SF_VtoV0P1_V0toP2P3_BASE{
protected:
    int _pol;
    static DecayTree* _exampleDecay;
public:
    virtual void printYourself(std::ostream& os=std::cout) const;
    SF_VtoV0P1_V0toP2P3(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_VtoV0P1_V0toP2P3_BASE(theDecay), _pol(pol)
    {}
    
    virtual ~SF_VtoV0P1_V0toP2P3(){}
    virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
    virtual std::complex<double> getNewVal(IDalitzEvent& evt);
    
    static const DecayTree& getExampleDecay();
    virtual const DecayTree& exampleDecay();
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoV0P1_V0toP2P3(" 
        + theDecay().oneLiner() + ")";
    }
    int getPolarisation() {return _pol;}
};


// ==========================================

class SF_VtoSV0_StoP2P3_BASE : public SpinFactor{
protected:
    MINT::const_counted_ptr<AssociatedDecayTree> S;
    
    virtual bool parseTree(const DalitzEventPattern& pat);
    
public:
    virtual void printYourself(std::ostream& os=std::cout) const=0;
    SF_VtoSV0_StoP2P3_BASE(const AssociatedDecayTree& theDecay) 
    : SpinFactor(theDecay, 3)
    , S(0)
    {}
    
    virtual ~SF_VtoSV0_StoP2P3_BASE(){}
    virtual double getVal(IDalitzEvent& evt)=0;
    virtual std::complex<double> getNewVal(IDalitzEvent& evt)=0;
    
    virtual const DecayTree& exampleDecay()=0;
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoSV0_StoP2P3_BASE(" 
        + theDecay().oneLiner() + ")";
    }
    
};

class SF_VtoSV0_StoP2P3 : public SF_VtoSV0_StoP2P3_BASE{
protected:
    int _pol;
    static DecayTree* _exampleDecay;
public:
    virtual void printYourself(std::ostream& os=std::cout) const;
    SF_VtoSV0_StoP2P3(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_VtoSV0_StoP2P3_BASE(theDecay), _pol(pol)
    {}
    
    virtual ~SF_VtoSV0_StoP2P3(){}
    virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
    virtual std::complex<double> getNewVal(IDalitzEvent& evt);
    
    static const DecayTree& getExampleDecay();
    virtual const DecayTree& exampleDecay();
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoSV0_StoP2P3(" 
        + theDecay().oneLiner() + ")";
    }
    int getPolarisation() {return _pol;}
};

// ==========================================

class SF_VtoAP1_AtoV0P2_BASE : public SpinFactor{
protected:
    MINT::const_counted_ptr<AssociatedDecayTree> A;
    
    virtual bool parseTree(const DalitzEventPattern& pat);
    
public:
    virtual void printYourself(std::ostream& os=std::cout) const=0;
    SF_VtoAP1_AtoV0P2_BASE(const AssociatedDecayTree& theDecay) 
    : SpinFactor(theDecay, 3)
    , A(0)
    {}
    
    virtual ~SF_VtoAP1_AtoV0P2_BASE(){}
    virtual double getVal(IDalitzEvent& evt)=0;
    virtual std::complex<double> getNewVal(IDalitzEvent& evt)=0;
    
    virtual const DecayTree& exampleDecay()=0;
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoAP1_AtoV0P2_BASE(" 
        + theDecay().oneLiner() + ")";
    }
    
};

class SF_VtoAP1_AtoV0P2 : public SF_VtoAP1_AtoV0P2_BASE{
protected:
    int _pol;
    static DecayTree* _exampleDecay;
public:
    virtual void printYourself(std::ostream& os=std::cout) const;
    SF_VtoAP1_AtoV0P2(const AssociatedDecayTree& theDecay, const int pol) 
    : SF_VtoAP1_AtoV0P2_BASE(theDecay), _pol(pol)
    {}
    
    virtual ~SF_VtoAP1_AtoV0P2(){}
    virtual double getVal(IDalitzEvent& evt){return getNewVal(evt).real();}
    virtual std::complex<double> getNewVal(IDalitzEvent& evt);
    
    static const DecayTree& getExampleDecay();
    virtual const DecayTree& exampleDecay();
    virtual std::string name() const{
        return "SpinFactor3:SF_VtoAP1_AtoV0P2(" 
        + theDecay().oneLiner() + ")";
    }
    int getPolarisation() {return _pol;}
};

#endif
//
