#ifndef Detector_h
#define Detector_h

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
public:
  SensitiveDetector(G4String);
  ~SensitiveDetector();

private:
  virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
  G4int PreviousEdepEventID;
  G4int CurrentEdepEventID;
  G4int TotalEdepEvent;
  G4int fCurrentEventID;
  G4int fCurrentTrackID;
  G4int TritonCount;
};

#endif
