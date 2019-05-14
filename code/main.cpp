/*
 * This main is used to test the qik_2s12v10_c class.
 */

#include <hwlib.hpp>
#include <beetle.hpp>
#include <comm.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // qik_2s12v10_c testing
    bool test_set_speed = false;
    //bool test_get_configuration_parameter = true;
    //bool test_get_error = true;

    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    r2d2::uart_ports_c uart_port_one = r2d2::uart_ports_c::uart1;

    r2d2::comm_c comm;
    auto beetle = r2d2::moving_platform::beetle_c(
        uart_port_one, 9600u, qik_2s12v10_reset_pin, comm);

    /*hwlib::cout << "Initializing qik 2s12v10.\n";
    qik_2s12v10_motorcontroller.init();
    qik_2s12v10_motorcontroller.get_configuration_parameter(1);
    hwlib::cout << "Initialization compleet.\n";*/

    while (1)
    {
        beetle.process();
    }
    


    if (test_set_speed) {
        //motor tests:
        hwlib::cout << "Testing both motors, full power forward.\n";
        beetle.set_speed(31);
        hwlib::wait_ms(2000);
        beetle.set_speed(0);

        hwlib::wait_ms(500);

        hwlib::cout << "Testing both motors, full power backward.\n";
        beetle.set_speed(-31);
        hwlib::wait_ms(2000);
        beetle.set_speed(0);

        hwlib::wait_ms(500);
        hwlib::cout << "Testing turning 90 degrees.\n";
        beetle.turn(90);

        hwlib::wait_ms(500);
        beetle.set_speed(0);
    }

    /*if (test_get_configuration_parameter) {
        hwlib::cout << "Testing the get_configuration_parameter function.\n";
        hwlib::cout << "Current PWM configuration (should be 0): "
                    << qik_2s12v10_motorcontroller.get_configuration_parameter(1)
                    << '\n';
        hwlib::wait_ms(500);
    }

    if (test_get_error) {
        hwlib::cout << "Testing the get_error function.\n";
        hwlib::cout << "Error byte: " << hwlib::bin
                    << qik_2s12v10_motorcontroller.get_error() << hwlib::dec
                    << '\n';
    }*/
    hwlib::cout << "All tests have been completed\n";

    return 0;
}