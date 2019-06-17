#pragma once
#include <base_module.hpp>
#include <hwlib.hpp>

namespace r2d2::moving_platform {

    /**
     * Class moving_platform_c is an abstract class that can be implemented for
     * all different kinds of motors This class exists so you can use a moving
     * platform even without knowing what kind of platform you have been
     * provided with.
     */
    class moving_platform_c : public base_module_c {
    private:
        /**
         * this speed represents a throtle in percentages (%)
         * This means that 100 is max and forward, -100 is backwards
         *
         * the angle is represented in degrees
         */
        int8_t speed;
        int16_t steering_angle;

    public:
        moving_platform_c(base_comm_c &comm)
            : base_module_c(comm), speed(0), steering_angle(0) {

            comm.listen_for_frames({r2d2::frame_type::MOVEMENT_CONTROL});
        }
        /**
         * @brief
         * This function will process the frames from the canbus. only reads
         * from canbus
         */
        void process() override {
            comm.request(r2d2::frame_type::MOVEMENT_CONTROL);
            while (comm.has_data()) {

                auto frame = comm.get_data();
                // Process the frame

                // Don't handle requests
                if (frame.request) {
                    continue;
                }

                const auto data =
                    frame.as_frame_type<frame_type::MOVEMENT_CONTROL>();

                if (data.brake) {
                    set_speed(0);
                    break;
                }

                if (data.speed) {
                    set_speed(data.speed);
                }
                if (data.rotation) {
                    turn(data.rotation);
                }
            }
        }

        /**
         * set the speed to the given value
         */
        virtual void set_speed(int8_t speed) = 0;

        /**
         * set the angle to the given value
         */
        virtual void set_steering(int16_t degrees) = 0;
        /**
         *	turns the moving platform
         */
        virtual void turn(int16_t degrees) = 0;

        /**
         * returns the speed value
         */
        int8_t get_speed() const;

        /**
         * returns the angle value
         */
        int16_t get_steering() const;

        /**
         *
         * Function to move a distance in cm.
         */
        virtual void move(uint16_t distance);
    };
} // namespace r2d2::moving_platform
