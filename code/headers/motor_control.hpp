#pragma once
#include <hwlib.hpp>

namespace r2d2::moving_platform {

    class motor_control_c {
    private:
        int8_t speed;

    public:
        virtual void set_speed(int8_t new_speed) = 0;

        int8_t get_speed() const {
            return speed;
        }
    };
} // namespace r2d2::moving_platform