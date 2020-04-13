{
    gSystem->AddLinkedLibs( "-lboost_filesystem -lboost_system" );
    gSystem->AddLinkedLibs( "-lowonparse" );
    gSystem->AddIncludePath( "-I../inc");
    gROOT->LoadMacro( "../src/ROOT/OwonTreeCreator/OwonTreeCreator.cpp++" );
    //gROOT->LoadMacro( "../src/ROOT/Drawer/Drawer.cpp+" );
}
