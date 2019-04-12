#include "hwlib.hpp"
#include "Qik2s12v10.hpp"

namespace r2d2::moving_platform {

	Qik2s12v10::Qik2s12v10(hwlib::pin_in& _receivePin, hwlib::pin_out& _transmitPin, hwlib::pin_out& _resetPin):
		receivePin(_receivePin),
		transmitPin(_transmitPin),
		resetPin(_resetPin){}
	
	void Qik2s12v10::set_speed(const int8_t &_speed){
		
	}
	void Qik2s12v10::setM0Speed(const int8_t &_speed){
		
	}
	void Qik2s12v10::setM1Speed(const int8_t &_speed){
		
	}
	
	void Qik2s12v10::init(const long& baudRate){
		
	}
	void Qik2s12v10::setM0Break(const unsigned char& brake){
		
	}
	void Qik2s12v10::setM1Break(const unsigned char& brake){
		
	}
	void Qik2s12v10::setBreaks(const unsigned char& brake){
		
	}
	
	unsigned char Qik2s12v10::getError(){
		return 0;
	}
	unsigned char Qik2s12v10::getConfigurationParameter(const unsigned char& parameter){
		return 0;
	}
	void Qik2s12v10::setConfigurationParameter(const unsigned char& parameter, const unsigned char& value){
		
	}
	
	unsigned char Qik2s12v10::getM0Current(){
		return 0;
	}
	unsigned char Qik2s12v10::getM1Current(){
		return 0;
	}
	unsigned int Qik2s12v10::getM0CurrentMilliamps(){
		return 0;
	}
	unsigned int Qik2s12v10::getM1CurrentMilliamps(){
		return 0;
	}
	
} // namespace r2d2::moving_platform