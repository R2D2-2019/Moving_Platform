#pragma once

#include <hwlib.hpp>
#include <pwm.hpp>

using namespace r2d2::pwm_lib;

namespace r2d2 {
    namespace moving_platform {
        /* @brief
         *this class controls the l9110 2dc channel motor driver to controll the
         *dc motor of the turtle.
         */
        class l9110 {
        private:
            int16_t speed = 0; // motor is idle by default
            pwm_c &channel_forward;
            hwlib::target::pin_out &direction_pin;

        public:
            /*
             *@brief
             *constucts l9110
             *@param  pwm_c: pwm channel_forward for pin IA
             *@param  hwlib::target::pin_out direction_pin for IB
             */
            l9110(pwm_c &channel_forward, hwlib::target::pin_out &direction_pin);

            /*
             *@brief
             *set new motor speed
             *@detail
             * set motor, speed cna be set from -255 to 255.
             * if speed is set below 0 then the motor will rototate backward
             */
            void set_speed(int16_t new_speed);
            /*
             *@brief
             *get the value of what the motor speed is set;
             *@return int: speed of motor
             */
            int16_t get_speed();
        };
    } // namespace moving_platform
} // namespace r2d2
