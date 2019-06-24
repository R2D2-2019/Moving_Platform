#include <rhino.hpp>

namespace r2d2::moving_platform {

    rhino_c::rhino_c(
        r2d2::moving_platform::qik_2s12v10_c &qik_2s12v10_motorcontroller,
        base_comm_c &comm, hwlib::adc &encode_m0, hwlib::adc &encode_m1)
        : moving_platform_c(comm),
          qik_2s12v10_motorcontroller(qik_2s12v10_motorcontroller),
          encode_m0(encode_m0),
          encode_m1(encode_m1) {
    }
    void rhino_c::set_speed(int8_t new_speed) {
        // the speed is given in percentage (-100 (backward) till 100
        // (foreward))

        // do not go out of range
        if (new_speed > 127) {
            new_speed = 127;
        } else if (new_speed < -127) {
            new_speed = -127;
        }
        // because of inaccuracies, when not touching the pendals of manual
        // control, a number beween -3 and 8 can be given Just to make sure the
        // robot will not move forward then and not react to quickly, a theshold
        // is made for -10 till 10.
        else if (new_speed < 10 && new_speed > -10) {
            new_speed = 0;
        }
        qik_2s12v10_motorcontroller.set_m0_speed(new_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(new_speed);
    }

    void rhino_c::turn(int8_t degrees) {

        // These booleans will be used to check wether the voltage is high or
        // low.These booleans are also to prevent the issue of multiple times
        // seeing the encoder value is high when it has actually not dropped
        // back down again.
        bool low_m0 = false;
        unsigned int counter_m0 = 0;
        bool low_m1 = false;
        unsigned int counter_m1 = 0;

        // Encode frequency for 1 turn of the wheel. the encoder has 64 point
        // per over 2 pins we count when de adc goes from low to high of 1 pin.
        // The gear ratio from the motor is 50:1 64/4*50 = 800
        int encode_1_full_turn = 800;

        // The encoder_target_svalue is the value the encoder needs to reach so
        // that the revolutions of the axle matches with the set degrees of the
        // rhino.
        unsigned int encoder_target_value =
            encode_1_full_turn * turn_factor / 360 * degrees;

        // Invert the motor speed and degrees if its a negative value.
        if (degrees < 0) {
            turn_speed = turn_speed * -1;
            degrees = degrees * -1;
        }
        // motor tests strats motors.
        qik_2s12v10_motorcontroller.set_m0_speed(turn_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(-1 * turn_speed);

        while (degrees != 0) {
            if (encode_m0.read() > adc_voltage) {
                if (low_m0 == true) {
                    counter_m0++;
                }
                low_m0 = false;
            } else {
                low_m0 = true;
            }

            if (encode_m1.read() > adc_voltage) {
                if (low_m1 == true) {
                    counter_m1++;
                }
                low_m1 = false;
            } else {
                low_m1 = true;
            }

            if (counter_m0 >= encoder_target_value) {
                qik_2s12v10_motorcontroller.set_m0_speed(0);
            }
            if (counter_m1 >= encoder_target_value) {
                qik_2s12v10_motorcontroller.set_m1_speed(0);
            }
            if (counter_m0 >= encoder_target_value &&
                counter_m1 >= encoder_target_value) {
                break;
            }
        }
    }

    void rhino_c::move(int8_t distance) {
    }

} // namespace r2d2::moving_platform