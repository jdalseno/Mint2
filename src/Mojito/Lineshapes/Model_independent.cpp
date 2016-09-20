#include "Mint/Model_independent.h"
#include "Mint/CLHEPPhysicalConstants.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TGraph.h"
#include "Mint/Utils.h"
#include "Mint/DalitzEventList.h"
#include "Mint/phaseSpaceIntegrals.h"
#include <cmath>
#include <complex>
#include <cstdlib>
#include "Math/Interpolator.h"
#include "Math/InterpolationTypes.h"

using namespace std;
using namespace MINT;


Model_independent::Model_independent( const AssociatedDecayTree& tree, const std::string& namePrefix): 
  BW_BW(tree, namePrefix)
    , _binCenters("BinCenters_inGeV2")
    , _interpolationTypeString("MI::InterpolationType",(std::string)"kCSPLINE")
    , _interpolationType(ROOT::Math::Interpolation::kCSPLINE)
{
    _binCenterVector = _binCenters.getVector();
    _nBins = _binCenterVector.size();
    if((string)_interpolationTypeString == "kLINEAR")_interpolationType = ROOT::Math::Interpolation::kLINEAR;
    if((string)_interpolationTypeString == "kPOLYNOMIAL")_interpolationType = ROOT::Math::Interpolation::kPOLYNOMIAL;
    if((string)_interpolationTypeString == "kAKIMA")_interpolationType = ROOT::Math::Interpolation::kAKIMA;
    if((string)_interpolationTypeString == "kCSPLINE_PERIODIC")_interpolationType = ROOT::Math::Interpolation::kCSPLINE_PERIODIC;
    if((string)_interpolationTypeString == "kAKIMA_PERIODIC")_interpolationType = ROOT::Math::Interpolation::kAKIMA_PERIODIC;
    
    vector<double> binCenterValues_Re = getBinCenterValues_Re();
    vector<double> binCenterValues_Im = getBinCenterValues_Im();
    _interpolator_Re = new ROOT::Math::Interpolator(_binCenterVector, binCenterValues_Re, _interpolationType);
    _interpolator_Im = new ROOT::Math::Interpolator(_binCenterVector, binCenterValues_Im, _interpolationType);
}

std::vector<double> Model_independent::getBinCenterValues_Re(){
    vector<double> vec;
    for (int i= 0; i<_nBins; i++) {
        vec.push_back(Re_Bin(i));
    }
    return vec;
} 

std::vector<double> Model_independent::getBinCenterValues_Im(){
    vector<double> vec;
    for (int i= 0; i<_nBins; i++) {
        vec.push_back(Im_Bin(i));
    }
    return vec;
} 

std::complex<double> Model_independent::getVal(IDalitzEvent& evt){
    setEventPtr(evt);
    resetInternals();
    
    double formFactor= 1.;
    if( _normBF == 1 ) formFactor = Fr();
    else if( _normBF == 0 ) formFactor = Fr_PDG_BL();  
    else if(_normBF == 2 ) formFactor = Fr_BELLE(0.);  
    
    return formFactor*getInterpolatedVal(mumsRecoMass2()/(GeV*GeV));
}

std::complex<double> Model_independent::getInterpolatedVal(const double s_inGeV2){
    
    if(_binCenterVector[0]>s_inGeV2)return 0;
    if(_binCenterVector[_nBins-1]<s_inGeV2)return 0;
    
    vector<double> binCenterValues_Re = getBinCenterValues_Re();
    vector<double> binCenterValues_Im = getBinCenterValues_Im();
    
    if(changedSinceLastCall()){
        delete _interpolator_Re;
        delete _interpolator_Im;
        _interpolator_Re = new ROOT::Math::Interpolator(_binCenterVector, binCenterValues_Re, _interpolationType);
        _interpolator_Im = new ROOT::Math::Interpolator(_binCenterVector, binCenterValues_Im, _interpolationType);
    }
    return complex<double>(_interpolator_Re->Eval(s_inGeV2),_interpolator_Im->Eval(s_inGeV2));
}


void Model_independent::plot(){
    
    TCanvas* c = new TCanvas();
    
	int n = _nBins;
    vector<double> binCenterValues_Re = getBinCenterValues_Re();
    vector<double> binCenterValues_Im = getBinCenterValues_Im();
    
    std::vector<double> Re(n), Im(n), mag(n), phase(n), delta(n), mass2(n);
	
    for (int i=0; i<n; i++) {
        Re[i] = binCenterValues_Re[i] ;
        Im[i] = binCenterValues_Im[i] ; 
        mag[i] = pow(binCenterValues_Re[i],2) + pow(binCenterValues_Im[i],2);
        delta[i] = 1./2. * atan( (binCenterValues_Im[i] - 1./2.)/binCenterValues_Re[i] ) + TMath::Pi() / 4.;
        phase[i] = arg(complex<double>(Re[i],Im[i]) );
        mass2[i] = _binCenterVector[i];
    }
    
    double m2_start = _binCenterVector[0];
    double step = 0.01;
    int nSteps = ( (_binCenterVector[n-1]) - m2_start)/step;
    std::vector<double> Re_itp(nSteps), Im_itp(nSteps), mag_itp(nSteps), phase_itp(nSteps), delta_itp(nSteps), mass2_itp(nSteps);

    for(int i=0; i<nSteps; i++){
        mass2_itp[i] = m2_start + i * step;
        Re_itp[i] = getInterpolatedVal(mass2_itp[i]).real();
        Im_itp[i] = getInterpolatedVal(mass2_itp[i]).imag() ; 
        mag_itp[i] = pow(Re_itp[i],2) + pow(Im_itp[i],2);
        phase_itp[i] = arg(complex<double>(Re_itp[i],Im_itp[i]) );
        delta_itp[i] = 1./2. * atan( (Im_itp[i] - 1./2.)/Re_itp[i] ) + TMath::Pi() / 4.;
    }

    TGraph* g = new TGraph(n,&Re[0],&Im[0]);
    g->SetTitle("Argand; Re(A) ; Im(A)");
    TGraph* g_itp = new TGraph(nSteps,&Re_itp[0],&Im_itp[0]);
    g_itp->SetLineColor(kRed);                                  
	g->Draw("A*");
    g_itp->Draw("Csame");
	c->Print(("argand_"+anythingToString(mumsPID())+".eps").c_str());    
    delete g;
    delete g_itp;
    
    g = new TGraph(n,&mass2[0],&mag[0]);
    g->SetTitle(" ; s[GeV^{2}] ; |A|^{2}");
    g_itp = new TGraph(nSteps,&mass2_itp[0],&mag_itp[0]);
    g_itp->SetLineColor(kRed);
	g->Draw("A*");
    g_itp->Draw("Csame");
	c->Print(("mag_"+anythingToString(mumsPID())+".eps").c_str());
    delete g;
    delete g_itp;
    
    g = new TGraph(n,&mass2[0],&phase[0]);
    g->SetTitle(" ; s[GeV^{2}] ; arg(A)");
    g_itp = new TGraph(nSteps,&mass2_itp[0],&phase_itp[0]);
    g_itp->SetLineColor(kRed);
	g->Draw("A*");
    g_itp->Draw("Csame");
	c->Print(("phase_"+anythingToString(mumsPID())+".eps").c_str());
    delete g;
    delete g_itp;
    
    g = new TGraph(n,&mass2[0],&delta[0]);
    g->SetTitle(" ; s[GeV^{2}] ; #delta(A)");
    g_itp = new TGraph(nSteps,&mass2_itp[0],&delta_itp[0]);     
    g_itp->SetLineColor(kRed);
	g->Draw("A*");
    g_itp->Draw("Csame");
	c->Print(("delta_"+anythingToString(mumsPID())+".eps").c_str());
    delete g;
    delete g_itp;
}


//
