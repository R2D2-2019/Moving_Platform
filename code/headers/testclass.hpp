#pragma once
#include <hwlib.hpp>
#include <qik_2s12v10.hpp>

    class beetlle_c {
    public:
        r2d2::moving_platform::qik_2s12v10_c *qik_2s12v10_motorcontroller;
        int speed;
    public:

        beetlle_c(r2d2::moving_platform::qik_2s12v10_c *qik_2s12v10_motorcontroller);

        /**
		* the speed set must not exceed 100
		*/
        void set_speed(const int8_t &new_speed);

        void set_steering(const int16_t &degrees);
        void turn(const int16_t &degrees);
        void move (const int8_t &distance);
        void move (const int8_t &x, const int8_t &y);
    };

