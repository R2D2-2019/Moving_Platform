#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    hwlib::ostream &
    operator<<(hwlib::ostream &out,
               const qik_2s12v10_set_configuration_parameter_return &rhs) {
        if (rhs == qik_2s12v10_set_configuration_parameter_return::command_ok) {
            out << "command ok";
        } else if (rhs ==
                   qik_2s12v10_set_configuration_parameter_return::bad_parameter) {
            out << "bad parameter";
        } else if (rhs ==
                   qik_2s12v10_set_configuration_parameter_return::bad_value) {
            out << "bad value";
        } else {
            out << "unknown return";
        }
        return out;
    };

    r2d2::usart::usart_connection_c &
    operator<<(r2d2::usart::usart_connection_c &out,
               const qik_2s12v10_registers &rhs) {
        return out << static_cast<uint8_t>(rhs);
    }
    r2d2::usart::usart_connection_c &operator<< (r2d2::usart::usart_connection_c &out, const qik_2s12v10_configuration_parameter &rhs){
        return out << static_cast<uint8_t>(rhs);
    }

    hwlib::ostream &operator<<(hwlib::ostream &out,
                               const qik_2s12v10_error &rhs) {
        switch (rhs) {
        case qik_2s12v10_error::motor_0_fault:
            out << "motor_0_fault";
            break;
        case qik_2s12v10_error::motor_1_fault:
            out << "motor_1_fault";
            break;
        case qik_2s12v10_error::motor_0_over_current:
            out << "motor_0_over_current";
            break;
        case qik_2s12v10_error::motor_1_over_current:
            out << "motor_1_over_current";
            break;
        case qik_2s12v10_error::serial_hardware_error:
            out << "serial_hardware_error";
            break;
        case qik_2s12v10_error::crc_error:
            out << "crc_error";
            break;
        case qik_2s12v10_error::format_error:
            out << "format_error";
            break;
        case qik_2s12v10_error::timeout:
            out << "timeout";
            break;
        default:
            out << "unkown error";
            break;
        }
        return out;
    }

    qik_2s12v10_c::qik_2s12v10_c(r2d2::usart::usart_ports &usart_port,
                                 unsigned int baud_rate,
                                 hwlib::pin_out &reset_pin)
        : reset_pin(reset_pin), usart_bus(baud_rate, usart_port) {
    }

    void qik_2s12v10_c::wait_for_bus(const qik_2s12v10_bus_state &required_state, uint8_t wait_ms_length){
        while (usart_bus.available() != static_cast<uint8_t>(required_state)) {
            usart_bus.receive();
            hwlib::wait_ms(wait_ms_length);
        } 
    }


    void qik_2s12v10_c::set_m0_speed(int8_t new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (new_speed >= 0) {
            usart_bus << qik_2s12v10_registers::set_motor_m0_forward
                      << new_speed;
        } else {
            usart_bus << static_cast<uint8_t>(
                             qik_2s12v10_registers::set_motor_m0_reverse)
                      << (-1 * new_speed);
        }
    }
    void qik_2s12v10_c::set_m1_speed(int8_t new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        if (new_speed >= 0) {
            usart_bus << qik_2s12v10_registers::set_motor_m1_forward
                      << new_speed;
        } else {
            usart_bus << qik_2s12v10_registers::set_motor_m1_reverse
                      << (-1 * new_speed);
        }
    }

    void qik_2s12v10_c::init() {
        // reset the qik 2s12v10
        reset_pin.write(0);
        hwlib::wait_ms(1);
        reset_pin.write(1);
        hwlib::wait_ms(10);

        // setup the uart communication
        usart_bus << qik_2s12v10_registers::autodetect_baudrate;
    }

    void qik_2s12v10_c::brake(const int8_t brake_amount) {
        usart_bus << qik_2s12v10_registers::set_motor_m0_brake << brake_amount;
        usart_bus << qik_2s12v10_registers::set_motor_m1_brake << brake_amount;
    }

    void qik_2s12v10_c::brake_m0(const int8_t brake_amount) {
        usart_bus << qik_2s12v10_registers::set_motor_m0_brake << brake_amount;
    }

    void qik_2s12v10_c::brake_m1(const int8_t brake_amount) {
        usart_bus << qik_2s12v10_registers::set_motor_m1_brake << brake_amount;
    }

    qik_2s12v10_error qik_2s12v10_c::get_error() {
        wait_for_bus(qik_2s12v10_bus_state::available);// clear the buffer
        usart_bus << qik_2s12v10_registers::get_error; // send request
        wait_for_bus(qik_2s12v10_bus_state::unavailable);                     // wait for answer
        return static_cast<qik_2s12v10_error>(
            usart_bus.receive()); // return answer
    }

    uint8_t
    qik_2s12v10_c::get_configuration_parameter(uint8_t parameter) {
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << qik_2s12v10_registers::get_config_parameter
                  << parameter; // send request
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return usart_bus.receive(); // return answer
    }
    
    qik_2s12v10_set_configuration_parameter_return qik_2s12v10_c::set_configuration_parameter(qik_2s12v10_configuration_parameter parameter, uint8_t value){
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << parameter
                  << parameter
                  << value; // send request
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return static_cast<qik_2s12v10_set_configuration_parameter_return>(usart_bus.receive()); // return answer
    }

    uint8_t qik_2s12v10_c::get_m0_current() {
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << qik_2s12v10_registers::get_motor_m0_current;
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return usart_bus.receive();
    }

    uint8_t qik_2s12v10_c::get_m1_current() {
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << qik_2s12v10_registers::get_motor_m1_current;
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return usart_bus.receive();
    }

    uint16_t qik_2s12v10_c::get_m0_current_milliamps() {
        return get_m0_current() * 150;
    }

    uint16_t qik_2s12v10_c::get_m1_current_milliamps() {
        return get_m0_current() * 150;
    }

    uint8_t qik_2s12v10_c::get_m0_speed(){
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << qik_2s12v10_registers::get_motor_m1_speed;
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return usart_bus.receive();
    }
    uint8_t qik_2s12v10_c::get_m1_speed(){
        wait_for_bus(qik_2s12v10_bus_state::available);
        usart_bus << qik_2s12v10_registers::get_motor_m1_speed;
        wait_for_bus(qik_2s12v10_bus_state::unavailable);
        return usart_bus.receive();
    }
} // namespace r2d2::moving_platform