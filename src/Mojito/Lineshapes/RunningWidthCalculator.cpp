// author: Philippe d'Argent (p.dargent@cern.ch)
// created: 21 Oct 2015
#include "Mint/CLHEPPhysicalConstants.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"
#include "Mint/Utils.h"
#include "Mint/DalitzEventList.h"
#include "Mint/RunningWidthCalculator.h"
#include "Mint/SignalGenerator.h"
#include "Mint/ResonanceProperties.h"
#include "Mint/ResonancePropertiesList.h"
#include "Mint/BW_BW.h"
#include "Mint/phaseSpaceIntegrals.h"
#include <cmath>
#include <complex>
#include "Math/Integrator.h"
#include "Math/Functor.h"
#include "Math/GSLIntegrator.h"
#include "Mint/Minimisable.h"
#include "Mint/Minimiser.h"

using namespace std;
using namespace MINT;


void RunningWidthCalculator::set_min_s_inGeV2(){
    // Calculate lower phase space limit
    double min_m = 0.;
    for(unsigned int i= 1; i<_pat.size(); i++){
        min_m += _pat[i].mass()/GeV;
    }
    _min_s_inGeV2 = min_m*min_m;
}

RunningWidthCalculator::RunningWidthCalculator(const DalitzEventPattern& pat):
    _pat(pat)
{
  cout << "RunningWidthCalculator::RunningWidthCalculator: I got called." << endl;
    set_min_s_inGeV2();
}

void RunningWidthCalculator::setDalitzEventPattern(const DalitzEventPattern& pat){
    _pat = pat;
    set_min_s_inGeV2();
}


TH1D* RunningWidthCalculator::makeHisto_dalitz(int nBins, double max_s_inGeV2, int nIntegrationEvents, IFastAmplitudeIntegrable* amps, string OutputFileName){
    
    if(_pat.size() != 4) {
        cout << "ERROR in RunningWidthCalculator::makeHisto_dalitz: I can handle only 3 body decays but the pattern " << _pat 
        << " has " << _pat.size()-1 << " final state particles. I'll return 0." << endl;
        return 0;    
    }
    
    if(amps==0) amps = new FitAmpSum(_pat);
    //Important! Ensures everything is initialised
    DalitzEventList eventTest;
    eventTest.generatePhaseSpaceEvents(1,_pat);
    amps->RealVal(eventTest[0]);  
    
    if(OutputFileName=="")
        OutputFileName = "RunningWidth_"+ParticlePropertiesList::getMe()->get(abs((int)_pat[0]))->name()+"_Dalitz.root";
    
    TFile* f= new TFile(OutputFileName.c_str(),"RECREATE");     
    TH1D* h= new TH1D("RunningWidth","RunningWidth",nBins,_min_s_inGeV2,max_s_inGeV2); 
      
    const ParticleProperties* pp = ParticlePropertiesList::getMe()->get((int)_pat[0]);
    const ResonanceProperties* rp = ResonancePropertiesList::getMe()->get((int)_pat[0]);
        
    for (int b=1; b<= h->GetNbinsX(); b++) {  
        // Set mass to new value in ResonancePropertiesList
        // and in ParticlePropertiesList !
        // This is necessary since SignalGenerator uses the mass in ParticlePropertiesList
        // but it should use the mass in ResonancePropertiesList. Fix some day.
        rp->changeMassForDebug(sqrt(h->GetXaxis()->GetBinCenter(b))*GeV);
        pp->setMass(sqrt(h->GetXaxis()->GetBinCenter(b))*GeV);
        
        // Generate integration events
        SignalGenerator sg(_pat,amps);
        sg.setWeighted();
        sg.noPrintout();
        DalitzEventList eventList;
        // Maybe we should somehow scale the number of integration events with mass, 
        // i.e. the available phase space ?
        sg.FillEventList(eventList, nIntegrationEvents);

        // Calculate the integral
        double integral = 0.;
        double sumw = 0.;
        for (unsigned int i= 0; i< eventList.size(); i++) {
            integral += amps->RealVal(eventList[i])*eventList[i].getWeight()/(eventList[i].getGeneratorPdfRelativeToPhaseSpace());
            sumw += eventList[i].getWeight()/(eventList[i].getGeneratorPdfRelativeToPhaseSpace());
        }
        
        PhaseSpaceIntegral3body ps;
        double phaseSpace = ps.getVal(_pat);
                
        h->SetBinContent(b,integral*phaseSpace/sumw);    
    }

    h->Scale(h->GetNbinsX()/h->Integral());
    
    h->Write();
    f->Write();  
    
    return h;
}


Double_t Bl_2(double q, double r, int l){
    
    double z= q*r;
    if(l==1)return (2*z*z)/(1.+z*z);
    if(l==2)return (13*pow(z,4))/(9.+3.*z*z+pow(z,4));
    else return 1.;
}

Double_t Q(double M, double m1, double m2)
{
    Double_t q2 = (M*M-(m1+m2)*(m1+m2) ) * (M*M-(m1-m2)*(m1-m2) ) / (4.*M*M) ;  
    if(q2<0){
        cout << " q2 = " << q2 << " M = " << M << " m1 = " << m1 << " m2 = " << m2 << endl;
        q2=0;
        
    }
    return sqrt(q2); 
}

Double_t Gamma_2body(double M, double m1, double m2, int l, double r){
    
    double q = Q(M,m1,m2);
    
    double bl_2 = Bl_2(q,r,l); 
    
    return q/M * bl_2;
}

Double_t Gamma_2body(Double_t *x, Double_t *par){
    if(sqrt(x[0])*GeV< (par[0]+par[1]))return 0;
    return Gamma_2body(sqrt(x[0])*GeV,par[0],par[1],(int)par[2],par[3]);
}

Double_t BW_resonance(double m12, double m1, double m2, int l, double mass , double width , double r ){
    double gamma = Gamma_2body(m12,m1,m2,l,r);
    double gamma0 = Gamma_2body(mass,m1,m2,l,r);
    
    gamma= width*gamma/gamma0;
    
    return m12*gamma/((mass*mass-m12*m12)*(mass*mass-m12*m12)+(mass*gamma)*(mass*gamma));
}

Double_t Gamma_mother_3body_byM12(Double_t *x, Double_t *par){
    
    //3 body decay : X -> (R->1 2) 3
    //par[0]: mother (X) radius
    //x[0]: m12, to be integrated over
    //par[i]: final state masses, i=1,2,3
    //par[4]: angular momentum in X -> R 3
    //par[5]: angular momentum in R -> 1 2
    //par[6]: resonance (R) mass
    //par[7]: resonance (R) width
    //par[8]: resonance (X) radius
    //par[10]: mother (X) mass

    return Gamma_2body(par[10],x[0],par[3],par[4],par[0])*BW_resonance(x[0],par[1],par[2],par[5],par[6],par[7],par[8])*x[0];
}

Double_t Gamma_mother_3body(Double_t *x, Double_t *par){
    
    const int nPar = 10;
    const int nPar_new = 11;
    double min_s12 = par[1]+par[2];
    double bachelorMass = par[3] ; 
    
    // Copy parameters
    Double_t par_new[nPar_new];
    for (int i=0; i< nPar; i++) {
        par_new[i] = par[i];
    }
    // Add m(1,2,3) as parameter
    par_new[10] = sqrt(x[0])*GeV;
    
    if(sqrt(x[0])*GeV-bachelorMass<min_s12)return 0.;
    
    TF1 *gamma_byS12 = new TF1("Gamma_mother_3body_byM12",Gamma_mother_3body_byM12,min_s12,par[9],nPar_new);
    gamma_byS12->SetParameters(par_new);
    double gamma  = gamma_byS12->Integral(min_s12, sqrt(x[0])*GeV-bachelorMass);
      
    return gamma;    
}


TH1D* RunningWidthCalculator::makeHisto_3body(int nBins, double max_s_inGeV2, const DecayTree& thisDcy, string OutputFileName){
     
     bool dbThis=false;
    
     // Some safety checks
     if(thisDcy.finalState().size() != 3){
         cout << "ERROR in RunningWidthCalculator::makeHisto_3body: I can handle only 3 body decays but the decay " << thisDcy 
         << " has " << thisDcy.finalState().size() << " final state particles. I'll return 0." << endl;
         return 0;
     }
    
     if(thisDcy.nDgtr() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body: "
        << " Mother should have 2 daughters, but it says it has "
        << thisDcy.nDgtr() << ". I'll return 0." << endl;
        return 0;
     }
     
     if( ! _pat.compatibleWith(thisDcy)){
         cout << "ERROR in RunningWidthCalculator::makeHisto_3body:"
         << "DalitzEventPattern and DecayTree are not compatible. I'll return 0." << endl;
         return 0;
     }
    
     // Everything is fine, now search the resonance
     MINT::const_counted_ptr<AssociatedDecayTree> R, bachelor;
     for(int i=0; i< thisDcy.nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = thisDcy.getDgtrTreePtr(i);
        if(dgtr->nDgtr() == 2) R = dgtr;
        else bachelor = dgtr; 
     }
    
     if(0==R || 0== bachelor){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body:"
        << " Didn't find resonance or bachelor. I'll return 0.  " << endl;
        return 0;
     }
    
     // Use the BW_BW class to add resonances to ResonanceProperitiesList 
     // and to get acces to angular momentum 
     AssociatingDecayTree associatingDecayTreeX(thisDcy);
     const AssociatedDecayTree X = associatingDecayTreeX.getTree(_pat);
    
     BW_BW BW_X(X);
     BW_BW BW_R(*R);
    
     // Define all parameters
     // 3 body decay : X -> (R->1 2) 3
     const int nPar = 10;
     Double_t par[nPar];
    
     // Mother (X) radius
     par[0] = ResonancePropertiesList::getMe()->get((int)_pat[0])->radius();
     // Final state (1,2,3) masses:   
     par[1] = ParticlePropertiesList::getMe()->get((int) R->getDgtrTreePtr(0)->getVal())->mass();
     par[2] = ParticlePropertiesList::getMe()->get((int) R->getDgtrTreePtr(1)->getVal())->mass();
     par[3] = ParticlePropertiesList::getMe()->get((int) bachelor->getVal())->mass();
     // Angular momentum in X -> R 3
     par[4] = (BW_X.twoLPlusOne()-1)/2; 
     // Angular momentum in R -> 1 2
     par[5] = (BW_R.twoLPlusOne()-1)/2; 
     // Resonance (R) mass, width and radius
     par[6] = ResonancePropertiesList::getMe()->get((int) R->getVal())->mass();
     par[7] = ResonancePropertiesList::getMe()->get((int) R->getVal())->width();
     par[8] = ResonancePropertiesList::getMe()->get((int) R->getVal())->radius();
     // Upper limit of s(1,2,3)
     par[9] = max_s_inGeV2 * GeV *GeV;
    
     if(dbThis) for(int i=0; i<nPar; i++) cout << "Parameter " << i << " = " << par[i] << endl;
     
    if(OutputFileName=="")
        OutputFileName = "RunningWidth_"+ParticlePropertiesList::getMe()->get(abs((int)_pat[0]))->name()+"_3body.root";
     TFile* f= new TFile(OutputFileName.c_str(),"RECREATE");  
     
     TF1 *gamma = new TF1("Gamma_mother_3body",Gamma_mother_3body,0,max_s_inGeV2,nPar);
     gamma->SetParameters(par); 
     
     TH1D* h= new TH1D("RunningWidth",";s [GeV^2]; #Gamma (s) [a.u.]",nBins,0,max_s_inGeV2); 
     for (int i=1; i<= h->GetNbinsX(); i++) {
         h->SetBinContent(i,gamma->Eval(h->GetXaxis()->GetBinCenter(i) ));    
     }  
     
     //h->Scale(1./h->Integral());    
     h->Write();
     f->Write();  
     return h;
}


TF1* RunningWidthCalculator::getRunningWidthFunction_3body(double max_s_inGeV2, const DecayTree& thisDcy){

    bool dbThis=false;
    
    // Some safety checks
    if(thisDcy.finalState().size() != 3){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body: I can handle only 3 body decays but the decay " << thisDcy 
        << " has " << thisDcy.finalState().size() << " final state particles. I'll return 0." << endl;
        return 0;
    }
    
    if(thisDcy.nDgtr() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body: "
        << " Mother should have 2 daughters, but it says it has "
        << thisDcy.nDgtr() << ". I'll return 0." << endl;
        return 0;
    }
    
    if( ! _pat.compatibleWith(thisDcy)){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body:"
        << "DalitzEventPattern and DecayTree are not compatible. I'll return 0." << endl;
        return 0;
    }
    
    // Everything is fine, now search the resonance
    MINT::const_counted_ptr<AssociatedDecayTree> R, bachelor;
    for(int i=0; i< thisDcy.nDgtr(); i++){
        const_counted_ptr<AssociatedDecayTree> dgtr = thisDcy.getDgtrTreePtr(i);
        if(dgtr->nDgtr() == 2) R = dgtr;
        else bachelor = dgtr; 
    }
    
    if(0==R || 0== bachelor){
        cout << "ERROR in RunningWidthCalculator::makeHisto_3body:"
        << " Didn't find resonance or bachelor. I'll return 0.  " << endl;
        return 0;
    }
    
    // Use the BW_BW class to add resonances to ResonanceProperitiesList 
    // and to get acces to angular momentum 
    AssociatingDecayTree associatingDecayTreeX(thisDcy);
    const AssociatedDecayTree X = associatingDecayTreeX.getTree(_pat);
    
    BW_BW BW_X(X);
    BW_BW BW_R(*R);
    
    // Define all parameters
    // 3 body decay : X -> (R->1 2) 3
    const int nPar = 10;
    Double_t par[nPar];
    
    // Mother (X) radius
    par[0] = ResonancePropertiesList::getMe()->get((int)_pat[0])->radius();
    // Final state (1,2,3) masses:   
    par[1] = ParticlePropertiesList::getMe()->get((int) R->getDgtrTreePtr(0)->getVal())->mass();
    par[2] = ParticlePropertiesList::getMe()->get((int) R->getDgtrTreePtr(1)->getVal())->mass();
    par[3] = ParticlePropertiesList::getMe()->get((int) bachelor->getVal())->mass();
    // Angular momentum in X -> R 3
    par[4] = (BW_X.twoLPlusOne()-1)/2; 
    // Angular momentum in R -> 1 2
    par[5] = (BW_R.twoLPlusOne()-1)/2; 
    // Resonance (R) mass, width and radius
    par[6] = ResonancePropertiesList::getMe()->get((int) R->getVal())->mass();
    par[7] = ResonancePropertiesList::getMe()->get((int) R->getVal())->width();
    par[8] = ResonancePropertiesList::getMe()->get((int) R->getVal())->radius();
    // Upper limit of s(1,2,3)
    par[9] = max_s_inGeV2 * GeV *GeV;
    
    if(dbThis) for(int i=0; i<nPar; i++) cout << "Parameter " << i << " = " << par[i] << endl;
    
    TF1 *gamma = new TF1("Gamma_mother_3body",Gamma_mother_3body,_min_s_inGeV2,max_s_inGeV2,nPar);
    gamma->SetParameters(par); 
    
    return gamma;
}


class BF_Integrand{
    int _index;
    double _m0;
    double _gamma0;
    std::vector<TF1*> _partialWidths;
    std::vector<FitParameter*> _fit_couplings;

public:
    BF_Integrand(std::vector<TF1*> partialWidths, std::vector<FitParameter*> fit_couplings, double m0, double gamma0):
        _index(0),_m0(m0), _gamma0(gamma0), _partialWidths(partialWidths), _fit_couplings(fit_couplings){}
    
    double operator()(Double_t* x, Double_t* par=0){ 
        return Eval(x[0]);
	(void)par;
    }
    
    double operator()(double x){
        return Eval(x);
    }
    
    double Eval(Double_t x){
        double totalWidth = 0.;
        double totalWidth_norm = 0.;
        
        double sum = 0.;
        for (unsigned int i=1; i<_fit_couplings.size(); i++) {
            sum += *_fit_couplings[i];
        }
                
        for (unsigned int i=0; i<_partialWidths.size(); i++) {
            double c;
            if(i==0) c = *_fit_couplings[0];//* (1.-sum);
            else c = *_fit_couplings[0]* *_fit_couplings[i];
            totalWidth += c * _partialWidths[i]->Eval(x);
            totalWidth_norm += c* _partialWidths[i]->Eval(_m0*_m0/(GeV*GeV));
        }
        totalWidth = totalWidth/totalWidth_norm * _gamma0/GeV;
        
        double c;
        if(_index==0) c = *_fit_couplings[0];//* (1.-sum);
        else c = *_fit_couplings[0]* *_fit_couplings[_index];
        
        return (c * _partialWidths[_index]->Eval(x))/ ( pow((x-_m0*_m0/(GeV*GeV)),2) +_m0*_m0/(GeV*GeV)*totalWidth*totalWidth);
    }
    
    void setIndex(const int i){
        _index = i;
    }
};


class chi2BF : public Minimisable{
   double _precision;
   std::vector<double> _BF; 
   double _m0;
   double _gamma0;
   double _s_min;
   double _s_max;
   std::vector<TF1*> _partialWidths;
   std::vector<FitParameter*> _fit_couplings; 
   
public:
    chi2BF(std::vector<double> BF, std::vector<TF1*> partialWidths, std::vector<FitParameter*> fit_couplings, double m0, double gamma0, double precision = 0.001):
        _precision(precision), _BF(BF), _m0(m0), _gamma0(gamma0), _partialWidths(partialWidths), _fit_couplings(fit_couplings)
    {
            _s_min= 0;
            _s_max= pow( (_m0+10*_gamma0)/GeV , 2);
    }
    double getVal(){
        BF_Integrand integrand(_partialWidths,_fit_couplings,_m0,_gamma0);
        double sum = 0.;
        vector<double> integrals;
        for (unsigned int i=0; i < _BF.size() ; i++) {
             integrand.setIndex(i);
             ROOT::Math::IntegratorOneDim integrator(integrand) ;
             double integral = integrator.Integral( _s_min, _s_max );
             integrals.push_back(integral);
        }
        double norm = 0.;
        for (unsigned int i=0; i < _BF.size() ; i++) {
            norm += integrals[i];
        }
        for (unsigned int i=0; i < _BF.size() ; i++) {
            sum += pow( (_BF[i]-integrals[i]/norm) /_precision ,2);
        }
        return sum;
    }
};


std::vector<double> RunningWidthCalculator::getPartialWidthCouplingsFromBF(std::vector<double> BF , std::vector<TF1*> partialWidths, double m0, double gamma0){

    //MinuitParameterSet mps();
    
    std::vector<FitParameter*> fit_couplings;
    
    FitParameter* scale = new FitParameter("scale",2,1,0.1);
    fit_couplings.push_back(scale);
    
    for (unsigned int i=1; i < BF.size() ; i++) {
        FitParameter* g =
	new FitParameter(("g_"+anythingToString((int) i)).c_str(),0,1,0.1);
	//new FitParameter(("g_"+std::to_string(i)).c_str(),0,1,0.1);
        fit_couplings.push_back(g);
    }
      
    chi2BF f(BF, partialWidths, fit_couplings, m0, gamma0);
    Minimiser mini(&f);
    mini.doFit();
    
    vector<double> couplings;
    double norm = 0.;
    
    
    double sum = 0.;
    for (unsigned int i=1; i<fit_couplings.size(); i++) {
        sum += *fit_couplings[i];
    }
    
    for (unsigned int i=0; i<fit_couplings.size(); i++) {
        double c;
        if(i==0) c = 1.;//(1.-sum);
        else c =  *fit_couplings[i];
        norm += c*partialWidths[i]->Eval(m0*m0/(GeV*GeV));
    }
    
    for (unsigned int i=0; i<fit_couplings.size(); i++) {
        double c;
        if(i==0) c = 1;//(1.-sum);
        else c =  *fit_couplings[i];
        couplings.push_back(c / norm * gamma0/GeV);
    }
    
    return couplings;
     
}


class Dispersion_Integrand{
    vector<TF1*> _gamma;
    vector<double> _g;
    double _singularity;
public:
    Dispersion_Integrand(vector<TF1*> f, vector<double> g, double singularity): 
        _gamma(f), _g(g), _singularity(singularity){
    }
    double operator()(double x){ 
            double sum = 0.;
            for (unsigned int i=0; i<_gamma.size(); i++) sum += _g[i]*_gamma[i]->Eval(x);
            return sum/(_singularity-x);
    }
};

TH1D* RunningWidthCalculator::makeRunningMassHisto_3body(int nBins, double max_s_inGeV2, vector<TF1*> gamma, vector<double> g, string OutputFileName){

    double absTol = 0.0001; 
    double relTol = 0.001; 
    unsigned int size = 1000; 
    int rule = 3;  
    double epsilon= 0.00001;
    
    if(OutputFileName=="")
        OutputFileName = "RunningMass_"+ParticlePropertiesList::getMe()->get(abs((int)_pat[0]))->name()+"_3body.root";
    TFile* f= new TFile(OutputFileName.c_str(),"RECREATE");  
    TH1D* h_m= new TH1D("RunningMass",";s [GeV^{2}]; m^{2} (s) [GeV^{2}]",nBins,_min_s_inGeV2,max_s_inGeV2); 
    for (int i=1; i<= h_m->GetNbinsX(); i++) {
        double singularity = h_m->GetXaxis()->GetBinCenter(i);
        if(singularity<_min_s_inGeV2){
            h_m->SetBinContent(i,0);
            continue;
        }
        Dispersion_Integrand integrand(gamma,g,singularity);
        ROOT::Math::IntegratorOneDim integrator(integrand, ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,  absTol , relTol , size ,  rule ) ;
        h_m->SetBinContent(i,(integrator.Integral(_min_s_inGeV2,singularity-epsilon) + integrator.Integral(singularity+epsilon,max_s_inGeV2*10000))/TMath::Pi()); 
    }
    
    //h_m->Scale(h_m->GetNbinsX()/abs(h_m->Integral()));    
    h_m->Write();
    f->Write();  
    
    return h_m;
    /*
     double singularity = 1.4;
     
     Dispersion_Integrand integrand(gamma,singularity,100);
     ROOT::Math::IntegratorOneDim integrator(integrand, ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,  absTol , relTol , size ,  rule )  ;
     double singularPts[3] = {_min_m_squared,singularity,singularity};
     std::vector<double> sp(singularPts, singularPts+3);  
     
     cout << "Int = " << integrator.Integral(_min_m_squared,singularity-epsilon) + integrator.IntegralUp(singularity+epsilon) << endl;
     
     
     Dispersion_Integrand_Subtraction integrand_sub(gamma,singularity);
     ROOT::Math::IntegratorOneDim integrator_sub(integrand_sub, ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,  absTol , relTol , size ,  rule )  ;
     double tmp = integrator_sub.Integral(_min_m_squared,3);
     
     cout << "Int = " << -1.*(tmp + gamma->Eval(singularity)* log( (3-singularity)/(singularity-_min_m_squared) ) ) + integrator.IntegralUp(3)  << endl;
     */
}



TF1* RunningWidthCalculator::getRunningWidthFunction_2body(double max_s_inGeV2, const DecayTree& thisDcy){
    
    bool dbThis=false;
    
    // Some safety checks
    if(thisDcy.finalState().size() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body: I can handle only 2 body decays but the decay " << thisDcy 
        << " has " << thisDcy.finalState().size() << " final state particles. I'll return 0." << endl;
        return 0;
    }
    
    if(thisDcy.nDgtr() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body: "
        << " Mother should have 2 daughters, but it says it has "
        << thisDcy.nDgtr() << ". I'll return 0." << endl;
        return 0;
    }
    
    if( ! _pat.compatibleWith(thisDcy)){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body:"
        << "DalitzEventPattern and DecayTree are not compatible. I'll return 0." << endl;
        return 0;
    }
        
    // Use the BW_BW class to add resonances to ResonanceProperitiesList 
    // and to get acces to angular momentum 
    AssociatingDecayTree associatingDecayTreeX(thisDcy);
    const AssociatedDecayTree X = associatingDecayTreeX.getTree(_pat);
    BW_BW BW_X(X);
    
    // Define all parameters
    // 2 body decay : X -> 1 2
    const int nPar = 4;
    Double_t par[nPar];
    
    // Final state (1,2) masses:   
    par[0] = ParticlePropertiesList::getMe()->get((int) X.getDgtrTreePtr(0)->getVal())->mass();
    par[1] = ParticlePropertiesList::getMe()->get((int) X.getDgtrTreePtr(1)->getVal())->mass();
    // Angular momentum in X -> 1 2
    par[2] = (BW_X.twoLPlusOne()-1)/2; 
    // Mother (X) radius
    par[3] = ResonancePropertiesList::getMe()->get((int)_pat[0])->radius();
        
    TF1 *gamma = new TF1("Gamma_2body",Gamma_2body,0,max_s_inGeV2,nPar);
    gamma->SetParameters(par); 
    
    return gamma;
    (void)dbThis;
}


TH1D* RunningWidthCalculator::makeHisto_2body(int nBins, double max_s_inGeV2, const DecayTree& thisDcy, string OutputFileName){
    
    bool dbThis=false;
    
    // Some safety checks
    if(thisDcy.finalState().size() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body: I can handle only 2 body decays but the decay " << thisDcy 
        << " has " << thisDcy.finalState().size() << " final state particles. I'll return 0." << endl;
        return 0;
    }
    
    if(thisDcy.nDgtr() != 2){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body: "
        << " Mother should have 2 daughters, but it says it has "
        << thisDcy.nDgtr() << ". I'll return 0." << endl;
        return 0;
    }
    
    if( ! _pat.compatibleWith(thisDcy)){
        cout << "ERROR in RunningWidthCalculator::makeHisto_2body:"
        << "DalitzEventPattern and DecayTree are not compatible. I'll return 0." << endl;
        return 0;
    }
    
    // Use the BW_BW class to add resonances to ResonanceProperitiesList 
    // and to get acces to angular momentum 
    AssociatingDecayTree associatingDecayTreeX(thisDcy);
    const AssociatedDecayTree X = associatingDecayTreeX.getTree(_pat);
    BW_BW BW_X(X);
    
    // Define all parameters
    // 2 body decay : X -> 1 2
    //const int nPar = 10;
    //Double_t par[nPar];
    
    // Mother (X) radius
    double r = ResonancePropertiesList::getMe()->get((int)_pat[0])->radius();
    // Final state (1,2) masses:   
    double m1 = ParticlePropertiesList::getMe()->get((int) X.getDgtrTreePtr(0)->getVal())->mass();
    double m2 = ParticlePropertiesList::getMe()->get((int) X.getDgtrTreePtr(1)->getVal())->mass();
    // Angular momentum in X -> 1 2
    double l = (BW_X.twoLPlusOne()-1)/2; 
        
    if(OutputFileName=="")
        OutputFileName = "RunningWidth_"+ParticlePropertiesList::getMe()->get(abs((int)_pat[0]))->name()+"_2body.root";
    TFile* f= new TFile(OutputFileName.c_str(),"RECREATE");  
    
    TH1D* h= new TH1D("RunningWidth",";s [GeV^2]; #Gamma (s) [a.u.]",nBins,_min_s_inGeV2,max_s_inGeV2); 
    for (int i=1; i<= h->GetNbinsX(); i++) {
        h->SetBinContent(i, Gamma_2body( sqrt(h->GetXaxis()->GetBinCenter(i))*GeV , m1,m2, l, r));
    }  
    
    //h->Scale(1./h->Integral());    
    h->Write();
    f->Write();  
    return h;
    (void)dbThis;
}



Double_t phaseSpaceIntegral(Double_t *x, Double_t *par){
    
    //x[0]: mumsRecoMass()^2
    //par[0]: mumsMass()
    //par[i]: final state masses (i=1,2,3)
    
    PhaseSpaceIntegral3body ps;
    double ps_val = ps.getVal(sqrt(x[0])*GeV,par[1],par[2],par[3]);
    double ps0_val =  ps.getVal(par[0],par[1],par[2],par[3]);
    
    double ps_ratio=0.;
    if(ps0_val>0)ps_ratio= ps_val/ps0_val;
    
    return ps_ratio * par[0]/(sqrt(x[0])*GeV);
}

TH1D* RunningWidthCalculator::makeHisto_phaseSpace(int nBins, double max_s_inGeV2, string OutputFileName){
    
    if(_pat.size() != 4) {
        cout << "ERROR in RunningWidthCalculator::makeHisto_phaseSpace: I can handle only 3 body decays but the pattern " << _pat 
        << " has " << _pat.size()-1 << " final state particles. I'll return 0." << endl;
        return 0;    
    }
    
    if(OutputFileName=="")
        OutputFileName = "RunningWidth_"+ParticlePropertiesList::getMe()->get(abs((int)_pat[0]))->name()+"_PhaseSpace.root";
    
    
    TFile* f= new TFile(OutputFileName.c_str(),"RECREATE");  
    TH1D* h= new TH1D("RunningWidth","RunningWidth",nBins,_min_s_inGeV2,max_s_inGeV2); 
        
    const int nPar = 4;
    Double_t par[nPar];
    par[0] = ParticlePropertiesList::getMe()->get((int)_pat[0])->mass();
    par[1] = ParticlePropertiesList::getMe()->get((int)_pat[1])->mass();
    par[2] = ParticlePropertiesList::getMe()->get((int)_pat[2])->mass();
    par[3] = ParticlePropertiesList::getMe()->get((int)_pat[3])->mass();
    
    TF1 *ps = new TF1("phaseSpaceIntegral",phaseSpaceIntegral,_min_s_inGeV2,max_s_inGeV2,nPar);
    ps->SetParameters(par);
    
    for (int i=1; i<= h->GetNbinsX(); i++) {
        h->SetBinContent(i,ps->Eval(h->GetXaxis()->GetBinCenter(i)));    
    }  
    
    h->Scale(h->GetNbinsX()/h->Integral());    
    h->Write();
    f->Write();  
    
    return h;    
}

TF1* RunningWidthCalculator::getRunningWidthFunction_phaseSpace(double max_s_inGeV2){
    
    if(_pat.size() != 4) {
        cout << "ERROR in RunningWidthCalculator::makeHisto_phaseSpace: I can handle only 3 body decays but the pattern " << _pat 
        << " has " << _pat.size()-1 << " final state particles. I'll return 0." << endl;
        return 0;    
    }
    
    const int nPar = 4;
    Double_t par[nPar];
    par[0] = ParticlePropertiesList::getMe()->get((int)_pat[0])->mass();
    par[1] = ParticlePropertiesList::getMe()->get((int)_pat[1])->mass();
    par[2] = ParticlePropertiesList::getMe()->get((int)_pat[2])->mass();
    par[3] = ParticlePropertiesList::getMe()->get((int)_pat[3])->mass();
    
    TF1 *ps = new TF1("phaseSpaceIntegral",phaseSpaceIntegral,0,max_s_inGeV2,nPar);
    ps->SetParameters(par);
    
    return ps;    
}

double Fermi_phaseSpace(double s, double lambda, double s0, double threshold){
    if (s<threshold)return 0;
    return sqrt(1.- threshold/s)/(1.+ exp(lambda * (s0-s) ) );
}

Double_t Fermi_phaseSpace(Double_t *x, Double_t *par){
    return Fermi_phaseSpace(x[0],par[0],par[1],par[2]);
}
 
TF1* RunningWidthCalculator::getRunningWidthFunction_Fermi(double max_s_inGeV2, double lambda, double s0){
    
    // Define all parameters
    const int nPar = 3;
    Double_t par[nPar];
    par[0] = lambda;
    par[1] = s0;
    par[2] = _min_s_inGeV2;

    TF1 *gamma = new TF1("Fermi_phaseSpace",Fermi_phaseSpace,0,max_s_inGeV2,nPar);
    gamma->SetParameters(par); 
    
    return gamma;
}


