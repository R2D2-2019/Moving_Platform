#include "qik_2s12v10.hpp"
#include "hwlib.hpp"
#include <hardware_usart.hpp>


namespace r2d2::moving_platform {

    const uint8_t qik_2s12v10_c::qik_autodetect_baudrate = 0xAA;
    const uint8_t qik_2s12v10_c::qik_request_firmwareversion = 0x81;
    const uint8_t qik_2s12v10_c::qik_motor_m0_set_forward = 0x88;
    const uint8_t qik_2s12v10_c::qik_motor_m0_set_reverse = 0x8A;
    const uint8_t qik_2s12v10_c::qik_motor_m1_set_forward = 0x8C;
    const uint8_t qik_2s12v10_c::qik_motor_m1_set_reverse = 0x8E;
    const uint8_t qik_2s12v10_c::qik_get_config_parameter = 0x83;
    const uint8_t qik_2s12v10_c::qik_get_error = 0x82;

    qik_2s12v10_c::qik_2s12v10_c(r2d2::uart_ports_c uart_port,
                                 unsigned int baudRate,
                                 hwlib::pin_out *_reset_pin)
        : reset_pin(_reset_pin), usart_bus(baudRate, uart_port) {
    }

    void qik_2s12v10_c::set_speed(const int8_t &_speed) {
        // todo: depending op the motor configuration, 127 may only be half
        // power, instead of full power todo: change the speed variable to the
        // actual speed
        uint8_t speed_byte = 0;
        if (_speed >= 0) {
            speed_byte = _speed;
            usart_bus << qik_motor_m0_set_forward << speed_byte;
            usart_bus << qik_motor_m1_set_forward << speed_byte;
        } else {
            speed_byte = -1 * _speed;
            usart_bus << qik_motor_m0_set_reverse << speed_byte;
            usart_bus << qik_motor_m1_set_reverse << speed_byte;
        }
    }
    void qik_2s12v10_c::set_m0_speed(const int8_t &_speed) {
        // todo: depending op the motor configuration, 127 may only be half
        // power, instead of full power todo: change the speed variable to the
        // actual speed
        uint8_t speed_byte = 0;
        if (_speed >= 0) {
            speed_byte = _speed;
            usart_bus << qik_motor_m0_set_forward << speed_byte;
        } else {
            speed_byte = -1 * _speed;
            usart_bus << qik_motor_m0_set_reverse << speed_byte;
        }
    }
    void qik_2s12v10_c::set_m1_speed(const int8_t &_speed) {
        // todo: depending op the motor configuration, 127 may only be half
        // power, instead of full power todo: change the speed variable to the
        // actual speed
        uint8_t speed_byte = 0;
        if (_speed >= 0) {
            speed_byte = _speed;
            usart_bus << qik_motor_m1_set_forward << speed_byte;
        } else {
            speed_byte = -1 * _speed;
            usart_bus << qik_motor_m1_set_reverse << speed_byte;
        }
    }

    void qik_2s12v10_c::init() {
        // reset the qik 2s12v10
        reset_pin->write(0);
        hwlib::wait_ms(1);
        reset_pin->write(1);
        hwlib::wait_ms(10);

        // setup the uart communication
        usart_bus << qik_autodetect_baudrate;

        // for testing
        hwlib::cout << "clearing buffer\n";
        while (usart_bus.available() > 0) {
            usart_bus.receive();
        } // clear the buffer
        hwlib::cout << "requesting firmware version\n";
        hwlib::wait_ms(1);
        usart_bus << qik_request_firmwareversion; // send request
        hwlib::wait_ms(1);
        hwlib::cout << "waiting for firmware version\n";
        while (!usart_bus.available()) {
        } // wait for answer
        hwlib::cout << "firmware version:";
        hwlib::cout << usart_bus.receive() << '\n'; // print answer
    }
    void qik_2s12v10_c::set_m0_brake(const uint8_t &brake) {
    }
    void qik_2s12v10_c::set_m1_brake(const uint8_t &brake) {
    }
    void qik_2s12v10_c::set_brakes(const uint8_t &brake) {
    }

    uint8_t qik_2s12v10_c::get_error() {
        while (usart_bus.available() > 0) {
            usart_bus.receive();
        }                           // clear the buffer
        usart_bus << qik_get_error; // send request
        while (!usart_bus.available()) {
        }                           // wait for answer
        return usart_bus.receive(); // return answer
    }
    uint8_t
    qik_2s12v10_c::get_configuration_parameter(const uint8_t &parameter) {
        while (usart_bus.available() > 0) {
            usart_bus.receive();
        }                                                   // clear the buffer
        usart_bus << qik_get_config_parameter << parameter; // send request
        while (!usart_bus.available()) {
        }                           // wait for answer
        return usart_bus.receive(); // return answer
    }
    void qik_2s12v10_c::set_configuration_parameter(const uint8_t &parameter,
                                                    const uint8_t &value) {
    }

    uint8_t qik_2s12v10_c::get_m0_current() {
        return 0;
    }
    uint8_t qik_2s12v10_c::get_m1_current() {
        return 0;
    }
    unsigned int qik_2s12v10_c::get_m0_current_milliamps() {
        return 0;
    }
    unsigned int qik_2s12v10_c::get_m1_current_milliamps() {
        return 0;
    }

} // namespace r2d2::moving_platform