#ifndef ElectronCF_h
#define ElectronCF_h

#include "AnalyzerCore.h"


class ElectronCF : public AnalyzerCore {

 public:
  //// constructors                                                                                                                                                             
  ElectronCF();
  ~ElectronCF();

  /// Functions from core
  virtual void BeginCycle() throw( LQError );
  virtual void BeginEvent()throw( LQError );
  virtual void ExecuteEvents()throw( LQError );
  virtual void EndCycle()throw( LQError );
  virtual void ClearOutputVectors()throw( LQError );
  
  void InitialiseAnalysis() throw( LQError );
  void MakeHistograms();
  void FillCutFlow(int cut, float w, TString tag);

 private:
  
  //
  // The output variables 
  //
  /// Vectors for output objetcs
  std::vector<snu::KMuon> out_muons;
  std::vector<snu::KElectron> out_electrons;

  float Z_el_denom;
  float Z_el_num;
  ClassDef ( ElectronCF, 1);
};
#endif
