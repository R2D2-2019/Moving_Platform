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
  /*
  r2d2::moving_platform::Qik2s12v10 motorController(r2d2::uart_ports_c::uart1, 9600u, &motorControllerReset);
  hwlib::cout << "initializing!\n";
  motorController.init();
  hwlib::cout << "initialized!\n";
  */

  // debugging
  auto txPin = hwlib::target::pin_out(2,23);
  txPin.write(1);
  // reset the qik 2s12v10
  motorControllerReset.write(0);
  hwlib::wait_ms(1);
  motorControllerReset.write(1);
  hwlib::wait_ms(10);

  hwlib::cout << "sending 0xAA (10101010) at 9600bps...";
  sendAt9600(0xAA, txPin);
  hwlib::cout << " done\n";

  while(true){
    hwlib::wait_ms(1000);
  }
}