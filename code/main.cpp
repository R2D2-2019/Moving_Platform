/*
 * This main is used to test the canbus or beetle class.
 */

#include <hwlib.hpp>
#include <beetle.hpp>
#include <comm.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

    // canbus beetle testing
    bool beetle_canbus = false;
    // beetle testing
    bool test_set_speed = true;

    auto qik_2s12v10_reset_pin = hwlib::target::pin_out(2, 25); // digital pin 5
    r2d2::usart::usart_ports usart_port = r2d2::usart::usart_ports::uart1;

    r2d2::comm_c comm;
    auto beetle = r2d2::moving_platform::beetle_c(usart_port, 9600u, qik_2s12v10_reset_pin, comm);

    if(beetle_canbus){
        while (1){
            beetle.process();
            hwlib::wait_ms(100);
        }
    }
    
    
    if (test_set_speed) {
        //motor tests:
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