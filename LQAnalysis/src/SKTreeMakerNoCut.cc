// $Id: SKTreeMakerNoCut.cc 1 2013-11-26 10:23:10Z jalmond $
/***************************************************************************
 * @Project: LQAnalyzer Frame - ROOT-based analysis framework for Korea SNU
 * @Package: LQCycles
 *
 * @author John Almond       <jalmond@cern.ch>           - SNU
 *
 ***************************************************************************/

/// Local includes
#include "SKTreeMakerNoCut.h"

//Core includes
#include "EventBase.h"                                                                                                                           


//// Needed to allow inheritance for use in LQCore/core classes
ClassImp (SKTreeMakerNoCut);


/**
 *   This is an Example Cycle. It inherits from AnalyzerCore. The code contains all the base class functions to run the analysis.
 *
 */
SKTreeMakerNoCut::SKTreeMakerNoCut() :  AnalyzerCore(), out_muons(0), out_electrons(0), out_jets(0), out_genjets(0), out_truth(0), nevents(0),pass_eventcut(0), pass_vertexcut(0) {

  // To have the correct name in the log:                                                                                                                            
  SetLogName("SKTreeMakerNoCut");




}

void SKTreeMakerNoCut::ExecuteEvents()throw( LQError ){
  
  FillCutFlow("NoCut", 1);

  if(!PassBasicEventCuts()){
    m_logger << DEBUG << "Fail MET filter cuts" << LQLogger::endmsg;
    throw LQError( "Fails basic cuts",  LQError::SkipEvent );
  }  
  FillCutFlow("EventCut", 1);
  
  std::vector<TString> triggerslist;
  triggerslist.clear(); /// PassTrigger will check ALL triggers if no entries are filled

  if (!eventbase->GetEvent().HasGoodPrimaryVertex()){
    m_logger <<  DEBUG << "Event FAILS HasGoodPrimaryVertex " << LQLogger::endmsg;
    throw LQError( "Has no PV",  LQError::SkipEvent );
  }
  FillCutFlow("VertexCut", 1);

 
  //////////////////////////////////////////////////////
  //////////// Select objetcs
  //////////////////////////////////////////////////////   
  

  //######   MUON SELECTION ###############
  Message("Selecting Muons", DEBUG);
  std::vector<snu::KMuon> skim_muons;
  eventbase->GetMuonSel()->SetPt(0.); 
  eventbase->GetMuonSel()->SetEta(5.);
  eventbase->GetMuonSel()->BasicSelection(out_muons, false); /// Muons For SKTree


  //###### JET SELECTION  ################
  Message("Selecting jets", DEBUG);
  eventbase->GetJetSel()->SetPt(0.);
  eventbase->GetJetSel()->SetEta(5.);
  eventbase->GetJetSel()->BasicSelection(out_jets);
  
  //###### GenJet Selection ##########
  if(!k_isdata) eventbase->GetGenJetSel()->BasicSelection(out_genjets);
  
  //###### Electron Selection ########
  Message("Selecting electrons", DEBUG);
  eventbase->GetElectronSel()->SetPt(0.); 
  eventbase->GetElectronSel()->SetEta(5.); 
  eventbase->GetElectronSel()->BasicSelection(out_electrons); 
  
  FillCutFlow("DiLep", 1);

  out_event   = eventbase->GetEvent();
  out_trigger = eventbase->GetTrigger();
  out_truth   = eventbase->GetTruth();
  
  return;
}// End of execute event loop
  


void SKTreeMakerNoCut::EndCycle()throw( LQError ){
  
  Message("In EndCycle" , INFO);
}


void SKTreeMakerNoCut::BeginCycle() throw( LQError ){
  
  Message("In begin Cycle", INFO);

  //DeclareVariable(obj, label ); //-> will use default treename: LQTree
  DeclareVariable(out_electrons, "KElectrons", "LQTree");
  DeclareVariable(out_muons, "KMuons");
  DeclareVariable(out_jets, "KJets");
  DeclareVariable(out_genjets, "KGenJets");
  DeclareVariable(out_trigger, "KTrigger");
  DeclareVariable(out_event, "KEvent");
  DeclareVariable(out_truth, "KTruth");

  return;
  
}

SKTreeMakerNoCut::~SKTreeMakerNoCut() {
  
  Message("In Analyzer Destructor" , INFO);

}


void SKTreeMakerNoCut::FillCutFlow(TString cut, float weight){


  if(GetHist("cutflow")) {
    GetHist("cutflow")->Fill(cut,weight);

  }
  else{
    AnalyzerCore::MakeHistograms("cutflow", 4,0.,4.);

    GetHist("cutflow")->GetXaxis()->SetBinLabel(1,"NoCut");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(2,"EventCut");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(3,"VertexCut");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(4,"DiLep");
  }
}



void SKTreeMakerNoCut::BeginEvent( )throw( LQError ){

  Message("In BeginEvent() " , DEBUG);

  return;
}


void SKTreeMakerNoCut::ClearOutputVectors() throw (LQError){
  //
  // Reset all variables declared in Declare Variable
  //
  out_muons.clear();
  out_electrons.clear();
  out_jets.clear();
  out_genjets.clear();
  out_truth.clear();

}



