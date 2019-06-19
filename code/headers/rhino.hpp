#pragma once
#include <base_module.hpp>
#include <hwlib.hpp>
#include <moving_platform.hpp>
#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    class rhino_c : public moving_platform_c {
    private:
        qik_2s12v10_c qik_2s12v10_motorcontroller;
        hwlib::adc &encode_m0;
        hwlib::adc &encode_m1;
        constexpr static uint8_t turn_factor = 66;
        uint8_t turn_speed = 80;

    public:
        /**
         * @brief
         * Constructor rhino_c
         * @param    qik_2s12v10_motorcontroller
         * @param    comm
         * @param    encode_m0
         * @param    encode_m1
         */
        rhino_c(qik_2s12v10_c &qik_2s12v10_motorcontroller, base_comm_c &comm,
                hwlib::adc &encode_m0, hwlib::adc &encode_m1);

        /**
         * @brief
         * This function will set the speed between -100 and 100 % speed
         * @detail
         * the speed set must not exceed -127 of 127, if it is, it will set back
         * to 127 or -127
         * @param    new_speed
         */
        void set_speed(int8_t new_speed) override;

        /**
         * @brief
         * This function will turn the rhino.
         * @detail
         * The rhino will turn while stationary. A value of 0 to 180 degrees is
         * tested to be accurate.
         * @param   degrees
         */
        void turn(int16_t degrees) override;
        /**
         * @brief
         * This function will move the platform a certain distance
         * @param    distance
         */
        void move(int8_t distance) override;
    };
} // namespace r2d2::moving_platform