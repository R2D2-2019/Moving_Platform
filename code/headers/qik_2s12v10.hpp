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
    class qik_2s12v10_c : motor_control_c {
    private:
        static const uint8_t qik_autodetect_baudrate;
        static const uint8_t qik_request_firmwareversion;
        static const uint8_t qik_motor_m0_set_forward;
        static const uint8_t qik_motor_m0_set_reverse;
        static const uint8_t qik_motor_m1_set_forward;
        static const uint8_t qik_motor_m1_set_reverse;
        static const uint8_t qik_get_config_parameter;
        static const uint8_t qik_get_error;

        hwlib::pin_out *reset_pin;
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
        qik_2s12v10_c(r2d2::uart_ports_c uart_port, unsigned int baudrate,
                      hwlib::pin_out *_reset_pin);

        /// @brief
        /// Sets the speed of both motors.
        /// @param _speed The speed of both motors. This value can be between
        /// -128 and 127, where -128 is full power backwards, 0 is no power and
        /// 127 is full power forward.
        void set_speed(const int8_t &_speed) override;
        /// @brief
        /// Sets the speed of the M0 motor.
        /// @param _speed The speed of the motor. This value can be between -128
        /// and 127, where -128 is full power backwards, 0 is no power and 127
        /// is full power forward.
        void set_m0_speed(const int8_t &_speed);
        /// @brief
        /// Sets the speed of the M1 motor.
        /// @param _speed The speed of the motor. This value can be between -128
        /// and 127, where -128 is full power backwards, 0 is no power and 127
        /// is full power forward.
        void set_m1_speed(const int8_t &_speed);

        /// @brief
        /// Initializes the qik2s12v10 by resetting it and then setting the baud
        /// rate.
        void init();
  
        void brake();
        

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
        /// Sets the value of the configurationParameter specified by parameter.
        /// @param parameter Specifies the parameter that should be set. The
        /// parameters can be found here: https://www.pololu.com/docs/0J29/5.a
        /// @param value The value that the configurationParameter should be set
        /// to.
        void set_configuration_parameter(const uint8_t &parameter,
                                         const uint8_t &value);

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
        /// by as much as 20%.
        unsigned int get_m0_current_milliamps();
        /// @brief
        /// This function uses the raw reading from motor M1 to estimate how
        /// much current flows through the motor in milliampere. Note that the
        /// value returned by this function can differ from the actual current
        /// by as much as 20%.
        unsigned int get_m1_current_milliamps();
    };
} // namespace r2d2::moving_platform
