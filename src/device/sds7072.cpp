#include <cmath>
#include <fstream>

//local include
#include "owon.h"
#include "sds7072.h"


namespace owon
{
    namespace sds7072 
    {
        float parser::volt_per_div( int32_t voltage_base )
        {
            int32_t base;
            switch( voltage_base % 3 )
            {
                case( 0 ) : base = 2; break;
                case( 1 ) : base = 5; break;
                case( 2 ) : base = 1; break;
            }
            //voltage per division (mV/div)
            return base * std::pow( 10, ( voltage_base + 1 ) / 3 );
        }


        float parser::time_per_div( int32_t time_base )
        {
            int32_t base; 
            switch( time_base % 3 )
            {
                case( 0 ) : base = 2; break;
                case( 1 ) : base = 5; break;
                case( 2 ) : base = 1; break;
            }
            //time per division (uS/div)
            return base * pow( 10, (time_base + 1) / 3 - 3 );  
        } 


        float parser::real_voltage( int16_t digital_voltage, float vpd )
        {
            //Real voltage (mV).
            //At the first sight it seems that the whole
            //voltage range (=voltagePerDiv * 10) should
            //be divided into 256 parts (as number of int8_t numbers)
            //but it appears to be 250 parts (1/250 = 0.004) 
            return ( (float)digital_voltage * 0.004 ) * vpd * 10;
        }


        void parser::parse_voltage( size_t ch, const data_t* data )
        {
            float vpd = volt_per_div( data->channel_info.VOLTAGE_BASE_CH );

            set_voltage_per_div( ch, vpd );  
            set_vertical_position( ch, real_voltage( data->channel_info.VERTICAL_POS, vpd ) );
        }


        void parser::parse_time( size_t ch, const data_t* data )
        {
            //time per division (nS/div)
            float tpd = time_per_div( data->channel_info.TIME_BASE );

            set_time_per_div( ch, tpd );  
            //time between two successive points (uS)
            set_time_step( ch, data->channel_info.TIME_STEP );
            //time before trigger from the left edge of the screen
            set_trigger_time( ch, (data->channel_info.WAVE_SIZE_1) * (data->channel_info.TIME_STEP) / 2 - (data->pre_channel_info.TRIGGER_TIME) );
        }


        /*MAIN FUNCTION*/
        void parser::parse( const std::string& path_to_file )
        {
            reset();

            std::ifstream file;
            file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
            try
            {
                file.open( path_to_file );
                //Read prechannel data
                data_t data;

                file.read( reinterpret_cast<char*>( &data.header_info ),
                           sizeof( data.header_info ) );
                serial = data.header_info.SN;

                file.read( reinterpret_cast<char*>( &data.pre_channel_info ),
                           sizeof( data.pre_channel_info ) );
                //Read channels
                for( size_t ch = 0; ch < channels.size(); ch++ )
                {
                    //Read 3 first characters before channel info
                    //It should be CH0 or CH1
                    file.read( reinterpret_cast<char*>( &data.channel_info.HEADER ),
                               sizeof( data.channel_info.HEADER ) );
                    file.seekg( - sizeof( data.channel_info.HEADER), std::ios::cur );

                    if( (data.channel_info.HEADER[0] == 'C') and
                        (data.channel_info.HEADER[1] == 'H') and
                        (data.channel_info.HEADER[2] - '1' == ch) )
                    {

                        file.read( reinterpret_cast<char*>( &data.channel_info ),
                                   sizeof( data.channel_info ) );
                        //Parse values related to the axes
                        parse_voltage( ch, &data );
                        parse_time( ch, &data );
                        //Read points
                        float real_time = 0.;//point's real time
                        float vpd = volt_per_div( data.channel_info.VOLTAGE_BASE_CH ); 
                        for( int32_t i = 0; i < data.channel_info.WAVE_SIZE_1; i++ )
                        {
                            int16_t digital_voltage;
                            file.read( reinterpret_cast<char*>( &(digital_voltage) ),
                                       sizeof( digital_voltage ) );
                            add_point( ch, { real_time, real_voltage( digital_voltage, vpd ) } );
                            real_time += data.channel_info.TIME_STEP;
                        }
                    }
                }
                file.close();

                status = true;

                return;
            }//try end
            catch( std::ifstream::failure& e )
            {
                print_warning( "Exception opening/reading/closing file '" + path_to_file + "'" );

                reset();

                return;
            }
        }//parse
    }//sds6062
}//owon
