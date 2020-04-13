#include "device/tds8204.h"

#include <iostream>
#include <iomanip>


namespace tds = owon::tds8204;


int main()
{
    tds::parser p;  
    //p.parse( "../../../../data/parsing/Channels/Ch1Ch2Ch3/19-06-08 14_20_09_158.bin" );
    p.parse( "../../data/two_channels.bin" );

    std::cout << "Number of channels: " << p.get_n_channels() << '\n';

    p.set_active_channel( owon::CH1 );
        std::cout << "PRIMARY INFO: " << p.get_active_channel() << "\n"; 
        std::cout << std::setw(25) << "Wave size: " << p.get_wave_size() << '\n';
        std::cout << std::setw(25) << "Voltage per div: " << p.get_voltage_per_div() << '\n';
        std::cout << std::setw(25) << "Vertical position: " << p.get_vertical_position() << '\n';
        std::cout << std::setw(25) << "Time per division: " << p.get_time_per_div() << '\n';
        std::cout << std::setw(25) << "Time step: " << p.get_time_step() << '\n';
        std::cout << std::setw(25) << "Trigger time: " << p.get_trigger_time() << '\n';
        std::cout << std::setw(25) << "Length: " << p.get_length() << '\n';
        std::cout << std::setw(25) << "Height: " << p.get_height() << '\n';

    p.set_active_channel( owon::CH2 );
        std::cout << "PRIMARY INFO: " << p.get_active_channel() << "\n"; 
        std::cout << std::setw(25) << "Wave size: " << p.get_wave_size() << '\n';
        std::cout << std::setw(25) << "Voltage per div: " << p.get_voltage_per_div() << '\n';
        std::cout << std::setw(25) << "Vertical position: " << p.get_vertical_position() << '\n';
        std::cout << std::setw(25) << "Time per division: " << p.get_time_per_div() << '\n';
        std::cout << std::setw(25) << "Time step: " << p.get_time_step() << '\n';
        std::cout << std::setw(25) << "Trigger time: " << p.get_trigger_time() << '\n';
        std::cout << std::setw(25) << "Length: " << p.get_length() << '\n';
        std::cout << std::setw(25) << "Height: " << p.get_height() << '\n';

    p.set_active_channel( owon::CH3 );
        std::cout << "PRIMARY INFO: " << p.get_active_channel() << "\n"; 
        std::cout << std::setw(25) << "Wave size: " << p.get_wave_size() << '\n';
        std::cout << std::setw(25) << "Voltage per div: " << p.get_voltage_per_div() << '\n';
        std::cout << std::setw(25) << "Vertical position: " << p.get_vertical_position() << '\n';
        std::cout << std::setw(25) << "Time per division: " << p.get_time_per_div() << '\n';
        std::cout << std::setw(25) << "Time step: " << p.get_time_step() << '\n';
        std::cout << std::setw(25) << "Trigger time: " << p.get_trigger_time() << '\n';
        std::cout << std::setw(25) << "Length: " << p.get_length() << '\n';
        std::cout << std::setw(25) << "Height: " << p.get_height() << '\n';

    p.set_active_channel( owon::CH4 );
        std::cout << "PRIMARY INFO: " << p.get_active_channel() << "\n"; 
        std::cout << std::setw(25) << "Wave size: " << p.get_wave_size() << '\n';
        std::cout << std::setw(25) << "Voltage per div: " << p.get_voltage_per_div() << '\n';
        std::cout << std::setw(25) << "Vertical position: " << p.get_vertical_position() << '\n';
        std::cout << std::setw(25) << "Time per division: " << p.get_time_per_div() << '\n';
        std::cout << std::setw(25) << "Time step: " << p.get_time_step() << '\n';
        std::cout << std::setw(25) << "Trigger time: " << p.get_trigger_time() << '\n';
        std::cout << std::setw(25) << "Length: " << p.get_length() << '\n';
        std::cout << std::setw(25) << "Height: " << p.get_height() << '\n';

        p.print();

    return 0; 
}
