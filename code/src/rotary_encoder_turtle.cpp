#include <rotary_encoder_turtle.hpp>

namespace r2d2::moving_platform {

    rotary_encoder_turtle::rotary_encoder_turtle(
        hwlib::target::pin_in &input_encoder)
        : input_encoder(input_encoder) {
    }

    bool rotary_encoder_turtle::read_encoder_data() {
        input_encoder.refresh();
        return input_encoder.read();
    }

    uint8_t rotary_encoder_turtle::get_speed() {
        // Encoder data state swap counter
        uint16_t encoder_step_counter = 0;

        // Track initial time
        uint_fast64_t time_start_measurement = hwlib::now_us();

        // Initial measurement of the encoder
        bool encoder_initial_measurement = read_encoder_data();

        while (interval <= (hwlib::now_us() - time_start_measurement)) {

            if (read_encoder_data() != encoder_initial_measurement) {
                encoder_step_counter++;
                encoder_initial_measurement = !encoder_initial_measurement;
            }
        }

        // Returns number of times the encoder read switched states
        return encoder_step_counter;
    }
} // namespace r2d2::moving_platform
