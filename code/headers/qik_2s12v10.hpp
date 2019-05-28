#pragma once

#include <hwlib.hpp>
#include <motor_control.hpp>
#include <usart_connection.hpp>


/// @file
namespace r2d2::moving_platform {

    enum class qik_2s12v10_set_configuration_parameter_return : uint8_t {
        command_ok = 0,
        bad_parameter,
        bad_value
    };

    enum class qik_2s12v10_configuration_parameter : uint8_t {
        device_id = 0,
        PWM_parameter,
        shut_down_motors_on_error,
        serial_timeout,
        motor_m0_acceleration,
        motor_m1_acceleration,
        motor_m0_brake_duration,
        motor_m1_brake_duration,
        motor_m0_current_limit,
        motor_m1_current_limit,
        motor_m0_current_limit_response,
        motor_m1_current_limit_response,
    };

    enum class qik_2s12v10_error : uint8_t {
        no_error = 0b00000000,
        motor_0_fault = 0b00000001,
        motor_1_fault = 0b00000010,
        motor_0_over_current = 0b00000100,
        motor_1_over_current = 0b00001000,
        serial_hardware_error = 0b00010000,
        crc_error = 0b00100000,
        format_error = 0b01000000,
        timeout = 0b10000000
    };

    enum class qik_2s12v10_registers : uint8_t {
        autodetect_baudrate = 0xAA,
        request_firmware_version = 0x81,
        set_motor_m0_forward = 0x88,
        set_motor_m0_reverse = 0x8A,
        set_motor_m1_forward = 0x8C,
        set_motor_m1_reverse = 0x8E,
        get_motor_m0_speed = 0x92,
        get_motor_m1_speed = 0x93,
        get_config_parameter = 0x83,
        set_config_parameter = 0x84,
        get_motor_m0_current = 0x90,
        get_motor_m1_current = 0x91,
        set_motor_m0_brake = 0x86,
        set_motor_m1_brake = 0x87,
        get_error = 0x82
    };

    /* @brief
     * Class that represents a qik2s12v10 motor controller.
     * @details
     * The qik2s12v10 is a motor controller that can control two brushed DC
     * motors. The qik2s12v10 can control the speed and brakes
     * of the individual motors. The qik2s12v10 can also set and return its
     * configuration parameter, which includes things like Acceleration rate
     * and PWM resolution and frequency. Lastly, the qik2s12v10 can check
     * for errors and return these.
     * For full documentation on the qik2s12v10, see this site:
     * https://www.pololu.com/docs/0J29
     */
    class qik_2s12v10_c {
    private:
        hwlib::pin_out &reset_pin;
        r2d2::usart::usart_connection_c &usart_bus;

        /*
         * @brief
         * Qik2x12v10 wait for bus function
         * @details
         * Waits for the usart bus to be free or busy, so that we know if we can
         * read/write.
         * @param wait_ms_length the length of the waits between the checks
         */
        void wait_for_bus(uint8_t wait_ms_length = 5);

    public:
        /** @brief
        * Qik2s12v10 constructor.
        * @details
        * Constructs a Qik2s12v10 with the given parameters.
        * @param uart_port The port that the qik_2s12v10_c class will use to
        * communicate with the qik 2s12v10 motor controller board.
        * @param baudrate The baud rate that the Qik2s12v10 will use in its
        * UART TTL serial communication in bps.
        * @param _reset_pin Pin that can be used to reset the qik2s12v10.
        */
        qik_2s12v10_c(r2d2::usart::usart_connection_c &uart_bus,
                      hwlib::pin_out &reset_pin);

        /** @brief
         * Initializes the qik2s12v10 by resetting it and then setting the baud
         * rate.
         */
        void init();

        /** @brief
         * Sets the speed of the M0 motor.
         * @param speed The speed of the motor. This value can be between -128
         * and 127, where -128 is full power backwards, 0 is no power and 127
         * is full power forward.
         */
        void set_m0_speed(int8_t new_speed);
        /** @brief
         * Sets the speed of the M1 motor.
         * @param speed The speed of the motor. This value can be between -128
         * and 127, where -128 is full power backwards, 0 is no power and 127
         * is full power forward.
         */
        void set_m1_speed(int8_t new_speed);

        /** @brief
         * This function sends a message to the motorcontroller. The
         * motorcontroller then uses the brakes of the motors.
         * @param The parameter is a number between 0 and 127. This number
         * indicates how quick/powerfull the brakes are.
         */
        void brake(const int8_t brake_amount);

        /** @brief
         * This function sends a message to the motorcontroller. The
         * motorcontroller then uses the brakes of motor 0.
         * @param The parameter is a number between 0 and 127. This number
         * indicates how quick/powerfull the brakes are.
         */
        void brake_m0(const int8_t brake_amount);

        /** @brief
         * This function sends a message to the motorcontroller. The
         * motorcontroller then uses the brakes of the motor 1.
         * @param The parameter is a number between 0 and 127. This number
         * indicates how quick/powerfull the brakes are.
         */
        void brake_m1(const int8_t brake_amount);

        /** @brief
         * Returns the errors that the qik2s12v10 has detected since this
         * function was last used. The meaning of each bit can be found here:
         * https://www.pololu.com/docs/0J29/5.c
         *
         * @return Returns the error if any.
         */
        uint8_t get_error_byte();

        /**
         * @brief
         * prints the errors returned by the getError function. 
         * Making debugging easier.
         * 
         */
        void print_errors();

        /* @brief
         * Returns the value that the parameter specified by parameter
         * currently has.
         * @param parameter Specifies the parameter that should be returned.
         * The parameters can be found here:
         * https://www.pololu.com/docs/0J29/5.a
         *
         * @return Returns the value of the requested parameter.
         */
        uint8_t get_configuration_parameter(
            qik_2s12v10_configuration_parameter parameter);

        /*
         * @brief
         * Sets the configuration parameter to the given value and returns if
         * the parameter was set or if there was an error while setting the
         * parameter
         * @param parameter specifies the parameter that should be changed.
         * @param value specifies what the parameter should be set to in the
         * chip The parameters can be found here:
         * https://www.pololu.com/docs/0J29/5.a
         *
         * @return returns a parameter_return wich is linked to an error.
         * eror codes can be found here:
         * https://www.pololu.com/docs/0J29/5.d
         */
        qik_2s12v10_set_configuration_parameter_return
        set_configuration_parameter(
            qik_2s12v10_configuration_parameter parameter, uint8_t value);

        /* @brief
         * Returns the raw reading from motor M0 that indicates how much
         * current flows through the motor at average over the last 5ms.
         * This reading is raw, so not converted to milliampere.
         *
         * @return Returns the raw current value of the 0 motor.
         */
        uint8_t get_m0_current();

        /* @brief
         * Returns the raw reading from motor M1 that indicates how much
         * current flows through the motor at average over the last 5ms. This
         * reading is raw, so not converted to milliampere.
         *
         * @return Returns the raw current value of the 1 motor.
         */
        uint8_t get_m1_current();

        /* @brief
         * This function uses the raw reading from motor M0 to estimate how
         * much current flows through the motor in milliampere. Note that the
         * value returned by this function can differ from the actual current
         * by as much as 20% and according to the datasheet the values needs to
         * be multiplied by 150.
         *
         * @return Returns the calculated current of the m0 motor in milliamps
         */
        uint16_t get_m0_current_milliamps();

        /* @brief
         * This function uses the raw reading from motor M1 to estimate how
         * much current flows through the motor in milliampere. Note that the
         * value returned by this function can differ from the actual current
         * by as much as 20% and according to the datasheet the values needs to
         * be multiplied by 150.
         *
         * @return Returns the calculated current of the m1 motor in milliamps
         */
        uint16_t get_m1_current_milliamps();

        uint8_t get_m0_speed();

        uint8_t get_m1_speed();
    };
} // namespace r2d2::moving_platform
