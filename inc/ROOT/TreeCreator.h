/*
 * Class to create TTree from list of .bin files
 */
#ifndef TREE_CREATOR_H
#define TREE_CREATOR_H

#include "../inc/oscilloscope.h"

#include <experimental/filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <exception>

#include <TROOT.h>


namespace fs = std::experimental::filesystem; 


class TreeCreator
{
    protected :
        owon::oscilloscope* fOsc;
        owon::CHANNEL       fActiveChannel;

        std::string         fPathToDataDir;
        std::string         fPathToTreeFile;
        std::string         fTreeFileName;

        Double_t            fBaselineTime;
        Double_t            fIntegralStart;
        Double_t            fIntegralStop;

        //Auxiliary methods
        std::string     MergePaths( const std::string& parent, const std::string& child );
        bool            CompareExtension( const std::string& fileName, const std::string& extension ); 

    public:
        enum SAMPLE { ALL, INCLUDE, EXCLUDE };

    public :
        TreeCreator( owon::oscilloscope* osc,
                     const std::string& pathToDataDir,
                     const std::string& pathToTreeFile,
                     const std::string& treeFileName ) :
            fOsc( osc ),
            fActiveChannel( owon::CH1 ),
            fPathToDataDir( pathToDataDir ),
            fPathToTreeFile( pathToTreeFile ),
            fTreeFileName( treeFileName + ".root" ),
            fBaselineTime( -1 ),
            fIntegralStart( -1 ),
            fIntegralStop( -1 )
        {
            if( osc == nullptr )
            {
                throw std::invalid_argument( "Bad oscilloscope pointer" );
            }
        }

    public:
        void SetPathToDataDir( const std::string& pathToDataDir )
        {
            fPathToDataDir = pathToDataDir;
        }
        void SetPathToTreeFile( const std::string& pathToTreeFile )
        {
            fPathToTreeFile = pathToTreeFile;
        }
        void SetTreeFileName( const std::string& treeFileName )
        {
            fTreeFileName = treeFileName + ".root";
        } 
        void SetGate( Double_t integralStart, Double_t integralStop )
        {
            fIntegralStart = integralStart;
            fIntegralStop = integralStop;
        } 
        void SetBaselineTime( Double_t baselineTime )
        {
            fBaselineTime = baselineTime;
        }
        void SetActiveChannel( owon::CHANNEL activeChannel )
        {
            fActiveChannel = activeChannel;
        }

    public:
        std::string GetPathToDataDir() { return fPathToDataDir; }
        std::string GetPathToTreeFile() { return fPathToTreeFile; }
        std::string GetTreeFileName() { return fTreeFileName; }

        void GetListOfSamples( std::vector< std::string >& sampleNames,
                               bool fullPath = true );
        void GetListOfFiles( std::vector< std::string >& fileNames,
                             bool fullPath = true,
                             std::string pathToParentDir = "" );

    public:
        bool CreateTree( SAMPLE mode = ALL, const std::string& target = "" );
};
#endif
