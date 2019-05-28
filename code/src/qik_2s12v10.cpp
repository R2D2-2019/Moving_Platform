#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {

    hwlib::ostream &
    operator<<(hwlib::ostream &out,
               const qik_2s12v10_set_configuration_parameter_return &rhs) {
        if (rhs == qik_2s12v10_set_configuration_parameter_return::command_ok) {
            out << "command ok";
        } else if (rhs == qik_2s12v10_set_configuration_parameter_return::
                              bad_parameter) {
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
    r2d2::usart::usart_connection_c &
    operator<<(r2d2::usart::usart_connection_c &out,
               const qik_2s12v10_configuration_parameter &rhs) {
        return out << static_cast<uint8_t>(rhs);
    }

    hwlib::ostream &operator<<(hwlib::ostream &out,
                               const qik_2s12v10_error &rhs) {
        switch (rhs) {
        case qik_2s12v10_error::motor_0_fault:
            out << "Motor_0_fault";
            break;
        case qik_2s12v10_error::motor_1_fault:
            out << "Motor_1_fault";
            break;
        case qik_2s12v10_error::motor_0_over_current:
            out << "Motor_0_over_current";
            break;
        case qik_2s12v10_error::motor_1_over_current:
            out << "Motor_1_over_current";
            break;
        case qik_2s12v10_error::serial_hardware_error:
            out << "Serial_hardware_error";
            break;
        case qik_2s12v10_error::crc_error:
            out << "Crc_error";
            break;
        case qik_2s12v10_error::format_error:
            out << "Format_error";
            break;
        case qik_2s12v10_error::timeout:
            out << "Timeout";
            break;
        case qik_2s12v10_error::no_error:
            out << "No error";
            break;
        default:
            out << "Unkown error";
            break;
        }
        return out;
    }

    qik_2s12v10_c::qik_2s12v10_c(r2d2::usart::usart_connection_c &usart_bus,
                                 hwlib::pin_out &reset_pin)
        : reset_pin(reset_pin), usart_bus(usart_bus) {
        init();
    }

    void qik_2s12v10_c::wait_for_bus(uint8_t wait_ms_length) {
        while (usart_bus.available() < 1) {
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
        uint8_t speed = 0;
        usart_bus << qik_2s12v10_registers::set_motor_m0_forward << speed;
    }

    void qik_2s12v10_c::brake_m1(const int8_t brake_amount) {
        usart_bus << qik_2s12v10_registers::set_motor_m1_brake << brake_amount;
        uint8_t speed = 0;
        usart_bus << qik_2s12v10_registers::set_motor_m1_forward << speed;
    }

    uint8_t qik_2s12v10_c::get_error_byte() {
        usart_bus << qik_2s12v10_registers::get_error; // send request
        wait_for_bus();                                // wait for answer
        return usart_bus.receive(); // return answer
    }

    void qik_2s12v10_c::print_errors(){
        uint8_t error_byte = get_error_byte();
        if (error_byte == 0){
            hwlib::cout << qik_2s12v10_error::no_error << '\n';
            return;
        }
        //check every bit
        for (size_t i = 0; i < 7; i++){
            // bitshift blackmagic
            uint8_t bit = (error_byte & (1 << i));
            if( bit > 0){
                hwlib::cout << static_cast<qik_2s12v10_error>(bit) << '\n';
            }
        }
        
        
    }

    uint8_t qik_2s12v10_c::get_configuration_parameter(
        qik_2s12v10_configuration_parameter parameter) {
        usart_bus << qik_2s12v10_registers::get_config_parameter
                  << parameter;
        wait_for_bus();
        return usart_bus.receive();
    }

    qik_2s12v10_set_configuration_parameter_return
    qik_2s12v10_c::set_configuration_parameter(
        qik_2s12v10_configuration_parameter parameter, uint8_t value) {
        usart_bus << parameter << value << 0x55 << 0x2A;
        wait_for_bus();
        return static_cast<qik_2s12v10_set_configuration_parameter_return>(
            usart_bus.receive());
    }

    uint8_t qik_2s12v10_c::get_m0_current() {
        usart_bus << qik_2s12v10_registers::get_motor_m0_current;
        wait_for_bus();
        return usart_bus.receive();
    }

    uint8_t qik_2s12v10_c::get_m1_current() {
        usart_bus << qik_2s12v10_registers::get_motor_m1_current;
        wait_for_bus();
        return usart_bus.receive();
    }

    uint16_t qik_2s12v10_c::get_m0_current_milliamps() {
        return get_m0_current() * 150;
    }

    uint16_t qik_2s12v10_c::get_m1_current_milliamps() {
        return get_m0_current() * 150;
    }

    uint8_t qik_2s12v10_c::get_m0_speed() {
        usart_bus << qik_2s12v10_registers::get_motor_m1_speed;
        wait_for_bus();
        return usart_bus.receive();
    }
    uint8_t qik_2s12v10_c::get_m1_speed() {
        usart_bus << qik_2s12v10_registers::get_motor_m1_speed;
        wait_for_bus();
        return usart_bus.receive();
    }
} // namespace r2d2::moving_platform