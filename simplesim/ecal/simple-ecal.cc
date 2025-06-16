


#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
//#include "RunManager.hh"
#include "G4MTRunManager.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4SteppingVerbose.hh"

#include "FTFP_BERT.hh"

#include "Randomize.hh"


void PrintUsage() {
    G4cerr << "Usage: " << G4endl;
    G4cerr << "simple-ecal [-m macro] [-u UIsession] [-t nThreads] [-vDefault]"
           << G4endl;
}


int main(int argc, char** argv)
{
    G4String macro;
    G4String session;
    G4bool verboseBestUnits = true;
#ifdef G4MULTITHREADED
    G4int nThreads = 0;
#endif
    for ( G4int i=1; i<argc; i=i+2 ) {
        if ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
        else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
        else if ( G4String(argv[i]) == "-t" ) {
            nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
        }
#endif
        else if ( G4String(argv[i]) =="-vDefault" ) {
            verboseBestUnits = false;
            --i;  // this option is not followed with a parameter
        }
        else {
            PrintUsage();
            return 1;
        }
    }

    G4UIExecutive *ui = nullptr;
    if ( !macro.size() ) {
        ui = new G4UIExecutive(argc, argv, session);
    }

    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    if ( verboseBestUnits ) {
        G4int precision = 4;
        G4SteppingVerbose::UseBestUnit(precision);
    }

    //auto * runManager = new secal::TheRunManager();
    auto * runManager = new G4MTRunManager();
#ifdef G4MULTITHREADED
    if ( nThreads > 0 ) {
        runManager->SetNumberOfThreads(nThreads);
    }
#endif

    auto detConstruction = new secal::DetectorConstruction();
    runManager->SetUserInitialization(detConstruction);

    auto physicsList = new FTFP_BERT;
    runManager->SetUserInitialization(physicsList);

    auto actionInitialization = new secal::ActionInitialization();
    runManager->SetUserInitialization(actionInitialization);

    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    auto UImanager = G4UImanager::GetUIpointer();

    // process macro
    if ( macro.size() ) {
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);
    }
    else {
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        if ( ui->IsGUI() ) 
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}
