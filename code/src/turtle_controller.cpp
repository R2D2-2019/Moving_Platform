#include <turtle_controller.hpp>

namespace R2D2::moving_platform {

    turtle_controller::turtle_controller(l9110 &motor_left, l9110 &motor_right,
                                         motory_encoder_turtle &encoder_left,
                                         motory_encoder_turtle &encoder_right)
        : motor_left(motor_left),
          motor_right(motor_right),
          encoder_left(encoder_left),
          encoder_right(encoder_right) {
    }

    void turtle_controller::set_speed(int16_t new_speed) {
        speed = new_speed;
    }

    void turtle_controller::update() {

        // left motor PID
        error_left = speed - encoder_left.get_speed();
        integral_left = integral_left + (error_left * 10);
        derivative = (error_left - prev_error_left) / 20;
        out = (KP * error_left + KI * integral_left + KD * derivative);
        // if the motors are set below 50 the stop moving
        // 50 is therefore the minimum motor speed
        if (out < 50) {
            out = 50;
        }
        // right motor PID
        motor_left.set_speed(out);
        prev_error_left = error_left;
        error_right = speed - encoder_right.get_speed();
        integral_right = integral_right + (error_right * 10);
        derivative = (error_right - prev_error_right) / 20;
        out = (KP * error_right + KI * integral_right + KD * derivative);
        // if the motors are set below 50 the stop moving
        // 50 is therefore the minimum motor speed
        if (out < 50) {
            out = 50;
        }
        motor_right.set_speed(out);
        prev_error_right = error_right;
    }
    void turtle_controller::turn(int degree, int8_t direction) {
        // turning constante from calculation, see dev log 11
        float turn_constante = .274;
        // calculates how many thick should be measured with the encoder
        int ticks_right = turn_constante * degree;
        // turn speed
        motor_right.set_speed(-200 * direction);
        motor_left.set_speed(200 * direction);
        while (ticks_right > 0) {
            // makes the wheel rotate until the encoder measured enough thicks
            ticks_right -= encoder_right.get_speed();
        }
        motor_left.set_speed(0);
        motor_right.set_speed(0);
    }
} // namespace R2D2::moving_platform
