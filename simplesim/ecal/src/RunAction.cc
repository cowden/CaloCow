
#include "RunAction.hh"
#include "G4Run.hh"
#include "CXHDF5.hh"
#include "G4Threading.hh"

#include <sstream>
#include <vector>

namespace secal {

RunAction::RunAction(CXHDF5 * data)
:G4UserRunAction(),
data_out_(data)
{ }

RunAction::~RunAction()
{
    delete data_out_; 
}

G4Run* RunAction::GenerateRun()
{
    return (new G4Run);
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    std::vector<int> dims(1);
    dims[0] = 25 + 3;
    data_out_->initialize(dims);

    int thrd = G4Threading::G4GetThreadId();
    std::stringstream ofilename;
    ofilename << data_out_->get_base_name() 
      << "_t_"
      << G4Threading::G4GetThreadId()
      << ".h5";
    data_out_->open_file(ofilename.str());
}

void RunAction::EndOfRunAction(const G4Run* run)
{ }

}
