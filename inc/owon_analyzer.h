#ifndef OWON_ANALYZER_H
#define OWON_ANALYZER_H


#include <stdexcept>
#include <vector>


namespace owon
{
    class oscilloscope;

    class analyzer
    {
        protected :
            const oscilloscope* osc; 

        //Auxiliary data 
        public :
            enum BASELINE { MODE, AVERAGE };

        protected :
            float integral_start;
            float integral_stop;
            float baseline_time;

            BASELINE baseline_method;

        protected :
            float baseline;
            float amplitude;
            float amplitude_time;
            float pkpk;
            float integral;

        //Auxiliary functions
        protected :
            float find_mode( std::vector< float >::iterator first,
                             std::vector< float >::iterator last ); 
            void calc_baseline_mode();
            void calc_baseline_average();
            void reset();

            void reset_intervals() { integral_start = integral_stop = baseline_time = -1.; }


        public :
            analyzer( const oscilloscope* osc );

            void analyze();


        public :
            void set_gate( float integral_start, float integral_stop );
            void set_baseline_time( float interval );

            void set_baseline_method( BASELINE method ) { baseline_method = method; }

            float get_gate_time() const { return integral_stop - integral_start; }
            float get_integral_start() const { return integral_start; }
            float get_integral_stop() const { return integral_stop; }
            float get_baseline() const { return baseline; }
            float get_amplitude() const { return amplitude; }
            float get_amplitude_time() const { return amplitude_time; }
            float get_pkpk() const { return pkpk; }
            float get_integral() const { return integral; }

            void print() const;
    };
}
#endif
