#ifndef SDS6062_H
#define SDS6062_H

#include <cinttypes>
#include <string>

//local includes
#include "oscilloscope.h"


namespace owon
{
    namespace sds7072
    {
        /*
         * The UNKNOWN's are the fields for which  
         * it is not determined what information they carry
         */
        #pragma pack(1)
        struct header_info_t
        {
            char    HEADER[10];
            char    SN[14];//serial number
            char    UNKNOWN[18];
        };
        #pragma pack()

        #pragma pack(1)
        struct pre_channel_info_t
        {
            float   TRIGGER_TIME;
            float   UNKNOWN_1;
            float   UNKNOWN_2;
        };
        #pragma pack()

        #pragma pack(1)
        struct channel_info_t
        {
            char        HEADER[3];
            int32_t     UNKNOWN_1;//ALWAYS NEGATIVE?
            int32_t     UNKNOWN_2;//ALWAYS 2?
            int32_t     UNKNOWN_3;
            int32_t     WAVE_SIZE_1;
            int32_t     WAVE_SIZE_2;
            int32_t     WAVE_SIZE_3;
            int32_t     TIME_BASE;
            int32_t     VERTICAL_POS;
            int32_t     VOLTAGE_BASE_CH;
            int32_t     PROBE;
            float       TIME_STEP;
            float       UNKNOWN_5;
            float       UNKNOWN_6;
            float       UNKNOWN_7;
        };
        #pragma pack()

        #pragma pack(1)
        struct data_t 
        {
            header_info_t header_info;
            pre_channel_info_t pre_channel_info;
            channel_info_t channel_info;
        }; 
        #pragma pack()

        class parser : public oscilloscope
        {
            protected :
                //auxiliary functions
                virtual float volt_per_div( int32_t voltage_base ) override;
                virtual float time_per_div( int32_t time_base ) override;

                float real_voltage( int16_t digital_voltage, float vpd );
                void parse_time( size_t ch, const data_t* data );
                void parse_voltage( size_t ch, const data_t* data );


            public :
                parser() :
                    oscilloscope( "sds7072", 2 )
                { }

                virtual void parse( const std::string& path_to_file ) override;
        };
    }//sds7072
}//owon
#endif
