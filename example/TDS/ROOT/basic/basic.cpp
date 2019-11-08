//ROOT includes
#include <TGraph.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TMarker.h>
#include <TAxis.h>
#include <TLegend.h>

//Parsing library includes
#include "device/tds8204.h"
#include "analyzer.h"


namespace tds = owon::tds8204;


void basic()
{
    //Create parser and parse binary file
    tds::parser p;
    //p.parse( "../../data/csi_tl.bin" );
    p.parse( "../../data/csi_tl.bin" );

    //Create analyzer
    owon::analyzer a( &p );

    //Create graph to store and visualize waveform
    TGraph* g = new TGraph();

    //Create lines to visualize trigger time, baseline etc...        
    TMarker* tm = new TMarker();
        tm->SetMarkerStyle( 23 );
    TLine* bl = new TLine(); 
        bl->SetLineStyle( 7 );
        bl->SetLineWidth( 2 );
    TLine* al = new TLine();
        al->SetLineWidth( 2 );

    if( p.set_active_channel( owon::CH1 ) )
    {
        g->Set( p.get_wave_size() );

        //Fill TGraph in this loop
        for( tds::parser::const_point_iterator point = p.cbegin(); point != p.cend(); ++point )
        {
            g->SetPoint( std::distance( p.cbegin(), point ), point->time, point->voltage );
        }

        tm->SetX( p.get_trigger_time() );
        tm->SetY( p.get_height()/2 );

        //Calc baseline, amplitude, pkpk and etc.
        a.set_baseline_time( 0.8*p.get_trigger_time() );
        a.analyze();

        bl->SetY1( a.get_baseline() );
        bl->SetY2( a.get_baseline() );
        bl->SetX1( 0 );
        bl->SetX2( p.get_length() );

        al->SetY1( -p.get_height()/2 );
        al->SetY2( p.get_height()/2 );
        al->SetX1( a.get_amplitude_time() );
        al->SetX2( a.get_amplitude_time() );
    }

    //Set axes' ranges and titles
    g->GetYaxis()->SetTitle( "voltage, mV" );
    g->GetYaxis()->SetRangeUser( -p.get_height()/2, p.get_height()/2 );
    g->GetXaxis()->SetTitle( "time, ns" );
    g->GetXaxis()->SetRangeUser( 0., p.get_length() );

    TCanvas* c = new TCanvas( "c", "c", 800, 600 );
        c->SetGrid();

    TLegend* leg = new TLegend( 0.8, 0.9, 0.9, 0.8 );
        leg->AddEntry( g, "data", "pl" );
        leg->AddEntry( tm, "trigger", "p" );
        leg->AddEntry( bl, "baseline", "l" );
        leg->AddEntry( al, "amplitude", "l" );

    g->Draw( "APL" );
    tm->Draw( "same" );
    bl->Draw( "same" );
    al->Draw( "same" );
    leg->Draw( "same" );
}
