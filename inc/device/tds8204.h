#ifndef TDS8204_H
#define TDS8204_H

#include <cinttypes>
#include <string>

//local includes
#include "owon_oscilloscope.h"


namespace owon
{
    namespace tds8204
    {
        #pragma pack(1)
        struct header_info_t 
        {  		
            char        HEADER[14];
        };
        #pragma pack()

        #pragma pack(1)
        struct pre_data_info_t
        {
            int16_t             WAVE_TYPE;
            int16_t             AVAIL_DATA_FLAG;  	
            int32_t             SAMPLE_RATE_INDEX;
            int32_t             STORAGE_SELECT;  
            int16_t             DATA_MODE;	
        
            int16_t             CHANNELS;//number of channels
            int16_t             CHANNEL_VAL[4];//???
            int32_t             VOLTAGE_BASE_CH[4];//voltage per division
            int32_t             GROUND_POS_CH[4];//vertical position (not real gnd)
            int32_t             REAL_VOLTAGE_BASE_CH[4];//(the same as VOLTAGE_BASE_CH)
            int32_t             REAL_GROUND_POS_CH[4];//(the same as GROUND_POS_CH)
            int32_t             FREQ[4];	
            int32_t             REFERENCE_FREQ[4]; 
            int32_t             ZERO_DIFFERENCE[4];
        
            int32_t             TIME_BASE;//time per division
            int32_t             TIME_DIFFERENCE;
            float               NEGATIVE_TIME;//negative time
            float               TIME_BEFORE_TRIG;
            float               TRIG_TOPS_TIME;//??? but = time before trigger
            float               TIME_STEP;//step between consequetive points
            float               POSITIVE_TIME;//positive time
        
            float               VOLTAGE_MAGNIFICATION[4];
        
            int32_t             DEEP_SIZE;
            int32_t             WAVE_SIZE;//seems to be data size (per channel)
        
            int16_t             PROBE_MAGNIFICATION[4];
            int32_t             FPGA_EXTRACTION;
            int32_t             DATA_OFFSET[4];
            int16_t             WAVEMATH_TYPE;
            int32_t             ZERO_POS;
        
            /*
             * The UNKNOWN's are the fields for which  
             * it is not determined what information they carry
             */
            int16_t             UNKNOWN[3];
        
            int32_t             DATA_SIZE;//seems to be data size (all channels)
        
            int32_t             MATH_DATA_OFFSET;
            int16_t             FFT_DATA_MODE; 	
            int32_t             WINDOWS_MODE;
            int32_t             FFT_VKD_CH;
            int32_t             HORIZONTAL_SCALE;
            int16_t             FFT_PROBE_MAGNIFICATION;
            float               FFT_DATA_FREQS;
            int32_t             SIGN;                   
            float               MATH_VALUE_PIXEL;
        };
        #pragma pack()

        #pragma pack(1)
        struct data_t
        {
            header_info_t header_info;        
            pre_data_info_t pre_data_info;
        }; 
        #pragma pack()


        class parser : public oscilloscope
        {
            protected :
                //auxiliary functions
                virtual float volt_per_div( int32_t voltage_base ) override;
                virtual float time_per_div( int32_t time_base ) override;

                float real_voltage( int8_t digital_voltage, float vpd );
                void parse_voltage( const data_t* es );
                void parse_time( const data_t* es );


            public :
                parser() :
                    oscilloscope( "tds8204", 4 )
                { } 

                virtual void parse( const std::string& path_to_file ) override;
        };
    }
}
#endif//TDS8204_H
