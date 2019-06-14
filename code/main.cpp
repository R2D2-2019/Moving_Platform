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

    auto encode_m0 = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
    auto encode_m1 = hwlib::target::pin_adc(hwlib::target::ad_pins::a1);
    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);
    r2d2::moving_platform::qik_2s12v10_c qik =
        r2d2::moving_platform::qik_2s12v10_c(usart, qik_2s12v10_reset_pin);
    /*
     * Test to turn the beetle 90 degrees.
     * code needs a go to the right class(beetle and rhino)
     */
    // Turn value, the wheels need to turn 0.62 for 90 degrees. 1 turn is one
    // full rotation of the wheel.
    float turn = 0.62;
    hwlib::wait_ms(100);
    // The puls. Its starts low.
    bool low_m0 = false;
    int counter_m0 = 0;
    bool low_m1 = false;
    int counter_m1 = 0;
    // The adc input. is between 3000 and 3800.
    unsigned int adc_voltage = 3500;
    // Encode frequency for 1 turn of the wheel. the encoder has 64 point per
    // over 2 pins we count when de adc goes from low to high of 1 pin. The gear
    // ratio from the motor is 50:1 64/4*50 = 800
    int encode_1_full_turn = 800;
    // Motor speed
    int motor_speed = 20;
    // motor tests strats motors.
    hwlib::cout << "start test \n";
    hwlib::wait_ms(10);
    qik.set_m0_speed(motor_speed);
    qik.set_m1_speed(motor_speed);

    while (true) {
        if (encode_m0.read() > adc_voltage) {
            if (low_m0 == true) {
                counter_m0++;
            }
            low_m0 = false;
        } else {
            low_m0 = true;
        }
        if (counter_m0 == (int(encode_1_full_turn * turn))) {
            qik.brake_m0(motor_speed);
        }

        if (encode_m1.read() > adc_voltage) {
            if (low_m1 == true) {
                counter_m1++;
            }
            low_m1 = false;
        } else {
            low_m1 = true;
        }
        if (counter_m1 == (int(encode_1_full_turn * turn))) {
            qik.brake_m1(motor_speed);
        }
        if (counter_m0 > (encode_1_full_turn * turn) &&
            counter_m1 > (encode_1_full_turn * turn)) {
            break;
        }
    }

    hwlib::cout << "All tests have been completed\n";

    return 0;
}
