#include <beetle.hpp>

namespace r2d2::moving_platform {

    beetle_c::beetle_c(r2d2::uart_ports_c &uart_port_one,
             unsigned int baud_rate,
             hwlib::pin_out &reset_pin, 
             base_comm_c &comm):
             moving_platform_c(comm),
             qik_2s12v10_motorcontroller (uart_port_one, baud_rate, reset_pin)
    {
        
        qik_2s12v10_motorcontroller.init();
        qik_2s12v10_motorcontroller.get_configuration_parameter(1);

    }
    void beetle_c::set_speed(const int8_t &new_speed){
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        // Todo: change the speed variable to the actual speed.
        int speed_temp = new_speed;
        if(speed_temp > 100){
            speed_temp = 100;
        } else if (speed_temp <-100){
            speed_temp = -100;
        }
        if(speed_temp >=-100 && speed_temp <=100){
            qik_2s12v10_motorcontroller.set_m0_speed(128*speed_temp/100);
            qik_2s12v10_motorcontroller.set_m1_speed(-(128*speed_temp/100));
        }
    }

    void beetle_c::set_steering(const int16_t &degrees){

    }
    void beetle_c::turn(const int16_t &degrees){
        
        if(degrees >=0){
            qik_2s12v10_motorcontroller.set_m0_speed(40);
            qik_2s12v10_motorcontroller.set_m1_speed(40);
            hwlib::cout<<((2222*degrees)/360) << "\n";
            hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetle is 222 sec and needs start up 
        }else{
            qik_2s12v10_motorcontroller.set_m0_speed(-40);
            qik_2s12v10_motorcontroller.set_m1_speed(-40);
            hwlib::wait_ms((2222*(-degrees)/360) + 60);
        }
        qik_2s12v10_motorcontroller.brake();
    }
    void beetle_c::move (const int8_t &distance){

    }
    void beetle_c::move (const int8_t &x, const int8_t &y){

    }
}