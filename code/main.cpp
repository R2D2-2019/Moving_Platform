#include <hwlib.hpp>
#include <qik_2s12v10.hpp>

int main(void) {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    while (true) {
        hwlib::wait_ms(1000);
    }
}