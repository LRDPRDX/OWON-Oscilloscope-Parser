#include "ROOT/TreeCreator.h"

#include "analyzer.h"
#include <TFile.h>
#include <TTree.h>


std::string TreeCreator::MergePaths( const std::string& parent,
                                     const std::string& child )
{
    if( parent.empty()  and  child.empty() )
    {
        return "";
    }

    if( parent.back() == '/' )
    {
        if( child.front() == '/' )
        {
            return parent.substr( 0, parent.size() - 1 ) + child;
        }
        else
        {
            return parent + child;
        }
    }
    else if( child.front() == '/' )
    {
        return parent + child;
    }
    else
    {
        return parent + '/' + child;
    }
}


bool TreeCreator::CompareExtension( const std::string& fileName,
                                    const std::string& extension )
{
    if( fileName.length() > extension.length() )
    {
        if( fileName.rfind( extension ) != (fileName.length() - extension.length()) )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}


void TreeCreator::GetListOfSamples( std::vector< std::string >& sampleNames,
                                    bool fullPath )
{
    sampleNames.clear();

    try
    {
        if( fs::exists( fPathToDataDir ) && fs::is_directory( fPathToDataDir ) )
        {
            fs::directory_iterator iter( fPathToDataDir );
            fs::directory_iterator end;

            while( iter != end )
            {
                if( fs::is_directory( iter->path() ) ) 
                {
                    if( fullPath )
                    {
                        sampleNames.push_back( iter->path().string() ); 
                    }
                    else
                    {
                        sampleNames.push_back( iter->path().filename() ); }
                }
                
                std::error_code ec;
                iter.increment( ec );
                if( ec )
                {
                    std::cerr << "WARNING :: Error while accessing : " << iter->path().string() << " :: " << ec.message() << "\n";
                }
            }
        }
    }
    catch( std::system_error& e )
    {
        std::cerr << "Exception :: " << e.what();
    }
}


void TreeCreator::GetListOfFiles( std::vector< std::string >& fileNames,
                                  bool fullPath,
                                  std::string pathToParentDir )
{
    fileNames.clear();
    if( pathToParentDir == "" ) { pathToParentDir = fPathToDataDir; }

    try
    {
        if( fs::exists( pathToParentDir ) && fs::is_directory( pathToParentDir ) )
        {
            fs::recursive_directory_iterator iter( pathToParentDir );
            fs::recursive_directory_iterator end;

            while( iter != end )
            {
                if( CompareExtension( (std::string)iter->path().string(), ".bin" ) )
                {
                    if( fullPath )
                    {
                        fileNames.push_back( iter->path().string() );
                    }
                    else
                    {
                        fileNames.push_back( iter->path().filename() );
                    }
                }
                
                std::error_code ec;
                iter.increment( ec );
                if( ec )
                {
                    std::cerr << "ERROR :: Error while accessing : " << iter->path().string() << " :: " << ec.message() << "\n";
                }
            }
        }
    }
    catch( std::system_error& e )
    {
        std::cerr << "Exception :: " << e.what();
    }
}


bool TreeCreator::CreateTree( SAMPLE mode, const std::string& target )
{
    std::vector< std::string > samples;
    GetListOfSamples( samples, false );
    std::sort( samples.begin(), samples.end() );

    if( samples.empty() )
    {
        samples = { "" };
    }

    TFile* fileTree = new TFile( MergePaths( fPathToTreeFile, fTreeFileName ).c_str(), "RECREATE" );

    Float_t baseline;
    Float_t amplitude;
    Float_t amplitudeTime;
    Float_t triggerTime;
    Float_t timeStep;
    Float_t integral;
    Float_t pkpk;
    UInt_t  size;
    std::string activeChannel;
    std::string pathToBinFile;
    std::string model;

    for( auto sample : samples )
    {
        if( (mode == SAMPLE::INCLUDE)  &&  (sample != target) )
        {
            continue;
        }
        else if( (mode == SAMPLE::EXCLUDE)  &&  (sample == target) )
        {
            continue;
        }

        std::cout << "INFO :: Processing of sample '" << sample << "' begins. Waiting ..." << std::endl;

        TTree tree( ("tree_"+sample).c_str(), ("tree_"+sample).c_str() );
            tree.Branch( "baseline", &baseline, "baseline/F" );
            tree.Branch( "amplitude", &amplitude, "amplitude/F" );
            tree.Branch( "amplitudeTime", &amplitudeTime, "amplitudeTime/F" );
            tree.Branch( "triggerTime", &triggerTime, "triggerTime/F" );
            tree.Branch( "timeStep", &timeStep, "timeStep/F" );
            tree.Branch( "integral", &integral, "integral/F" );
            tree.Branch( "pkpk", &pkpk, "pkpk/F" );
            tree.Branch( "size", &size, "size/I" );
            tree.Branch( "activeChannel", &activeChannel );
            tree.Branch( "pathToBinFile", &pathToBinFile );
            tree.Branch( "model", &model );

        owon::analyzer analyzerObject( fOsc );
        analyzerObject.set_baseline_method( owon::analyzer::BASELINE::AVERAGE );

        std::vector< std::string > files;

        GetListOfFiles( files, true, MergePaths( fPathToDataDir, sample ) );

        for( auto file : files )
        {
            fOsc->parse( file );
            fOsc->set_active_channel( fActiveChannel );

            analyzerObject.set_gate( fIntegralStart, fIntegralStop );
            analyzerObject.set_baseline_time( fBaselineTime );
            analyzerObject.analyze();

            baseline = analyzerObject.get_baseline();
            amplitude = analyzerObject.get_amplitude();
            amplitudeTime = analyzerObject.get_amplitude_time();
            triggerTime = fOsc->get_trigger_time();
            timeStep = fOsc->get_time_step();
            integral = analyzerObject.get_integral();
            pkpk = analyzerObject.get_pkpk();
            size = fOsc->get_wave_size();
            activeChannel = owon::ch_to_str( fOsc->get_active_channel() );
            pathToBinFile = file;
            model = fOsc->get_model();

            tree.Fill();
        }
        tree.Write();

        std::cout << "|\n|---OK!\n";
    }

    fileTree->Close();
    delete fileTree;

    return true; 
}
