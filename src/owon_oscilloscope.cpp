#include <stdexcept>

#include "owon.h"
#include "owon_oscilloscope.h"


namespace owon
{
    oscilloscope::oscilloscope( std::string model_name, size_t number_of_channels ) :
        model( model_name ),
        serial( "N/A" ),

        status( false ),
        ac( CH1 ),

        channels( number_of_channels )
    {
        if( (number_of_channels < 1)  ||  (number_of_channels > 4) )
        {
            throw std::invalid_argument( "Number of available channels must be between 1 and 4 (inclusive)" );
        }
    };


    bool oscilloscope::set_active_channel( CHANNEL active_channel )
    {
        if( active_channel < channels.size() )
        {
            ac = active_channel;
            return true;
        }
        else
        {
            print_warning( ch_to_str( active_channel ) + " is unavailable on this device. Processing will be continued with channel CH1 set as the active one" );
            return false;
        }
    }


    void oscilloscope::reset()
    {
        ac = CH1;
        serial = "N/A";

        for( auto& ch : channels )
        {
            ch.reset();
        }

        status = false;
    } 


    void oscilloscope::print() const
    {
        print_header( "OSCILLOSCOPE", '*', 80 );
        print_param( "ModeL", model, 25 );
        print_param( "Serial", serial, 25 );
        print_param( "Status", status, 25 );
        print_param( "# of channels", get_n_channels(), 25 );
        print_param( "Active channel", ch_to_str( ac ), 25 );
        print_hbar( '-', 80 );
        for( auto& ch : channels )
        {
            ch.print();
        }
        print_hbar( '*', 80 );
    }
}
