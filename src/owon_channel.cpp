#include "owon_channel.h"
#include "owon.h"


namespace owon
{
    void channel::reset()
    {
        points.clear();

        voltage_per_div     = 0.;
        time_per_div        = 0.;
        vertical_position   = 0.;
        time_step           = 0.;
        trigger_time        = 0.;
    }


    void channel::print() const
    {
        print_header( "CHANNEL", '=', 60 );
        print_param( "Number of points", points.size(), 25 );
        print_param( "Voltage/div (mV)", voltage_per_div, 25 );
        print_param( "Time/div (uS)", time_per_div, 25 );
        print_param( "Vertical pos (mV)", vertical_position, 25 );
        print_param( "Time step (uS)", time_step, 25 );
        print_param( "Trigger time (uS)", trigger_time, 25 );
        print_param( "Length (uS)", length(), 25 );
        print_hbar( '=', 60 );
    }
}
