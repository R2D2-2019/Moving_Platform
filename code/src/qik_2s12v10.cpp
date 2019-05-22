#include <qik_2s12v10.hpp>

namespace r2d2::moving_platform {


    qik_2s12v10_c::qik_2s12v10_c(r2d2::uart_ports_c &uart_port,
                                 unsigned int baud_rate,
                                 hwlib::pin_out &reset_pin): 
        reset_pin(reset_pin), 
        usart_bus(baud_rate, uart_port) 
        {
            hwlib::cout<<"qik made \n";
            init();
            get_configuration_parameter(1);
            hwlib::cout<<"constructor qik done \n";

        }

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
        hwlib::cout << "m0 speed\n";
        if (new_speed >= 0) {
            usart_bus << qik_motor_m0_set_forward << new_speed;
        } else {
            usart_bus << qik_motor_m0_set_reverse << (-1 * new_speed);
        }
    }
    void qik_2s12v10_c::set_m1_speed(const int8_t &new_speed) {
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        hwlib::cout << "m1 speed \n";
        if (new_speed >= 0) {
            usart_bus << qik_motor_m1_set_forward << new_speed;
        } else {
            usart_bus << qik_motor_m1_set_reverse << (-1 * new_speed);
        }
    }

    void qik_2s12v10_c::init() {
        // reset the qik 2s12v10
        hwlib::cout << "it is in init\n";
        reset_pin.write(0);
        hwlib::wait_ms(1);
        reset_pin.write(1);
        hwlib::wait_ms(10);

        hwlib::cout << "go to usart bus\n";
        // setup the uart communication
        usart_bus << qik_autodetect_baudrate;
        hwlib::cout << "send done \n";
    }

    void qik_2s12v10_c::brake(){
        set_speed(0);
    }

    void qik_2s12v10_c::turn(int8_t degrees){       //temp: now only for beetle, needs to be universal
        if(degrees >=0){
            set_m0_speed(40);
            set_m1_speed(40);
            hwlib::cout<<((2222*degrees)/360) << "\n";
            hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetle is 222 sec and needs start up 
        }else{
            set_m0_speed(-40);
            set_m1_speed(-40);
            hwlib::wait_ms((2222*(-degrees)/360) + 60);
        }
        brake();
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
        hwlib::cout << "send config par \n";
        while (usart_bus.available() > 0) {
            usart_bus.receive();
            hwlib::wait_ms(0.05);
        } 
        hwlib::cout << "bus available >0 \n";                                                  // clear the buffer
        usart_bus << qik_get_config_parameter << parameter; // send request
        
        while (!usart_bus.available()) {
            hwlib::wait_ms(50);     // don't use all time
        }                           // wait for answer
        hwlib::cout << "usart_bus available\n";
        return usart_bus.receive(); // return answer
        return 0;
    }
} // namespace r2d2::moving_platform
