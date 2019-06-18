/*
 * This main is used to test the turtle class.
 */

#include <comm.hpp>
#include <hwlib.hpp>
#include <l9110.hpp>
#include <pwm.hpp>
#include <rotary_encoder_turtle.hpp>
#include <turtle_controller.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    auto pin_d2 = hwlib::target::pin_in(hwlib::target::pins::d2);
    auto pin_d3 = hwlib::target::pin_in(hwlib::target::pins::d3);

    // Motor left forward channel
    auto pwm_channel_0 = pwm_c(0);
    // Motor left backward channel
    auto pwm_channel_1 = pwm_c(1);

    // Motor right forward channel
    auto pwm_channel_2 = pwm_c(2);
    // Motor right backward channel
    auto pwm_channel_3 = pwm_c(3);

    auto turtle_motor_left =
        r2d2::moving_platform::l9110(pwm_channel_0, pwm_channel_1);
    auto turtle_motor_right =
        r2d2::moving_platform::l9110(pwm_channel_2, pwm_channel_3);

    auto left_rotary_encoder =
        r2d2::moving_platform::rotary_encoder_turtle(pin_d2);
    auto right_rotary_encoder =
        r2d2::moving_platform::rotary_encoder_turtle(pin_d3);

    auto turtle = r2d2::moving_platform::turtle_controller(
        turtle_motor_left, turtle_motor_right, left_rotary_encoder,
        right_rotary_encoder);

    hwlib::cout << "Testing turtle driving forward\n";
    turtle.set_speed(10);
    hwlib::wait_ms(500);
    turtle.update();
    hwlib::wait_ms(3000);

    hwlib::cout << "Testing turtle driving backwards\n";
    turtle.set_speed(-10);
    hwlib::wait_ms(500);
    turtle.update();
    hwlib::wait_ms(3000);

    hwlib::cout << "Stopping the turtle\n";
    turtle.set_speed(0);
    hwlib::wait_ms(500);
    turtle.update();
    hwlib::wait_ms(3000);

    hwlib::cout << "Tests completed\n";
}