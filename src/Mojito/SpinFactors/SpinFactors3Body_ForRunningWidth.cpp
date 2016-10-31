// author: Philippe d'Argent (p.dargent@cern.ch)
// created: 21 Oct 2015
#include "Mint/SpinFactors.h"
#include "Mint/SpinFactors3Body_ForRunningWidth.h"
#include "Mint/DecayTree.h"
#include "Mint/Utils.h"
#include "Mint/CLHEPSystemOfUnits.h"

#include "Mint/ZTspin1.h"
#include "Mint/ZTspin2.h"
#include "Mint/LeviCivita.h"
#include "Mint/SpinSumV.h"
#include "Mint/SpinSumT.h"
#include "Mint/ZTgperp.h"
#include "Mint/SymmLorentzMatrix.h"
#include "Mint/polVector.h"

using namespace std;
using namespace MINT;

DecayTree* SF_AtoVP1_VtoP2P3::_exampleDecay=0;
DecayTree* SF_AtoVP1_VtoP2P3_D::_exampleDecay=0;
DecayTree* SF_AtoSP1_StoP2P3::_exampleDecay=0;

DecayTree* SF_VtoV0P1_V0toP2P3::_exampleDecay=0;
DecayTree* SF_VtoSV0_StoP2P3::_exampleDecay=0;
DecayTree* SF_VtoAP1_AtoV0P2::_exampleDecay=0;

//==================================================================
//==================================================================


const DecayTree& SF_AtoVP1_VtoP2P3::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(20213);
        _exampleDecay->addDgtr(211, 113)->addDgtr(211,-211);
    }
    return *_exampleDecay;
}

const DecayTree& SF_AtoVP1_VtoP2P3::exampleDecay(){
    return getExampleDecay();
}

const DecayTree& SF_AtoVP1_VtoP2P3_D::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(20213);
        _exampleDecay->addDgtr(211, 113)->addDgtr(211,-211);
        _exampleDecay->getVal().setL(2);
    }
    return *_exampleDecay;
}

const DecayTree& SF_AtoVP1_VtoP2P3_D::exampleDecay(){
    return getExampleDecay();
}


bool SF_AtoVP1_VtoP2P3_BASE::parseTree(const DalitzEventPattern& pat){
    bool debugThis=false;
    
    if(fsPS.size() < 3) fsPS.reserve(3);
    for(int i=0; i< theDecay(pat).nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = 
        theDecay(pat).getDgtrTreePtr(i);
        
        if(dgtr->getVal().SVPAT() == "V") V = dgtr;
        else if(dgtr->getVal().SVPAT() == "P") fsPS[0] = dgtr;
    }
    if(0==V || 0==fsPS[0]){
        cout << "ERROR in SF_AtoVP1_VtoP2P3::parseTree"
        << " Didn't find V or P1 " << V.get() << ", " << fsPS[0].get() << endl;
        return false;
    }
    
    if(V->nDgtr() != 2){
        cout << "ERROR in SF_AtoVP1_VtoP2P3::parseTree"
        << " V should have 2 daughters, but it says it has "
        << V->nDgtr() << "."
        << endl;
        return false;
    }
    fsPS[1] = V->getDgtrTreePtr(0);
    fsPS[2] = V->getDgtrTreePtr(1);
    
    // normalOrder(fsPS[2], fsPS[3]);
    
    if(debugThis){
        cout << "parsed Tree: V:\n"
        << *V
        << endl;
        for(int i=0; i<3; i++){
            cout << "fsPS[" << i << "]\n"
            << *(fsPS[i])
            << endl;
        }
    }
    // this->printYourself();
    return true;
}

complex<double> SF_AtoVP1_VtoP2P3::getNewVal(IDalitzEvent& evt){
    bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
    
    TLorentzVector pV = p(1, evt) + p(2, evt);  
    TLorentzVector qV = p(1, evt) - p(2, evt);
    TLorentzVector pA = p(0, evt) + p(1, evt) + p(2, evt); 
    
    double MassV = mRes(V, evt);   
    ZTspin1 LV(qV,pV,MassV);
    polVector e = polVector(pA,pA.M(),getPolarisation());  
       
    complex<double> sfm(LV.Dot(e.Re()),LV.Dot(e.Im()));  
    return sfm/GeV;
    (void)debugThis;
}
void SF_AtoVP1_VtoP2P3::printYourself(ostream& os) const{
    //  bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
    os << "spin factor SF_AtoVP1_VtoP2P3"
    << "\n\t    parsed tree " << theDecay().oneLiner()
    << "\n      like this:" << endl;
    this->printParsing(os);
}
       
       
complex<double> SF_AtoVP1_VtoP2P3_D::getNewVal(IDalitzEvent& evt){
   bool debugThis = false;
   
   if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
      
      TLorentzVector pV = p(1, evt) + p(2, evt);  
      TLorentzVector qV = p(1, evt) - p(2, evt);
      TLorentzVector pA = p(0, evt) + p(1, evt) + p(2, evt); 
      TLorentzVector qA = p(0, evt) - pV; 
      
      double MassV = mRes(V, evt);   
      ZTspin1 LV(qV,pV,MassV);
      ZTspin2 LA(qA,pA,pA.M());
      //SpinSumT PA(pA,pA.M());

      polVector e = polVector(pA,pA.M(),getPolarisation());  
      TLorentzVector tmp = LA.Contract(LV);
      
      complex<double> sfm(tmp.Dot(e.Re()),tmp.Dot(e.Im()));  
      return sfm/(GeV*GeV*GeV);   
      //return complex<double>(PA.Sandwich(e.Re(),LV,qA,qA),PA.Sandwich(e.Im(),LV,qA,qA));
      (void)debugThis;
      }

void SF_AtoVP1_VtoP2P3_D::printYourself(ostream& os) const{
          //  bool debugThis = false;
          
          if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
          os << "spin factor SF_AtoVP1_VtoP2P3_D"
          << "\n\t    parsed tree " << theDecay().oneLiner()
          << "\n      like this:" << endl;
          this->printParsing(os);
}
       
//=========================================================

const DecayTree& SF_AtoSP1_StoP2P3::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(20213);
        _exampleDecay->addDgtr(211, 9010221)->addDgtr(211,-211);
    }
    return *_exampleDecay;
}

const DecayTree& SF_AtoSP1_StoP2P3::exampleDecay(){
    return getExampleDecay();
}

bool SF_AtoSP1_StoP2P3_BASE::parseTree(const DalitzEventPattern& pat){
    bool debugThis=false;
    
    if(fsPS.size() < 3) fsPS.reserve(3);
    for(int i=0; i< theDecay(pat).nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = 
        theDecay(pat).getDgtrTreePtr(i);
        
        if(dgtr->getVal().SVPAT() == "S") S = dgtr;
        else if(dgtr->getVal().SVPAT() == "P") fsPS[0] = dgtr;
    }
    if(0==S || 0==fsPS[0]){
        cout << "ERROR in SF_AtoSP1_StoP2P3::parseTree"
        << " Didn't find S or P1 " << S.get() << ", " << fsPS[0].get() << endl;
        return false;
    }
    
    if(S->nDgtr() != 2){
        cout << "ERROR in SF_AtoSP1_StoP2P3::parseTree"
        << " S should have 2 daughters, but it says it has "
        << S->nDgtr() << "."
        << endl;
        return false;
    }
    fsPS[1] = S->getDgtrTreePtr(0);
    fsPS[2] = S->getDgtrTreePtr(1);
    
    // normalOrder(fsPS[2], fsPS[3]);
    
    if(debugThis){
        cout << "parsed Tree: S:\n"
        << *S
        << endl;
        for(int i=0; i<3; i++){
            cout << "fsPS[" << i << "]\n"
            << *(fsPS[i])
            << endl;
        }
    }
    // this->printYourself();
    return true;
}

complex<double> SF_AtoSP1_StoP2P3::getNewVal(IDalitzEvent& evt){
    bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
    
    TLorentzVector pS = p(1, evt) + p(2, evt);  
    TLorentzVector pA = p(0, evt) + pS; 
    TLorentzVector qA = p(0, evt) - pS; 
    
    polVector e = polVector(pA,pA.M(),getPolarisation());  
    ZTspin1 LA(qA,pA,pA.M());

    complex<double> sfm(LA.Dot(e.Re()),LA.Dot(e.Im()));  
    return sfm/GeV;   
    (void)debugThis;
}

void SF_AtoSP1_StoP2P3::printYourself(ostream& os) const{
    //  bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
    os << "spin factor SF_AtoSP1_StoP2P3"
    << "\n\t    parsed tree " << theDecay().oneLiner()
    << "\n      like this:" << endl;
    this->printParsing(os);
}

//=========================================================

const DecayTree& SF_VtoV0P1_V0toP2P3::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(100323);
        _exampleDecay->addDgtr(321, 113)->addDgtr(211,-211);
    }
    return *_exampleDecay;
}

const DecayTree& SF_VtoV0P1_V0toP2P3::exampleDecay(){
    return getExampleDecay();
}

bool SF_VtoV0P1_V0toP2P3_BASE::parseTree(const DalitzEventPattern& pat){
    bool debugThis=false;
    
    if(fsPS.size() < 3) fsPS.reserve(3);
    for(int i=0; i< theDecay(pat).nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = 
        theDecay(pat).getDgtrTreePtr(i);
        
        if(dgtr->getVal().SVPAT() == "V") V0 = dgtr;
        else if(dgtr->getVal().SVPAT() == "P") fsPS[0] = dgtr;
    }
    if(0==V0 || 0==fsPS[0]){
        cout << "ERROR in SF_VtoV0P1_V0toP2P3::parseTree"
        << " Didn't find V0 or P1 " << V0.get() << ", " << fsPS[0].get() << endl;
        return false;
    }
    
    if(V0->nDgtr() != 2){
        cout << "ERROR in SF_VtoV0P1_V0toP2P3::parseTree"
        << " V0 should have 2 daughters, but it says it has "
        << V0->nDgtr() << "."
        << endl;
        return false;
    }
    fsPS[1] = V0->getDgtrTreePtr(0);
    fsPS[2] = V0->getDgtrTreePtr(1);
    
    // normalOrder(fsPS[2], fsPS[3]);
    
    if(debugThis){
        cout << "parsed Tree: S:\n"
        << *V0
        << endl;
        for(int i=0; i<3; i++){
            cout << "fsPS[" << i << "]\n"
            << *(fsPS[i])
            << endl;
        }
    }
    // this->printYourself();
    return true;
}

complex<double> SF_VtoV0P1_V0toP2P3::getNewVal(IDalitzEvent& evt){
    bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
    
    TLorentzVector pV0 = p(1, evt) + p(2, evt);  
    TLorentzVector qV0 = p(1, evt) - p(2, evt);  
    TLorentzVector pV = p(0, evt) + pV0; 
    TLorentzVector qV = p(0, evt) - pV0; 
    
    polVector e = polVector(pV,pV.M(),getPolarisation());  
    ZTspin1 LV(qV,pV,pV.M());
    ZTspin1 LV0(qV0,pV0,pV0.M());

    complex<double> sfm(LeviCivita(e.Re(),pV,LV,LV0),LeviCivita(e.Im(),pV,LV,LV0));

    return sfm/(GeV*GeV*GeV);   
    (void)debugThis;
}

void SF_VtoV0P1_V0toP2P3::printYourself(ostream& os) const{
    //  bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
    os << "spin factor SF_VtoV0P1_V0toP2P3"
    << "\n\t    parsed tree " << theDecay().oneLiner()
    << "\n      like this:" << endl;
    this->printParsing(os);
}

//=========================================================

const DecayTree& SF_VtoSV0_StoP2P3::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(100443);
        _exampleDecay->addDgtr(443, 9010221)->addDgtr(211,-211);
    }
    return *_exampleDecay;
}

const DecayTree& SF_VtoSV0_StoP2P3::exampleDecay(){
    return getExampleDecay();
}

bool SF_VtoSV0_StoP2P3_BASE::parseTree(const DalitzEventPattern& pat){
    bool debugThis=false;
    
    if(fsPS.size() < 3) fsPS.reserve(3);
    for(int i=0; i< theDecay(pat).nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = 
        theDecay(pat).getDgtrTreePtr(i);
        
        if(dgtr->getVal().SVPAT() == "S") S = dgtr;
        else if(dgtr->getVal().SVPAT() == "V") fsPS[0] = dgtr;
    }
    if(0==S || 0==fsPS[0]){
        cout << "ERROR in SF_VtoSV0_StoP2P3::parseTree"
        << " Didn't find S or V0 " << S.get() << ", " << fsPS[0].get() << endl;
        return false;
    }
    
    if(S->nDgtr() != 2){
        cout << "ERROR in SF_VtoSV0_StoP2P3::parseTree"
        << " S should have 2 daughters, but it says it has "
        << S->nDgtr() << "."
        << endl;
        return false;
    }
    fsPS[1] = S->getDgtrTreePtr(0);
    fsPS[2] = S->getDgtrTreePtr(1);
    
    // normalOrder(fsPS[2], fsPS[3]);
    
    if(debugThis){
        cout << "parsed Tree: S:\n"
        << *S
        << endl;
        for(int i=0; i<3; i++){
            cout << "fsPS[" << i << "]\n"
            << *(fsPS[i])
            << endl;
        }
    }
    // this->printYourself();
    return true;
}

complex<double> SF_VtoSV0_StoP2P3::getNewVal(IDalitzEvent& evt){
    bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
    
    TLorentzVector pV0 = p(0, evt);  
    TLorentzVector pV = p(1, evt) + p(2, evt) + pV0; 
    
    polVector e = polVector(pV,pV.M(),getPolarisation());  
    polVector eV0 = polVector(pV0,pV0.M(),getPolarisation());  
    eV0.conj();   

    return e.Dot(eV0);
    (void)debugThis;
}

void SF_VtoSV0_StoP2P3::printYourself(ostream& os) const{
    //  bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
    os << "spin factor SF_VtoSV0_StoP2P3"
    << "\n\t    parsed tree " << theDecay().oneLiner()
    << "\n      like this:" << endl;
    this->printParsing(os);
}

//=========================================================

const DecayTree& SF_VtoAP1_AtoV0P2::getExampleDecay(){
    if(0==_exampleDecay){
        _exampleDecay = new DecayTree(100443);
        _exampleDecay->addDgtr(321, 120553)->addDgtr(443,-211);
    }
    return *_exampleDecay;
}

const DecayTree& SF_VtoAP1_AtoV0P2::exampleDecay(){
    return getExampleDecay();
}

bool SF_VtoAP1_AtoV0P2_BASE::parseTree(const DalitzEventPattern& pat){
    bool debugThis=false;
    
    if(fsPS.size() < 3) fsPS.reserve(3);
    for(int i=0; i< theDecay(pat).nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = 
        theDecay(pat).getDgtrTreePtr(i);
        
        if(dgtr->getVal().SVPAT() == "A") A = dgtr;
        else if(dgtr->getVal().SVPAT() == "P") fsPS[1] = dgtr;
    }
    if(0==A || 0==fsPS[1]){
        cout << "ERROR in SF_VtoAP1_AtoV0P2::parseTree"
        << " Didn't find A or P1 " << A.get() << ", " << fsPS[1].get() << endl;
        return false;
    }
    
    if(A->nDgtr() != 2){
        cout << "ERROR in SF_VtoAP1_AtoV0P2::parseTree"
        << " A should have 2 daughters, but it says it has "
        << A->nDgtr() << "."
        << endl;
        return false;
    }
    
    
    if(A->getDgtrTreePtr(0)->getVal().SVPAT() == "V") {
            fsPS[0] = A->getDgtrTreePtr(0);
            fsPS[2] = A->getDgtrTreePtr(1);
    }
    else if(A->getDgtrTreePtr(1)->getVal().SVPAT() == "V") {
        fsPS[0] = A->getDgtrTreePtr(1);
        fsPS[2] = A->getDgtrTreePtr(0);
    }
    
    if(0==fsPS[2] || 0==fsPS[0]){
        cout << "ERROR in SF_VtoAP1_AtoV0P2::parseTree"
        << " Didn't find V0 or P2 " << fsPS[0].get() << ", " << fsPS[2].get() << endl;
        return false;
    }
    
    // normalOrder(fsPS[2], fsPS[3]);
    
    if(debugThis){
        cout << "parsed Tree: A:\n"
        << *A
        << endl;
        for(int i=0; i<3; i++){
            cout << "fsPS[" << i << "]\n"
            << *(fsPS[i])
            << endl;
        }
    }
    // this->printYourself();
    return true;
}

complex<double> SF_VtoAP1_AtoV0P2::getNewVal(IDalitzEvent& evt){
    bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2] )) parseTree(evt.eventPattern());
    
    TLorentzVector pV0 = p(0, evt);  
    TLorentzVector pV = p(1, evt) + p(2, evt) + pV0; 
    TLorentzVector pA = p(2, evt) + pV0; 
    
    SpinSumV PA(pA,pA.M());
    
    polVector e = polVector(pV,pV.M(),getPolarisation());  
    polVector eV0 = polVector(pV0,pV0.M(),getPolarisation());  
    eV0.conj();   
    
    TLorentzVectorC tmp( PA.Dot(eV0.Re()) , PA.Dot(eV0.Im()));
    
    return eV0.Dot(tmp);
    (void)debugThis;
  
}

void SF_VtoAP1_AtoV0P2::printYourself(ostream& os) const{
    //  bool debugThis = false;
    
    if(! ( fsPS[0] && fsPS[1] && fsPS[2]) ) return;
    os << "spin factor SF_VtoAP1_AtoV0P2"
    << "\n\t    parsed tree " << theDecay().oneLiner()
    << "\n      like this:" << endl;
    this->printParsing(os);
}


