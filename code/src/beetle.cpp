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
        int min_degrees = 15;
        int max_degrees = 90;
        if ((degrees < min_degrees && degrees > -min_degrees) ||
            degrees > max_degrees || degrees < -max_degrees) {
            degrees = 0;
        }
        float turn = 2.40;
        // The encoder code checks if the pulse goes from low to high. This is
        // why we start the bool low.
        bool low_m0 = true;
        int counter_m0 = 0;
        bool low_m1 = true;
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
        qik_2s12v10_motorcontroller.set_m1_speed(-motor_speed);

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
        }
    }
    void beetle_c::move(int16_t distance) {
        // wheel has a circumference of 39 cm
        // Encode frequency for 1 turn of the wheel. the encoder has 64 point
        // per over 2 pins we count when de adc goes from low to high of 1 pin.
        // The gear ratio from the motor is 50:1 64/4*50 = 800
        // The wheel has a circumference of 39 cm
        //  it cost 3 cm to stop the beetle
        const int16_t encoder_rotations = (distance - 3) * 800 / 39;
        // The encoder code checks if the pulse goes from low to high. This is
        // why we start the bool low.
        bool low_m0 = true;
        int counter_m0 = 0;
        int16_t counter_m0_total = 0;
        bool low_m1 = true;
        int counter_m1 = 0;
        int16_t counter_m1_total = 0;
        // The adc input. is between 3000 and 3800.
        unsigned int adc_voltage = 3500;
        // The speed of the beetle while moving in a staight line.
        int master_power = 25;
        // Initialise slavePower as masterPower - 5 so we don’t get huge error
        // for the first few iterations
        int slave_power = master_power - 5;
        qik_2s12v10_motorcontroller.set_m1_speed(-slave_power);
        qik_2s12v10_motorcontroller.set_m0_speed(-master_power);
        // proportional control setup
        int error = 0;
        // kp 2 workt te best. we only using integers because calculation floats
        // on embeded is not that fast.
        int kp = 2;
        // interval between proportional control updates in mircosecond.
        uint_fast64_t interval = 50000;
        // sets the tick.
        int tick = hwlib::now_us();

        while (counter_m0_total < encoder_rotations ||
               counter_m1_total < encoder_rotations) {

            // encoder for motor 0
            if (motor_encoder_m0.read() > adc_voltage) {
                if (low_m0 == true) {
                    counter_m0++;
                    counter_m0_total++;
                }
                low_m0 = false;
            } else {
                low_m0 = true;
            }
            // brake the motor if its has reached the target
            if (counter_m0_total == encoder_rotations) {
                qik_2s12v10_motorcontroller.brake_m0(0);
            }

            // encoder for motor 1
            if (motor_encoder_m1.read() > adc_voltage) {
                if (low_m1 == true) {
                    counter_m1++;
                    counter_m1_total++;
                }
                low_m1 = false;
            } else {
                low_m1 = true;
            }

            // brake the motor if its has reached the target
            
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