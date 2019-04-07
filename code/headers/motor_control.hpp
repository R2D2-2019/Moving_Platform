#pragma once
#include "hwlib.hpp"

class motor_control_c {
protected:
    signed char speed;
public:
	virtual void set_speed(const signed char &_speed) = 0;

    inline signed char get_speed() const {
		return speed;
    }
};
