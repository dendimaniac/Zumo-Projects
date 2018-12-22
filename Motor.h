/**
 * @file    Motor.h
 * @brief   Motor header file
 * @details If you want to use Motor methods, Include Motor.h file.
*/
#ifndef MOTOR_H_ 
#define MOTOR_H_ 

#include "project.h"

void motor_start(); // start motor PWM timers
void motor_stop();  // stop motor PWM timers

/* moving forward */
void motor_forward(uint8 speedL, uint8 speedR, uint32 delay);

void motor_turn(uint8 l_speed, uint8 r_speed, uint32 delay);

/* moving left when l_speed < r_speed or moving right when l_speed > r_speed */
void maze_turn(int speedL, int speedR, uint32 delay);

/* moving backward */
void motor_backward(uint8 speedL, uint8 speedR, uint32 delay);

/* instantly turn left */
void motor_instant_left(uint8 speedL, uint8 speedR, uint32 delay);

/* instantly turn right */
void motor_instant_right(uint8 speedL, uint8 speedR, uint32 delay);

#endif