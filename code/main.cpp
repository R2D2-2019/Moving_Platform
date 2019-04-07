#include "hwlib.hpp"
#include "headers/L9110.hpp"

void move(const int &dist, motor_control_c &a, motor_control_c &b) {
    //int time = hwlib::now_us();
    a.set_speed(1);
    b.set_speed(1);
}


int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);

	auto a1 = hwlib::target::pin_out(due::pins::d36);
    auto a2 = hwlib::target::pin_out(due::pins::d37);
    auto b1 = hwlib::target::pin_out(due::pins::d38);
    auto b2 = hwlib::target::pin_out(due::pins::d39);

	L9110_c motor_a(a1,a2);
    L9110_c motor_b(b1,b2);

    for (;;){
        move(1, motor_a, motor_b);
    }
}