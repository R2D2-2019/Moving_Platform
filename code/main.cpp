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

	auto direction_pin_0 = hwlib::target::pin_out(hwlib::target::pins::d38);
	auto direction_pin_1 = hwlib::target::pin_out(hwlib::target::pins::d40);

    auto turtle_motor_left =
        r2d2::moving_platform::l9110(pwm_channel_0, direction_pin_0);
    auto turtle_motor_right =
        r2d2::moving_platform::l9110(pwm_channel_1, direction_pin_1);

    auto left_rotary_encoder =
        r2d2::moving_platform::rotary_encoder_turtle(pin_d2);
    auto right_rotary_encoder =
        r2d2::moving_platform::rotary_encoder_turtle(pin_d3);

    auto turtle = r2d2::moving_platform::turtle_controller(
        turtle_motor_left, turtle_motor_right, left_rotary_encoder,
        right_rotary_encoder);


	
	hwlib::cout << "TEST: Turtle driving forward\n";
    for (size_t i = 0; i < 255; i++) {
        turtle.set_speed(i);
        turtle.update();
        hwlib::cout << "Current speed: " << i << '\n';
        hwlib::wait_ms(100);
    }

	hwlib::cout << "TEST: Turtle driving backwards\n";
    for (size_t i = 0; i < 255; i++) {
        turtle.set_speed(-1 * i);
        turtle.update();
        hwlib::cout << "Current speed: -" << i << '\n';
        hwlib::wait_ms(100);
    }


    hwlib::cout << "Tests completed\n";
}