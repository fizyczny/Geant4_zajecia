#ifndef Crystal_H
#define Crystal_H 1
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

class Crystal
{
    public:
    Crystal(double rMin, double rMax, double length);
    void Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo);
    
    private:
    void ConstructCylinder(double rMin, double rMax, double length);
    //void ConstructNaIDet();
    G4LogicalVolume* ConstructAluLayer();
    G4LogicalVolume* ConstructTeflonLayer();
    G4LogicalVolume* ConstructCrystal();
    G4LogicalVolume* cylinderLogVol;   
    

};

#endif
