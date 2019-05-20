#include <rhino.hpp>

namespace r2d2::moving_platform {

    rhino_c::rhino_c(r2d2::usart::usart_ports &usart_port,
                       unsigned int baud_rate, hwlib::pin_out &reset_pin,
                       base_comm_c &comm)
        : moving_platform_c(comm),
          qik_2s12v10_motorcontroller(usart_port, baud_rate, reset_pin) {

        qik_2s12v10_motorcontroller.init();
        qik_2s12v10_motorcontroller.get_configuration_parameter(1);
    }
    void rhino_c::set_speed(int8_t new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        // Todo: change the speed variable to the actual speed.

        // the speed is given in percentage (-100 (backward) till 100
        // (foreward))
        speed = new_speed;
        // do not go out of range
        if (speed > 100) {
            speed = 100;
        } else if (speed < -100) {
            speed = -100;
        }
        // because of inaccuracies, when not touching the pendals of manual
        // control, a number beween -3 and 8 can be given Just to make sure the
        // robot will not move forward then and not react to quickly, a theshold
        // is made for -10 till 10.
        else if (speed < 10 && speed > -10) {
            speed = 0;
            qik_2s12v10_motorcontroller.set_m0_speed(0);
            qik_2s12v10_motorcontroller.set_m1_speed(0);
        }
    }

    void rhino_c::set_steering(int16_t degrees) {
    }
    void rhino_c::turn(int16_t degrees) {
        int new_degrees = degrees;
        //to make sure the platform is still turning if speed is more than 90,
        //a speedcap is made at 90.
        if (speed >= 90){
            speed = 90;
        }
        // because of inaccuracies, when moving forward, the steer of manual
        // control will give a number beween -10 and 10 Just to make sure the
        // robot will move forward and not react to quickly, a theshold is made
        // for -15 till 15.
        if (new_degrees < 15 && new_degrees > -15) {
            new_degrees = 0;
        }
        //calculating a percentage of the degrees
        int percentage = new_degrees * 100 / 90;
        int new_speed_m0;
        int new_speed_m1;

        if (speed == 0){    //this makes sure the platform can still turn if the speed is 0.
            new_speed_m0 = speed + 100 * (percentage) / 100;
            new_speed_m1 = speed + 100 * (-percentage) / 100;
        }else{
            new_speed_m0 = speed + (((200%(speed + 100)) * percentage) / 100);
            new_speed_m1 = speed + (((200%(speed + 100)) * -percentage) / 100);
        }
        qik_2s12v10_motorcontroller.set_m0_speed(new_speed_m0);
        qik_2s12v10_motorcontroller.set_m1_speed(new_speed_m1);
    }
    
    void rhino_c::move(int8_t distance) {
    }
    void rhino_c::move(int8_t x, int8_t y) {
    }
} // namespace r2d2::moving_platform