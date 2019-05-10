#pragma once
#include <hwlib.hpp>
#include <base_module.hpp>

namespace r2d2::moving_platform {
	
	/**
	 * Class moving_platform_c is an abstract class that can be implemented for all different kinds of motors
	 * This class exists so you can use a moving platform even without knowing what kind of platform you have been provided with. 
	 */
    class moving_platform_c : public base_module_c {
    protected:
        /**
		 * this speed represents a throtle in percentages (%)
		 * This means that 100 is max and forward, -100 is backwards
		 * 
		 * the angle is represented in degrees
		 */
        int8_t speed;
        int16_t steering_angle;

		/**
		 * set the speed to the given value
		 */
        virtual void set_speed(const int8_t &speed) = 0;
		
		/**
		 * set the angle to the given value
		 */
        virtual void set_steering(const int16_t &steering) = 0;

		/**
		 * returns the speed value
		 */
        int8_t get_speed() const {
            return speed;
        };
		
		/**
		 * returns the angle value
		 */
        int8_t get_steering() const {
            return steering_angle;
        };

        /**
		 * functiuons for testing purpose
		 * !not te be implemented or used in final product!
		 */
        virtual void move(const int8_t &distance) = 0;
        virtual void move(const int8_t &x, const int8_t &y) = 0;

    public:
        moving_platform_c(base_comm_c &comm) : base_module_c(comm) {
            comm.listen_for_frames();
        }

        void process() override {
            while (comm.has_data()) {
                auto frame = comm.get_data();

                // Process the frame
            }
        }
    };
} // namespace r2d2::moving_platform
