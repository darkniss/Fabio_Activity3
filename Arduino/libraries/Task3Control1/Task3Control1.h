#ifndef TASK3CONTROL1_h
#define TASK3CONTROL1_h


//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\Task3Sense1\Task3Sense1.h"
#include "..\basic_speed_PID\basic_speed_PID.h"
#else
#include <Arduino.h>
#include <Task3Sense1.h>
#include <basic_speed_PID.h>
#endif
//Have included libraries above which include classes of components we will use. (Might not need all libraries, can uncomment if they are needed)
//The following library implements the control unit of the system.


class control{
	protected:
        static const unsigned long default_check_interval = 500;
        unsigned long check_interval;
        unsigned long last_check_time;
        basic_speed_PID PID_controller;
        int mapped_pwm;
	public:
		control()               //default constructor
		{
			check_interval = default_check_interval;
			last_check_time = 0;
            mapped_pwm = 0;
		}
    
    
        control(unsigned long in_check_interval, double ref_kp, double ref_ki, double ref_kd, int ref_control_interval_time)        //constructor with arguments
        {
            setup_controller(in_check_interval, ref_kp, ref_ki, ref_kd, ref_control_interval_time);
            last_check_time=0;
            mapped_pwm = 0;
        }
    
    
        void setup_controller(unsigned long in_check_interval, double ref_kp, double ref_ki, double ref_kd, int ref_control_interval_time)     //set up with arguments
        {
            check_interval = in_check_interval;
            PID_controller.set_gainvals(ref_kp, ref_ki, ref_kd);
            PID_controller.set_ref_control_interval_ms(ref_control_interval_time);
        }
    
    
        unsigned long getInterCheck()             //returns set check interval
        {
            return check_interval;
        }
		
    
        boolean isTimeToTakeMeasurement()      //verifies if it's time to take a new input measurement, and updates check time if so
        {
            
            unsigned long current_time = millis();        //check current time using internal clock
        
            if(current_time<last_check_time)
                updateCheckTime();
        
            if((current_time - last_check_time) >= check_interval){
                return true;
            }
            else
                return false;
        }
    
    
		boolean isTimeToTakeMeasurementAndUpdate()      //verifies if it's time to take a new input measurement, and updates check time if so
		{
			unsigned long current_time = millis();	    //check current time using internal clock
            
            if(current_time<last_check_time)
                updateCheckTime();
            
            if((current_time - last_check_time) >= check_interval){
                updateCheckTime();
				return true;
            }
			else
				return false;
		}
    
    
        void updateCheckTime()              //stores the most recent check time
        {
            last_check_time = millis();
        }
    
    
        int return_PID_output(double target_speed, double current_speed){   //target speed = reference speed, current speed = current RPM
            return PID_controller.ComputePID_output(target_speed, current_speed);
        }
        
        void resetPID(){ //function to reset the PID controller
        	PID_controller.reset_pidcontrol();
        }
    
};
#endif
