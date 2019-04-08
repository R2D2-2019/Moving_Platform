#pragma once
#include "hwlib.hpp"

namespace r2d2::moving_platform {

    class motor_control_c {
    protected:
        signed int8_t speed;

    public:
        virtual void set_speed(const int8_t &_speed) = 0;

        inline int8_t get_speed() const {
            return speed;
        }
    };
} // namespace r2d2::moving_platform