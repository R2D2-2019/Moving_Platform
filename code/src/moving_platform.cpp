#include <moving_platform.hpp>

namespace r2d2::moving_platform {

    moving_platform_c::moving_platform_c(base_comm_c &comm)
        : base_module_c(comm) {
        comm.listen_for_frames({r2d2::frame_type::MOVEMENT_CONTROL});
    }

    void moving_platform_c::process() {
        comm.request(r2d2::frame_type::MOVEMENT_CONTROL);
        while (comm.has_data()) {

            auto frame = comm.get_data();
            // Process the frame

            // Don't handle requests
            if (frame.request) {
                continue;
            }

            const auto data =
                frame.as_frame_type<frame_type::MOVEMENT_CONTROL>();

            if (data.brake) {
                set_speed(0);
                break;
            } else {
                set_speed(data.speed);
                turn(data.rotation);
            }
        }
    }

    void moving_platform_c::move(uint16_t distance) {
    }

} // namespace r2d2::moving_platform