#include <TGraph.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TAxis.h>
//ROOT
#include <TLegend.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TMultiGraph.h>

//STL
#include <ostream>

//Parsing library includes
#include "device/sds8302.h"
#include "owon_analyzer.h"


namespace sds = owon::sds8302;

void two_channels()
{
    //Create parser and parse binary file
    sds::parser p;
    p.parse( "../../data/sds8302.bin" );

    TMultiGraph* mg = new TMultiGraph();
    TLegend* leg    = new TLegend( 0.8, 0.9, 0.9, 0.8 );

    std::vector<owon::CHANNEL> channels = { owon::CH1, owon::CH2 };
    std::vector<Color_t> colors         = { kRed, kYellow };

    for( size_t i = 0; i < channels.size(); i++ )
    {
        TGraph* g = new TGraph();
            g->SetMarkerColor( colors[i] );
            g->SetLineColor( colors[i] );

        //choose channel
        p.set_active_channel( channels[i] );
        {
            g->Set( p.get_wave_size() );

            //Fill the graph in this loop
            for( owon::oscilloscope::const_point_iterator point = p.cbegin(); point != p.cend(); ++point )
            {
                g->SetPoint( std::distance( p.cbegin(), point ), point->time, point->voltage + p.get_vertical_position() );
            }
        }

        mg->Add( g );
        leg->AddEntry( g, Form( "data (%s)", owon::ch_to_str(channels[i]).c_str() ), "l" );
    }

    //Trigger position marker
    TMarker* ttm = new TMarker();
        ttm->SetMarkerColor( kWhite );
        ttm->SetMarkerStyle( 23 );
    //Set trigger marker's coordinates
    ttm->SetX( p.get_trigger_time() );
    ttm->SetY( p.get_height()/2 );
    leg->AddEntry( ttm, "Trigger", "p" );

    //Some styling
    gStyle->SetTitleTextColor( kWhite ); 

    mg->SetTitle( Form( "%s and %s", owon::ch_to_str(owon::CH1).c_str(),
                                     owon::ch_to_str(owon::CH2).c_str() ) );
    mg->GetYaxis()->SetTitle( "voltage [mV]" );
    mg->GetYaxis()->CenterTitle( kTRUE );
    mg->GetYaxis()->SetAxisColor( kWhite );
    mg->GetYaxis()->SetLabelColor( kWhite );
    mg->GetYaxis()->SetTitleColor( kWhite );
    mg->GetYaxis()->SetRangeUser( -p.get_height()/2, p.get_height()/2 );
    mg->GetXaxis()->SetTitle( "time [us]" );
    mg->GetXaxis()->CenterTitle( kTRUE );
    mg->GetXaxis()->SetRangeUser( 0., p.get_length() );
    mg->GetXaxis()->SetAxisColor( kWhite );
    mg->GetXaxis()->SetLabelColor( kWhite );
    mg->GetXaxis()->SetTitleColor( kWhite );

    leg->SetTextColor( kWhite );
    leg->SetFillColor( kBlack ); 
    leg->SetLineColor( kWhite );

    TCanvas* c = new TCanvas( "c", "c", 800, 600 );
        c->SetGrid();
        c->SetFillColor( kBlack );

    mg->Draw( "APL" );
    ttm->Draw( "same" );
    leg->Draw( "same" );
}
