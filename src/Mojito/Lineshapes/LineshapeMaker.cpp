// author: Jonas Rademacker (Jonas.Rademacker@bristol.ac.uk)
// status:  Mon 9 Feb 2009 19:18:04 GMT

#include "Mint/LineshapeMaker.h"
#include "Mint/ILineshape.h"
#include "Mint/Utils.h"
#include "Mint/AssociatedDecayTree.h"
//#include "Mint/IDalitzEventAccess.h"
#include "Mint/BW_BW.h"
#include "Mint/SBW.h"
#include "Mint/FermiPS_BW.h"
#include "Mint/Histo_BW.h"
#include "Mint/GounarisSakurai.h"
#include "Mint/Lass.h"
#include "Mint/Flatte.h"
#include "Mint/NonRes.h"
#include "Mint/Bugg_BW.h"
#include "Mint/RhoOmegaGS.h"
#include "Mint/singleTopHatShape.h"
#include "Mint/Rho0Omega.h"
#include "Mint/Model_independent.h"
#include <iostream>

using namespace std;
using namespace MINT;

/* 
   possible options
   ALWAYS_BW
   RHO_OMEGA
   RhoOmegaGS
   Flatte
   GS
   ExpNonRes
   PowNonRes
   SBW
   FermiPS_BW
   Histo_BW
   Bugg
   Model_independent
*/

ILineshape* LineshapeMaker(const AssociatedDecayTree* tree
			   , const std::string& lineshapePrefix
			   , const std::string& lopt
			   , const std::vector<double>& numOptions 
			   ){
  bool dbThis=true;

  cout << "in LineshapeMaker: lineshapePrefix = " << lineshapePrefix << endl;

  if(0 == tree) return 0;
  
  if(dbThis){
    cout << "LineshapeMaker called with " 
	 << tree->getVal().pdg() 
	 << " lopt = " << lopt << endl;
      cout << "final state size = " << tree->finalState().size() << endl;
    cout << tree->getVal().pdg() << ", "
	 << (abs(tree->getVal().pdg())%1000)
	 << endl;
  }
    
  if(A_is_in_B("MI_"+anythingToString(abs(tree->getVal().pdg())), lopt)){
        if(dbThis) cout << "LineshapeMaker returns Model independent line shape for PID="<<  abs(tree->getVal().pdg())<< endl;
        return new Model_independent(*tree, lineshapePrefix);
  }  
      
  NamedParameter<string> _default3bodyBW("Default3bodyBW", (std::string) "PhaseSpace");
  
  if(tree->finalState().size()==3){
      if(A_is_in_B("SBW_", lopt)){
          if(dbThis) cout << "LineshapeMaker returns SBW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new SBW(*tree, lineshapePrefix);
      }  
      else if(A_is_in_B("FermiPS", lopt)){
          if(dbThis) cout << "LineshapeMaker returns FermiPS_BW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new FermiPS_BW(*tree, lineshapePrefix);
      }    
      else if(A_is_in_B("HistoPS", lopt)){
          if(dbThis) cout << "LineshapeMaker returns Histo_BW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new Histo_BW(*tree, lineshapePrefix);
      }  
      else if(A_is_in_B("SBW_", lopt)){
          if(dbThis) cout << "LineshapeMaker returns SBW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new SBW(*tree, lineshapePrefix);
      }  
      else if(tree->getVal().isNonResonant()){
          if( A_is_in_B("NonRes", lopt) ){
              cout << "LineshapeMaker: "
              << "\n\t> returning Non-resonant lineshape"
              << endl;
              if( A_is_in_B("Exp", lopt) ) return new NonRes(*tree, "Exp", lineshapePrefix);
              else if( A_is_in_B("Pow", lopt) ) return new NonRes(*tree, "Pow", lineshapePrefix);
              else return new NonRes(*tree, "", lineshapePrefix);
          }
      }
      else if((std::string)_default3bodyBW == "PhaseSpace"){
          if(dbThis) cout << "LineshapeMaker returns Histo_BW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new Histo_BW(*tree, lineshapePrefix);
      }  
      else if((std::string)_default3bodyBW == "SBW"){
          if(dbThis) cout << "LineshapeMaker returns SBW for PID="<<  abs(tree->getVal().pdg())<< endl;
          return new SBW(*tree, lineshapePrefix);
      }  
  }
    

  if(A_is_in_B("SBW_"+anythingToString(abs(tree->getVal().pdg())), lopt)){
    if(dbThis) cout << "LineshapeMaker returns SBW for PID="<<  abs(tree->getVal().pdg())<< endl;
    return new SBW(*tree, lineshapePrefix);
  }
    
  if(A_is_in_B("HistoPS_"+anythingToString(abs(tree->getVal().pdg())), lopt)){
    if(dbThis) cout << "LineshapeMaker returns Histo_BW for PID="<<  abs(tree->getVal().pdg())<< endl;
    return new Histo_BW(*tree, lineshapePrefix);
  }

  if(abs(tree->getVal().pdg()) == 9000221 || abs(tree->getVal().pdg()) == 999001 ){ // sigma
        if(A_is_in_B("Bugg", lopt)){
            if(dbThis) cout << "LineshapeMaker: " << "\n\t> returning Bugg lineshape" << endl;
            return new Bugg_BW(*tree, lineshapePrefix);  
        }
  }
    
  if(A_is_in_B("ALWAYS_BW", lopt)){
    if(dbThis) cout << "LineshapeMaker returns BW_BW" << endl;
    return new BW_BW(*tree, lineshapePrefix);
  }

  if((abs(tree->getVal().pdg())%1000)==113){
    if(abs(tree->getVal().pdg()) == 113 && A_is_in_B("RHO_OMEGA", lopt)){
      if(dbThis)cout << "LineshapeMaker returning rho-omega lineshape"<< endl;
      return new Rho0Omega(*tree, lineshapePrefix);
    }else if(A_is_in_B("RhoOmegaGS", lopt)) return new RhoOmegaGS(*tree, lineshapePrefix);    
    else if((abs(tree->getVal().pdg())%1000)==113 && A_is_in_B("GS", lopt)){
      if(dbThis) cout << "LineshapeMaker: return GS lineshape" << endl;
      return new GounarisSakurai(*tree, lineshapePrefix);
    }else{
      if(dbThis)cout << "WARNING: LineshapeMaker:"
		     << " returning plain Breit-Wigner (BW_BW) for rho"
		     << endl;
      return new BW_BW(*tree, lineshapePrefix);
    }
  }else if(abs(tree->getVal().pdg()) == 10321 || abs(tree->getVal().pdg()) == 10311 ){ // K0*(1430), charged or neutral
    if(A_is_in_B("Lass", lopt)){
      cout << "LineshapeMaker: "
	   << "\n\t> returning Lass lineshape"
	   << endl;
      return new Lass(*tree, lineshapePrefix);
    }else{
      cout << "WARNING: LineshapeMaker:"
	   << " returning plain Breit-Wigner (BW_BW) for K0*(1430)"
	   << endl;
      return new BW_BW(*tree, lineshapePrefix);
    }
  }else if(abs(tree->getVal().pdg()) == 9010221 ){ // f0(980)
    if(A_is_in_B("Flatte", lopt)){
      cout << "LineshapeMaker: "
	   << "\n\t> returning Flatte lineshape"
	   << endl;
      return new Flatte(*tree, lineshapePrefix);
    }else{
      cout << "WARNING: LineshapeMaker:"
	   << " returning plain Breit-Wigner (BW_BW) for f0(980)"
	   << endl;
      return new BW_BW(*tree, lineshapePrefix);
    }
  }else if(tree->getVal().isNonResonant()){
      if( A_is_in_B("NonRes", lopt) ){
          cout << "LineshapeMaker: "
          << "\n\t> returning Non-resonant lineshape"
          << endl;
      if( A_is_in_B("Exp", lopt) )
	return new NonRes(*tree, "Exp", lineshapePrefix);
      else if( A_is_in_B("Pow", lopt) )
	return new NonRes(*tree, "Pow", lineshapePrefix);
      else
	return new NonRes(*tree, "", lineshapePrefix);
      }else if(A_is_in_B("TopHats", lopt) && numOptions.size() >= 2){
	return new singleTopHatShape(*tree, numOptions[0], numOptions[1], lineshapePrefix); // options[0]=from, options[1]=to
      }else{
          cout << "WARNING: LineshapeMaker:"
          << " returning plain Breit-Wigner (BW_BW) for non-resonant"
          << endl;
          return new BW_BW(*tree, lineshapePrefix);
    }
  }else{
    if(dbThis) cout << "LineshapeMaker returns BW_BW" << endl;
    return new BW_BW(*tree, lineshapePrefix);
  }

}

//
