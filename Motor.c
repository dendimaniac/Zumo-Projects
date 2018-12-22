/**
 * @file    Motor.c
 * @brief   Basic methods for operating motor sensor. For more details, please refer to Motor.h file. 
 * @details included in Zumo shield
*/
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"


/**
* @brief    Starting motor sensors
* @details  
*/
void motor_start()
{
    PWM_Start();
}


/**
* @brief    Stopping motor sensors
* @details
*/
void motor_stop()
{
    PWM_Stop();
}


/**
* @brief    Moving motors forward
* @details  giveing same speed to each side of PWM to make motors go forward
* @param    uint8 speed : speed value
* @param    uint32 delay : delay time
*/
void motor_forward(uint8 speedL, uint8 speedR, uint32 delay)
{
    MotorDirLeft_Write(0);      // set LeftMotor forward mode
    MotorDirRight_Write(0);     // set RightMotor forward mode
    PWM_WriteCompare1(speedL); 
    PWM_WriteCompare2(speedR);
    vTaskDelay(delay);
}


/**
* @brief    Moving motors to make a turn 
* @details  moving left when l_speed < r_speed or moving right when l_speed > r_speed
* @param    uint8 l_speed : left motor speed value
* @param    uint8 r_speed : right motor speed value
* @param    uint32 delay : delay time
*/
void motor_turn(uint8 l_speed, uint8 r_speed, uint32 delay)
{
    PWM_WriteCompare1(l_speed); 
    PWM_WriteCompare2(r_speed); 
    vTaskDelay(delay);
}

void maze_turn(int speedL, int speedR, uint32 delay) {
    if (speedL < 0) {
        MotorDirLeft_Write(1);
        speedL = -speedL;
    }
    if (speedR<0) {
        MotorDirRight_Write(1);
        speedR = -speedR;
    }
    PWM_WriteCompare1(speedL); 
    PWM_WriteCompare2(speedR); 
    vTaskDelay(delay);
}

void motor_instant_left(uint8 speedL, uint8 speedR, uint32 delay)
{
    MotorDirLeft_Write(1);      // set LeftMotor backward mode
    MotorDirRight_Write(0);     // set RightMotor backward mode
    PWM_WriteCompare1(speedL); 
    PWM_WriteCompare2(speedR); 
    vTaskDelay(delay);
}

void motor_instant_right(uint8 speedL, uint8 speedR, uint32 delay)
{
    MotorDirLeft_Write(0);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor backward mode
    PWM_WriteCompare1(speedL); 
    PWM_WriteCompare2(speedR); 
    vTaskDelay(delay);
}

/**
* @brief    Moving motors backward
* @details  setting backward mode to each motors and gives same speed to each side of PWM
* @param    uint8 speed : speed value
* @param    uint32 delay : delay time
*/
void motor_backward(uint8 speedL, uint8 speedR, uint32 delay)
{
    MotorDirLeft_Write(1);      // set LeftMotor backward mode
    MotorDirRight_Write(1);     // set RightMotor backward mode
    PWM_WriteCompare1(speedL); 
    PWM_WriteCompare2(speedR); 
    vTaskDelay(delay);
}
