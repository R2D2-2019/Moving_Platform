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
