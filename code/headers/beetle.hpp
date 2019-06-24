#pragma once
#include <base_module.hpp>
#include <hwlib.hpp>
#include <moving_platform.hpp>
#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    class beetle_c : public moving_platform_c {
    private:
        qik_2s12v10_c &qik_2s12v10_motorcontroller;
        hwlib::adc &motor_encoder_m0;
        hwlib::adc &motor_encoder_m1;
        constexpr static uint8_t min_degrees = 15;
        constexpr static uint8_t max_degrees = 90;

    public:
        /**
         * @brief
         * Constructor beetle_c
         * @param    comm
         * @param    qik_2s12v10_motorcontroller
         * @param    motor_encoder_m0
         * @param    motor_encoder_m1
         */
        beetle_c(qik_2s12v10_c &qik_2s12v10_motorcontroller, base_comm_c &comm,
                 hwlib::adc &motor_encoder_m0, hwlib::adc &motor_encoder_m1);

        /**
         * @brief
         * This function will set the speed between -100 and 100 % speed
         * @detail
         * the speed set must not exceed -100 of 100, if it is, it will set back
         * to 100 or -100
         * @param    int8_t: new speed percentage
         */
        void set_speed(int8_t new_speed) override;

        /**
         * @brief
         * This function will turn the beetle between -90 and 90
         * @detail
         * The degrees set must not exceed -90 of 90, if it is, it will set back
         * to -90 or 90
         * @param    int8_t: new degrees
         */
        void turn(int8_t degrees) override;
    };
} // namespace r2d2::moving_platform
