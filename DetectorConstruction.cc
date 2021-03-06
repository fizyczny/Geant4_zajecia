//
// $Id: DetectorConstruction.cc 12.16.2016 A. Fijalkowska $
//
/// \file DetectorConstruction.cc
/// \brief DetectorConstruction class
//
//
#include "DetectorConstruction.hh"
#include "G4PVPlacement.hh" //tworzenie physical volume
#include "G4SystemOfUnits.hh" //jednostki
#include "G4VisAttributes.hh" //sposob wyświetlania, kolory itp
#include "G4Box.hh" //prostopadłościan
#include "G4Tubs.hh" //walec
#include "G4ThreeVector.hh" //trzyelementowy wektor wbudowany w geant
#include "globals.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"


DetectorConstruction::DetectorConstruction()
{
    worldLogic = 0L;
    fantomLogVol=0L;
    cylinderLogVol=0L;
    man = G4NistManager::Instance();
}



DetectorConstruction::~DetectorConstruction() 
{
    if (worldLogic!=0L)
        delete worldLogic;
    if (fantomLogVol!=0L)
	delete fantomLogVol;
    if (cylinderLogVol!=0L)
	delete cylinderLogVol;
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4VPhysicalVolume* worldPhys = ConstructWorld();
    ConstructHumanFantom();
    ConstructCylinder();
    ConstructNaIDet();
    return worldPhys;
}


G4VPhysicalVolume* DetectorConstruction::ConstructWorld()
{

    G4double worldX = 1.5*m;
    G4double worldY = 1.5*m;
    G4double worldZ = 1.5*m;
    
    G4Box* worldSolid = new G4Box("worldSolid",worldX,worldY,worldZ);

   
    //G4Material* vaccum = new G4Material("GalacticVacuum", 1., 1.01*g/mole,
      //                     CLHEP::universe_mean_density, 
        //                   kStateGas, 3.e-18*pascal, 2.73*kelvin);

    G4Material* air= man->FindOrBuildMaterial("G4_AIR");

    worldLogic = new G4LogicalVolume(worldSolid, air,"worldLogic", 0,0,0);
                                             
    //worldLogic->SetVisAttributes(G4VisAttributes::Invisible);
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "world", 0, false, 0); //matka,false,nr_kopii
    return worldPhys;

}
G4Material* DetectorConstruction::MakeWater()
{
    G4Element* H = man->FindOrBuildElement("H");
    G4Element* O = man->FindOrBuildElement("O");
    G4Material* water = new G4Material("water", 1.0*g/cm3, 2);
    water->AddElement(H, 2);
    water->AddElement(O, 1);
    return water;
}

void DetectorConstruction::ConstructHumanFantom()
{   
    G4LogicalVolume* spineLogic = ConstructHumanSpine(); 

    G4double radiusMin = 0;
    G4double radiusMax = 15*cm;
    G4double length = 170*cm; 
    G4Tubs* fantomSolid = new G4Tubs("fantomSolid", radiusMin, radiusMax, length/2., 0*deg, 360*deg);
    G4Material* water=MakeWater();
    fantomLogVol = new G4LogicalVolume(fantomSolid, water, "fantomLogVol");

     
    G4VisAttributes* fantomVisAtt = new G4VisAttributes( G4Colour(1,0.8,0.8));
    fantomVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    //fantomVisAtt->SetForceSolid(true);
	
    fantomLogVol->SetVisAttributes(fantomVisAtt); 

    G4ThreeVector pos(10*cm,0,42.5*cm);    
    new G4PVPlacement(0, pos, spineLogic, "spinePhys", fantomLogVol, 0, 0);
    G4ThreeVector pos2(0,0,0);
    new G4PVPlacement(0, pos2, fantomLogVol, "fantomPhys", worldLogic, 0, 0);
}


G4LogicalVolume* DetectorConstruction:: ConstructHumanSpine()
{

    G4double radiusMin = 0;
    G4double radiusMax = 3*cm;
    G4double length = 85*cm; 
    G4Tubs* spineSolid = new G4Tubs("spineSolid", radiusMin, radiusMax, length/2., 0*deg, 360*deg);
    G4Material* bone= man->FindOrBuildMaterial("G4_B-100_BONE");
    G4LogicalVolume* spineLogic = new G4LogicalVolume(spineSolid, bone, "spineLogic");

     
    G4VisAttributes* spineVisAtt = new G4VisAttributes( G4Colour(0,1,0));
    spineVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
    spineVisAtt->SetForceSolid(true);
    spineLogic->SetVisAttributes(spineVisAtt); 
    return spineLogic;
	
}

void DetectorConstruction::ConstructCylinder()
{
     G4Material* ppy = man->FindOrBuildMaterial("G4_POLYPROPYLENE");
     G4double rMin = 40*cm;
     G4double rMax = 55*cm;
     G4double length = 15*cm;
     G4Tubs* theCylinder = new G4Tubs("theCylinder", rMin, rMax, length/2, 0*deg, 360*deg);
     cylinderLogVol = new G4LogicalVolume(theCylinder, ppy, "cylinderLogVol");

     G4VisAttributes* cylinderAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5, 0.7));
     cylinderAtt->SetForceAuxEdgeVisible(true);
     cylinderAtt->SetForceSolid(true);
     cylinderLogVol->SetVisAttributes(cylinderAtt);

     G4ThreeVector pos(0,0, 0);
     new G4PVPlacement(0,pos,cylinderLogVol, "cylinderPhy",worldLogic,0,0);
}


void DetectorConstruction:: ConstructNaIDet()
{
     G4LogicalVolume* aluLogic = ConstructAluLayer(); 
     G4LogicalVolume* teflonLogic = ConstructTeflonLayer();   
     G4LogicalVolume* NaILogic = ConstructNaILayer();   

     G4ThreeVector pos(0,0, 0);
     new G4PVPlacement(0, pos, NaILogic, "NaIPhys", teflonLogic, 0,0); 
     new G4PVPlacement(0, pos, teflonLogic, "teflonPhys", aluLogic, 0,0); 
     
     double ringRadius=46.6*cm;
     G4ThreeVector naiDetPos(0,ringRadius,0);
     G4RotationMatrix* rot = new G4RotationMatrix();
     rot->rotateX(90*deg);
     new G4PVPlacement(rot, naiDetPos, aluLogic, "aluPhys", cylinderLogVol, 0,0); 
     
     for (int i=1;i<=36; i++)
     {
     	
     	double angle=i*10*deg;
     
     	G4ThreeVector naiDetPos2(ringRadius*sin(angle),ringRadius*cos(angle),0);
     	G4RotationMatrix* rot2 = new G4RotationMatrix();
     	rot2->rotateZ(angle);
     	rot2->rotateX(90*deg);
     	new G4PVPlacement(rot2, naiDetPos2, aluLogic, "aluPhys", cylinderLogVol, 0,i); //numer kopii
     }

     
}
	
G4LogicalVolume* DetectorConstruction:: ConstructAluLayer()
{
     G4double rMin=0;
     G4double rMax=3.6*cm;
     G4double hLenght=5.6*cm;
     G4Tubs* aluSolid = new G4Tubs("aluSolid", rMin, rMax, hLenght, 0*deg, 360*deg);
     
     G4Material* alu = new G4Material("aluminium", 2.7*g/cm3, 1);
     G4Element* Al = man->FindOrBuildElement("Al");
     alu ->AddElement(Al,1);
     
     G4LogicalVolume* aluLogic = new G4LogicalVolume(aluSolid, alu,"aluLogic");
     G4VisAttributes* aluVis = new G4VisAttributes(G4Colour(1,0.4,0));
     aluVis->SetForceAuxEdgeVisible(true);
     //aluVis->SetForceSolid(true);
     aluLogic->SetVisAttributes(aluVis);

     return aluLogic;
}

G4LogicalVolume* DetectorConstruction:: ConstructTeflonLayer()
{
     G4double rMin=0;
     G4double rMax=3.3*cm;
     G4double hLenght=5.3*cm;
     G4Tubs* solid = new G4Tubs("solid", rMin, rMax, hLenght, 0*deg, 360*deg);
     
     G4Material* teflon = man->FindOrBuildMaterial("G4_TEFLON ");
     
     G4LogicalVolume* teflonLogic = new G4LogicalVolume(solid, teflon,"teflonLogic");
     G4VisAttributes* visAttr = new G4VisAttributes(G4Colour(1, 1, 1));
     visAttr->SetForceAuxEdgeVisible(true);
     //visAttr->SetForceSolid(true);
     teflonLogic->SetVisAttributes(visAttr);

     return teflonLogic;
}

G4LogicalVolume* DetectorConstruction:: ConstructNaILayer()
{
     G4double rMin=0;
     G4double rMax=3*cm;
     G4double hLenght=5*cm;
     G4Tubs* solid = new G4Tubs("solid", rMin, rMax, hLenght, 0*deg, 360*deg);
     
     G4Material* NaI = man->FindOrBuildMaterial("G4_SODIUM_IODIDE");
     
     G4LogicalVolume* NaILogic = new G4LogicalVolume(solid, NaI,"NaILogic");
     G4VisAttributes* visAttr = new G4VisAttributes(G4Colour(0, 1, 1));
     visAttr->SetForceAuxEdgeVisible(true);
     visAttr->SetForceSolid(true);
     NaILogic->SetVisAttributes(visAttr);

     return NaILogic;
}


void DetectorConstruction::ConstructSDandField() 
{


}






