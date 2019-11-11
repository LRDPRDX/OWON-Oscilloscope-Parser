{
    gSystem->AddLinkedLibs( "/usr/lib/gcc/x86_64-linux-gnu/5/libstdc++fs.a" );
    gSystem->AddLinkedLibs( "/usr/lib/libowonparse.so" );
    gSystem->AddIncludePath( "-I../inc");
    gROOT->LoadMacro( "../src/ROOT/OwonTreeCreator/OwonTreeCreator.cpp+" );
    //gROOT->LoadMacro( "../src/ROOT/Drawer/Drawer.cpp+" );
}
