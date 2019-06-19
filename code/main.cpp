
#include <beetle.hpp>
#include <comm.hpp>
#include <hardware_usart.hpp>
#include <hwlib.hpp>
#include <rhino.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // Bool to change between process function and rhino turn test.
    bool rhino_motor_test = true;

    // Create two encode pins for the motors.
    auto encode_m0 = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
    auto encode_m1 = hwlib::target::pin_adc(hwlib::target::ad_pins::a1);

    // Create reset pin for the qik board
    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5

    // Create usart object.
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(11500);

    // Create qik_controller for robot
    r2d2::moving_platform::qik_2s12v10_c qik =
        r2d2::moving_platform::qik_2s12v10_c(usart, qik_2s12v10_reset_pin);

    // create internal commmunication
    r2d2::comm_c comm;

    // Construct rhino object
    auto rhino =
        r2d2::moving_platform::rhino_c(qik, comm, encode_m0, encode_m1);

    if (!rhino_motor_test) {
        while (1) {
            rhino.process();
            hwlib::wait_ms(100);
        }
    }

    if (rhino_motor_test) {
        hwlib::cout << "Testing the rhino to turn 90 degrees left and right.\n";
        hwlib::wait_ms(500);
        rhino.turn(90);
        hwlib::wait_ms(1'000);
        rhino.turn(-90);
        hwlib::wait_ms(1'000);
    }

    return 0;
}