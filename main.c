/**
* @mainpage ZumoBot Project
* @brief    You can make your own ZumoBot with various sensors.
* @details  <br><br>
    <p>
    <B>General</B><br>
    You will use Pololu Zumo Shields for your robot project with CY8CKIT-059(PSoC 5LP) from Cypress semiconductor.This 
    library has basic methods of various sensors and communications so that you can make what you want with them. <br> 
    <br><br>
    </p>
    
    <p>
    <B>Sensors</B><br>
    &nbsp;Included: <br>
        &nbsp;&nbsp;&nbsp;&nbsp;LSM303D: Accelerometer & Magnetometer<br>
        &nbsp;&nbsp;&nbsp;&nbsp;L3GD20H: Gyroscope<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Reflectance sensor<br>
        &nbsp;&nbsp;&nbsp;&nbsp;Motors
    &nbsp;Wii nunchuck<br>
    &nbsp;TSOP-2236: IR Receiver<br>
    &nbsp;HC-SR04: Ultrasonic sensor<br>
    &nbsp;APDS-9301: Ambient light sensor<br>
    &nbsp;IR LED <br><br><br>
    </p>
    
    <p>
    <B>Communication</B><br>
    I2C, UART, Serial<br>
    </p>
*/
#define zumoMqtt "Zumo024/lap"
#include <stdlib.h>
#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
/**
 * @file    main.c
 * @brief   
 * @details  ** Enable global interrupt since Zumo library uses interrupts. **<br>&nbsp;&nbsp;&nbsp;CyGlobalIntEnable;<br>
*/

#if 0
// Hello World!
void zmain(void)
{
    printf("\nHello, World!\n");

    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
 }   
#endif

#if 0
// Name and age
void zmain(void)
{
    char name[32];
    int age;
    
    
    printf("\n\n");
    
    printf("Enter your name: ");
    //fflush(stdout);
    scanf("%s", name);
    printf("Enter your age: ");
    //fflush(stdout);
    scanf("%d", &age);
    
    printf("You are [%s], age = %d\n", name, age);

    while(true)
    {
        BatteryLed_Write(!SW1_Read());
        vTaskDelay(100);
    }
 }   
#endif


#if 0
//battery level//
void zmain(void)
{
    ADC_Battery_Start();        

    int16 adcresult =0;
    float volts = 0.0;

    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed

    for(;;)
    {
        char msg[80];
        ADC_Battery_StartConvert(); // start sampling
        if(ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT)) {   // wait for ADC converted value
            adcresult = ADC_Battery_GetResult16(); // get the ADC value (0 - 4095)
            // convert value to Volts
            // you need to implement the conversion
            
            // Print both ADC results and converted value
            printf("%d %f\r\n",adcresult, volts);
        }
        vTaskDelay(500);
    }
 }   
#endif

#if 0
// button
void zmain(void)
{
    while(1) {
        printf("Press button within 5 seconds!\n");
        int i = 50;
        while(i > 0) {
            if(SW1_Read() == 0) {
                break;
            }
            vTaskDelay(100);
            --i;
        }
        if(i > 0) {
            printf("Good work\n");
            while(SW1_Read() == 0) vTaskDelay(10); // wait until button is released
        }
        else {
            printf("You didn't press the button\n");
        }
    }
}
#endif

#if 0
// button
void zmain(void)
{
    printf("\nBoot\n");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 
    
    //uint8 button;
    //button = SW1_Read(); // read SW1 on pSoC board
    // SW1_Read() returns zero when button is pressed
    // SW1_Read() returns one when button is not pressed
    
    bool led = false;
    
    for(;;)
    {
        // toggle led state when button is pressed
        if(SW1_Read() == 0) {
            led = !led;
            BatteryLed_Write(led);
            if(led) printf("Led is ON\n");
            else printf("Led is OFF\n");
            Beep(1000, 150);
            while(SW1_Read() == 0) vTaskDelay(10); // wait while button is being pressed
        }        
    }
 }   
#endif

#if 0
//ultrasonic sensor//
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    
    while(1) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\n", d);
        vTaskDelay(200);
    }
}   
#endif

#if 0
//IR receiverm - how to wait for IR remote commands
void zmain(void)
{
    IR_Start();
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    bool led = false;
    // Toggle led when IR signal is received
    for(;;)
    {
        IR_wait();  // wait for IR command
        led = !led;
        BatteryLed_Write(led);
        if(led) printf("Led is ON\n");
        else printf("Led is OFF\n");
    }    
 }   
#endif

#if 0
//IR receiver - read raw data
void zmain(void)
{
    IR_Start();
    
    uint32_t IR_val; 
    
    printf("\n\nIR test\n");
    
    IR_flush(); // clear IR receive buffer
    printf("Buffer cleared\n");
    
    // print received IR pulses and their lengths
    for(;;)
    {
        if(IR_get(&IR_val, portMAX_DELAY)) {
            int l = IR_val & IR_SIGNAL_MASK; // get pulse length
            int b = 0;
            if((IR_val & IR_SIGNAL_HIGH) != 0) b = 1; // get pulse state (0/1)
            printf("%d %d\r\n",b, l);
        }
    }    
 }   
#endif

#if 0
//reflectance
void zmain(void)
{
    struct sensors_ ref;
    struct sensors_ dig;

    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    

    for(;;)
    {
        // read raw sensor values
        reflectance_read(&ref);
        // print out each period of reflectance sensors
        printf("%5d %5d %5d %5d %5d %5d\r\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
        
        // read digital values that are based on threshold. 0 = white, 1 = black
        // when blackness value is over threshold the sensors reads 1, otherwise 0
        reflectance_digital(&dig); 
        //print out 0 or 1 according to results of reflectance period
        printf("%5d %5d %5d %5d %5d %5d \r\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);        
        
        vTaskDelay(50);
    }
}   
#endif

#if 0
//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(255,255,5000);     // moving forward
    motor_turn(200,0,10000);     // turn
    motor_turn(0,200,10000);     // turn
    motor_backward(255,255,5000);    // moving backward
     
    motor_forward(0,0,0);         // stop motors

    motor_stop();               // disable motor controller
    
    for(;;)
    {

    }
}
#endif

#if 0
/* Example of how to use the Accelerometer!!!*/
void zmain(void)
{
    struct accData_ data;
    /*
    motor_start();
    motor_forward(0,0,0);
    */
    
    printf("Accelerometer test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    for(;;)
    {
        //motor_forward(50,50,50);
        LSM303D_Read_Acc(&data);
        printf("%8d %8d %8d\n",data.accX, data.accY, data.accZ);
        vTaskDelay(50);
    }
 }   
#endif    

#if 0
// MQTT test
void zmain(void)
{
    int ctr = 0;

    printf("\nBoot\n");
    send_mqtt(zumoMqtt, "Boot");

    //BatteryLed_Write(1); // Switch led on 
    BatteryLed_Write(0); // Switch led off 

    for(;;)
    {
        printf("Ctr: %d, Button: %d\n", ctr, SW1_Read());
        print_mqtt(zumoMqtt, "Ctr: %d, Button: %d", ctr, SW1_Read());

        vTaskDelay(1000);
        ctr++;
    }
 }
#endif

#if 0
void zmain(void)
{    
    struct accData_ data;
    struct sensors_ ref;
    struct sensors_ dig;
    
    printf("MQTT and sensor test...\n");

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Accelerometer Ok...\n");
    }
    
    int ctr = 0;
    reflectance_start();
    for(;;)
    {
        LSM303D_Read_Acc(&data);
        // send data when we detect a hit and at 10 second intervals
        if(data.accX > 1500 || ++ctr > 1000) {
            printf("Acc: %8d %8d %8d\n",data.accX, data.accY, data.accZ);
            print_mqtt("Zumo01/acc", "%d,%d,%d", data.accX, data.accY, data.accZ);
            reflectance_read(&ref);
            printf("Ref: %8d %8d %8d %8d %8d %8d\n", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);       
            print_mqtt("Zumo01/ref", "%d,%d,%d,%d,%d,%d", ref.l3, ref.l2, ref.l1, ref.r1, ref.r2, ref.r3);
            reflectance_digital(&dig);
            printf("Dig: %8d %8d %8d %8d %8d %8d\n", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            print_mqtt("Zumo01/dig", "%d,%d,%d,%d,%d,%d", dig.l3, dig.l2, dig.l1, dig.r1, dig.r2, dig.r3);
            ctr = 0;
        }
        vTaskDelay(10);
    }
 }   

#endif

#if 0
void zmain(void)
{    
    RTC_Start(); // start real time clock
    
    RTC_TIME_DATE now;

    // set current time
    now.Hour = 12;
    now.Min = 34;
    now.Sec = 56;
    now.DayOfMonth = 25;
    now.Month = 9;
    now.Year = 2018;
    RTC_WriteTime(&now); // write the time to real time clock

    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            printf("%2d:%02d.%02d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
 }   
#endif

//Week 2//

//Assignment 1//

#if 0
void zmain(void)
{
    uint8_t state = 1;
    bool isPressed = 1;
    int time = 500;
    for (;;) {
        isPressed = SW1_Read();
        if(isPressed == 0) {
            for (int i = 1; i <= 18; i++) {
                if (i == 7 || i == 9 || i == 11) {
                    time = 1500;
                }
                else {
                    time = 500;
                }
                BatteryLed_Write(state);
                vTaskDelay(time);
                state = ~state;
            }
        }
    }
}
#endif

//Assignment 2//

#if 0
void zmain(void) {  
    int timeDiff, age;
    printf("Enter your age: ");
    TickType_t timeStart;
    timeStart = xTaskGetTickCount();
    scanf("%d", &age);
    TickType_t timeEnd;
    timeEnd = xTaskGetTickCount();
    timeDiff = timeEnd - timeStart;
    if (age <= 21) {
        if (timeDiff < 3000) {
            printf("Super fast dude!");
        }
        else if ((timeDiff >= 3000) && (timeDiff <= 5000)) {
            printf("So mediocre.");
        }
        else {
            printf("My granny is faster than you!");
        }
    }
    else if ((age >= 22) && (age <= 50)) {
        if (timeDiff < 3000) {
            printf("Be quick or be dead");
        }
        else if ((timeDiff >= 3000) && (timeDiff <= 5000)) {
            printf("You're so average.");
        }
        else {
            printf("Have you been smoking something illegal?");
        }
    }
    else {
        if (timeDiff < 3000) {
            printf("Still going strong");
        }
        else if ((timeDiff >= 3000) && (timeDiff <= 5000)) {
            printf("You are doing ok for your age.");
        }
        else {
            printf("Do they still allow you to drive?");
        }
    }
}
#endif

//Assignment3//

#if 0
void zmain(void) {
    ADC_Battery_Start();
    
    double returnedValue = 0;
    double voltage = 0;
    bool state = 1;
    bool isPressed = 0;
    
    ADC_Battery_StartConvert();
    for (;;) {
        ADC_Battery_IsEndConversion(ADC_Battery_WAIT_FOR_RESULT);
        returnedValue = ADC_Battery_GetResult16();
        voltage = ((returnedValue * 5)/4095) * 1.541;
        if (voltage >= 4) {
            if (SW1_Read() == 0) {
                BatteryLed_Write(0);
                isPressed = 0;
                continue;
            }
            if (isPressed == 0) {
                continue;
            }
        }
        BatteryLed_Write(state);
        vTaskDelay(500);
        state = !state;
        isPressed = 1;
    }
}
#endif

//Week 3//

//Assignment 1//

#if 0
void zmain(void) {
    motor_start();              // enable motor controller
    motor_forward(0,0,0);         // set speed to zero to stop motors

    vTaskDelay(3000);
    
    motor_forward(195,185,1900);
    motor_turn(195,0,470);
    motor_forward(195,175,1630);
    motor_turn(195,0,470);
    motor_forward(195,175,1700);
    motor_turn(130,0,1000);
    motor_forward(178,140,1100);
    motor_turn(130,0,450);
    motor_forward(178,170,1100);
    
    motor_forward(0,0,0);
    motor_stop();
}
#endif

//Assignment 2//

#if 0
void zmain(void) {
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();                          // enable motor controller
    motor_forward(0,0,0); 
    int d = 0;
    
    while(1) {
        d = Ultra_GetDistance();
        if (d <= 10) {
            motor_forward(0,0,0);
            Beep(500,150);
            motor_backward(50,255,425);
            continue;
        }
        motor_forward(255,240,1);
    }
}
#endif

//Assignment 3//

#if 0
void zmain(void) {
    struct accData_ data;
    int currentValueX = 0, newValueX = 0, diffX = 0;
    uint8_t dir = 0;
    
    motor_start();
    motor_forward(0,0,0);

    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    for(;;)
    {
        motor_backward(255,255,1);
        LSM303D_Read_Acc(&data);
        currentValueX = data.accX;
        LSM303D_Read_Acc(&data);
        newValueX = data.accX;
        diffX = currentValueX - newValueX;
        print_mqtt(zumoMqtt,"%d\n", diffX);
        if (diffX >= 7000) {
            dir = rand() % 2;
            motor_forward(0,0,0);
            if (dir == 0) {
                motor_backward(50,255,325);
            }
            else {
                motor_backward(255,50,325);
            }
        }
    }
}
#endif

//Week 4//

//Assignment 1//

#if 0
void zmain(void) {
    struct sensors_ ref;
    struct sensors_ dig;
    
    int count = 0;
    bool isWhite = false;
    bool isPressed = true;

    motor_start();
    motor_forward(0,0,0);
    
    IR_Start();
    IR_flush();
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    
    for(;;)
    {
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(2000);
        }
        if(isPressed == false) {
            reflectance_read(&ref);      
            reflectance_digital(&dig);
            
            if (ref.l1 >= 11000 && ref.r1 >= 11000) {
                motor_forward(130,115,0);
            }
            else if (ref.l1 < 11000 && ref.r1 >= 11000) {
                motor_forward(130,90,0);
            }
            else if (ref.l1 >= 11000 && ref.r1 < 11000) {
                motor_forward(90,130,0);
            }
            
            if (isWhite == true && (dig.l3 == 1 && dig.r3 == 1)) {
                isWhite = false;
                count++;
            }
            else if (dig.l3 == 0 && dig.r3 == 0) {
                isWhite = true;
            }
            if (count == 1) {
                motor_forward(0,0,0);
                IR_wait();
                count++;
            }
            else if (count == 5) {
                motor_forward(0,0,0);
                break;
            }
        }
    }
}
#endif

//Assignment 2//

#if 0
void zmain(void) {
    struct sensors_ ref;
    struct sensors_ dig;
    
    int count = 0;
    bool isWhite = true;
    bool isPressed = true;

    motor_start();
    motor_forward(0,0,0);
    
    IR_Start();
    IR_flush();
    
    reflectance_start();
    reflectance_set_threshold(20000, 9000, 11000, 11000, 9000, 20000);
    
    for(;;)
    {
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(1000);
        }
        if(isPressed == false) {
            reflectance_read(&ref);            
            reflectance_digital(&dig);
            if (ref.l1 >= 18000 && ref.r1 >= 18000) {
                motor_forward(100,100,0);
            }
            else if (ref.l1 < 18000 && ref.r1 >= 18000) {
                motor_forward(100,20,0);
            }
            else if (ref.l1 >= 18000 && ref.r1 < 18000) {
                motor_forward(20,100,0);
            }
            else if (ref.l3 <= 7000 && ref.r3 <= 7000) {
                if (count == 3) {
                    if (ref.l1 < 23000 && ref.r1 < 23000)
                    {
                        motor_instant_left(255,255,0);
                    }
                }
                else if (count == 4 || count == 5) {
                    if (ref.l1 < 23000 && ref.r1 < 23000) {
                        motor_instant_right(150,150,0);
                    }
                }
            }
            
            if (isWhite == true && (dig.r3 == 1 && dig.l3 == 1)) {
                isWhite = false;
                count++;
            }
            else if (dig.l3 == 0 && dig.r3 == 0) {
                isWhite = true;
            }
            if (count == 1) {
                motor_forward(0,0,0);
                IR_wait();
                count++;
            }
            else if (count == 6) {
                motor_forward(0,0,0);
                break;
            }
        }
    }
}
#endif

//Assignment 3//

#if 0
void zmain(void) {
    struct sensors_ ref;
    struct sensors_ dig;
    
    int count = 0;
    bool isWhite = true;
    bool isPressed = true;

    motor_start();
    motor_forward(0,0,0);
    
    IR_Start();
    IR_flush();
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    
    for(;;)
    {
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(2000);
        }
        if(isPressed == false) {
            reflectance_read(&ref);       
            reflectance_digital(&dig);
            
            // Follow the line //
            
            if (ref.l1 >= 23000 && ref.r1 >= 23000) {
                motor_forward(255,255,0);
            }
            else if (ref.l1 > ref.r1) {
                motor_forward(0,255,0);
            }
            else if (ref.r1 > ref.l1) {
                motor_forward(255,0,0);
            }
            
            // Start and Stop on lines //
            
            if (isWhite == true && dig.l3 == 1 && dig.r3 == 1) {
                isWhite = false;
                count++;
            }
            if (dig.l3 == 0 && dig.r3 == 0) {
                isWhite = true;
            }
            if (count == 1) {
                motor_forward(0,0,0);
                IR_wait();
                count++;
            }
            if (count == 3) {
                motor_forward(0,0,0);
                break;
            }
        }
    }
}
#endif

// Week 5 //

// Assignment 1 //

#if 0
void zmain(void) {
    int hour = 0, minute = 0;
    
    RTC_Start();
    RTC_TIME_DATE now;
    
    send_mqtt(zumoMqtt, "Boot");

    printf("\nEnter the current hour: \n");
    scanf("%d", &hour);
    printf("Enter the current minute: \n");
    scanf("%d", &minute);
    now.Hour = hour;
    now.Min = minute;
    now.Sec = 0;
    RTC_WriteTime(&now);
    
    for(;;)
    {
        if(SW1_Read() == 0) {
            // read the current time
            RTC_DisableInt(); /* Disable Interrupt of RTC Component */
            now = *RTC_ReadTime(); /* copy the current time to a local variable */
            RTC_EnableInt(); /* Enable Interrupt of RTC Component */

            // print the current time
            print_mqtt(zumoMqtt, "%d:%d.%d\n", now.Hour, now.Min, now.Sec);
            
            // wait until button is released
            while(SW1_Read() == 0) vTaskDelay(50);
        }
        vTaskDelay(50);
    }
}
#endif

// Assignment 2 //

#if 0
void zmain(void) {
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();                          // enable motor controller
    motor_forward(0,0,0); 
    int d = 0, dir = 0;
    bool isPressed = true;
    
    send_mqtt(zumoMqtt, "Boot");
    
    while(1) {
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(500);
        }
        
        if(isPressed == false) {
            d = Ultra_GetDistance();
            if (d <= 15) {
                dir = rand() % 2;
                motor_forward(0,0,0);
                if (dir == 0) {
                    print_mqtt(zumoMqtt, "Right\n");
                    motor_backward(50,255,325);
                }
                else {
                    print_mqtt(zumoMqtt, "Left\n");
                    motor_backward(255,50,325);
                }
                continue;
            }
            motor_forward(255,255,0);
        }
    }
}
#endif

// Assignment 3 //

#if 0
void zmain(void) {
    struct sensors_ dig;
    bool isPressed = true;
    bool white=0;
    TickType_t timeStart = 0;
    TickType_t timeEnd = 0;
    int timeDiff;
    int count=0;
    motor_start();
    motor_forward(0,0,0);
    
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);
    
    IR_Start();
    IR_flush();
    
    send_mqtt(zumoMqtt, "Boot\n");
    
    for(;;){
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(500);
        }
        
        if(isPressed == false) {
            reflectance_digital(&dig);
            motor_forward(100,100,0);

            if (white==1 && (dig.l3==1 && dig.r3==1)) {
                white=0;
                count++;
                timeEnd = xTaskGetTickCount();
                timeDiff = timeEnd - timeStart;
                print_mqtt(zumoMqtt, "%d\n", timeDiff);
                motor_forward(0,0,0);
                IR_wait();
                timeStart = xTaskGetTickCount();
            }
            
            if (dig.l3==0 && dig.r3==0){
                white=1;
            }
            if (count==0){
                timeStart = xTaskGetTickCount();
            }            
        }
    }
}
#endif

// Follow the line final project //

#if 0
void zmain(void) {
    struct sensors_ ref;
    struct sensors_ dig;
    
    TickType_t timeIR = 0, timeEnd = 0, timeBackLine = 0,timeOutLine = 0;
    
    int count = 0,timeDiff = 0;
    
    bool isWhite = true;
    bool isPressed = true;
    bool onLine= 1;
    
    motor_start();
    motor_forward(0,0,0);
    
    IR_Start();
    IR_flush();
    
    reflectance_start();
    reflectance_set_threshold(18000, 18000, 11000, 11000, 18000, 18000);
    
    for(;;)
    {
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(1000);
        }
        else if(isPressed == false) {
            
            reflectance_read(&ref);       
            reflectance_digital(&dig);
            
            // Follow the line //
            
            if (ref.l1 >= 23000 && ref.r1 >= 23000) {
                motor_forward(255,255,0);
            }
            else if (ref.l1 < 23000 && ref.r1 >= 23000) {
                motor_forward(255,200,0);
            }
            else if (ref.l1 >= 23000 && ref.r1 < 23000) {
                motor_forward(200,255,0);
            }
            else if (ref.r2 >= 10000) {
                motor_forward(255,0,0);
            }
            else if (ref.l2 >= 10000) {
                motor_forward(0,255,0);
            }            
            
            // Start and Stop on lines //
            
            //Only checks for black line once after seeing white
            if (isWhite == true && dig.l3 == 1 && dig.r3 == 1 && dig.l2 == 1 && dig.r2 == 1) {
                isWhite = false;
                count++;
                if (count == 1) {
                    motor_forward(0,0,0);
                    print_mqtt("Zumo024/ready","line");
                    IR_wait();
                    timeIR = xTaskGetTickCount();
                    print_mqtt("Zumo024/start", "%d", timeIR);
                }
                //timeStart = xTaskGetTickCount();
                else if (count == 3) {
                    motor_forward(0,0,0);
                    timeEnd = xTaskGetTickCount();
                    print_mqtt("Zumo024/stop", "%d", timeEnd);
                    timeDiff = timeEnd - timeIR;
                    print_mqtt("Zumo024/time", "%d", timeDiff);
                    break;
                }    
            }
            else if (dig.l3 == 0 && dig.r3 == 0) {
                isWhite = true;
            }
            
            //Check and print whenever the robot is either online or offline.
            if (onLine==0 && dig.l1==1 && dig.r1==1){
                onLine=1;
                timeBackLine= xTaskGetTickCount();
                print_mqtt("Zumo024/line", "%d", timeBackLine);
            }
            else if (onLine == 1 && dig.l1==0 && dig.r1==0){
                onLine=0;
                timeOutLine= xTaskGetTickCount();
                print_mqtt("Zumo024/miss", "%d", timeOutLine);
            }
        }
    }
}
#endif

// Maze final project //

#if 1
void moveToMiddle(int c);

TickType_t timeIR = 0, timeEnd = 0;

int maze[13][9] = { {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},          //A preset layout of the grid
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},          //-1 are unmoveable locations
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1, 0, 0, 0, 0, 0, 0, 0,-1},
				 {-1,-1,-1,-1,-1,-1,-1,-1,-1} };
int row[3] = { 1,0,0 }; //The modification for direction when on rows
int col[3] = { 0,1,-1 };    //The modification for direction when on cols
int r = 0, c = 0, count = -1, timeDiff = 0;
bool check = false;

void followLine(struct sensors_ dig) {  //Makes robot follows the line
	int speed = 100, speedLarge = 100, speedSmall = -50;
	if (dig.l1 == 1 && dig.r1 == 1) {
		motor_forward(speed, speed, 15);
    }
	else if (dig.l1 == 1 && dig.r1 == 0) {
		maze_turn(speedSmall, speedLarge, 15);
    }
	else if (dig.l1 == 0 && dig.r1 == 1) {
		maze_turn(speedLarge, speedSmall, 15);
    }
}

void moveToInter(struct sensors_ dig, int c) {  //Runs robot to the next intersection
    //If the robot not on either left (c == 1) or right (c == 7) ends
	if ((c != 1) && (c != 7)) {
		while (!((dig.l3 == 1) && (dig.r3 == 1))) {
			reflectance_digital(&dig);
			followLine(dig);
		}
		while (!((dig.l3 == 0) && (dig.r3 == 0))) {
			reflectance_digital(&dig);
			followLine(dig);
		}
	}
	else {
		if (c == 7) {
			while (!(dig.l3 == 1)) {
				reflectance_digital(&dig);
				followLine(dig);
			}
			while (!(dig.l3 == 0)) {
				reflectance_digital(&dig);
				followLine(dig);
			}
		}
		else {
			while (!(dig.r3 == 1)) {
				reflectance_digital(&dig);
				followLine(dig);
			}
			while (!(dig.r3 == 0)) {
				reflectance_digital(&dig);
				followLine(dig);
			}
		}
	}
	motor_forward(0, 0, 0); //After reaching the intersection, stop
}


void chooseTurn(struct sensors_ dig, int direction, int left, int right)  {   //Chosing which direction to turn
	if (direction == 1) {
		do {
			reflectance_digital(&dig);
			maze_turn(left, -right, 5);
		} while ((dig.l1 == 1) || (dig.r1 == 1));
		do {
			reflectance_digital(&dig);
			maze_turn(left, -right, 15);
		} while (dig.l1 == 0);
		motor_forward(0, 0, 0);
	}
	else if (direction == 2) {
		do {
			reflectance_digital(&dig);
			maze_turn(-right, left, 5);
		} while ((dig.l1 == 1) || (dig.r1 == 1));
		do {
			reflectance_digital(&dig);
			maze_turn(-right, left, 15);
		} while (dig.r1 == 0);
		motor_forward(0, 0, 0);
	}
	while (!((dig.l3 == 0) && (dig.r3 == 0))) {
		reflectance_digital(&dig);
		followLine(dig);
	}
	reflectance_digital(&dig);
	followLine(dig);
	motor_backward(50, 50, 50);
	motor_forward(0, 0, 0);
}


void checkObstacle(int *a) {    //Check whether there is a obstacle
	int p = 0;
	p = Ultra_GetDistance();
	if ((p < 18) && (p > 0)) {
		*a = -1;
	}
}

void firstPart() {  //Move robot to (0,0)
	uint8 button = 1; struct sensors_ dig; bool condition = false; int count = 0;
	reflectance_start(); motor_start(); IR_Start(); IR_flush();

	motor_forward(0, 0, 0);
	do {
		button = SW1_Read();
		if (!button) {
            vTaskDelay(500);
			for (;;) {
				motor_forward(50, 50, 5);
				reflectance_digital(&dig);
				if ((dig.l3 == 1) && (dig.r3 == 1)) {
					motor_forward(0, 0, 0);
					condition = true;
					break;
				}
			}
		}
	} while (condition == false);

	condition = false;
    print_mqtt("Zumo024/ready","maze");
	IR_wait();
    timeIR = xTaskGetTickCount();
    print_mqtt("Zumo024/start", "%d", timeIR);
	condition = !condition;

	do {
		reflectance_digital(&dig);
		followLine(dig);
		if ((dig.l3 == 1) && (dig.r3 == 1) && (condition == false)) {
			condition = true;
		}
		if ((dig.l3 == 0) && (dig.r3 == 0) && (condition == true)) {
			condition = false;
			count++;
		}
	} while (count != 2);

	motor_forward(0, 0, 0);
}

void dfs(int r, int c, int direction) { //Find the path
	struct sensors_ dig; reflectance_start();
	if (check == true) return;

	maze[r][c] = -1;

	print_mqtt("Zumo024/position", "%d %d", r, c - 4);

	switch (direction) {
	case 0: //If the direction is forward, check the obstacle in that direction.
		checkObstacle(&maze[r + row[direction]][c + col[direction]]);
		break;

	case 1: //If the direction is left, check the obstacle in front, then turn right, check for obstacle again
		checkObstacle(&maze[r + row[direction]][c + col[direction]]);
		direction = 2; reflectance_digital(&dig); chooseTurn(dig, direction, 100, 75); direction = 0;
		checkObstacle(&maze[r + row[direction]][c + col[direction]]);
		break;

	case 2: //If the direction is right, check the obstacle in front, then turn left, check for obstacle again
		checkObstacle(&maze[r + row[direction]][c + col[direction]]);
		direction = 1; reflectance_digital(&dig); chooseTurn(dig, direction, 100, 75); direction = 0;
		checkObstacle(&maze[r + row[direction]][c + col[direction]]);
		break;
	}
	if (r == 11) {  //If on the last row, go to the middle of the row and finish the maze
		motor_forward(0, 0, 0); check = true; moveToMiddle(c);
		return;
	}

	for (int i = 0; i < 3; i++) {
        //If the next intersection is within the grid and the robot is not on the last line
		if ((r + row[i] >= 0) && (r + row[i] <= 11) && (c + col[i] >= 0) && (c + col[i] <= 8) && (maze[r + row[i]][c + col[i]] != -1) && (check == false)) {
			if (i != 0) {
				reflectance_digital(&dig);
				chooseTurn(dig, i, 100, 75);
				checkObstacle(&maze[r + row[i]][c + col[i]]);
                
                //If the next intersection is possible to move to, go to it, else turn right
				if (maze[r + row[i]][c + col[i]] != -1) {
					direction = i;
					reflectance_digital(&dig);
					moveToInter(dig, c);
					dfs(r + row[i], c + col[i], direction);
				}
				else chooseTurn(dig, 2, 100, 95);
			}
			else {
				direction = i;
				reflectance_digital(&dig);
				moveToInter(dig, c);
				dfs(r + row[i], c + col[i], direction);
			}
		}
	}
}

void moveToMiddle(int c) {  //Move robot to (13,0)
	struct sensors_ dig; reflectance_start();
	if (c != 4) {
		if (c < 4) {    //Move depending on whether the robot is on the left or right side of middle point
			reflectance_digital(&dig);
			chooseTurn(dig, 1, 100, 95);
			while (c != 4) {
				reflectance_digital(&dig);
				moveToInter(dig, c);
                c++;
                print_mqtt("Zumo024/position", "11 %d", c - 4);
			}
			chooseTurn(dig, 2, 100, 95);
		}
		else {
			reflectance_digital(&dig);
			chooseTurn(dig, 2, 100, 95);
			while (c != 4) {
				reflectance_digital(&dig);
				moveToInter(dig, c);
                c--;
                print_mqtt("Zumo024/position", "11 %d", c - 4);
			}
			chooseTurn(dig, 1, 100, 95);
		}
	}
}

void lastPart() {   //Move robot from (13,0) to the end
	motor_forward(0, 0, 0);
	struct sensors_ dig; bool black = false; int count = 0;
	reflectance_start();
    r=11;
	do {
		reflectance_digital(&dig);
		followLine(dig);
		if ((dig.l3 == 1) && (dig.r3 == 1) && (black == false)) {
			black = true;
			count++;
            r++;
            print_mqtt("Zumo024/position", "%d %d", r, c - 4);
		}
		if ((dig.l3 == 0) && (dig.r3 == 0) && (black == true))  {
			black = false;
		}
	} while (count != 2);
    
    motor_forward(100,100,1500);
	motor_forward(0, 0, 0);
    timeEnd = xTaskGetTickCount();
    print_mqtt("Zumo024/stop", "%d", timeEnd);
    timeDiff = timeEnd - timeIR;
    print_mqtt("Zumo024/time", "%d", timeDiff);
}

void zmain(void) {
	firstPart();
	Ultra_Start();

	r = 0, c = 4;
	dfs(r, c, 0);
	lastPart();
}
#endif

// SUMO Final Project //

#if 0
struct sensors_ ref;
struct sensors_ dig;
struct accData_ data;
   
int count = 0,timeDiff = 0;
int currentValueX = 0, newValueX = 0, diffX = 0;
int currentValueY = 0, newValueY = 0, diffY = 0;
   
bool isHit = false;
bool isWhite = true;
bool isPressed = true;
    
TickType_t timeIR = 0, timeHit = 0, timeEnd = 0;
    
void checkHit(void) {
    //Since the hit is somewhat instantly, checking the accelerometer quickly can catch the difference
    LSM303D_Read_Acc(&data);
    currentValueX = data.accX;
    
    LSM303D_Read_Acc(&data);
    newValueX = data.accX;
          
    diffX = currentValueX - newValueX;
    
    //Only print hit once
    if (isHit == false && diffX > 10000){
        isHit = true;
        timeHit = xTaskGetTickCount();
        print_mqtt("Zumo024/hit", "%d", timeHit);
    }
    else if (isHit == true && diffX <= 10000) {
        isHit = false;
    }
}

void checkBW(void) {
    reflectance_digital(&dig);
    
    //Only do this once until detect white again
    if (isWhite == true && (dig.r3 == 1 || dig.l3 == 1)) {
        isWhite = false;
        count++;
        if (count == 1) {   //To stop on the black circle
            motor_forward(0,0,0);
            print_mqtt("Zumo024/ready","zumo");
            IR_wait();
            timeIR = xTaskGetTickCount();
            print_mqtt("Zumo024/start", "%d", timeIR);
        }
        else if (count > 1) {
            motor_forward(0,0,0);
            if (dig.r3 == 1) {
                motor_backward(255,255,50);
                motor_instant_left(255,255,300);
            }
            else if (dig.l3 == 1) {
                motor_backward(255,255,50);
                motor_instant_right(255,255,300);
            }
        }
    }
    else if (dig.l3 == 0 && dig.r3 == 0) {
        isWhite = true;
        if (count > 1) {
            checkHit();
            //print_mqtt("Zumo024/21", "%d", count);
        }
    }
}
    
void zmain(void) {
    motor_start();
    motor_forward(0,0,0);
    
    IR_Start();
    IR_flush();
    
    reflectance_start();
    reflectance_set_threshold(18000, 11000, 15000, 15000, 11000, 18000);
    
    if(!LSM303D_Start()){
        printf("LSM303D failed to initialize!!! Program is Ending!!!\n");
        vTaskSuspend(NULL);
    }
    else {
        printf("Device Ok...\n");
    }
    
    for(;;){
        if (SW1_Read()== 0) {
            isPressed = !isPressed;
            vTaskDelay(500);
        }
        if(isPressed == false) {
            //Simply keep going forward and checking for black circle and hits
            motor_forward(255,255,0);
            
            checkBW();
            
            if (SW1_Read()== 0){ 
                motor_forward(0,0,0);
                isPressed= !isPressed;
                timeEnd = xTaskGetTickCount();
                print_mqtt("Zumo024/stop", "%d", timeEnd);
                timeDiff= timeEnd - timeIR;
                print_mqtt("Zumo024/time", "%d", timeDiff);
                break;
            }
        }
    }    
}
#endif
/*END OF FILE*/