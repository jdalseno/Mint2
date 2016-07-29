// author: Jonas Rademacker (Jonas.Rademacker@bristol.ac.uk)
// status:  Mon 9 Feb 2009 19:18:01 GMT
#include "Mint/FitParameter.h"
#include "Mint/NamedParameter.h"
#include "Mint/Minimiser.h"
#include "Mint/Neg2LL.h"
#include "Mint/Neg2LLSum.h"
#include "Mint/DalitzEventList.h"

#include "Mint/CLHEPPhysicalConstants.h"


#include "Mint/PdfBase.h"
#include "Mint/DalitzPdfBase.h"
#include "Mint/DalitzPdfBaseFastInteg.h"
#include "Mint/DalitzSumPdf.h"

#include "Mint/FitAmplitude.h"
#include "Mint/FitAmpSum.h"
#include "Mint/FitAmpIncoherentSum.h"

#include "Mint/DalitzEvent.h"

#include "Mint/AmpRatios.h"

#include "Mint/IEventGenerator.h"
#include "Mint/DalitzBWBoxSet.h"
#include "Mint/DalitzBoxSet.h"

#include "Mint/SignalGenerator.h"
#include "Mint/FromFileGenerator.h"

#include "Mint/LASSO.h"

#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"

#include "TRandom2.h"
#include "TRandom3.h"
#include <ctime>

#include <iostream>

#include "Mint/DalitzPdfNormChecker.h"
#include "Mint/DalitzPdfBaseFlexiFastInteg.h"


#include "Mint/Chi2Binning.h"

using namespace std;
using namespace MINT;

#define MULTIAMPS

class AmpsPdf
  : public DalitzPdfBaseFastInteg
{
protected:
  TRandom* _localRnd;
  SignalGenerator* _sgGen;
  FromFileGenerator* _fileGen;
  IEventGenerator<IDalitzEvent>* _chosenGen;
  NamedParameter<std::string> _integratorSource;
  std::string _integratorFileName;
public:
  double un_normalised_noPs(IDalitzEvent& evt){
    double ampSq =  _amps->RealVal(evt); 
    
    return ampSq;// * getEvent()->phaseSpace();
 

  }
  
  AmpsPdf(const DalitzEventPattern& pat
          , IFastAmplitudeIntegrable* amps
          , double precision=1.e-4
          , std::string method="efficient"
          , std::string fname =  "SignalIntegrationEvents.root", bool genMoreEvents = false
          , MinuitParameterSet* mps=0
          )
    : DalitzPdfBaseFastInteg(pat, 0, amps, precision, mps)
    , _localRnd(0)
    , _sgGen(0)
    , _fileGen(0)
    , _chosenGen(0)
    , _integratorSource("IntegratorSource", (std::string) "old", (char*) 0) 
      ////having new here will overwrite your tuple!!
    , _integratorFileName(fname)
  {
    cout << " AmpsPdf with integ method " << method << endl;
    bool nonFlat = "efficient" == method;
    bool generateNew = ((string)_integratorSource == (string)"new");
    if(nonFlat){
      cout << "AmpsPdf uses nonFlat integration." << endl;
      if(generateNew){
        _sgGen =  new SignalGenerator(pat, amps);
        _sgGen->setWeighted();
        _sgGen->setSaveEvents(_integratorFileName);
        _chosenGen = _sgGen;
      }else{
	// here, SignalGenerator is used by FromFileGenerator, to fill
	// up missing events in case more are needed than found in the
	// file.  Since we don't know with which random seed the
	// events in the file were generated, we supply a random
	// number generator with randomised seed.
        cout << "Using else, AmpsPdf.h from ampFit " << endl;
	_localRnd = new TRandom3(time(0));
	_sgGen =  new SignalGenerator(pat, amps, _localRnd);
	_sgGen->setWeighted();
	_sgGen->dontSaveEvents();// saving events is done by FromFileGenerator
	if(genMoreEvents) _fileGen   = new FromFileGenerator(_integratorFileName, _sgGen);
	else{
	  _fileGen = new FromFileGenerator(_integratorFileName, 0);
	  cout << "not going to generate any more events" << endl;
	}
  
	_chosenGen = _fileGen;
      }
      
      this->setEventGenerator(_chosenGen);
    }else{
      cout << "AmpsPdf uses flat integration." << endl;
    }
  }
  
  IFastAmplitudeIntegrable* getAmpSum(){ return _amps;}
  
  ~AmpsPdf(){
    if(0 != _fileGen)  delete _fileGen;
    if(0 != _sgGen)    delete _sgGen;
    if(0 != _localRnd) delete _localRnd;
  }
};




int ampFit(){
  time_t startTime = time(0);
  //MinuitParameterSet& fitMPS(*MinuitParameterSet::getDefaultSet());
  MinuitParameterSet fitMPS;//(*MinuitParameterSet::getDefaultSet());
  MinuitParameterSet dummy;

  cout << "pset pointer in default " << MinuitParameterSet::getDefaultSet() << endl;
  cout << "pset pointer in ampsFit " << &fitMPS << endl;

  cout << "sizes " << MinuitParameterSet::getDefaultSet()->size()
       << ", " << fitMPS.size() << endl;


  TRandom3 ranLux;
  NamedParameter<int> RandomSeed("RandomSeed", 0);
  ranLux.SetSeed((int)RandomSeed);
  gRandom = &ranLux;

  FitAmplitude::AutogenerateFitFile();

  NamedParameter<double>  lambda("lambda", 1.);

  NamedParameter<int> MakeIntegrators("MakeIntegrators", 1);
  bool makeIntegrators = (int) MakeIntegrators;
  cout << "MakeIntegrator= " << MakeIntegrators << endl;

  NamedParameter<string> InputFileName("InputFileName", (std::string) "");
  bool generateNew = (std::string) InputFileName == "";

  string InputFileName1 = ((string)InputFileName) + "_1.root";
  string InputFileName2 = ((string)InputFileName) + "_2.root";
  string InputFileName3 = ((string)InputFileName) + "_3.root";
  string InputFileName4 = ((string)InputFileName) + "_4.root";
  string InputFileName5 = ((string)InputFileName) + "_5.root";

  NamedParameter<string> IntegratorEventFile("IntegratorEventFile"
					     , (std::string) "SignalIntegrationEvents"
					     , (char*) 0);
  string SgIntegratorEventFile1 = "Sg" + ((string)IntegratorEventFile) + "_1.root";
  string SgIntegratorEventFile2 = "Sg" + ((string)IntegratorEventFile) + "_2.root";
  string SgIntegratorEventFile3 = "Sg" + ((string)IntegratorEventFile) + "_3.root";
  string SgIntegratorEventFile4 = "Sg" + ((string)IntegratorEventFile) + "_4.root";
  string SgIntegratorEventFile5 = "Sg" + ((string)IntegratorEventFile) + "_5.root";

  string BgIntegratorEventFile1 = "Bg" + ((string)IntegratorEventFile) + "_1.root";
  string BgIntegratorEventFile2 = "Bg" + ((string)IntegratorEventFile) + "_2.root";
  string BgIntegratorEventFile3 = "Bg" + ((string)IntegratorEventFile) + "_3.root";
  string BgIntegratorEventFile4 = "Bg" + ((string)IntegratorEventFile) + "_4.root";
  string BgIntegratorEventFile5 = "Bg" + ((string)IntegratorEventFile) + "_5.root";



  NamedParameter<string> IntegratorInputFile("IntegratorInputFile"
					     , (std::string) "sgIntegrator"
					     , (char*) 0);
  NamedParameter<int>  Nevents("Nevents", 10000);
  NamedParameter<int>  doScan("doScan", 0);
  NamedParameter<std::string> integMethod("IntegMethod", (std::string) "efficient");
  NamedParameter<double> integPrecision("IntegPrecision", 1.e-4);

  NamedParameter<int> EventPattern("Event Pattern", 421, -321, 211, 211, -211);
  DalitzEventPattern pat(EventPattern.getVector());

  NamedParameter<int> doNormCheck("doNormCheck", 0);
  NamedParameter<int> saveEvents("SaveEvents", 1);
  NamedParameter<int> doFinalStats("DoFinalStats", 1);
  cout << " got event pattern: " << pat << endl;


  DalitzEventList eventList1, eventList2, eventList3, eventList4, eventList5;

  cout << "1 pset pointer in default " << MinuitParameterSet::getDefaultSet() << endl;
  cout << "1 pset pointer in ampsFit " << &fitMPS << endl;

  cout << "1 sizes " << MinuitParameterSet::getDefaultSet()->size()
       << ", " << fitMPS.size() << endl;

  if(! generateNew){
    cout << "reading events from file " << InputFileName1 << endl;
    eventList1.fromFile(InputFileName1);
    cout << " I've got " << eventList1.size() << " events (1)." << endl;
 
    cout << "reading events from file " << InputFileName2 << endl;
    eventList2.fromFile(InputFileName2);
    cout << " I've got " << eventList2.size() << " events (2)." << endl;
 
    cout << "reading events from file " << InputFileName3 << endl;
    eventList3.fromFile(InputFileName3);
    cout << " I've got " << eventList3.size() << " events (3)." << endl;
  
    cout << "reading events from file " << InputFileName4 << endl;
    eventList4.fromFile(InputFileName4);
    cout << " I've got " << eventList4.size() << " events (4)." << endl;

    cout << "reading events from file " << InputFileName5 << endl;
    eventList5.fromFile(InputFileName5);
    cout << " I've got " << eventList5.size() << " events (5)." << endl;
 }

  if(generateNew){
    SignalGenerator sg(pat);

    cout << "Generating " << Nevents << " signal events (1)." << endl;
    sg.FillEventList(eventList1, Nevents);
    if((int) saveEvents)eventList1.save("KKpipi_1.root");

    cout << "Generating " << Nevents << " signal events (2)." << endl;
    sg.FillEventList(eventList2, Nevents);
    if((int) saveEvents)eventList2.save("KKpipi_2.root");

    cout << "Generating " << Nevents << " signal events (3)." << endl;
    sg.FillEventList(eventList3, Nevents);
    if((int) saveEvents)eventList3.save("KKpipi_3.root");

    cout << "Generating " << Nevents << " signal events (4)." << endl;
    sg.FillEventList(eventList4, Nevents);
    if((int) saveEvents)eventList4.save("KKpipi_4.root");

    cout << "Generating " << Nevents << " signal events (5)." << endl;
    sg.FillEventList(eventList5, Nevents);
    if((int) saveEvents)eventList4.save("KKpipi_5.root");
  }
  
  //DalitzHistoSet datH = eventList.histoSet();
  //datH.save("plotsFromEventList.root");


  cout << "2 pset pointer in default " << MinuitParameterSet::getDefaultSet() << endl;
  cout << "2 pset pointer in ampsFit " << &fitMPS << endl;

  cout << "2 sizes " << MinuitParameterSet::getDefaultSet()->size()
       << ", " << fitMPS.size() << endl;

  /*
  AmpsPdf amps(pat, &fitMPS
	       , integPrecision
	       , integMethod
	       , (std::string) IntegratorEventFile); 
  */

  FitAmpSum ampSum1(pat, &fitMPS);
  FitAmpIncoherentSum ampSumBg1(pat, &dummy, "Inco1_");

#ifdef MULTIAMPS
  /*
  FitAmpSum ampSum2(pat, &fitMPS);
  FitAmpSum ampSum3(pat, &fitMPS);
  FitAmpSum ampSum4(pat, &fitMPS);
  FitAmpSum ampSum5(pat, &fitMPS);

  FitAmpIncoherentSum ampSumBg2(pat, &dummy, "Inco2_");
  FitAmpIncoherentSum ampSumBg3(pat, &dummy, "Inco3_");
  FitAmpIncoherentSum ampSumBg4(pat, &dummy, "Inco4_");
  FitAmpIncoherentSum ampSumBg5(pat, &dummy, "Inco5_");
  */
#endif

  counted_ptr<FitAmpSum> ampSum2Ptr = ampSum1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSum3Ptr = ampSum1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSum4Ptr = ampSum1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSum5Ptr = ampSum1.GetCloneSameFitParameters();

  counted_ptr<FitAmpSum> ampSumBg2Ptr = ampSumBg1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSumBg3Ptr = ampSumBg1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSumBg4Ptr = ampSumBg1.GetCloneSameFitParameters();
  counted_ptr<FitAmpSum> ampSumBg5Ptr = ampSumBg1.GetCloneSameFitParameters();
 

  FitParameter sgFrac("SignalFraction");

  AmpsPdf ampsSg1(pat, &ampSum1
	       , integPrecision
	       , integMethod
		, SgIntegratorEventFile1, true, &fitMPS); 


#ifdef MULTIAMPS
  // Note: Crash also occurs if we use the same ampSum in each AmpsPdf
  AmpsPdf ampsSg2(pat, ampSum2Ptr.get()      // <<< putting ampSum1 here also crashes.
	       , integPrecision
	       , integMethod
	       , SgIntegratorEventFile2, true, &fitMPS); 

  AmpsPdf ampsSg3(pat, ampSum3Ptr.get()
	       , integPrecision
	       , integMethod
	       , SgIntegratorEventFile3, true, &fitMPS); 

  AmpsPdf ampsSg4(pat, ampSum4Ptr.get()
	       , integPrecision
	       , integMethod
	       , SgIntegratorEventFile4, true, &fitMPS); 

  AmpsPdf ampsSg5(pat, ampSum5Ptr.get()
	       , integPrecision
	       , integMethod
	       , SgIntegratorEventFile5, true, &fitMPS); 
#endif
  AmpsPdf ampsBg1(pat, &ampSumBg1
	       , integPrecision
	       , integMethod
		, BgIntegratorEventFile1, true, &fitMPS); 


#ifdef MULTIAMPS
  // Note: Crash also occurs if we use the same ampSum in each AmpsPdf
  AmpsPdf ampsBg2(pat, ampSumBg2Ptr.get()
	       , integPrecision
	       , integMethod
	       , BgIntegratorEventFile2, true, &fitMPS); 

  AmpsPdf ampsBg3(pat, ampSumBg3Ptr.get()
	       , integPrecision
	       , integMethod
	       , BgIntegratorEventFile3, true, &fitMPS); 

  AmpsPdf ampsBg4(pat, ampSumBg4Ptr.get()
	       , integPrecision
	       , integMethod
	       , BgIntegratorEventFile4, true, &fitMPS); 

  AmpsPdf ampsBg5(pat, ampSumBg5Ptr.get()
	       , integPrecision
	       , integMethod
	       , BgIntegratorEventFile5, true, &fitMPS); 
#endif


  DalitzSumPdf amps1(sgFrac, ampsSg1, ampsBg1);
#ifdef MULTIAMPS
  DalitzSumPdf amps2(sgFrac, ampsSg2, ampsBg2);
  DalitzSumPdf amps3(sgFrac, ampsSg3, ampsBg3);
  DalitzSumPdf amps4(sgFrac, ampsSg4, ampsBg4);
  DalitzSumPdf amps5(sgFrac, ampsSg5, ampsBg5);
#endif

  cout << "3 pset pointer in default " << MinuitParameterSet::getDefaultSet() << endl;
  cout << "3 pset pointer in ampsFit " << &fitMPS << endl;

  cout << "3 sizes " << MinuitParameterSet::getDefaultSet()->size()
       << ", " << fitMPS.size() << endl;

  if(! makeIntegrators){
    ampsSg1.setIntegratorFileName("sgIntegrator1");
#ifdef MULTIAMPS
    ampsSg2.setIntegratorFileName("sgIntegrator2");
    ampsSg3.setIntegratorFileName("sgIntegrator3");
    ampsSg4.setIntegratorFileName("sgIntegrator4");
    ampsSg5.setIntegratorFileName("sgIntegrator5");
#endif
  }
  if(! makeIntegrators){
    ampsBg1.setIntegratorFileName("bgIntegrator1");
#ifdef MULTIAMPS
    ampsBg2.setIntegratorFileName("bgIntegrator2");
    ampsBg3.setIntegratorFileName("bgIntegrator3");
    ampsBg4.setIntegratorFileName("bgIntegrator4");
    ampsBg5.setIntegratorFileName("bgIntegrator5");
#endif
  }


#ifdef MULTIAMPS
  /* this also crashes
  Neg2LL fcn1(amps1, eventList1);//, &fitMPS);
  Neg2LL fcn2(amps1, eventList2);//, &fitMPS);
  Neg2LL fcn3(amps1, eventList3);//, &fitMPS);
  Neg2LL fcn4(amps1, eventList4);//, &fitMPS);
  */

  Neg2LL fcn1(amps1, eventList1, &fitMPS);
  Neg2LL fcn2(amps2, eventList2, &fitMPS);
  Neg2LL fcn3(amps3, eventList3, &fitMPS);
  Neg2LL fcn4(amps4, eventList4, &fitMPS);
  Neg2LL fcn5(amps5, eventList5, &fitMPS);


  LASSO lasso(&ampsSg3, lambda);

  //Neg2LLSum fcn(&fcn1, &fcn2, &fcn3, &fcn4, &fcn5, &lasso, &fitMPS);
  //Neg2LLSum fcn(&fcn1, &fcn2,  &fcn3, &fcn4, &lasso, &fitMPS);
  Neg2LLSum fcn(&fitMPS, &fcn1, &fcn2,  &fcn3, &fcn4, &lasso);
#else
  Neg2LL fcn(amps1, eventList1);
#endif

  if((int) doNormCheck){
    DalitzPdfNormChecker nc(&amps1, pat);
    nc.checkNorm();
  }



  Minimiser mini(&fcn);

  mini.doFit();

  mini.printResultVsInput();
  ampsSg1.saveEachAmpsHistograms("singleAmpHistos");

  if(makeIntegrators){
    cout << "now saving all integrators" <<endl;
    ampsSg1.saveIntegrator("sgIntegrator1");
#ifdef MULTIAMPS
    ampsSg2.saveIntegrator("sgIntegrator2");
    ampsSg3.saveIntegrator("sgIntegrator3");
    ampsSg4.saveIntegrator("sgIntegrator4");
    ampsSg5.saveIntegrator("sgIntegrator5");
#endif
    ampsBg1.saveIntegrator("sgIntegrator1");
#ifdef MULTIAMPS
    ampsBg2.saveIntegrator("bgIntegrator2");
    ampsBg3.saveIntegrator("bgIntegrator3");
    ampsBg4.saveIntegrator("bgIntegrator4");
    ampsBg5.saveIntegrator("bgIntegrator5");
#endif
  }

  cout << "now calling doFinalStat" << endl;

  if((bool) doFinalStats){
    ampsSg1.doFinalStats(&mini);
#ifdef MULTIAMPS
    ampsSg2.doFinalStats(&mini);
    ampsSg3.doFinalStats(&mini);
    ampsSg4.doFinalStats(&mini);
    ampsSg5.doFinalStats(&mini);
#endif
  }

  cout<<"final fcn1 getVal() "<< fcn1.getVal()<<endl;
#ifdef MULTIAMPS
  cout<<"final fcn2 getVal() "<< fcn2.getVal()<<endl;
  cout<<"final fcn3 getVal() "<< fcn3.getVal()<<endl;
  cout<<"final fcn4 getVal() "<< fcn4.getVal()<<endl;
  cout<<"final fcn5 getVal() "<< fcn5.getVal()<<endl;
#endif
  //return 0;

  cout << " ampFit done. Took " << (time(0) - startTime)/60. 
       << " min. Returning 0." << endl;
  return 0;
}


int main(){

  return ampFit();

}
//
