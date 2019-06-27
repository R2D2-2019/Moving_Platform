#pragma once
#include <base_module.hpp>
#include <hwlib.hpp>
#include <moving_platform.hpp>
#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    class rhino_c : public moving_platform_c {
    private:
        qik_2s12v10_c qik_2s12v10_motorcontroller;
        hwlib::adc &motor_encoder_m0;
        hwlib::adc &motor_encoder_m1;
        constexpr static uint8_t turn_factor = 66;
        uint8_t turn_speed = 80;
        constexpr static unsigned int adc_voltage = 3500;

    public:
        /**
         * @brief
         * Constructor rhino_c
         * @param    qik_2s12v10_motorcontroller: the qik motorcontroller board.
         * @param    comm: the internal communication bus.
         * @param    motor_encoder_m0: this is the encoder for motor_m0
         * @param    motor_encoder_m1: this is the encoder for motor_m1
         */
        rhino_c(qik_2s12v10_c &qik_2s12v10_motorcontroller, base_comm_c &comm,
                hwlib::adc &motor_encoder_m0, hwlib::adc &motor_encoder_m1);

        /**
         * @brief
         * This function will set the speed between -100 and 100 % speed
         * @detail
         * the speed set must not exceed -127 of 127, if it is, it will set back
         * to 127 or -127
         * @param     int8_t: new_speed
         */
        void set_speed(int8_t new_speed) override;

        /**
         * @brief
         * This function will turn the rhino.
         * @detail
         * The degrees set can be between -127 and 127
         * @param    int8_t: new degrees
         */
        void turn(int8_t degrees) override;
        /**
         * @brief
         * This function will move the platform a certain distance in cm
         * @param    uint16_t: distance
         * Minimal moving distance is 10 cm.
         */
        void move(int8_t distance);
    };
} // namespace r2d2::moving_platform