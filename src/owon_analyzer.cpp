#include <cmath>
#include <algorithm>

#include "owon_analyzer.h"
#include "owon_oscilloscope.h"
#include "owon.h"


namespace owon
{
    analyzer::analyzer( const oscilloscope* osc ) :
        osc( osc ),

        integral_start( -1. ),
        integral_stop( -1. ),
        baseline_time( -1. ),
        baseline_method( BASELINE::AVERAGE ),       

        baseline( 0. ),
        amplitude( 0. ),
        amplitude_time( 0. ),
        pkpk( 0. ),
        integral( 0. )
    {
        if( osc == nullptr )
        {
            throw std::invalid_argument( "Pointer to parser object is invalid" );
        }
    }


    float analyzer::find_mode( std::vector< float >::iterator first,
                               std::vector< float >::iterator last )
    {
        //sort first
        std::sort( first, last );

        //initial values
        float most_found_element = *first;
        int   most_found_element_count = 0;
        float current_element = *first;
        int   current_element_count = 0;

        for( std::vector< float >::iterator it = first; it != last; ++it )
        {
            if( *it == current_element )
            {
                current_element_count++;
            } 
            else
            {
                if( current_element_count > most_found_element_count )
                {
                    most_found_element = current_element;
                    most_found_element_count = current_element_count;
                }
                current_element = *it;
                current_element_count = 1;
           }
        }
        if( current_element_count > most_found_element_count )
        {
            most_found_element_count = current_element_count;
            most_found_element = current_element;
        }

        return most_found_element;
    }


    void analyzer::calc_baseline_average()
    {
        baseline = 0.;
        size_t n_points = 0;
        for( oscilloscope::const_point_iterator point = osc->cbegin();
             point != osc->cend();
             ++point, n_points++ )
        {
            if( point->time >= baseline_time )
            {
                break;
            }
            baseline += point->voltage;            
        }
        if( n_points != 0 ){ baseline /= n_points; }
        else
        {
            print_warning( "Pretrigger is empty. No points to calculate the baseline" );
        }
    }


    void analyzer::calc_baseline_mode()
    {
        std::vector< float > baseline_vector;

        //fill baseline_vector with points before the trigger
        for( oscilloscope::const_point_iterator point = osc->cbegin();
             point != osc->cend();
             ++point )
        {
            if( point->time >= baseline_time )
            {
                break;
            }
            baseline_vector.push_back( point->voltage );            
        }

        if( !baseline_vector.empty() )
        {
            //set the most found voltage as baseline
            baseline = find_mode( baseline_vector.begin(), baseline_vector.end() );
        }
        else
        {
            print_warning( "Pretrigger is empty. No points to calculate the baseline" );
        }
    }


    void analyzer::reset()
    {
        baseline = 0.;
        amplitude = 0.;
        amplitude_time = 0.;
        pkpk = 0.;
        integral = 0.;
    }


    void analyzer::analyze()
    {
        reset();

        if( !osc->get_status() )
        {
            print_warning( "STATUS: NOT PARSED (from analyze)" );
            return;
        }

        //How to calculate the baseline position
        switch( baseline_method )
        {
            case( BASELINE::MODE ) :
                calc_baseline_mode();
                break;

            case( BASELINE::AVERAGE ) :
                calc_baseline_average();
                break;
        }

        //For pkpk
        float voltage_max = 0.;
        float voltage_min = 0.;

        for( oscilloscope::const_point_iterator point = osc->cbegin();
             point != osc->cend();
             ++point )
        {
            static bool first_point = true;

            if( !first_point )
            {
                //Amplitude part
                if( std::fabs( (baseline - point->voltage) ) >= amplitude )
                {
                    amplitude = std::fabs( (baseline - point->voltage ) );
                    amplitude_time = point->time;
                }
                //Peak to peak part
                if( point->voltage <= voltage_min ) { voltage_min = point->voltage; }
                if( point->voltage >= voltage_max ) { voltage_max = point->voltage; }
            } 
            else
            {
                //Amplitude time
                amplitude = std::fabs( (baseline - point->voltage) );
                amplitude_time = point->time;
                //Peak to peak part
                voltage_min = point->voltage;
                voltage_max = point->voltage;

                first_point = false;
            }
            //Integral part
            if( (point->time >= integral_start)  &&  (point->time <= integral_stop) )  
            {
                integral += ( baseline - point->voltage );
            }
        }

        pkpk = voltage_max - voltage_min;
        integral *= osc->get_time_step(); 

        reset_intervals();
    }


    void analyzer::set_gate( float start, float stop )
    {
        if( osc->get_status() )
        {
            if( (start < 0.0) || (start > osc->get_length()) ) { start = 0.0; }
            if( (stop < 0.0) || (stop > osc->get_length()) ) { stop = osc->get_length(); }

            this->integral_start = start;
            this->integral_stop = stop;
        }
        else
        {
            print_warning( "STATUS: NOT PARSED. Call parser's parse() function before setting the gate." );

            return;
        }
    }


    void analyzer::set_baseline_time( float interval )
    {
        if( osc->get_status() )
        {
            if( (interval < 0.0) || (interval > osc->get_length()) )
            {
                interval = 0.5 * osc->get_trigger_time();
            }
            this->baseline_time = interval;
        }
        else
        {
            print_warning( "STATUS: NOT PARSED. Call parser's parse() function before setting the time for calculation of the baseline." );

            return;
        }
    }


    void analyzer::print() const
    {
        print_header( "ANALYZER", 'o', 60 );
        print_param( "Baseline", baseline, 25 );
        print_param( "Amplitude [mV]", amplitude, 25 );
        print_param( "Amplitude time [us]", amplitude_time, 25 );
        print_param( "PkPk [mV]", pkpk, 25 );
        print_param( "Integral [us x mV]", integral, 25 );
        print_hbar( 'o', 60 );
    }
}
