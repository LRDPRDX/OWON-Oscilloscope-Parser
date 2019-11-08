/*
 * This is a minimal example. It just how to print
 * information about a waveform file using the 'print' members.
 * For more complicated example 
 * see the example for sds6062 model: ../sds6062
 */

//Parsing library includes
#include "device/sds7072.h"
#include "analyzer.h"

namespace sds = owon::sds7072;


int main()
{
    //Create parser and parse binary file
    sds::parser p;
    p.parse( "../../data/sds7072.bin" );

    owon::analyzer a( &p );


    if( p.set_active_channel( owon::CH1 ) )
    {
        a.set_baseline_time( -1 );//see documentation
        a.set_gate( -1, -1 );//see documentation

        a.analyze();
    }

    p.print();
    a.print();

}
