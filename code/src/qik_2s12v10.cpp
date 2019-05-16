#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    qik_2s12v10_c::qik_2s12v10_c(r2d2::uart_ports_c &uart_port,
                                 unsigned int baud_rate,
                                 hwlib::pin_out &reset_pin): 
        reset_pin(reset_pin), 
        usart_bus(baud_rate, uart_port) 
        {}

    void qik_2s12v10_c::set_speed(const int8_t &new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        // Todo: change the speed variable to the actual speed.
        if (new_speed >= 0) {
            usart_bus << qik_motor_m0_set_forward << new_speed;
            usart_bus << qik_motor_m1_set_reverse << new_speed;
        } else {
            usart_bus << qik_motor_m0_set_reverse << (-1 * new_speed);
            usart_bus << qik_motor_m1_set_forward << (-1 * new_speed);
        }
    }
    void qik_2s12v10_c::set_m0_speed(const int8_t &new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (new_speed >= 0) {
            usart_bus << qik_motor_m0_set_forward << new_speed;
        } else {
            usart_bus << qik_motor_m0_set_reverse << (-1 * new_speed);
        }
    }
    void qik_2s12v10_c::set_m1_speed(const int8_t &new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (new_speed >= 0) {
            usart_bus << qik_motor_m1_set_forward << new_speed;
        } else {
            usart_bus << qik_motor_m1_set_reverse << (-1 * new_speed);
        }
    }

    void qik_2s12v10_c::init() {
        // reset the qik 2s12v10
        reset_pin.write(0);
        hwlib::wait_ms(1);
        reset_pin.write(1);
        hwlib::wait_ms(10);

        // setup the uart communication
        usart_bus << qik_autodetect_baudrate;
    }

    void qik_2s12v10_c::brake(const int8_t &brake_amount){
        usart_bus << qik_motor_m0_brake << brake_amount;
        usart_bus << qik_motor_m1_brake << brake_amount;
    }

    void qik_2s12v10_c::brake_m0(const int8_t &brake_amount){
        usart_bus << qik_motor_m0_brake << brake_amount;
    }

    void qik_2s12v10_c::brake_m1(const int8_t &brake_amount){
        usart_bus << qik_motor_m1_brake << brake_amount;
    }

    uint8_t qik_2s12v10_c::get_error() {
        while (usart_bus.available() > 0) {
            usart_bus.receive();
            hwlib::wait_ms(0.05);
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
            hwlib::wait_ms(0.05);
        }                                                   // clear the buffer
        usart_bus << qik_get_config_parameter << parameter; // send request
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);     // don't use all time
        }                           // wait for answer
        return usart_bus.receive(); // return answer
    }
        
        
    uint8_t qik_2s12v10_c::get_m0_current(){
        while (usart_bus.available() > 0) { // clear buffer
            usart_bus.receive();
            hwlib::wait_ms(0.05);
        }
        usart_bus << qik_get_motor_m0_current;
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);
        }
        return usart_bus.receive();
        
    }

    uint8_t qik_2s12v10_c::get_m1_current(){
        while (usart_bus.available() > 0) { // clear buffer
            usart_bus.receive();
            hwlib::wait_ms(0.05);
        }
        usart_bus << qik_get_motor_m1_current;
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);
        }
        return usart_bus.receive();
    }

    size_t qik_2s12v10_c::get_m0_current_milliamps(){
        return get_m0_current() * 150;
    }

    size_t qik_2s12v10_c::get_m1_current_milliamps(){
        return get_m0_current() * 150;
    }

} // namespace r2d2::moving_platform
