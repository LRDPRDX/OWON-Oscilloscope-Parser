#ifndef OWON_OSCILLOSCOPE_H
#define OWON_OSCILLOSCOPE_H

#include <vector>
#include <iostream>
#include <string>

//local includes
#include "owon_channel.h"


namespace owon
{
    //Entire structure of a binary file
    struct data_t;


    class oscilloscope
    {
        /*
        public :
            enum POLARITY { NEGATIVE, POSITIVE };
        */


        protected :
            std::string                 model;
            std::string                 serial;

            bool                        status;
            CHANNEL                     ac;
            //POLARITY                    polarity;

            std::vector<owon::channel>  channels;


        protected:
            void set_voltage_per_div( size_t ch, float vpd ) { channels[ch].voltage_per_div = vpd; }
            void set_vertical_position( size_t ch, float vp ) { channels[ch].vertical_position = vp; }
            void set_time_per_div( size_t ch, float tpd ) { channels[ch].time_per_div = tpd; }
            void set_time_step( size_t ch, float ts ) { channels[ch].time_step = ts; }
            void set_trigger_time( size_t ch, float tt ) { channels[ch].trigger_time = tt; }
            void add_point( size_t ch, point p ) { channels[ch].points.push_back( p ); }

            void reset();


        protected :
            virtual float volt_per_div( int32_t voltage_base ) = 0;
            virtual float time_per_div( int32_t time_base ) = 0;


        public :
            //Point iterator
            typedef typename std::vector<owon::point>::const_iterator const_point_iterator;  

            const_point_iterator cbegin() const noexcept { return channels[ac].points.cbegin(); } 
            const_point_iterator cend() const noexcept { return channels[ac].points.cend(); } 

        public :
            oscilloscope( std::string model_name, size_t number_of_channels );

        public :
            bool set_active_channel( CHANNEL active_channel );
            //void set_polarity( POLARITY pol ) { polarity = pol; }

            //Getters
            std::string get_serial() const { return serial; }
            std::string get_model() const  { return model; }

            bool        get_status() const { return status; } 
            unsigned    get_n_channels() const { return channels.size(); }
            CHANNEL     get_active_channel() const { return ac; }
            unsigned    get_wave_size() const { return channels[ac].points.size(); }
            float       get_voltage_per_div() const { return channels[ac].voltage_per_div; }
            float       get_vertical_position() const { return channels[ac].vertical_position; }
            float       get_time_per_div() const { return channels[ac].time_per_div; }
            float       get_time_step() const { return channels[ac].time_step; }
            float       get_trigger_time() const { return channels[ac].trigger_time; }
            float       get_length() const { return channels[ac].length(); } 
            float       get_height() const { return channels[ac].height(); }

            void print() const;

        public :
            //Device specific function
            virtual void parse( const std::string& path_to_file ) = 0;
    };
}
#endif
