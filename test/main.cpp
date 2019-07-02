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

    std::vector<uint8_t> send_buffer = usart.get_send_bytes();

    // The init function will be called inside the constructor. This will write
    // the value of 0xAA in hex or 170 in decimal.
    REQUIRE(usart.get_send_byte() == 170);

    send_buffer = usart.get_send_bytes();
    // Testing if send buffer is empty.
    REQUIRE(send_buffer.size() == 0);

    SECTION("Testing motor_m0") {
        // set variable for motorspeed.
        int speed = 100;

        // Testing if send buffer is empty.
        REQUIRE(send_buffer.size() == 0);

        motor_controller.set_m0_speed(speed);
        // The first byte that has to be send on the usart bus, is the register
        // of the m0 motor forward. This corresponds to the value 0x88 in
        // hexadecimal or value 136 as a uint8_t
        REQUIRE(usart.get_send_byte() == 136);

        // Check second byte is equal to the speed
        REQUIRE(usart.get_send_byte() == speed);

        // Testing if send buffer is empty. Which should be since these are the
        // only USART commands that were sent.
        REQUIRE(send_buffer.size() == 0);

        // Set the speed as a negative number, it should respond as going in
        // reverse.
        speed = -100;

        send_buffer = usart.get_send_bytes();

        // Testing if send buffer is empty.
        REQUIRE(send_buffer.size() == 0);

        motor_controller.set_m0_speed(speed);
        // The first byte needs to correspond to the register address of
        // m0_motor_reverse which is 0x8A(hex) or 138(decimal).
        REQUIRE(usart.get_send_byte() == 138);

        // Check if the correct speed byte is transmitted. The qik
        // implementation writes 100 instead of -100, thats why the speed was
        // inverted from negative to positive in this test.
        REQUIRE(usart.get_send_byte() == speed * -1);

        // Testing if send buffer is empty.
        REQUIRE(send_buffer.size() == 0);
    }

    SECTION("Testing motor_m1") {
        // set variable for motorspeed.
        int speed = 100;
        std::vector<uint8_t> send_buffer = usart.get_send_bytes();

        motor_controller.set_m1_speed(speed);
        // The first byte that has to be send on the usart bus, is the register
        // of the m1_motor_forward. This corresponds to the value 0x8C in
        // hexadecimal or value 140 as a uint8_t
        REQUIRE(usart.get_send_byte() == 140);

        // Check second byte is equal to the speed
        REQUIRE(usart.get_send_byte() == speed);

        // Set the speed as a negative number, it should respond as going in
        // reverse.
        speed = -100;

        motor_controller.set_m1_speed(speed);
        // The first byte needs to correspond to the register address of
        // m1_motor_reverse which is 0x8E(hex) or 142(decimal).
        REQUIRE(usart.get_send_byte() == 142);

        // Check if the correct speed byte is transmitted. The qik
        // implementation writes 100 instead of -100, thats why the speed was
        // inverted from negative to positive in this test.
        REQUIRE(usart.get_send_byte() == speed * -1);

        // Testing if send buffer is empty. Which should be since these are the
        // only USART commands that were sent.
        REQUIRE(send_buffer.size() == 0);
    }
}

TEST_CASE("Testing brake functions", "[qik_2s12v10]") {
    hwlib::pin_out_test reset_pin_test;
    auto usart = r2d2::usart::test_usart_c();
    auto motor_controller =
        r2d2::moving_platform::qik_2s12v10_c(usart, reset_pin_test);
    // The init function will be called inside the constructor. This will write
    // the value of 0xAA in hex or 170 in decimal.
    REQUIRE(usart.get_send_byte() == 170);
    std::vector<uint8_t> send_buffer = usart.get_send_bytes();

    // Check if send buffer is empty, which should be after getting the init
    // byte.
    REQUIRE(send_buffer.size() == 0);

    SECTION("Testing general brake function with positive value") {
        int brake_amount = 50;
        motor_controller.brake(brake_amount);

        // This should send 4 bytes over the usart bus. First the register
        // of motor_m0_brake, then the value of 50. Then set the speed of
        // motor_m0 to speed 0. Then the motor_m1_brake with value of 50. then
        // set the speed of motor_m1 to speed 0.
        REQUIRE(usart.get_send_byte() == 134);
        REQUIRE(usart.get_send_byte() == brake_amount);
        // Set speed of motor_m0 to 0.
        REQUIRE(usart.get_send_byte() == 136);
        REQUIRE(usart.get_send_byte() == 0);
        // motor_m1 brake with value of 50.
        REQUIRE(usart.get_send_byte() == 135);
        REQUIRE(usart.get_send_byte() == brake_amount);
        // Set speed of motor_m1 to 0.
        REQUIRE(usart.get_send_byte() == 140);
        REQUIRE(usart.get_send_byte() == 0);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);
    }
    SECTION("Testing general brake function with negative value") {
        int brake_amount = -50;
        motor_controller.brake(brake_amount);

        // This should send 4 bytes over the usart bus. First the register
        // of motor_m0_brake, then with 127, because int -50 is
        // converted to a uint8_t, and limited to max 127m then the register of
        // motor_m1_brake and again the brake amount.
        REQUIRE(usart.get_send_byte() == 134);
        REQUIRE(usart.get_send_byte() == 127);
        // Set speed of motor_m0 to 0.
        REQUIRE(usart.get_send_byte() == 136);
        REQUIRE(usart.get_send_byte() == 0);
        // motor_m1 brake with value of 127.
        REQUIRE(usart.get_send_byte() == 135);
        REQUIRE(usart.get_send_byte() == 127);
        // Set speed of motor_m1 to 0.
        REQUIRE(usart.get_send_byte() == 140);
        REQUIRE(usart.get_send_byte() == 0);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);
    }

    SECTION("Testing brake on seperate motors with positive value") {
        int speed = 0;
        int brake_amount = 50;
        motor_controller.brake_m0(brake_amount);

        // This should send 4 bytes over the usart bus. First the register of
        // motor_m0_brake(value of 134), then the brake amount. Then the
        // motor_m0_forward register, then speed 0.
        REQUIRE(usart.get_send_byte() == 134);
        REQUIRE(usart.get_send_byte() == brake_amount);
        REQUIRE(usart.get_send_byte() == 136);
        REQUIRE(usart.get_send_byte() == speed);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);

        motor_controller.brake_m1(brake_amount);

        // This should send 4 bytes over the usart bus. First the register of
        // motor_m1_brake(value of 135), then the brake amount. Then the
        // motor_m1_forward register, then speed 0.
        REQUIRE(usart.get_send_byte() == 135);
        REQUIRE(usart.get_send_byte() == brake_amount);
        REQUIRE(usart.get_send_byte() == 140);
        REQUIRE(usart.get_send_byte() == speed);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);
    }
    SECTION("Testing brake on seperate motors with negative value") {
        int speed = 0;
        int brake_amount = -50;
        motor_controller.brake_m0(brake_amount);

        // This should send 4 bytes over the usart bus. First the register of
        // motor_m1_brake(value of 134), then with 127, because int -50 is
        // converted to a uint8_t, and limited to max 127. Then the
        // motor_m1_forward register, then speed 0.
        REQUIRE(usart.get_send_byte() == 134);
        REQUIRE(usart.get_send_byte() == 127);
        REQUIRE(usart.get_send_byte() == 136);
        REQUIRE(usart.get_send_byte() == speed);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);

        motor_controller.brake_m1(brake_amount);

        // This should send 4 bytes over the usart bus. First the register of
        // motor_m1_brake(value of 135), then with 127, because int -50 is
        // converted to a uint8_t, and limited to max 127 Then the
        // motor_m1_forward register, then speed 0.
        REQUIRE(usart.get_send_byte() == 135);
        REQUIRE(usart.get_send_byte() == 127);
        REQUIRE(usart.get_send_byte() == 140);
        REQUIRE(usart.get_send_byte() == speed);

        send_buffer = usart.get_send_bytes();
        // Check if send buffer is empty, which should be after these 4 bytes.
        REQUIRE(send_buffer.size() == 0);
    }
}
