#include "hwlib.hpp"
#include <qik_2s12v10.hpp>

int main(void) {
  // kill the watchdog
  WDT->WDT_MR = WDT_MR_WDDIS;
  hwlib::wait_ms(1000);

  // qik_2s12v10_c testing

  auto motorControllerReset = hwlib::target::pin_out(2,25);
  
  r2d2::moving_platform::qik_2s12v10_c motorController(r2d2::uart_ports_c::uart1, 9600u, &motorControllerReset);
  hwlib::cout << "initializing!\n";
  motorController.init();
  hwlib::cout << "initialized!\n";

  hwlib::cout << "Current PWM configuration: " << motorController.get_configuration_parameter(1) << '\n';

  while(true){
    motorController.set_speed(100);
    hwlib::wait_ms(2000);
    motorController.set_speed(0);
    hwlib::wait_ms(2000);
    motorController.set_speed(-100);
    hwlib::wait_ms(2000);
  }
}