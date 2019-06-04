/*
 * This main is used to test the canbus or beetle class.
 */
#include <hardware_usart.hpp>
#include <beetle.hpp>
#include <comm.hpp>
#include <hwlib.hpp>


int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // canbus beetle testing
    bool beetle_canbus = false;
    // beetle testing
    bool test_set_speed = true;

    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart1>(9600);
    r2d2::moving_platform::qik_2s12v10_c qik = r2d2::moving_platform::qik_2s12v10_c(usart, qik_2s12v10_reset_pin);

    r2d2::comm_c comm;
    auto beetle = r2d2::moving_platform::beetle_c(qik, comm);

    if (beetle_canbus) {
        while (1) {
            beetle.process();
            hwlib::wait_ms(100);
        }
    }

    if (test_set_speed) {
        // motor tests:
        hwlib::cout << "Testing both motors, 31% power forward.\n";
        beetle.set_speed(80);
        beetle.turn(0);
        hwlib::wait_ms(1000);
        beetle.set_speed(0);

        hwlib::wait_ms(500);

        hwlib::cout << "Testing both motors, 31% power backward.\n";
        beetle.set_speed(-80);
        beetle.turn(0);
        hwlib::wait_ms(2000);
        beetle.set_speed(0);

        hwlib::wait_ms(500);
        hwlib::cout << "Testing turning 60 degrees left.\n";
        beetle.set_speed(40);
        beetle.turn(60);
        hwlib::wait_ms(3000);
        beetle.set_speed(0);

        hwlib::wait_ms(500);
        beetle.set_speed(0);
    }

    hwlib::cout << "All tests have been completed\n";

    return 0;
}
