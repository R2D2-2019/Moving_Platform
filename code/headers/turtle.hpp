#pragma once

#include "l9110.hpp"
#include <hwlib.hpp>
#include <motory_encoder_turtle.hpp>

namespace R2D2 {
    namespace moving_platform {
        class turtle {
        private:
            int speed;
            l9110 &motor_left, &motor_right;
            motory_encoder_turtle &encoder_left, &encoder_right;

        public:
            turtle(l9110 &motor_left, l9110 &motor_right,
                   motory_encoder_turtle &encoder_left,
                   motory_encoder_turtle &encoder_right);
            void update();
        };
    } // namespace moving_platform
} // namespace R2D2
