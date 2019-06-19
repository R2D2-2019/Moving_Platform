/*
 * This main is used to test the canbus or beetle class.
 */
#include <beetle.hpp>
#include <comm.hpp>
#include <hardware_usart.hpp>
#include <hwlib.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // Create QIK board that handles the motors.
    auto encode_m0 = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
    auto encode_m1 = hwlib::target::pin_adc(hwlib::target::ad_pins::a1);
    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);
    r2d2::moving_platform::qik_2s12v10_c qik =
        r2d2::moving_platform::qik_2s12v10_c(usart, qik_2s12v10_reset_pin);

    // Create internalcommunication
    r2d2::comm_c comm;

    // Create The moving platform the Beetle
    auto beetle =
        r2d2::moving_platform::beetle_c(qik, comm, encode_m0, encode_m1);

    while (true) {
        beetle.process();
        hwlib::wait_ms(100);
    }

    return 0;
}
