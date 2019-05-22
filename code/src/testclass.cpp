#include <testclass.hpp>


    beetlle_c::beetlle_c(r2d2::moving_platform::qik_2s12v10_c *qik_2s12v10_motorcontroller):
             qik_2s12v10_motorcontroller(qik_2s12v10_motorcontroller),
             speed(0)
             
    {
        qik_2s12v10_motorcontroller->init();
        qik_2s12v10_motorcontroller->get_configuration_parameter(1);
    }

    void beetlle_c::set_speed(const int8_t &new_speed){
        // Todo: depending on the motor configuration, 127 may not be the
        // maximum value
        // Todo: change the speed variable to the actual speed.
        hwlib::cout << "set speed beetlle \n";
        speed = new_speed;
        if(speed > 100){
            speed = 100;
        } else if (speed <-100){
            speed = -100;
        } else if (speed < 10 && speed > -10){
            speed = 0;
            qik_2s12v10_motorcontroller->set_m0_speed(0);
            qik_2s12v10_motorcontroller->set_m1_speed(0);
        }
        //if(speed >=-100 && speed <=100){
            //qik_2s12v10_motorcontroller.set_m0_speed(128*speed/100);
            //qik_2s12v10_motorcontroller.set_m1_speed(-(128*speed/100));
        //}
    }

    void beetlle_c::set_steering(const int16_t &degrees){

    }
    void beetlle_c::turn(const int16_t &degrees){
        int new_degrees = degrees;
        if(new_degrees < 15 && new_degrees > -15){
            new_degrees = 0;
            
        }
        
        if(speed != 0){
            if(new_degrees >=0 ){
                // turn left
                qik_2s12v10_motorcontroller->set_m0_speed((speed+new_degrees/4)/3);
                qik_2s12v10_motorcontroller->set_m1_speed(-(speed+new_degrees/4));
                //hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetlle is 222 sec and needs start up 
            } else{
                // turn right
                qik_2s12v10_motorcontroller->set_m0_speed((speed+new_degrees/4));
                qik_2s12v10_motorcontroller->set_m1_speed(-(speed+new_degrees/4)/3);
                //hwlib::wait_ms((2222*(-degrees)/360) + 60);
            }
        }
        
    }
    void beetlle_c::move (){
        int new_degrees = degrees;
        if(new_degrees < 15 && new_degrees > -15){
            new_degrees = 0;
            
        }
        
        if(speed != 0){
            if(new_degrees >=0 ){
                // turn left
                qik_2s12v10_motorcontroller->set_m0_speed((speed+new_degrees/4)/3);
                qik_2s12v10_motorcontroller->set_m1_speed(-(speed+new_degrees/4));
                //hwlib::wait_ms(((2222*degrees)/360) + 60);  // one round for beetlle is 222 sec and needs start up 
            } else{
                // turn right
                qik_2s12v10_motorcontroller->set_m0_speed((speed+new_degrees/4));
                qik_2s12v10_motorcontroller->set_m1_speed(-(speed+new_degrees/4)/3);
                //hwlib::wait_ms((2222*(-degrees)/360) + 60);
            }
        }

    }
    void beetlle_c::move (const int8_t &x, const int8_t &y){

    }
