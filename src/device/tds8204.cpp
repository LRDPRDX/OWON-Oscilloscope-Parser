#include <cmath>
#include <fstream>

//local includes
#include "owon.h"
#include "tds8204.h"


namespace owon
{
    namespace tds8204
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
            return base * pow( 10, ( voltage_base + 1 ) / 3 );
        }


        float parser::time_per_div( int32_t time_base )
        {
            int32_t base; 
            switch( time_base % 3 )
            {
                case( 0 ) : base = 1; break;
                case( 1 ) : base = 2; break;
                case( 2 ) : base = 5; break;
            }
            //time per division (us/div)
            return base * pow( 10, time_base / 3 - 3 );
        }


        float parser::real_voltage( int8_t digital_voltage, float vpd )
        {
            //Real voltage (mV).
            //At the first sight it seems that the whole
            //voltage range (=voltagePerDiv * 10) should
            //be divided into 256 parts (as number of int8_t numbers)
            //but it appears to be 250 parts (1/250 = 0.004) 
            return ( (float)digital_voltage * 0.004 ) * vpd * 10;
        }


        void parser::parse_voltage( const data_t* data )
        {
            for( size_t ch = 0; ch < channels.size(); ch++ )
            {
                //voltage per division (mV/div)
                set_voltage_per_div( ch, volt_per_div( data->pre_data_info.VOLTAGE_BASE_CH[ch] ) );
            }
        }


        void parser::parse_time( const data_t* data )
        {
            for( size_t ch = 0; ch < channels.size(); ch++ )
            {
                //time per division (nS/div)
                set_time_per_div( ch,  time_per_div( data->pre_data_info.TIME_BASE ) );
                //time between two successive points (uS)
                set_time_step( ch, data->pre_data_info.TIME_STEP );
                //time before trigger (uS)
                set_trigger_time( ch, ( -data->pre_data_info.NEGATIVE_TIME - data->pre_data_info.TIME_BEFORE_TRIG ) );
            }
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
                //Read initial info
                data_t data;
                //Read prechannel data
                file.read( reinterpret_cast<char*>( &data ), sizeof( data ) );
                //
                parse_voltage( &data );
                parse_time( &data );
                //Read points 
                for( size_t ch = 0; ch < data.pre_data_info.CHANNELS; ch++ )
                {
                    float real_time = 0.;//point's real time
                    float vpd = volt_per_div( data.pre_data_info.VOLTAGE_BASE_CH[ch] );
                    //fill waveform
                    for( int32_t i = 0; i < data.pre_data_info.WAVE_SIZE; i++ )
                    {
                        int8_t digital_voltage;
                        file.read( reinterpret_cast<char*>( &digital_voltage ),
                                   sizeof( digital_voltage ) );

                        add_point( data.pre_data_info.CHANNEL_VAL[ch], { real_time, real_voltage( digital_voltage, vpd ) } );

                        real_time += data.pre_data_info.TIME_STEP;
                    }
                }
                file.close();
            }
            catch( const std::ifstream::failure& e )
            {
                print_warning( "Exception opening/reading/closing file '" + path_to_file + "'" );
                std::cout << e.what() << std::endl;

                reset();

                return;
            }
            catch( const std::out_of_range& e )
            {
                print_warning( "Exception extracting data. Probably binary file contains wrong data" );
                std::cout << e.what() << std::endl;

                reset();

                return;
            }

            status = true;
        }
    }//TDS8204
}//owon
