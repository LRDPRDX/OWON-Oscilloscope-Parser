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
    tc.SetBaselineTime( 0 );//from 0 to trigger (see documentation)
    tc.SetGate( 0, 3 );//from trigger to 3us after trigger (see documentation)

    //To create only one TTree from the dir "./Data/First" use this
    tc.CreateTree( TreeCreator::SAMPLE::INCLUDE, "First" );
    //To create TTrees from all directories use this
    //tc.CreateTree();
}
