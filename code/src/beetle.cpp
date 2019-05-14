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
        speed = new_speed;
        if(speed > 100){
            speed = 100;
        } else if (speed <-100){
            speed = -100;
        } else if (speed < 10 && speed > -10){
            speed = 0;
            qik_2s12v10_motorcontroller.set_m0_speed(0);
            qik_2s12v10_motorcontroller.set_m1_speed(0);
        }
        //if(speed >=-100 && speed <=100){
            //qik_2s12v10_motorcontroller.set_m0_speed(128*speed/100);
            //qik_2s12v10_motorcontroller.set_m1_speed(-(128*speed/100));
        //}
    }

    void beetle_c::set_steering(const int16_t &degrees){

    }
    void beetle_c::turn(const int16_t &degrees){
        int new_degrees = degrees;
        if(new_degrees < 15 && new_degrees > -15){
            new_degrees = 0;
            
        }
        
        if(speed != 0){
            if(new_degrees >=0 ){
                // turn left
                qik_2s12v10_motorcontroller.set_m0_speed((speed+new_degrees/4)/3);
                qik_2s12v10_motorcontroller.set_m1_speed(-(speed+new_degrees/4));
                //hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetle is 222 sec and needs start up 
            } else{
                // turn right
                qik_2s12v10_motorcontroller.set_m0_speed((speed+new_degrees/4));
                qik_2s12v10_motorcontroller.set_m1_speed(-(speed+new_degrees/4)/3);
                //hwlib::wait_ms((2222*(-degrees)/360) + 60);
            }
        }
        
    }
    void beetle_c::move (const int8_t &distance){

    }
    void beetle_c::move (const int8_t &x, const int8_t &y){

    }
}