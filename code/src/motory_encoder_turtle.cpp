#include <rotary_encoder_turtle.hpp>

namespace r2d2::moving_platform {

    rotary_encoder_turtle::rotary_encoder_turtle(
        hwlib::target::pin_in &input_encoder)
        : input_encoder(input_encoder) {
    }

    uint8_t rotary_encoder_turtle::get_speed() {
        uint8_t counter;
        bool encoder_previus_measurement, encoder_current_measument;
        uint_fast64_t time_start_measurement;

        time_start_measurement = hwlib::now_us();
        encoder_previus_measurement = input_encoder.read();
        while (hwlib::now_us() - time_start_measurement <= interval) {
            encoder_current_measument = input_encoder.read();
            if (encoder_current_measument != encoder_previus_measurement) {
                counter++;
                encoder_previus_measurement = encoder_current_measument;
            }
        }
        return counter;
    }
} // namespace r2d2::moving_platform
