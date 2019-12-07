#include "roverLib.hpp"

#include <Arduino.h>


Robot::Robot() : distSense(TRIG_PIN, ECHO_PIN)
{
    Serial.begin(9600);
    
    tail.attach(MOTOR_TAIL);
    /*
    NOTE: The left and right motors do not need to be attached since this is
            done in the turn90 and drive functions
    */
    Serial.println("------------------------------------");
    Serial.println("Robot setup complete");

}


//Turns the robot 90 degrees in either the left or right direction
void Robot::turn90(DIRECTION dir)
{

    /*
    This code is for driving continuous rotation servos. The ones that I am
    jitter a lot while idle at the 90 degree position. I order to overcome
    this the motors are only turned on when they are needed to move and are
    then turned off by detaching the motor from the servo object.
    */

    //Setup the servos
    right.attach(MOTOR_RIGHT);
    left.attach(MOTOR_LEFT);

    //Check which direction was given
    if(dir == RIGHT)
    {
        Serial.println("------------------------------------");
        Serial.println("Turning to the right");
        //Make the servos start moving then wait
        right.write(45 + TRIM_RIGHT);
        left.write(45 + TRIM_LEFT);
    }
    else
    {
        Serial.println("------------------------------------");
        Serial.println("Turning to the left");
        //Make the servos start moving then wait
        right.write(45 + TRIM_RIGHT);
        left.write(45 + TRIM_LEFT);
        
    }
    
    //Delay to allow the robot to complete its turn
    delay(ROT_DELAY);

    //Turn off the motors to avoid the jitter problem
    right.detach();
    left.detach();
}

//Sets the angle of the robots tail
void Robot::setTailPos(int angle)
{
    Serial.println("------------------------------------");
    Serial.print("Set tail position to ");
    Serial.println(angle);

    tail.write(angle);
}


//Drive forward and only turn when a object is detected
void Robot::drive()
{

    Serial.println("------------------------------------");
    Serial.println("Driving forward");
    //Start driving forwared
    right.attach(MOTOR_RIGHT);
    left.attach(MOTOR_LEFT);
    right.write(80 + TRIM_RIGHT);
    left.write(100 + TRIM_LEFT);
    setTailPos(75);

    //Busy loop until the sensor detects somthing
    boolean obstacleFound = false;
    int repeatTrigger = 0;
    while(obstacleFound == false)
    {
          //Make sure that the distance is with in the acceptable range. If it is
          //not then report that an obstacle was found
          int dist = distSense.getDist();
          delay(25);
          if(dist <= DIST_BOUND && dist != -1)
          {
            repeatTrigger++;
            if(repeatTrigger >= 5)
            {
                Serial.println("------------------------------------");
                Serial.print("Obstacle detected at a distance of ");
                Serial.print(dist);
                Serial.println("cm");
                //This will cause the loop to exit
                obstacleFound = true;
            
            }

          }
          else
          {
            repeatTrigger = 0;
          }
    }

    //Stop for .5 seconds when an object is detected
    Serial.println("------------------------------------");
    Serial.println("Stoping Robot");
    right.detach();
    left.detach();
    setTailPos(60);
    delay(500);

    //Turn the robot to the right
    turn90(RIGHT);
}
