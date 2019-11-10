/*
 * This example ilustrates the using of GET methods
 * of the 'parser' and 'analyzer' classes.
 * See documentation for more info.
 */
#include <iostream>
#include <iomanip>

//Parsing library includes
#include "device/sds6062.h"
#include "owon_analyzer.h"


namespace sds = owon::sds6062;


int main()
{
    //Create parser and parse binary file
    sds::parser p;
    p.parse( "../../data/sds6062.bin" );

    //This is how do we know how many channels available
    std::cout << "Number of channels: " << p.get_n_channels() << '\n';
    //Choose channel to analyze
    if( p.set_active_channel( owon::CH1 ) )
    {
        //Iterate over points and print them
        for( sds::parser::const_point_iterator point = p.cbegin();
             point != p.cend();
             ++point )
        {
            //Point's fields can be accessed here: time(x-axis) and voltage(y-axis)
            //std::cout << point->time << " " << point->voltage << '\n';
        }
        std::cout << '\n';

        //Print information of the first channel:
        std::cout << "PRIMARY INFO " << p.get_active_channel() << "\n"; 
        std::cout << std::setw(25) << "STATUS: " << p.get_status() << "\n";
        std::cout << std::setw(25) << "Wave size: " << p.get_wave_size() << '\n';
        std::cout << std::setw(25) << "Voltage per div: " << p.get_voltage_per_div() << '\n';
        std::cout << std::setw(25) << "Vertical position: " << p.get_vertical_position() << '\n';
        std::cout << std::setw(25) << "Time per division: " << p.get_time_per_div() << '\n';
        std::cout << std::setw(25) << "Time step: " << p.get_time_step() << '\n';
        std::cout << std::setw(25) << "Trigger time: " << p.get_trigger_time() << '\n';
        std::cout << std::setw(25) << "Length: " << p.get_length() << '\n';
        std::cout << std::setw(25) << "Height: " << p.get_height() << '\n';
    }

    //Create analyzer to calculate baseline, integral etc...
    owon::analyzer a( &p );

    //Choose a channel to analyze
    //However it is not necessary because of the previous
    //setting active channel to CH1
    p.set_active_channel( owon::CH1 );
    {
        //Calculate parameters
        a.set_gate( 0.8*p.get_trigger_time(), p.get_length() );//specify integrating range
        a.set_baseline_time( 0.8*p.get_trigger_time() );//specify range for baseline calculation
        a.set_baseline_method( owon::analyzer::AVERAGE );//method of baseline calculation (default)
        a.analyze();

        //Print information of ch1
        std::cout << '\n';       
        std::cout << "SECONDARY INFO " << p.get_active_channel() << "\n";
        std::cout << std::setw(25) << "Baseline value: " << a.get_baseline() << '\n';
        std::cout << std::setw(25) << "Amplitude value: " << a.get_amplitude() << '\n';
        std::cout << std::setw(25) << "Amplitude time value: " << a.get_amplitude_time() << '\n';
        std::cout << std::setw(25) << "Peak to peak value: " << a.get_pkpk() << '\n';
        std::cout << std::setw(25) << "Integral value: " << a.get_integral() << '\n';
    }
}
