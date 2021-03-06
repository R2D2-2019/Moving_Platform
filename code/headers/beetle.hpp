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
        constexpr static float turn_factor = 2.4;
        constexpr static uint8_t move_speed = 25;
        constexpr static uint16_t adc_voltage = 3500;
        constexpr static uint16_t encode_1_full_turn = 800;
        constexpr static uint8_t stopping_distance = 3;
        constexpr static uint8_t wheel_circumference = 39;
        constexpr static uint64_t interval = 50000;
        constexpr static uint8_t min_degrees = 15;
        constexpr static uint8_t max_degrees = 90;

    public:
        /**
         * @brief
         * Constructor beetle_c
         * @param    qik_2s12v10_motorcontroller: the qik motorcontroller board.
         * @param    comm: the internal communication bus.
         * @param    motor_encoder_m0: this is the encoder for motor_m0
         * @param    motor_encoder_m1: this is the encoder for motor_m1
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
        /**
         * @brief
         * This function will move the platform a certain distance in cm
         * @param    uint16_t: distance
         * Minimal moving distance is 10 cm.
         */
        void move(uint16_t distance);
    };
} // namespace r2d2::moving_platform
