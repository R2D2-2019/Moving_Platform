#include <moving_platform.hpp>

namespace r2d2::moving_platform {

    int8_t moving_platform_c::get_speed() const {
        return speed;
    }

    int16_t moving_platform_c::get_steering() const {
        return steering_angle;
    }
    void moving_platform_c::move(uint16_t distance) {
    }

} // namespace r2d2::moving_platform