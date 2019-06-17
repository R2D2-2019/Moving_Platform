#pragma once

#include "l9110.hpp"
#include <hwlib.hpp>
#include <rotary_encoder_turtle.hpp>

namespace r2r2 {
    namespace moving_platform {
        class turtle {
        private:
            int speed;
            l9110 &motor_left, &motor_right;
            rotary_encoder_turtle &encoder_left, &encoder_right;

        public:
            turtle(l9110 &motor_left, l9110 &motor_right,
                   rotary_encoder_turtle &encoder_left,
                   rotary_encoder_turtle &encoder_right);
            void update();
        };
    } // namespace moving_platform
} // namespace r2d2