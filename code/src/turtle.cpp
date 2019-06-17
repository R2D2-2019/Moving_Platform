#include "turtle.hpp"

namespace r2d2::moving_platform {

    turtle::turtle(l9110 &motor_left, l9110 &motor_right,
                   rotary_encoder_turtle &encoder_left,
                   rotary_encoder_turtle &encoder_right)
        : motor_left(motor_left),
          motor_right(motor_right),
          encoder_left(encoder_left),
          encoder_right(encoder_right) {
    }

    void turtle::update() {
        if (encoder_left.get_speed() < speed) {
            motor_left.set_speed(motor_left.get_speed() + 2);
        } else if (encoder_left.get_speed() > speed) {
            motor_left.set_speed(motor_left.get_speed() - 2);
        }
        if (encoder_right.get_speed() < speed) {
            motor_right.set_speed(motor_right.get_speed() + 2);
        } else if (encoder_left.get_speed() > speed) {
            motor_right.set_speed(motor_right.get_speed() - 2);
        }
    }

} // namespace r2d2::moving_platform