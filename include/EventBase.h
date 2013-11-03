#ifndef EventBase_h
#define EventBase_h

#include <iostream>
using namespace std;


#include "TLorentzVector.h"
#include <vector>
#include "LQEvent.h"
#include "MuonSelection.h"
#include "ElectronSelection.h"
#include "TauSelection.h"
#include "JetSelection.h"
#include "EventSelection.h"
#include "KMuon.h"
#include "KElectron.h"
#include "KJet.h"

class  EventBase {

 public:
  EventBase();
  EventBase(LQEvent kbase);
  EventBase(EventBase& evbase);
  EventBase& operator= (const EventBase& obj);
  ~EventBase();
  
  inline MuonSel* GetMuonSel() const {return k_muonsel;}
  inline ElectronSel* GetElectronSel() const {return k_electronsel;}
  inline JJ* GetJetSel() const {return k_jetsel;}
  inline TauSel* GetTauSel() const {return k_tausel;}
  inline EventSel* GetEventSel() const {return k_eventsel;}

  inline std::vector<snu::KMuon> GetBaseMuons() const {return k_LQevent.GetBaseMuons();}
  inline std::vector<snu::KElectron> GetBaseElectrons() const {return k_LQevent.GetBaseElectrons();}
  inline std::vector<snu::KJet> GetBaseJets() const {return k_LQevent.GetBaseJets();}
  inline snu::KEvent GetBaseEvent() const {return k_LQevent.GetBaseEvent();}

  ///Copy constructor
  EventBase(const EventBase& sb);

  inline LQEvent GetEventBase() const {return k_LQevent;}
  
  LQEvent k_LQevent;
  MuonSel* k_muonsel;
  ElectronSel* k_electronsel;
  JJ* k_jetsel;
  TauSel* k_tausel;
  EventSel* k_eventsel;
  
 
};

#endif