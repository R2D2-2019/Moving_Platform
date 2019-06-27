#include <l9110.hpp>

using namespace r2d2::pwm_lib;
namespace r2d2::moving_platform {

    l9110::l9110(pwm_c &channel_forward, hwlib::target::pin_out &direction_pin)
        : channel_forward(channel_forward), direction_pin(direction_pin) {
    }

    void l9110::set_speed(int16_t new_speed) {
        speed = new_speed;
        // Checks whether motor has to be set forward or backward
        if (speed >= 0) {
            channel_forward.set_duty_cycle(speed);
            direction_pin.write(1);
        } else {
            channel_forward.set_duty_cycle(255 - (-1 * speed));
            direction_pin.write(0);
        }
    }

    int16_t l9110::get_speed() {
        return speed;
    }

} // namespace r2d2::moving_platform
