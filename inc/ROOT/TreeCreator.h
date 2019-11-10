/*
 * Class to create TTree from list of .bin files
 */
#ifndef OWON_TREE_CREATOR_H
#define OWON_TREE_CREATOR_H

#include "../inc/owon_oscilloscope.h"

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
        Double_t            fBeforeTrigger;
        Double_t            fAfterTrigger;

        //Auxiliary methods
        std::string     MergePaths( const std::string& parent, const std::string& child ) const;
        bool            CompareExtension( const std::string& fileName, const std::string& extension ) const; 

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
            fBaselineTime( 0 ),
            fBeforeTrigger( 0 ),
            fAfterTrigger( 0 )
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
        void SetGate( Double_t beforeTrigger, Double_t afterTrigger )
        {
            fBeforeTrigger = beforeTrigger;
            fAfterTrigger = afterTrigger;
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
        std::string GetPathToDataDir() const { return fPathToDataDir; }
        std::string GetPathToTreeFile() const { return fPathToTreeFile; }
        std::string GetTreeFileName() const { return fTreeFileName; }

        void GetListOfSamples( std::vector< std::string >& sampleNames,
                               bool fullPath = true ) const;
        void GetListOfFiles( std::vector< std::string >& fileNames,
                             bool fullPath = true,
                             std::string pathToParentDir = "" ) const;

    public:
        void CreateTree( SAMPLE mode = ALL, const std::string& target = "" );
};
#endif
