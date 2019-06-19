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
        speed = new_speed;
        // do not go out of range
        if (speed > 127) {
            speed = 127;
        } else if (speed < -127) {
            speed = -127;
        }
        // because of inaccuracies, when not touching the pendals of manual
        // control, a number beween -3 and 8 can be given Just to make sure the
        // robot will not move forward then and not react to quickly, a theshold
        // is made for -10 till 10.
        else if (speed < 10 && speed > -10) {
            speed = 0;
        }
        qik_2s12v10_motorcontroller.set_m0_speed(speed);
        qik_2s12v10_motorcontroller.set_m1_speed(speed);
    }

    void rhino_c::turn(int16_t degrees) {

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

        // Invert the motor speed and degrees if its a negative value.
        if (degrees < 0) {
            // turn right
            turn_speed = turn_speed * -1;
            degrees = degrees * -1;
        }
        // motor tests strats motors.
        qik_2s12v10_motorcontroller.set_m0_speed(turn_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(-1 * turn_speed);

        while (true && degrees != 0) {
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

            if (counter_m0 ==
                (int(encode_1_full_turn * turn_factor / 360 * degrees))) {
                qik_2s12v10_motorcontroller.set_m0_speed(0);
            }
            if (counter_m1 ==
                (int(encode_1_full_turn * turn_factor / 360 * degrees))) {
                qik_2s12v10_motorcontroller.set_m1_speed(0);
            }
            if (counter_m0 >
                    (encode_1_full_turn * turn_factor / 360 * degrees) &&
                counter_m1 >
                    (encode_1_full_turn * turn_factor / 360 * degrees)) {
                break;
            }
        }
    }

    void rhino_c::move(int8_t distance) {
    }

} // namespace r2d2::moving_platform