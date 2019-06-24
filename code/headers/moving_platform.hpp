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

    public:
        moving_platform_c(base_comm_c &comm);

        /**
         * @brief
         * This function will process the frames from the canbus. only reads
         * from canbus
         */
        void process() override;

        /**
         * set the speed to the given value
         */
        virtual void set_speed(int8_t speed) = 0;

        /**
         *	turns the moving platform
         */
        virtual void turn(int8_t degrees) = 0;

        /**
         * returns the speed value
         */
        int8_t get_speed() const;

        /**
         *
         * Function to move a distance in cm.
         */
        virtual void move(uint16_t distance);
    };
} // namespace r2d2::moving_platform
