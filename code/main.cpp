
#include <beetle.hpp>
#include <comm.hpp>
#include <hardware_usart.hpp>
#include <hwlib.hpp>
#include <rhino.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    bool rhino_motor_test = true;
    auto encode_m0 = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
    auto encode_m1 = hwlib::target::pin_adc(hwlib::target::ad_pins::a1);
    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);

    r2d2::moving_platform::qik_2s12v10_c qik =
        r2d2::moving_platform::qik_2s12v10_c(usart, qik_2s12v10_reset_pin);
    // create internal commmunication
    r2d2::comm_c comm;

    auto rhino =
        r2d2::moving_platform::rhino_c(qik, comm, encode_m0, encode_m1);
    if (!rhino_motor_test) {
        while (1) {
            rhino.process();
            hwlib::wait_ms(100);
        }
    }

    if (rhino_motor_test) {
        hwlib::cout << "Testing both motors, 31% power forward.\n";
        rhino.set_speed(80);
        rhino.turn(0);
        hwlib::wait_ms(1000);
        rhino.set_speed(0);

        hwlib::wait_ms(500);

        hwlib::cout << "Testing both motors, 31% power backward.\n";
        rhino.set_speed(-80);
        rhino.turn(0);
        hwlib::wait_ms(2000);
        rhino.set_speed(0);

        hwlib::wait_ms(500);
        hwlib::cout << "Testing turning 90 degrees left.\n";
        rhino.set_speed(40);
        rhino.turn(90);
        hwlib::wait_ms(3000);
        rhino.set_speed(0);

        hwlib::wait_ms(500);
        rhino.set_speed(0);
    }

    hwlib::cout << "All tests have been completed\n";

    return 0;
}