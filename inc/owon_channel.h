#ifndef OWON_CHANNEL_H
#define OWON_CHANNEL_H

#include <ostream>
#include <vector>
#include <string>


namespace owon
{
    class oscilloscope;

    //Channel enumeration and pretty printing
    enum CHANNEL : unsigned { CH1, CH2, CH3, CH4 };
    inline std::string ch_to_str( const CHANNEL ch )
    {
        switch( ch )
        {
            case( CH1 ) : return "CH1";
            case( CH2 ) : return "CH2";
            case( CH3 ) : return "CH3";
            case( CH4 ) : return "CH4";
            default     : return "N/A";
        }
    }
    inline std::ostream& operator<<( std::ostream& os, const CHANNEL& ch )
    {
        switch( ch )
        {
            case( CH1 ) : os << "CH1"; break;            
            case( CH2 ) : os << "CH2"; break;            
            case( CH3 ) : os << "CH3"; break;            
            case( CH4 ) : os << "CH4"; break;            
            default     : os << "N/A"; break;
        }

        return os;
    }


    struct point
    {
        float time;
        float voltage;

        point( float time, float voltage ) :
            time( time ),
            voltage( voltage )
        { }
    };


    class channel
    {
        protected :
            std::vector<owon::point> points;

            float   voltage_per_div;
            float   time_per_div;
            float   vertical_position;
            float   time_step;
            float   trigger_time;


        public :
            channel() :
                voltage_per_div( 0. ),
                time_per_div( 0. ),
                vertical_position( 0. ),
                time_step( 0. ),
                trigger_time( 0. )    
            { }

            float   length() const { return time_step * points.size(); } 
            float   height() const { return 10 * voltage_per_div; }
            bool    is_empty() const { return points.empty(); }

            void    reset();
            void    print() const;


            friend class oscilloscope;
    }; 
}
#endif
