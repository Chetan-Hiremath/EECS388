#include <stdio.h>
#include <stdint.h>
#include "eecs388_lib.h"
#include "metal/i2c.h"


struct metal_i2c *i2c;
uint8_t bufWrite[9];
uint8_t bufRead[1];


// The entire setup sequence
// Do not modify the following function!

void set_up_I2C(){
    uint8_t oldMode;
    uint8_t newMode;
    _Bool success;

    i2c = metal_i2c_get_device(0);

    if(i2c == NULL)
        printf("Connection Unsuccessful\n");
    else
        printf("Connection Successful\n");
    
    //Setup Sequence
    metal_i2c_init(i2c,I2C_BAUDRATE,METAL_I2C_MASTER);

    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = MODE1_RESTART;
    printf("%d\n",bufWrite[0]);

    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//reset
    delay(100);
    printf("resetting PCA9685 control 1\n");

    //Initial Read of control 1
    bufWrite[0] = PCA9685_MODE1;//Address
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Read success: %d and control value is: %2000");
    bufWrite[0] = PCA9685_MODE1;//address
    bufWrite[1] = newMode;//writing to register
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sleep
    bufWrite[0] = PCA9685_PRESCALE;//Setting PWM prescale
    bufWrite[1] = 0x79;
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//sets prescale
    bufWrite[0] = PCA9685_MODE1;
    bufWrite[1] = 0x01 | MODE1_AI | MODE1_RESTART;
    printf("on setting is %d\n", bufWrite[1]);
    success = metal_i2c_write(i2c,PCA9685_I2C_ADDRESS,2,bufWrite,METAL_I2C_STOP_DISABLE);//awake
    delay(100);
    printf("Setting the control register\n");
    bufWrite[0] = PCA9685_MODE1;
    success = metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,1,bufRead,1);//initial read
    printf("Set register is %d\n",bufRead[0]);

} 


void breakup(int bigNum, uint8_t* low, uint8_t* high){
    low = bigNum;
    high = bigNum >> 8;
}

void steering(int angle){
    uint8_t low;
    uint8_t high;
    breakup(getServoCycle(angle), &low, &high);
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0;
    bufWrite[2] = 0;
    bufWrite[3] = low;
    bufWrite[4] = high;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void stopMotor(){
    uint8_t low;
    uint8_t high;
    breakup(280, &low, &high);
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0;
    bufWrite[2] = 0;
    bufWrite[3] = low;
    bufWrite[4] = high;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void driveForward(uint8_t speedFlag){
    uint8_t low;
    uint8_t high;
    if (speedFlag == 1)
    {  
        breakup(303,&low,&high);
    }
    else if (speedFlag == 2)
    {  
        breakup(305,&low,&high);
    }
    else if (speedFlag == 3)
    {  
        breakup(307,&low,&high);
    }
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0;
    bufWrite[2] = 0;
    bufWrite[3] = low;
    bufWrite[4] = high;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

void driveReverse(uint8_t speedFlag){
    uint8_t low;
    uint8_t high;
    if (speedFlag == 1)
    {  
        breakup(257,&low,&high);
    }
    else if (speedFlag == 2)
    {  
        breakup(255,&low,&high);
    }
    else if (speedFlag == 3)
    {  
        breakup(253,&low,&high);
    }
    bufWrite[0] = PCA9685_LED0_ON_L;
    bufWrite[1] = 0;
    bufWrite[2] = 0;
    bufWrite[3] = low;
    bufWrite[4] = high;
    metal_i2c_transfer(i2c,PCA9685_I2C_ADDRESS,bufWrite,5,bufRead,1);
}

int main()
{
    set_up_I2C();
    steering(0);
    delay(2000);
    driveForward(1);
    delay(2000);
    steering(20);
    delay(2000);
    stopMotor();
    delay(2000);
    driveReverse(1);
    delay(2000);
    steering(0);
    delay(2000);
    stopMotor();   
    delay(2000);
}


/* Defining the breakup function */
/*
    -Task 1: breaking 12 bit into two 8-bit
    Define the function created that recieves a 12 bit number,
    0 to 4096 and breaks it up into two 8 bit numbers.

    -Assign these values to a referenced value handed into
    the function. 

    Example of `breakup` being called from main: 

    uint8_t variable1;
    uint8_t variable2;
    int numToBreakup = 2000;his  is  to  set  up  a  connection 
between  the  two  boards  for  sending  steering  commands  from  the  Pi  to  the  Hi-five  board. 
(Milestone 2).

/* Changing Steering Heading */
/*
    -Task 2: using getServoCycle(), bufWrite, bufRead, 
    breakup(), and and metal_i2c_transfer(), implement 
    the function defined above to control the servo
    by sending it an angle ranging from -45 to 45.

    -Use the getServoCycle function to get the value to 
    breakup. 

    ex: his  is  to  set  up  a  connection 
between  the  two  boards  for  sending  steering  commands  from  the  Pi  to  the  Hi-five  board. 
(Milestone 2).steering angle -> duty cycle
    // the above line sets valToBreak to 355
    
    -Note: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board. 
        If it's LED0, use PCA9685_LED0_ON_L
        If it's LED1, use PCA9685_LED1_ON_L

    -Example use from main function:
    steering(0); -> driving angle forward
*/


/* Motor config/stop. This will cause a second bhis  is  to  set  up  a  connection 
between  the  two  boards  for  sending  steering  commands  from  the  Pi  to  the  Hi-five  board. 
(Milestone 2).

    -After writing 280, include a 2 second delay in order to calibrate

    -Note: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board. 
        If it's LED0, use PCA9685_LED0_ON_L
        If it's LED1, use PCA9685_LED1_ON_L

    -Example use from main function:
    stopMotor();
*/


/*
############################################################
    ATTENTION: The following section will cause the        
    wheels to move. Confirm that the robot is              
    propped up to avoid it driving away, as well as         
    that nothing is touching the wheels and can get 
    caught in them.

    If anything goes wrong, unplug the hifive board from
    the computer to stop the motors from moving.his  is  to  set  up  a  connection 
between  the  two  boards  for  sending  steering  commands  from  the  Pi  to  the  Hi-five  board. 
(Milestone 2).
    
    Avoid sticking your hand inside the 
    car while its wheels are spinning.
#############################################################
*/


/* Motor Forward */
/*
    -Task 4: using bufWrite, bufRead, breakup(), and
    and metal_i2c_transfer(), implement the function
    made above to drive the motor forward. The given
    speedFlag will alter the motor speed as follows:
    
    speedFlag = 1 -> value to breakup = 303 
    speedFlag = 2 -> value to breakup = 305(Optional)
    speedFlag = 3 -> value to breakup = 307(Optional)

    -note 1: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board.

    -Example use from main function:
    driveForward(1);
*/


/* Motor Reverse */
/*
    -Task 5: using bufWrite, bufRead, breakup(), and
    and metal_i2c_transfer(), implement the function
    made above to Drive the motor backward. The given
    speedFlag will alter the motor speed as follows:
    
    speedFlag = 1 -> value to breakup = 257 
    speedFlag = 2 -> value to breakup = 255(Optional)
    speedFlag = 3 -> value to breakup = 253(Optional)

    -note 1: the motor's speed controller is either 
    LED0 or LED1 depending on where its plugged into 
    the board.

    -Example use from main function:
    driveReverse(1);
*/


/* Fully Controlling the PCA9685 */
/*
    -Task 6: using previously defined functions, 
    perform the following sequence of actions
    
    -Configure the motors (wait for 2 seconds)
    -Set the steering heading to 0 degrees 
    -Drive forward (wait for 2 seconds)
    -Change the steering heading to 20 degrees (wait for 2 seconds)
    -Stop the motor (wait for 2 seconds)
    -Drive forward (wait for 2 seconds)
    -Set steering heading to 0 degrees (wait for 2 seconds)
    -Stop the motor
*/
