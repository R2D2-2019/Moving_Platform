#pragma once

#include <hwlib.hpp>
#include <motor_control.hpp>
#include <hardware_usart.hpp>

/// @file

namespace r2d2::moving_platform {

    /// @brief
    /// Class that represents a qik2s12v10 motor controller.
    /// @details
    /// The qik2s12v10 is a motor controller that can control two brushed DC
    /// motors. The qik2s12v10 can control the speed and brakes
    /// of the individual motors. The qik2s12v10 can also set and return its
    /// configuration parameter, which includes things like Acceleration rate
    /// and PWM resolution and frequency. Lastly, the qik2s12v10 can check
    /// for errors and return these.
    /// For full documentation on the qik2s12v10, see this site:
    /// https://www.pololu.com/docs/0J29
    class qik_2s12v10_c {
    private:
        const uint8_t qik_autodetect_baudrate = 0xAA;
        const uint8_t qik_request_firmwareversion = 0x81;
        const uint8_t qik_motor_m0_set_forward = 0x88;
        const uint8_t qik_motor_m0_set_reverse = 0x8A;
        const uint8_t qik_motor_m1_set_forward = 0x8C;
        const uint8_t qik_motor_m1_set_reverse = 0x8E;
        const uint8_t qik_get_config_parameter = 0x83;
        const uint8_t qik_get_error = 0x82;
        const uint8_t qik_get_motor_m0_current = 0x90;
        const uint8_t qik_get_motor_m1_current = 0x91;
        const uint8_t qik_motor_m0_brake = 0x86;
        const uint8_t qik_motor_m1_brake = 0x87;

        hwlib::pin_out &reset_pin;
        r2d2::hardware_usart_c usart_bus;

    public:
        /// @brief
        /// Qik2s12v10 constructor.
        /// @details
        /// Constructs a Qik2s12v10 with the given parameters.
        /// @param uart_port The port that the qik_2s12v10_c class will use to
        /// communicate with the qik 2s12v10 motor controller board.
        /// @param baudrate The baud rate that the Qik2s12v10 will use in its
        /// UART TTL serial communication in bps.
        /// @param _reset_pin Pin that can be used to reset the qik2s12v10.
        qik_2s12v10_c(r2d2::uart_ports_c &uart_port, unsigned int baudrate,
                      hwlib::pin_out &reset_pin);
					  
		/// @brief
        /// Initializes the qik2s12v10 by resetting it and then setting the baud
        /// rate.
        void init();

        /// @brief
        /// Sets the speed of both motors.
        /// @param speed The speed of both motors. This value can be between
        /// -128 and 127, where -128 is full power backwards, 0 is no power and
        /// 127 is full power forward.
        void set_speed(const int8_t &new_speed);
        /// @brief
        /// Sets the speed of the M0 motor.
        /// @param speed The speed of the motor. This value can be between -128
        /// and 127, where -128 is full power backwards, 0 is no power and 127
        /// is full power forward.
        void set_m0_speed(const int8_t &new_speed);
        /// @brief
        /// Sets the speed of the M1 motor.
        /// @param speed The speed of the motor. This value can be between -128
        /// and 127, where -128 is full power backwards, 0 is no power and 127
        /// is full power forward.
        void set_m1_speed(const int8_t &new_speed);


        enum qik_2s12v10_error{
            motor_0_fault                     = 0b10000000,
            motor_1_fault                     = 0b01000000,
            motor_0_over_current              = 0b00100000,
            motor_1_over_current              = 0b00010000,
            serial_hardware_error             = 0b00001000,
            crc_error                         = 0b00000100,
            format_error                      = 0b00000010,
            timeout                           = 0b00000001
        };
  

        /// @brief
        /// This function sends a message to the motorcontroller. The motorcontroller
        /// then uses the brakes of the motors. 
        /// @param The parameter is a number between 0 and 127. This number indicates
        /// how quick/powerfull the brakes are.
        void brake(const int8_t brake_amount);

        /// @brief
        /// This function sends a message to the motorcontroller. The motorcontroller
        /// then uses the brakes of motor 0. 
        /// @param The parameter is a number between 0 and 127. This number indicates
        /// how quick/powerfull the brakes are.
        void brake_m0(const int8_t brake_amount);

        /// @brief
        /// This function sends a message to the motorcontroller. The motorcontroller
        /// then uses the brakes of the motor 1. 
        /// @param The parameter is a number between 0 and 127. This number indicates
        /// how quick/powerfull the brakes are.
        void brake_m1(const int8_t brake_amount);

        /// @brief
        /// Returns the errors that the qik2s12v10 has detected since this
        /// function was last used. The meaning of each bit can be found here:
        /// https://www.pololu.com/docs/0J29/5.c
        uint8_t get_error();
        /// @brief
        /// Returns the value that the parameter specified by parameter
        /// currently has.
        /// @param parameter Specifies the parameter that should be returned.
        /// The parameters can be found here:
        /// https://www.pololu.com/docs/0J29/5.a
        uint8_t get_configuration_parameter(const uint8_t &parameter);


        /// @brief
        /// Returns the raw reading from motor M0 that indicates how much
        /// current flows through the motor at average over the last 5ms. This
        /// reading is raw, so not converted to milliampere.

        uint8_t get_m0_current();
        
        /// @brief
        /// Returns the raw reading from motor M1 that indicates how much
        /// current flows through the motor at average over the last 5ms. This
        /// reading is raw, so not converted to milliampere.

        uint8_t get_m1_current();

        /// @brief
        /// This function uses the raw reading from motor M0 to estimate how
        /// much current flows through the motor in milliampere. Note that the
        /// value returned by this function can differ from the actual current
        /// by as much as 20% and according to the datasheet the values needs to be multiplied by 150.

        size_t get_m0_current_milliamps();

        /// @brief
        /// This function uses the raw reading from motor M1 to estimate how
        /// much current flows through the motor in milliampere. Note that the
        /// value returned by this function can differ from the actual current
        /// by as much as 20% and according to the datasheet the values needs to be multiplied by 150.

        size_t get_m1_current_milliamps();
    };
} // namespace r2d2::moving_platform
