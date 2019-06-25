#include <l9110.hpp>

using namespace r2d2::pwm_lib;
namespace r2d2::moving_platform {

    l9110::l9110(pwm_c &channel_forward, pwm_c &channel_backward)
        : channel_forward(channel_forward), channel_backward(channel_backward) {
    }

    void l9110::set_speed(int16_t new_speed) {
        speed = new_speed;
        // check if motor has to be set forward of backward
        if (speed >= 0) {
            channel_forward.set_duty_cycle(speed);
            channel_backward.set_duty_cycle(0);
        } else {
            channel_backward.set_duty_cycle(-1 * speed);
            channel_forward.set_duty_cycle(0);
        }
    }

    int16_t l9110::get_speed() {
        return speed;
    }

} // namespace r2d2::moving_platform
