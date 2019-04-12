#pragma once

#include "hwlib.hpp"
#include "motor_control.hpp"

/// @file

namespace r2d2::moving_platform {

	/// @brief
	/// Class that represents a qik2s12v10 motor controller.
	/// @details
	/// The qik2s12v10 is a motor controller that can control two brushed DC motors. The qik2s12v10, and this class, can control the speed and brakes of the individual motors.
	/// The qik2s12v10 can also set and return its configuration parameter, which includes things like Acceleration rate and PWM resolution and frequency.
	/// Furthermore, the qik2s12v10 can check for errors and return these.
	class Qik2s12v10 {
	private:
		/// @brief
		/// Pin that will be used to receive information from the UART TTL communication with the qik2s12v10.
		hwlib::pin_in& receivePin;
		/// @brief
		/// Pin that will be used to send information to the qik2s12v10 using UART TTL communication.
		hwlib::pin_out& transmitPin;
		/// @brief
		/// Pin that can be used to reset the qik2s12v10.
		hwlib::pin_out& resetPin;
		
	public:
		/// @brief
		/// Qik2s12v10 constructor.
		/// @details
		/// Constructs a Qik2s12v10 using the given pins.
		/// @param _receivePin Pin that will be used to receive information from the UART TTL communication with the qik2s12v10.
		/// @param _transmitPin Pin that will be used to send information to the qik2s12v10 using UART TTL communication.
		/// @param _resetPin Pin that can be used to reset the qik2s12v10.
		Qik2s12v10(hwlib::pin_in& _receivePin, hwlib::pin_out& _transmitPin, hwlib::pin_out& _resetPin);
		
		/// @brief
		/// Sets the speed of both motors.
		/// @param _speed The speed of both motors. This value can be between -128 and 127, where -128 is full power backwards, 0 is no power and 127 is full power forward.
		void set_speed(const int8_t &_speed);
		/// @brief
		/// Sets the speed of the M0 motor.
		/// @param _speed The speed of the motor. This value can be between -128 and 127, where -128 is full power backwards, 0 is no power and 127 is full power forward.
        	void setM0Speed(const int8_t &_speed);
		/// @brief
		/// Sets the speed of the M1 motor.
		/// @param _speed The speed of the motor. This value can be between -128 and 127, where -128 is full power backwards, 0 is no power and 127 is full power forward.
        	void setM1Speed(const int8_t &_speed);
		
		/// @brief
		/// Initializes the qik2s12v10 by resetting it and then setting the baud rate.
		/// @param baudRate The baud rate that the Qik2s12v10 will use in its UART TTL serial communication in Bps.
		void init(const long& baudRate);
		/// @brief
		/// Sets the brake for the M0 motor.
		/// @param brake The amount of brake that will be applied to the motor, in the range [0,128] where 0 is no brake and 127 is full brake.
		void setM0Break(const unsigned char& brake);
		/// @brief
		/// Sets the brake for the M1 motor.
		/// @param brake The amount of brake that will be applied to the motor, in the range [0,128] where 0 is no brake and 127 is full brake.
		void setM1Break(const unsigned char& brake);
		/// @brief
		/// Sets the brake for both motors.
		/// @param brake The amount of brake that will be applied to the motors, in the range [0,128] where 0 is no brake and 127 is full brake.
		void setBreaks(const unsigned char& brake);
		
		/// @brief
		/// Returns the errors that the qik2s12v10 has detected since this function was last used. The meaning of each bit can be found here:
		/// https://www.pololu.com/docs/0J29/5.c
		unsigned char getError();
		/// @brief
		/// Returns the value that the parameter specified by parameter currently has.
		/// @param parameter Specifies the parameter that should be returned. The parameters can be found here:
		/// https://www.pololu.com/docs/0J29/5.a
		unsigned char getConfigurationParameter(const unsigned char& parameter);
		/// @brief
		/// Sets the value of the configurationParameter specified by parameter.
		/// @param parameter Specifies the parameter that should be set. The parameters can be found here:
		/// https://www.pololu.com/docs/0J29/5.a
		/// @param value The value that the configurationParameter should be set to.
		void setConfigurationParameter(const unsigned char& parameter, const unsigned char& value);
		
		/// @brief
		/// Returns the raw reading from motor M0 that indicates how much current flows through the motor at average over the last 5ms.
		/// This reading is raw, so not converted to milliampere.
		unsigned char getM0Current();
		/// @brief
		/// Returns the raw reading from motor M1 that indicates how much current flows through the motor at average over the last 5ms.
		/// This reading is raw, so not converted to milliampere.
		unsigned char getM1Current();
		/// @brief
		/// This function uses the raw reading from motor M0 to estimate how much current flows through the motor in milliampere.
		/// Note that the value returned by this function can differ from the actual current by as much as 20%.
		unsigned int getM0CurrentMilliamps();
		/// @brief
		/// This function uses the raw reading from motor M1 to estimate how much current flows through the motor in milliampere.
		/// Note that the value returned by this function can differ from the actual current by as much as 20%.
		unsigned int getM1CurrentMilliamps();
		
    };
} // namespace r2d2::moving_platform
