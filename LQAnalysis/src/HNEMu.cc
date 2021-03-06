// $id: ExampleAnalyzer.cc 1 2013-11-26 10:23:10Z jalmond $
/***************************************************************************
 * @Project: LQHNEMu Frame - ROOT-based analysis framework for Korea SNU
 * @Package: LQCycles
 *
 * @author John Almond       <jalmond@cern.ch>           - SNU
 *
 ***************************************************************************/

/// Local includes
#include "HNEMu.h"

//Core includes
#include "Reweight.h"
#include "EventBase.h"                                                                                                                           
#include "BaseSelection.h"

//// Needed to allow inheritance for use in LQCore/core classes
ClassImp (HNEMu);

/**
 *   This is an Example Cycle. It inherits from AnalyzerCore. The code contains all the base class functions to run the analysis.
 *
 */
HNEMu::HNEMu() :  AnalyzerCore(),  out_electrons(0) {

  // To have the correct name in the log:                                                                                                                            
  SetLogName("HNEMu");

  Message("In HNEMu constructor", INFO);
  //
  // This function sets up Root files and histograms Needed in ExecuteEvents
  InitialiseAnalysis();
  
  
  MakeCleverHistograms(sighist,"OS_2Jet");

  MakeCleverHistograms(sighist,"SS_1Jet");
  MakeCleverHistograms(sighist,"SS_0bjet");
  MakeCleverHistograms(sighist,"SS_bjet");
  MakeCleverHistograms(sighist,"SS_DiJet");
  MakeCleverHistograms(sighist,"SS_lowmass_40");
  MakeCleverHistograms(sighist,"SS_lowmass_50");
  MakeCleverHistograms(sighist,"SS_lowmass_60");
  MakeCleverHistograms(sighist,"SS_lowmass_70");
  MakeCleverHistograms(sighist,"SS_lowmass_80");
  MakeCleverHistograms(sighist,"SS_lowmassCR");

  MakeCleverHistograms(sighist,"SS_highmass_90");
  MakeCleverHistograms(sighist,"SS_highmass_100");
  MakeCleverHistograms(sighist,"SS_highmass_125");
  MakeCleverHistograms(sighist,"SS_highmass_150");
  MakeCleverHistograms(sighist,"SS_highmass_175");
  MakeCleverHistograms(sighist,"SS_highmass_200");
  MakeCleverHistograms(sighist,"SS_highmass_250");
  MakeCleverHistograms(sighist,"SS_highmass_300");
  MakeCleverHistograms(sighist,"SS_highmass_350");
  MakeCleverHistograms(sighist,"SS_highmass_400");
  MakeCleverHistograms(sighist,"SS_highmass_500");


  MakeCleverHistograms(sighist,"SS_highmassCR");



  MakeCleverHistograms(sighist,"SSemu_1Jet");
  MakeCleverHistograms(sighist,"SSemu_DiJet");
  MakeCleverHistograms(sighist,"SSmue_1Jet");
  MakeCleverHistograms(sighist,"SSmue_DiJet");

  MakeCleverHistograms(sighist,"SSmue_2Jet");
  MakeCleverHistograms(sighist,"SSmue_3Jet");
  MakeCleverHistograms(sighist,"SSmue_4Jet");
  MakeCleverHistograms(sighist,"SSmue_BJet");
  MakeCleverHistograms(sighist,"SSmue_0BJet");

  MakeCleverHistograms(sighist,"SSemu_2Jet");
  MakeCleverHistograms(sighist,"SSemu_3Jet");
  MakeCleverHistograms(sighist,"SSemu_4Jet");
  MakeCleverHistograms(sighist,"SSemu_BJet");
  MakeCleverHistograms(sighist,"SSemu_0BJet");

}


void HNEMu::InitialiseAnalysis() throw( LQError ) {
  
  /// Initialise histograms
  MakeHistograms();  
  //
  // You can out put messages simply with Message function. Message( "comment", output_level)   output_level can be VERBOSE/INFO/DEBUG/WARNING 
  // You can also use m_logger << level << "comment" << int/double  << LQLogger::endmsg;
  //

   return;
}


void HNEMu::ExecuteEvents()throw( LQError ){
    
  m_logger << DEBUG << "RunNumber/Event Number = "  << eventbase->GetEvent().RunNumber() << " : " << eventbase->GetEvent().EventNumber() << LQLogger::endmsg;
  m_logger << DEBUG << "isData = " << isData << LQLogger::endmsg;
  
  FillEventCutFlow("NoCut","", 1.);
  
  if(!PassBasicEventCuts())  throw LQError( "Fails basic cuts",  LQError::SkipEvent );
  
  FillEventCutFlow("EventCut", "",weight);
  /// Trigger List 
  std::vector<TString> triggerslist;  
  triggerslist.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
  triggerslist.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
  
  if(isData){
    if(!PassTrigger(triggerslist, prescale)) return;
  }
  else weight*= TriggerScaleFactorEMu();

  //// if the trigger that fired the event is prescaled you can reweight the event accordingly using the variable prescale
  
  if (!eventbase->GetEvent().HasGoodPrimaryVertex()) throw LQError( "Fails basic cuts",  LQError::SkipEvent );
  FillEventCutFlow("VertexCut","", weight);
  
  /// Correct MC for pileup     
  float pileup_up_sys_factor = 1.;
  float pileup_down_sys_factor = 1.;
  
  if (MC_pu&&!k_isdata) {
    weight  = weight* reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()), 0)* MCweight;
    pileup_up_sys_factor = reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()), 1) / reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()), 0);
    pileup_down_sys_factor = reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()), -1) / reweightPU->GetWeight(int(eventbase->GetEvent().PileUpInteractionsTrue()), 0);
  }
    
  TString fake_loose_region = "looseregion2";
  TString fake_loose_label = "HNTight_loosereg2";

  //// Get the collection of electrons
  std::vector<snu::KElectron> electronAnalysisColl                   = GetElectrons(true,  true, fake_loose_label , weight);
  std::vector<snu::KMuon> muons = GetMuons("tight");
  
  if(!isData){
    for(std::vector<snu::KElectron>::iterator it = electronAnalysisColl.begin(); it != electronAnalysisColl.end(); it++){
      weight *= ElectronScaleFactor(it->Eta(), it->Pt(), true, 0);
    }
    
    for(std::vector<snu::KMuon>::iterator it = muons.begin(); it != muons.end(); it++){
      weight *= MuonScaleFactor(it->Eta(), it->Pt(), 0);
    }
    
  }
  
  

  vector<snu::KTruth> truth =  eventbase->GetTruth();
  
  std::vector<snu::KElectron> electronVetoColl       = GetElectrons(false, false, "veto"); 
  std::vector<snu::KElectron> electronLooseColl      = GetElectrons(false, false, "loose"); 

  std::vector<snu::KMuon> muonVetoColl  = GetMuons("veto");

  /// JETS
  std::vector<snu::KJet> jetColl             = GetJets("NoLeptonVeto");
  std::vector<snu::KJet> jetColl_lepveto     = GetJets("ApplyLeptonVeto");
  std::vector<snu::KJet> jetColl_lepveto_mva = GetJets("ApplyPileUpID");

  ///// count number of bjets in the event (using cvs medium WP)
  int nbjet_m=0;

  for(unsigned int ij=0; ij <jetColl_lepveto_mva.size(); ij++){
    if(jetColl_lepveto_mva.at(ij).CombinedSecVertexBtag() > 0.679) nbjet_m++;
  }
  int nbjet = nbjet_m;
  
  
  
  TString reg = fake_loose_region;
  /// before third lepton veto no fake estimate can be done.
  if(k_running_nonprompt){
    
    weight      *= Get_DataDrivenWeight_EM(muons, electronAnalysisColl,   eventbase->GetEvent().JetRho());
    
  }
 


  if(!(electronAnalysisColl.size() == 1 && muons.size() == 1)) return;
  
  snu::KParticle emu =  electronAnalysisColl.at(0) + muons.at(0);
  
  if(emu.M()  < 10.) return;

  if ((electronVetoColl.size() + muonVetoColl.size()) ==2){
    if(electronAnalysisColl.at(0).Charge() != muons.at(0).Charge()){
      if(jetColl_lepveto_mva.size() > 1){
	FillCLHist(sighist, "OS_2Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      }
    }
  }

  weight              *= WeightCFEvent(electronAnalysisColl,muons,  k_running_chargeflip);
  if(WeightCFEvent(electronAnalysisColl, muons,  k_running_chargeflip) == 0.) throw LQError( "Fails basic cuts",  LQError::SkipEvent );

  ///// NOW OS event is weighted for CF sample

  if ((electronVetoColl.size() + muonVetoColl.size()) >2) throw LQError( "Fails basic cuts",  LQError::SkipEvent );  
  
  
  
  if(electronAnalysisColl.at(0).Pt() > 20. || muons.at(0).Pt() > 20 ){
    if(jetColl_lepveto_mva.size() == 1&& emu.M() > 100.)
      FillCLHist(sighist, "SS_1Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    if(jetColl_lepveto_mva.size() > 1){
      FillCLHist(sighist, "SS_DiJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      if(nbjet==0){
	FillCLHist(sighist, "SS_0bjet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      }
      else{
	FillCLHist(sighist, "SS_bjet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      }
    }
    
  }
  
  cout << "Number of jets = " << jetColl_lepveto_mva.size() << endl; 
  /// Low Mass
  int index_j1(0),index_j2(0);
  float wmassjj=1000000.;
  if(jetColl_lepveto_mva.size() > 1){
    
    for(unsigned int ij=0; ij < jetColl_lepveto_mva.size()-1; ij++){
      for(unsigned int ij2=ij+1; ij2 < jetColl_lepveto_mva.size(); ij2++){
	snu::KParticle jjtmp = jetColl_lepveto_mva.at(ij) + jetColl_lepveto_mva.at(ij2) ;
	if(fabs(jjtmp.M() - 80.4) < wmassjj) {
	  wmassjj = fabs(jjtmp.M() - 80.4);
	  index_j1=ij;
	  index_j2=ij2;
	}
      }
    }
  } 
  
  
  if(jetColl_lepveto_mva.size() > 1){
    snu::KParticle emujj = jetColl_lepveto_mva.at(index_j1) + jetColl_lepveto_mva.at(index_j2) + electronAnalysisColl.at(0) + muons.at(0) ;
    snu::KParticle jj = jetColl_lepveto_mva.at(index_j1) + jetColl_lepveto_mva.at(index_j2);

    if(emujj.M() < 200. && emujj.M() > 80.){
      if(jj.M() < 120.){
	if(eventbase->GetEvent().PFMET() < 30. && nbjet ==0){
	  FillCLHist(sighist, "SS_lowmass_40", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	  FillCLHist(sighist, "SS_lowmass_50", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	  FillCLHist(sighist, "SS_lowmass_60", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	  FillCLHist(sighist, "SS_lowmass_70", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	  if( emujj.M() >  100.) FillCLHist(sighist, "SS_lowmass_80", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	  
	}
	if((eventbase->GetEvent().PFMET() > 50 && nbjet == 0 ) || (eventbase->GetEvent().PFMET() < 30. && nbjet !=0)){
	  FillCLHist(sighist, "SS_lowmassCR", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
	}
      }
    }
  }
  

  
  if(true){
    /// Low Mass
      int index_j1(0),index_j2(0);
      if(jetColl_lepveto_mva.size() > 1){
	
	float wmassjj=1000000.;
	for(unsigned int ij=0; ij < jetColl_lepveto_mva.size()-1; ij++){
	  for(unsigned int ij2=ij+1; ij2 < jetColl_lepveto_mva.size(); ij2++){
	    snu::KParticle jjtmp = jetColl_lepveto_mva.at(ij) + jetColl_lepveto_mva.at(ij2) ;
	    if(fabs(jjtmp.M() - 80.4) < wmassjj) {
	      wmassjj = fabs(jjtmp.M() - 80.4);
	      index_j1=ij;
	      index_j2=ij2;
	    }
	  }
	}
      }
      if(jetColl_lepveto_mva.size() > 1){
	snu::KParticle emujj = jetColl_lepveto_mva.at(index_j1) + jetColl_lepveto_mva.at(index_j2) + emu ;
	snu::KParticle jj = jetColl_lepveto_mva.at(index_j1) + jetColl_lepveto_mva.at(index_j2)  ;
	
	if(emujj.M() > 80.){
	  if ((jj.M() < 110.) && (jj.M() > 50)) {
	    if(jetColl_lepveto_mva.at(0).Pt() > 30.){
	      if(emu.M() > 15){
		if(eventbase->GetEvent().PFMET() < 35. && nbjet ==0){
		  FillCLHist(sighist, "SS_highmass_90", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  FillCLHist(sighist, "SS_highmass_100", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 140. && ((electronAnalysisColl.at(0).Pt()> 25. && muons.at(0).Pt() > 20)||  (electronAnalysisColl.at(0).Pt()> 20. && muons.at(0).Pt() > 25)))
		    FillCLHist(sighist, "SS_highmass_125", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 160. && ((electronAnalysisColl.at(0).Pt()> 35. && muons.at(0).Pt() > 25)||  (electronAnalysisColl.at(0).Pt()> 25. && muons.at(0).Pt() > 35)))
                    FillCLHist(sighist, "SS_highmass_150", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 200. && ((electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 35)||  (electronAnalysisColl.at(0).Pt()> 35. && muons.at(0).Pt() > 45)))
                    FillCLHist(sighist, "SS_highmass_175", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 220. && ((electronAnalysisColl.at(0).Pt()> 55. && muons.at(0).Pt() > 35)||  (electronAnalysisColl.at(0).Pt()> 35. && muons.at(0).Pt() > 55)))
                    FillCLHist(sighist, "SS_highmass_200", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 270. && ((electronAnalysisColl.at(0).Pt()> 70. && muons.at(0).Pt() > 45)||  (electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 70)))
                    FillCLHist(sighist, "SS_highmass_250", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 290. && ((electronAnalysisColl.at(0).Pt()> 100. && muons.at(0).Pt() > 45)||  (electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 100)))
                    FillCLHist(sighist, "SS_highmass_300", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 290. && ((electronAnalysisColl.at(0).Pt()> 100. && muons.at(0).Pt() > 45)||  (electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 100)))
                    FillCLHist(sighist, "SS_highmass_350", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 290. && ((electronAnalysisColl.at(0).Pt()> 100. && muons.at(0).Pt() > 45)||  (electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 100)))
                    FillCLHist(sighist, "SS_highmass_400", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		  if(emujj.M() > 290. && ((electronAnalysisColl.at(0).Pt()> 100. && muons.at(0).Pt() > 45)||  (electronAnalysisColl.at(0).Pt()> 45. && muons.at(0).Pt() > 100)))
                    FillCLHist(sighist, "SS_highmass_500", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		}

		if((eventbase->GetEvent().PFMET() > 50 && nbjet == 0 ) || (eventbase->GetEvent().PFMET() < 35. && nbjet !=0)){
		  FillCLHist(sighist, "SS_highmassCR", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
		}
	      }
	    }
	  }
	}
      }
  }


  if(electronAnalysisColl.at(0).Pt() > 20. && muons.at(0).Pt() > 15){
    if(jetColl_lepveto_mva.size() == 1&& emu.M() > 100.)  
      FillCLHist(sighist, "SSemu_1Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    
    if(jetColl_lepveto_mva.size() > 1)
      FillCLHist(sighist, "SSemu_DiJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    
    if(jetColl_lepveto_mva.size() == 2)
      FillCLHist(sighist, "SSemu_2Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    
    if(jetColl_lepveto_mva.size() == 3)
      FillCLHist(sighist, "SSemu_3Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    
    if(jetColl_lepveto_mva.size() > 3)
      FillCLHist(sighist, "SSemu_4Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    

    if(jetColl_lepveto_mva.size() > 1){
      if(nbjet == 0)
	FillCLHist(sighist, "SSemu_0BJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      else
	FillCLHist(sighist, "SSemu_BJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    }
    
  }
  
    if(electronAnalysisColl.at(0).Pt() > 15. && muons.at(0).Pt() > 20.){
      if(jetColl_lepveto_mva.size() == 1&& emu.M() > 100.)
	FillCLHist(sighist, "SSmue_1Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);

    if(jetColl_lepveto_mva.size() > 1)
      FillCLHist(sighist, "SSmue_DiJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);

    if(jetColl_lepveto_mva.size() == 2)
      FillCLHist(sighist, "SSmue_2Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);

    if(jetColl_lepveto_mva.size() == 3)
      FillCLHist(sighist, "SSmue_3Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);

    if(jetColl_lepveto_mva.size() > 3)
      FillCLHist(sighist, "SSmue_4Jet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);

    if(jetColl_lepveto_mva.size() > 1){
      if(nbjet == 0)
	FillCLHist(sighist, "SSmue_0BJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
      else
	FillCLHist(sighist, "SSmue_BJet", eventbase->GetEvent(), muons ,electronAnalysisColl,jetColl_lepveto_mva, weight);
    }
  }
  
}// End of exeucte event loop



float HNEMu::WeightCFEvent(std::vector<snu::KElectron> electrons, std::vector<snu::KMuon> muons, bool runchargeflip, bool useoldrates){

  if(electrons.size()!=1) return 0.;
  if(muons.size()!=1) return 0.;
  
  if(runchargeflip) {
    if(electrons.at(0).Charge() != muons.at(0).Charge()) {
      float cf1=  CFRate(electrons.at(0), useoldrates);

      return  cf1;
    }// OS requirement
    else return 0.;
  }// cf requirement
  else {
    if(electrons.at(0).Charge() != muons.at(0).Charge()) return 0.;
  }
  
  return 1.;
  
}


void HNEMu::EndCycle()throw( LQError ){
  
  Message("In EndCycle" , INFO);
  m_logger << INFO << "Number of os mc events = " << m_os_Z_nw  << LQLogger::endmsg; 
  m_logger << INFO << "Number of os mc events (weighted) = " << m_os_Z  << LQLogger::endmsg; 
  m_logger << INFO << "Number of ss mc events = " << m_ss_Z_nw  << LQLogger::endmsg; 
  m_logger << INFO << "Number of ss mc events (weighted)= " << m_ss_Z  << LQLogger::endmsg; 
}


void HNEMu::BeginCycle() throw( LQError ){
  
  Message("In begin Cycle", INFO);
  
  string analysisdir = getenv("FILEDIR");  
  
  if(!k_isdata) reweightPU = new Reweight((analysisdir + "MyDataPileupHistogram_69400.root").c_str());

  //
  //If you wish to output variables to output file use DeclareVariable
  // clear these variables in ::ClearOutputVectors function
  //DeclareVariable(obj, label, treename );
  //DeclareVariable(obj, label ); //-> will use default treename: LQTree
  

  return;
  
}

HNEMu::~HNEMu() {
  
  Message("In HNEMu Destructor" , INFO);
  if(!k_isdata)delete reweightPU;

 }
     

void HNEMu::FillEventCutFlow(TString cut, TString label , float weight){

  if(GetHist(label + "_eventcutflow")) {
    GetHist(label + "_eventcutflow")->Fill(cut,weight);

  }
  else{
    AnalyzerCore::MakeHistograms(label + "_eventcutflow",19,0.,19.);

    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(1,"NoCut");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(2,"NoCut_w");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(3,"eventcut");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(4,"TriggerCut");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(5,"VertexCut");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(6,"DiEl");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(7,"eedR");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(8,"SSDiEl");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(9,"SS_lepveto");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(10,"DiJet");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(11,"Presel");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(12,"Presel_noZ");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(13,"Presel_nobjet");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(14,"lowmass");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(15,"lowmassCR");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(16,"mediummass");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(17,"mediummassCR");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(18,"highmass");
    GetHist(label + "_eventcutflow")->GetXaxis()->SetBinLabel(19,"highmassCR");
  }
  
}

     
void HNEMu::FillCutFlow(TString cut, float weight){
  
  if(GetHist("cutflow")) {
    GetHist("cutflow")->Fill(cut,weight);
  }
  else{
    AnalyzerCore::MakeHistograms("cutflow",16,0.,16.);
    
    GetHist("cutflow")->GetXaxis()->SetBinLabel(1,"SS_NoCut");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(2,"SS_Tight");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(3,"SS_Tight_convveto");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(4,"SS_Tight_d0veto");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(5,"SS_Tight_reliso");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(6,"SS_Medium_chargeconst");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(7,"SS_Tight_chargeconst");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(8,"SS_Tight_noclosejet");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(9,"SS_anal_el");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(10,"Signal_anal");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(11,"Signal_Tightlooseiso_d0");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(12,"Signal_Mediumlooseiso_d0");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(13,"Signal_drcut1");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(14,"Signal_drcut2");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(15,"Signal_anal_dr1");
    GetHist("cutflow")->GetXaxis()->SetBinLabel(16,"Signal_anal_dr2");
  }
}
     
     
void HNEMu::FillIsoCutFlow(TString cut, float weight){
       
  
  if(GetHist("isocutflow")) {
    GetHist("isocutflow")->Fill(cut,weight);
    
  }
  else{
    AnalyzerCore::MakeHistograms("isocutflow",36,0.,36.);
    
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(1,"iso_d0_03_iso3_60");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(2,"iso_d0_03_iso3_50");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(3,"iso_d0_03_iso3_40");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(4,"iso_d0_03_iso3_30");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(5,"iso_d0_03_iso3_20");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(6,"iso_d0_03_iso3_10");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(7,"iso_d0_03_iso3_09");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(8,"iso_d0_03_iso3_075");
    
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(10,"iso_d0_02_iso3_60");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(11,"iso_d0_02_iso3_50");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(12,"iso_d0_02_iso3_40");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(13,"iso_d0_02_iso3_30");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(14,"iso_d0_02_iso3_20");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(15,"iso_d0_02_iso3_10");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(16,"iso_d0_02_iso3_09");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(17,"iso_d0_02_iso3_075");

    GetHist("isocutflow")->GetXaxis()->SetBinLabel(19,"iso_d0_01_iso3_60");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(20,"iso_d0_01_iso3_50");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(21,"iso_d0_01_iso3_40");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(22,"iso_d0_01_iso3_30");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(23,"iso_d0_01_iso3_20");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(24,"iso_d0_01_iso3_10");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(25,"iso_d0_01_iso3_09");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(26,"iso_d0_01_iso3_075");

    GetHist("isocutflow")->GetXaxis()->SetBinLabel(28,"iso_d0_005_iso3_60");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(29,"iso_d0_005_iso3_50");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(30,"iso_d0_005_iso3_40");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(31,"iso_d0_005_iso3_30");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(32,"iso_d0_005_iso3_20");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(33,"iso_d0_005_iso3_10");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(34,"iso_d0_005_iso3_09");
    GetHist("isocutflow")->GetXaxis()->SetBinLabel(35,"iso_d0_005_iso3_075");

    
    
  }
}


void HNEMu::BeginEvent( )throw( LQError ){

  Message("In BeginEvent() " , DEBUG);

  return;
}


///############### THESE ARE FUNCTIONS SPECIFIC TO THIS CYCLE

void HNEMu::MakeHistograms(){
  //// Additional plots to make
    
  maphist.clear();
  AnalyzerCore::MakeHistograms();
  Message("Made histograms", INFO);
  /**
   *  Remove//Overide this HNEMuCore::MakeHistograms() to make new hists for your analysis
   **/
  
}


void HNEMu::ClearOutputVectors() throw(LQError) {

  // This function is called before every execute event (NO need to call this yourself.
  
  // Add any new output vector you create to this list. 
  // if you do not the vector will keep keep getting larger when it is filled in ExecuteEvents and will use excessive amoun of memory
  //
  // Reset all variables declared in Declare Variable
  //
  out_muons.clear();
  out_electrons.clear();
}




//  LocalWords:  masscuts jetResdown
