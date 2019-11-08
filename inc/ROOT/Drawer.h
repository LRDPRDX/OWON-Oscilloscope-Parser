#ifndef DRAWER_H
#define DRAWER_H

#include <string>

#include <TROOT.h>                                                                         
#include <TChain.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TArrow.h>
#include <TMarker.h>
#include <TPolyLine.h>
#include <TPolyMarker.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TF1.h>
#include <TH1F.h>


class Drawer
{
    private :
        TTree*          fTree;//pointer to analyzed tree
        Long64_t        fNEntries;
        Long64_t        fCurrentEntry;
        //Declaration of leaf types
        Float_t         baseline;
        Float_t         amplitude;        
        Float_t         amplitudeTime;
        Float_t         triggerTime;
        Float_t         timeStep;
        Float_t         integral;
        Float_t         pkpk;
        Int_t           size;
        std::string*    pathToBinFile;
        //For drawing
        TCanvas*        fCanvas;
        TGraph*         fWaveform;
        TLine*          fBaselineLine;
        TMarker*        fTriggerTimeMarker;
 
        void            InitTree( TTree *tree );
        void            InitGraphics(); 
 
 
    public :
        Drawer( TTree* tree );
        ~Drawer();
 
        void            Next( const std::string& cut );
        void            Draw( const std::string& path );
        void            AverageWaveform( const std::string& sampleName, const std::string& cut );
};
#endif
