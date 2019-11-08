#include "ROOT/TreeCreator.h"
#include "device/tds8204.h"


void CreateTree()
{
    //Create parser of the model which produced the binaries
    owon::tds8204::parser p;

    //Create TreeCreator:
    //The first argument --- the pointer to the parser which
    //will be used to parse the binaries.
    //
    //The second argument --- the path to the directory
    //containing binaries (the documentation is recommended here) 
    //
    //The third argument --- the path where to place the
    //resulting TTree
    //
    //The fourth argument --- the name of the file
    //where the resulting TTree will be placed (without an extension)
    TreeCreator tc( &p, "./Data/", "./", "myFirstTree" );
    //Set active channel
    tc.SetActiveChannel( owon::CH1 );
    //Set analysis config
    tc.SetBaselineTime( -1 );//from 0 to 0.5*trigger (see documentation)
    tc.SetGate( -1, 3 );//from 0 to 3us (see documentation)

    tc.CreateTree();
}
