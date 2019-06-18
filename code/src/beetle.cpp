#include <beetle.hpp>

namespace r2d2::moving_platform {

    beetle_c::beetle_c(
        r2d2::moving_platform::qik_2s12v10_c &qik_2s12v10_motorcontroller,
        base_comm_c &comm, hwlib::adc &motor_encoder_m0,
        hwlib::adc &motor_encoder_m1)
        : moving_platform_c(comm),
          qik_2s12v10_motorcontroller(qik_2s12v10_motorcontroller),
          motor_encoder_m0(motor_encoder_m0),
          motor_encoder_m1(motor_encoder_m1) {
    }
    void beetle_c::set_speed(int8_t new_speed) {
        // the speed is given in percentage (-100 (backward) till 100
        // (forward))

        // do not go out of range
        if (new_speed > 100) {
            set_speed(100);
        } else if (new_speed < -100) {
            set_speed(-100);
        }
        // because of inaccuracies, when not touching the pendals of manual
        // control, a number beween -3 and 8 can be given Just to make sure
        // the robot will not move forward then and not react to quickly, a
        // theshold is made for -10 till 10.

        else if (new_speed < 10 && new_speed > -10) {
            set_speed(0);
        } else {
            set_speed(new_speed);
        }
        qik_2s12v10_motorcontroller.set_m0_speed(new_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(-new_speed);
    } // namespace r2d2::moving_platform

    void beetle_c::turn(int16_t degrees) {
        // because of inaccuracies, when moving forward, the steer of manual
        // control will give a number beween -10 and 10 Just to make sure the
        // robot will move forward and not react to quickly, a theshold is made
        // for -15 till 15.
        int min_degrees = 15;
        int max_degrees = 90;
        if ((degrees < min_degrees && degrees > -min_degrees) ||
            degrees > max_degrees || degrees < -max_degrees) {
            degrees = 0;
        }
        float turn = 2.40;
        // The puls. Its starts low.
        bool low_m0 = false;
        int counter_m0 = 0;
        bool low_m1 = false;
        int counter_m1 = 0;
        // The adc input. is between 3000 and 3800.
        unsigned int adc_voltage = 3500;
        // Encode frequency for 1 turn of the wheel. the encoder has 64 point
        // per over 2 pins we count when de adc goes from low to high of 1 pin.
        // The gear ratio from the motor is 50:1 64/4*50 = 800
        int encode_1_full_turn = 800;
        // Motor speed
        // Default turn speed.
        int motor_speed = 20;
        // Turn the right way.
        if (degrees < 0) {
            // turn right
            motor_speed = -motor_speed;
            degrees = -degrees;
        }
        // motor tests strats motors.
        qik_2s12v10_motorcontroller.set_m0_speed(motor_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(motor_speed);

        while (true && degrees != 0) {
            if (motor_encoder_m0.read() > adc_voltage) {
                if (low_m0 == true) {
                    counter_m0++;
                }
                low_m0 = false;
            } else {
                low_m0 = true;
            }
            if (counter_m0 ==
                (int(encode_1_full_turn * turn / 360 * degrees))) {
                qik_2s12v10_motorcontroller.brake_m0(20);
            }

            if (motor_encoder_m1.read() > adc_voltage) {
                if (low_m1 == true) {
                    counter_m1++;
                }
                low_m1 = false;
            } else {
                low_m1 = true;
            }
            if (counter_m1 ==
                (int(encode_1_full_turn * turn / 360 * degrees))) {
                qik_2s12v10_motorcontroller.brake_m1(20);
            }
            if (counter_m0 > (encode_1_full_turn * turn / 360 * degrees) &&
                counter_m1 > (encode_1_full_turn * turn / 360 * degrees)) {
                break;
            }
            // wait so the while loop aint blocking
            hwlib::wait_ms(0.1);
        }
    }
} // namespace r2d2::moving_platform