#include <ostream>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <hwlib.hpp>
#include <qik_2s12v10.hpp>
#include <test_usart.hpp>

// Qik motorcontroller tests
TEST_CASE("Get error tests", "[qik_2s12v10]") {
    hwlib::pin_out_test reset_pin_test;
    auto usart = r2d2::usart::test_usart_c();
    auto motor_controller =
        r2d2::moving_platform::qik_2s12v10_c(usart, reset_pin_test);

    uint8_t test_error_code;
    SECTION("No errors recieved") {
        test_error_code = 0;
    }

    SECTION("Single error") {
        test_error_code = 1;
    }

    // since the error is a byte and each bit identifies as a different error
    // this should be 0b00000011 aka 2 for 2 errors.
    SECTION("Multiple errors") {
        test_error_code = 2;
    }

    usart.add_receive_byte(test_error_code);
    REQUIRE(motor_controller.get_error_byte() == test_error_code);
}

TEST_CASE("Setting motor speed", "[qik_2s12v10]") {
    hwlib::pin_out_test reset_pin_test;
    auto usart = r2d2::usart::test_usart_c();
    auto motor_controller =
        r2d2::moving_platform::qik_2s12v10_c(usart, reset_pin_test);

    // The init function will be called inside the constructor. This will write
    // the value of 0xAA in hex or 170 in decimal.
    REQUIRE(usart.get_send_byte() == 170);

    SECTION("Testing motor_m0") {
        // set variable for motorspeed.
        int speed = 100;

        motor_controller.set_m0_speed(speed);
        // The first byte that has to be send on the usart bus, is the register
        // of the m0 motor forward. This corresponds to the value 0x88 in
        // hexadecimal or value 136 as a uint8_t
        REQUIRE(usart.get_send_byte() == 136);

        // Check second byte is equal to the speed
        REQUIRE(usart.get_send_byte() == speed);

        // Set the speed as a negative number, it should respond as going in
        // reverse.
        speed = -100;

        motor_controller.set_m0_speed(speed);
        // The first byte needs to correspond to the register address of
        // m0_motor_reverse which is 0x8A(hex) or 138(decimal).
        REQUIRE(usart.get_send_byte() == 138);

        // Check if the correct speed byte is transmitted. The qik
        // implementation writes 100 instead of -100, thats why the speed was
        // inverted from negative to positive in this test.
        REQUIRE(usart.get_send_byte() == speed * -1);
    }
    SECTION("Testing motor_m1") {
        // set variable for motorspeed.
        int speed = 100;

        motor_controller.set_m1_speed(speed);
        // The first byte that has to be send on the usart bus, is the register
        // of the m0 motor forward. This corresponds to the value 0x8C in
        // hexadecimal or value 140 as a uint8_t
        REQUIRE(usart.get_send_byte() == 140);

        // Check second byte is equal to the speed
        REQUIRE(usart.get_send_byte() == speed);

        // Set the speed as a negative number, it should respond as going in
        // reverse.
        speed = -100;

        motor_controller.set_m1_speed(speed);
        // The first byte needs to correspond to the register address of
        // m0_motor_reverse which is 0x8E(hex) or 142(decimal).
        REQUIRE(usart.get_send_byte() == 142);

        // Check if the correct speed byte is transmitted. The qik
        // implementation writes 100 instead of -100, thats why the speed was
        // inverted from negative to positive in this test.
        REQUIRE(usart.get_send_byte() == speed * -1);
    }
}
