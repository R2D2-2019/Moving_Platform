#include <qik_2s12v10.hpp>

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
                                 hwlib::pin_out *_reset_pin): 
        reset_pin(_reset_pin), 
        usart_bus(baudRate, uart_port) 
        {}

    void qik_2s12v10_c::set_speed(const int8_t &_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        // Todo: change the speed variable to the actual speed.
        if (_speed >= 0) {
            usart_bus << qik_motor_m0_set_forward << _speed;
            usart_bus << qik_motor_m1_set_reverse << _speed;
        } else {
            usart_bus << qik_motor_m0_set_reverse << (-1 * _speed);
            usart_bus << qik_motor_m1_set_forward << (-1 * _speed);
        }
    }
    void qik_2s12v10_c::set_m0_speed(const int8_t &_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (_speed >= 0) {
            usart_bus << qik_motor_m0_set_forward << _speed;
        } else {
            usart_bus << qik_motor_m0_set_reverse << (-1 * _speed);
        }
    }
    void qik_2s12v10_c::set_m1_speed(const int8_t &_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (_speed >= 0) {
            usart_bus << qik_motor_m1_set_forward << _speed;
        } else {
            usart_bus << qik_motor_m1_set_reverse << (-1 * _speed);
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
    }

    void qik_2s12v10_c::brake(){
        set_speed(0);
    }

    void qik_2s12v10_c::turn(int8_t degrees){
        if(degrees >=0){
            set_m0_speed(40);
            set_m1_speed(40);
            hwlib::cout<<((2222*degrees)/360) << "\n";
            hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetle is 222 sec and needs start up 
        }else{
            set_m0_speed(-40);
            set_m1_speed(-40);
            hwlib::wait_ms(((2222*(-degrees)/360) + 60);
        }
        brake();
    }

    uint8_t qik_2s12v10_c::get_error() {
        while (usart_bus.available() > 0) {
            usart_bus.receive();
        }                           // clear the buffer
        usart_bus << qik_get_error; // send request
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);     // don't use all time
        }                           // wait for answer
        return usart_bus.receive(); // return answer
    }
    uint8_t qik_2s12v10_c::get_configuration_parameter(const uint8_t &parameter) {
        while (usart_bus.available() > 0) {
            usart_bus.receive();
        }                                                   // clear the buffer
        usart_bus << qik_get_config_parameter << parameter; // send request
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);     // don't use all time
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
