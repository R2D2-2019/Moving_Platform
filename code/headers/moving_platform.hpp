#pragma once
#include "hwlib.hpp"
#include <base_module.hpp>

namespace r2d2::moving_platform {

    class moving_platform_c : public base_module_c {
    private:
        // vars
        int8_t speed, angle;

        // functions
        virtual void set_speed(const int8_t &speed) = 0;
        virtual void set_steering(const int8_t &steering) = 0;
        virtual void turn(const int16_t &degrees) = 0;

        inline int8_t get_speed() const {
            return speed;
        };
        inline int8_t get_steering() const {
            return angle;
        };

        // test
        virtual void move(const int8_t &distance) = 0;
        virtual void move(const int8_t &x, const int8_t &y) = 0;

    public:
        moving_platform_c(base_comm_c &comm) : base_module_c(comm) {
            //comm.listen_for_frames();
        }

        void process() override {
            while (comm.has_data()) {
                auto frame = comm.get_data();

                // Process the frame
            }
        }
    };
} // namespace r2d2::moving_platform