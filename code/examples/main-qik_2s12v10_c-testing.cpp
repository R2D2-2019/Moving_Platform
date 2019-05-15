/*
 * This main is used to test the qik_2s12v10_c class.
 */

#include <hwlib.hpp>
#include <qik_2s12v10.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // qik_2s12v10_c testing
    bool test_set_speed = true;
    bool test_get_configuration_parameter = true;
    bool test_get_error = true;
    bool test_get_motor_current = true;

    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    r2d2::uart_ports_c uart_port_one = r2d2::uart_ports_c::uart1;

    auto qik_2s12v10_motorcontroller = r2d2::moving_platform::qik_2s12v10_c(
        uart_port_one, 9600u, qik_2s12v10_reset_pin);

    hwlib::cout << "Initializing qik 2s12v10.\n";
    qik_2s12v10_motorcontroller.init();
    qik_2s12v10_motorcontroller.get_configuration_parameter(1);
    hwlib::cout << "Initialization compleet.\n";

    if (test_set_speed) {
        //error_test:
        hwlib::cout << "Testing the get_error function.\n";
        hwlib::cout << "Error byte: " << hwlib::bin
                    << qik_2s12v10_motorcontroller.get_error() << hwlib::dec
                    << '\n';
        

        //motor tests:
        hwlib::cout << "Testing both motors, full power forward.\n";
        qik_2s12v10_motorcontroller.set_speed(127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();

        hwlib::wait_ms(500);

        hwlib::cout << "Testing both motors, full power backward.\n";
        qik_2s12v10_motorcontroller.set_speed(-127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();

        hwlib::wait_ms(500);

        hwlib::cout << "Testing motor m0, full power forward.\n";
        qik_2s12v10_motorcontroller.set_m0_speed(127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();

        hwlib::wait_ms(500);

        hwlib::cout << "Testing motor m0, full power backward.\n";
        qik_2s12v10_motorcontroller.set_m0_speed(-127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();


        hwlib::cout << "Testing motor m1, full power forward.\n";
        qik_2s12v10_motorcontroller.set_m1_speed(127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();

        hwlib::cout << "Testing motor m1, full power backward.\n";
        qik_2s12v10_motorcontroller.set_m1_speed(-127);
        hwlib::wait_ms(2000);
        qik_2s12v10_motorcontroller.brake();

        hwlib::cout << "Motor tests compleet.\n";
        hwlib::wait_ms(500);
    }

    if (test_get_configuration_parameter) {
        hwlib::cout << "Testing the get_configuration_parameter function.\n";
        hwlib::cout << "Current PWM configuration (should be 0): "
                    << qik_2s12v10_motorcontroller.get_configuration_parameter(1)
                    << '\n';
        hwlib::wait_ms(500);
    }
    if (test_get_motor_current) {
        hwlib::cout << "Testing the get_m0_current_milliamps function.\n";
        hwlib::cout << qik_2s12v10_motorcontroller.get_m0_current_milliamps() << '\n';
        hwlib::cout << "Testing the get_m1_current_milliamps function.\n";
        hwlib::cout << qik_2s12v10_motorcontroller.get_m1_current_milliamps() << '\n';
    }
    if (test_get_error) {
        hwlib::cout << "Testing the get_error function.\n";
        hwlib::cout << "Error byte: " << hwlib::bin
                    << qik_2s12v10_motorcontroller.get_error() << hwlib::dec
                    << '\n';
    }
    hwlib::cout << "All tests have been completed\n";

    return 0;
}