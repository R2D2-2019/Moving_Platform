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
            new_speed = 100;
        } else if (new_speed < -100) {
            new_speed = -100;
        }
        // because of inaccuracies, when not touching the pendals of manual
        // control, a number beween -3 and 8 can be given Just to make sure
        // the robot will not move forward then and not react to quickly, a
        // theshold is made for -10 till 10.

        else if (new_speed < 10 && new_speed > -10) {
            new_speed = 0;
        }
        qik_2s12v10_motorcontroller.set_m0_speed(new_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(new_speed);
    } // namespace r2d2::moving_platform

    void beetle_c::turn(int8_t degrees) {
        // because of inaccuracies, when moving forward, the steer of manual
        // control will give a number beween -10 and 10 Just to make sure the
        // robot will move forward and not react to quickly, a theshold is made
        // for -15 till 15.

        const uint8_t min_degrees = 15;
        const uint8_t max_degrees = 90;
        if ((degrees < min_degrees && degrees > -min_degrees) ||
            degrees > max_degrees || degrees < -max_degrees) {
            degrees = 0;
        }
        // The encoder code checks if the pulse goes from low to high. This is
        // why we start the bool low.
        bool m0_state = false;
        uint_fast16_t counter_m0 = 0;
        bool m1_state = false;
        uint_fast16_t counter_m1 = 0;
        // Encode frequency for 1 turn of the wheel. the encoder has 64 point
        // per over 2 pins we count when de adc goes from low to high of 1 pin.
        // The gear ratio from the motor is 50:1 64/4*50 = 800

        // Motor speed
        // Default turn speed.
        uint8_t motor_speed = move_speed;
        // Turn the right way.
        if (degrees < 0) {
            // turn right
            motor_speed = -motor_speed;
            degrees = -degrees;
        }
        // motor tests strats motors.
        qik_2s12v10_motorcontroller.set_m0_speed(motor_speed);
        qik_2s12v10_motorcontroller.set_m1_speed(-motor_speed);

        while (true && degrees != 0) {
            if (motor_encoder_m0.read() > adc_voltage) {
                if (m0_state == false) {
                    counter_m0++;
                }
                m0_state = true;
            } else {
                m0_state = false;
            }

            if (counter_m0 == (uint_fast16_t(encode_1_full_turn * turn_factor /
                                             360 * degrees))) {
                qik_2s12v10_motorcontroller.brake_m0(20);
            }

            if (motor_encoder_m1.read() > adc_voltage) {
                if (m1_state == false) {
                    counter_m1++;
                }
                m1_state = true;
            } else {
                m1_state = false;
            }

            if (counter_m1 == (uint_fast16_t(encode_1_full_turn * turn_factor /
                                             360 * degrees))) {

                qik_2s12v10_motorcontroller.brake_m1(20);
            }
            
            if (counter_m0 >
                    (encode_1_full_turn * turn_factor / 360 * degrees) &&
                counter_m1 >
                    (encode_1_full_turn * turn_factor / 360 * degrees)) {
                break;
            }
        }
    }
    void beetle_c::move(uint16_t distance) {
        // Encode frequency for 1 turn of the wheel. the encoder has 64 point
        // per over 2 pins we count when de adc goes from low to high of 1 pin.
        uint_fast16_t encoder_rotations;
        if (distance >= 10) {
            encoder_rotations = (distance - stopping_distance) *
                                encode_1_full_turn / wheel_circumference;
        } else {
            encoder_rotations = 0;
        }
        // The encoder code checks if the pulse goes from low to high. This is
        // why we start the bool low.
        bool m0_state = false;
        uint_fast8_t counter_m0 = 0;
        uint_fast16_t counter_m0_total = 0;
        bool m1_state = false;
        uint_fast8_t counter_m1 = 0;
        uint_fast16_t counter_m1_total = 0;
        // The speed of the beetle while moving in a staight line.
        uint_fast8_t master_power = move_speed;
        // Initialise slavePower as masterPower - 5 so we don’t get huge error
        // for the first few iterations
        uint_fast8_t slave_power = master_power - 5;
        qik_2s12v10_motorcontroller.set_m1_speed(-slave_power);
        qik_2s12v10_motorcontroller.set_m0_speed(-master_power);
        // proportional control setup
        int_fast8_t error = 0;
        // kp 2 workt te best. we only using integers because calculation floats
        // on embeded is not that fast.
        const uint_fast8_t kp = 2;
        // sets the tick.
        int tick = hwlib::now_us();

        while (counter_m0_total <= encoder_rotations ||
               counter_m1_total <= encoder_rotations) {

            // encoder for motor 0
            if (motor_encoder_m0.read() > adc_voltage) {
                if (m0_state == false) {
                    counter_m0++;
                    counter_m0_total++;
                }
                m0_state = true;
            } else {
                m0_state = false;
            }
            // brake the motor if its has reached the target
            if (counter_m0_total == encoder_rotations) {
                qik_2s12v10_motorcontroller.brake_m0(0);
            }

            // encoder for motor 1
            if (motor_encoder_m1.read() > adc_voltage) {
                if (m1_state == false) {
                    counter_m1++;
                    counter_m1_total++;
                }
                m1_state = true;
            } else {
                m1_state = false;
            }

            // brake the motor if its has reached the target
            // Moter0 has a consistent speed and motor1 has adjustble speed so
            // it can minimize the error.
            if (counter_m1_total == encoder_rotations) {
                qik_2s12v10_motorcontroller.brake_m1(0);
            } else if (counter_m1_total < encoder_rotations) {
                // the lowest the motor can power de motor can revieve is 10
                slave_power = ((slave_power > 10) ? slave_power : 10);
                qik_2s12v10_motorcontroller.set_m1_speed(-slave_power);
            }

            // Master slave method
            // update every interval
            if (hwlib::now_us() - tick > interval) {
                error = counter_m0 - counter_m1;
                slave_power += error / kp;
                counter_m0 = 0;
                counter_m1 = 0;
                tick = hwlib::now_us();
            }
        }
    }
} // namespace r2d2::moving_platform