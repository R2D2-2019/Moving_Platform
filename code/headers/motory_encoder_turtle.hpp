#pragma once

#include <hwlib.hpp>

namespace r2d2 {
    namespace moving_platform {
        class rotary_encoder_turtle {
        private:
            hwlib::target::pin_in &input_encoder;
            uint_fast64_t interval = 250000; // the time to measure the speed

        public:
            /*
             *@brief
             *conctucts the encoder class for the turtle
             @param pin_in: pin to connect to DO of the encoder
             */
            rotary_encoder_turtle(hwlib::target::pin_in &input_encoder);
            /*@brief
            *measure the speed of the encoder
            @return returns the speed of the speed measured by the encoder
            */
            uint8_t get_speed();
        };
    } // namespace moving_platform
} // namespace R2D2
