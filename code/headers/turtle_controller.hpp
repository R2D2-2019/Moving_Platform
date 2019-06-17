#pragma once

#include "hwlib.hpp"
#include "l9110.hpp"
#include <rotary_encoder_turtle.hpp>
#include <pwm.hpp>
//#include <turtle.hpp>
namespace r2d2 {
    namespace moving_platform {
        class turtle_controller {
        private:
            l9110 &motor_left, &motor_right;
            rotary_encoder_turtle &encoder_left, &encoder_right;

            int16_t speed = 0; // speed of the robot

            // all the value for the PID, please search for PID on the internet
            // to know where al the value`s are
            int16_t prev_error_left = 0;
            int16_t integral_left = 0;
            // numbers are from testing pid
            float KP = 0.4;
            float KI = 0.4;
            float KD = 0.9;
            int16_t error_left;
            int derivative;
            int16_t prev_error_right = 0;
            int16_t integral_right = 0;
            int16_t error_right;
            int out;

        public:
            /*
             *@brief
             *conctucts the encoder class for the turtle
             @param motor_left : left motor of the turtle
             @param motor_right : right motor of the turtle
             @param encoder_right : right encoder if  the turtle
             @param encoder_left : left encoder if  the turtle
             */
            turtle_controller(l9110 &motor_left, l9110 &motor_right,
                              rotary_encoder_turtle &encoder_left,
                              rotary_encoder_turtle &encoder_right);
            /*@brief
            *set the speed of how fast the turtle should drive
            *@detail
            *The value of the speed should be what as the desired value what the
            *encoder should return. this function is only to set a parameter and
            *does not change the hardware.
            @param new_speed : the value of the speed
            */
            void set_speed(int16_t new_speed);
            /*@brief
             *Does one iration loop of the PID to corrects the motor drive as
             *fast as the speed that is set.
             */
            void update();
            /*@brief
             *turns the robot after stopping first.
             *@param degree : the disired turn angle
             **@param turn_left : if set 1 if will turn left else if set -1 it
             *will turn right
             */
            void turn(int degree, int8_t direction);
        };
    } // namespace moving_platform
} // namespace R2D2
