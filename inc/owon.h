#ifndef OWON_H
#define OWON_H

#include <iostream>
#include <iomanip>
#include <string>


namespace owon
{
    //Printing function
    template< typename T >
    inline void print_param( const std::string& param_description,
                             T param_value,
                             size_t description_fill_length )
    {
        std::cout << std::setw( description_fill_length ) << param_description << " : " << param_value << "\n";
    }

    inline void print_header( const std::string& header, char decorator, size_t length )
    {
        if( header.length() >= length )
        {
            std::cout << header << "\n";
        }
        else
        {
            size_t left_side = (length - header.length()) / 2;
            size_t right_side = length - left_side - header.length();
            std::cout << std::setfill( decorator ) << std::setw( left_side + header.length() ) << header << std::setw( right_side + 1 ) << "\n"; 
        }
        std::cout << std::setfill( ' ' );

        return;
    }


    inline void print_hbar( char decorator, size_t length )
    {
        std::cout << std::setw( length + 1 ) << std::setfill( decorator ) << "\n";
        std::cout << std::setfill( ' ' );
    }

    inline void print_warning( const std::string& warning_message )
    {
        std::cerr << "WARNING :: " << warning_message << std::endl; 
    }
}
#endif 
