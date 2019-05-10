#pragma once
#include <hwlib.hpp>

namespace r2d2::moving_platform {

	/**
	 * Class motor_control_c is an abstract class that can be implemented for all different kinds of motors
	 * This class exists so you can use the motor even without knowing what motor you have been provided with. 
	 */
    class motor_control_c {
    private:
        /**
		    * this speed represents a throtle in percentages (%)
		    * This means that 100 is max and forward, -100 is backwards
		    */
        int8_t speed;

    public:
        /**
		    * the speed set must not exceed 100
		    */
        virtual void set_speed(int8_t &new_speed) = 0;

        /**
		    * returns the speed value
		    */
        int8_t get_speed() const {
            return speed;
        }
    };
} // namespace r2d2::moving_platform
