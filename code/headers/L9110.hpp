#pragma once
#include "motor_control.hpp"

class L9110_c : public motor_control_c {
private:
    hwlib::pin_out &a;
    hwlib::pin_out &b;

public:
    L9110_c(hwlib::pin_out &_a, hwlib::pin_out &_b) 
		: a(_a), b(_b) {}

	void set_speed(const signed char &_speed) override;   
};