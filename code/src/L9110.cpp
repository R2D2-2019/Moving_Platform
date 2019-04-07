#include "..\headers\L9110.hpp"

void L9110_c::set_speed(const signed char &_speed) {
    speed = _speed;
    if (speed == 0) {
        a.write(false);
        b.write(false);
    } else if (speed > 0) {
        a.write(true);
        b.write(false);
    } else {
        a.write(false);
        b.write(true);
    }
}
