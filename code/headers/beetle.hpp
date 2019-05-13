#pragma once
#include <hwlib.hpp>
#include <qik_2s12v10.hpp>
#include <moving_platform.hpp>
#include <base_module.hpp>

namespace r2d2::moving_platform {

    class beetle_c : public moving_platform_c {
    private:
        qik_2s12v10_c qik_2s12v10_motorcontroller;

    public:

        beetle_c(r2d2::uart_ports_c &uart_port,
             unsigned int baud_rate,
             hwlib::pin_out &reset_pin, base_comm_c &comm);

        /**
		* the speed set must not exceed 100
		*/
        void set_speed(const int8_t &new_speed) override;

        void set_steering(const int16_t &degrees) override;
        void turn(const int16_t &degrees) override;
        void move (const int8_t &distance) override;
        void move (const int8_t &x, const int8_t &y) override;
    };
} // namespace r2d2::moving_platform
