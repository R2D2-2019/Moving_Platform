#include "hwlib.hpp"
#include <Qik2s12v10.hpp>
// helper function for debugging
void sendAt9600(uint8_t byte, hwlib::target::pin_out txPin){
  txPin.write(0); // low startbit
  hwlib::wait_ns(104166);
  for(unsigned int i = 0; i<8; i++){
    txPin.write((byte>>i) & 1);
    hwlib::wait_ns(104166);
  }
  txPin.write(1); // high endbit
  hwlib::wait_ns(104166);
}

int main(void) {
  // kill the watchdog
  WDT->WDT_MR = WDT_MR_WDDIS;
  hwlib::wait_ms(1000);

  // Qik2s12v10 testing

  auto motorControllerReset = hwlib::target::pin_out(2,25);
  
  r2d2::moving_platform::Qik2s12v10 motorController(r2d2::uart_ports_c::uart1, 9600u, &motorControllerReset);
  hwlib::cout << "initializing!\n";
  motorController.init();
  hwlib::cout << "initialized!\n";
  
  while(true){
    motorController.set_speed(100);
    hwlib::wait_ms(2000);
    motorController.set_speed(0);
    hwlib::wait_ms(2000);
  }
}