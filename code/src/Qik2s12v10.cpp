#include "hwlib.hpp"
#include "Qik2s12v10.hpp"
#include <hardware_usart.hpp>

namespace r2d2::moving_platform {

	const uint8_t Qik2s12v10::qikAutodetectBaudRate = 0xAA;
	const uint8_t Qik2s12v10::qikRequestFirmwareversion = 0x81;
	const uint8_t Qik2s12v10::qikMotorM0SetForward = 0x88;
	const uint8_t Qik2s12v10::qikMotorM0SetReverse = 0x8A;
	const uint8_t Qik2s12v10::qikMotorM1SetForward = 0x8C;
	const uint8_t Qik2s12v10::qikMotorM1SetReverse = 0x8E;

	Qik2s12v10::Qik2s12v10(r2d2::uart_ports_c uart_port, unsigned int baudRate, hwlib::pin_out* _resetPin):
		resetPin(_resetPin),
		usart_bus(baudRate, uart_port){}

	void Qik2s12v10::set_speed(const int8_t &_speed){
		// todo: change the speed variable to the actual speed
		uint8_t speedByte = 0;
		if(_speed >= 0){
			speedByte = _speed;
			usart_bus << qikMotorM0SetForward << speedByte;
			usart_bus << qikMotorM1SetForward << speedByte;
		}else{
			speedByte = _speed + 127;
			usart_bus << qikMotorM0SetReverse << speedByte;
			usart_bus << qikMotorM1SetReverse << speedByte;
		}
	}
	void Qik2s12v10::setM0Speed(const int8_t &_speed){
		uint8_t speedByte = 0;
		if(_speed >= 0){
			speedByte = _speed;
			usart_bus << qikMotorM0SetForward << speedByte;
		}else{
			speedByte = _speed + 127;
			usart_bus << qikMotorM0SetReverse << speedByte;
		}
	}
	void Qik2s12v10::setM1Speed(const int8_t &_speed){

	}

	void Qik2s12v10::init(){
		// reset the qik 2s12v10
		resetPin->write(0);
		hwlib::wait_ms(1);
		resetPin->write(1);
		hwlib::wait_ms(10);

		// setup the uart communication
		usart_bus << qikAutodetectBaudRate;

		// for testing
		char tmp;
		while(usart_bus.available()){tmp = usart_bus.receive();} // clear the buffer
		hwlib::cout << "requesting firmware version\n";
		hwlib::wait_ms(1);
		usart_bus << qikRequestFirmwareversion; // send request
		hwlib::wait_ms(1);
		hwlib::cout << "waiting for firmware version\n";
		while(!usart_bus.available()){} // wait for answer
		hwlib::cout << "firmware version:";
		hwlib::cout << usart_bus.receive() << '\n'; // print answer

	}
	void Qik2s12v10::setM0Brake(const uint8_t& brake){

	}
	void Qik2s12v10::setM1Brake(const uint8_t& brake){

	}
	void Qik2s12v10::setBrakes(const uint8_t& brake){

	}

	uint8_t Qik2s12v10::getError(){
		return 0;
	}
	uint8_t Qik2s12v10::getConfigurationParameter(const uint8_t& parameter){
		return 0;
	}
	void Qik2s12v10::setConfigurationParameter(const uint8_t& parameter, const uint8_t& value){

	}

	uint8_t Qik2s12v10::getM0Current(){
		return 0;
	}
	uint8_t Qik2s12v10::getM1Current(){
		return 0;
	}
	unsigned int Qik2s12v10::getM0CurrentMilliamps(){
		return 0;
	}
	unsigned int Qik2s12v10::getM1CurrentMilliamps(){
		return 0;
	}

} // namespace r2d2::moving_platform