//These are the values which allow for adjustment/tuning of the robot
#define MOTOR_RIGHT 7
#define MOTOR_LEFT 5
#define MOTOR_TAIL 3
#define TRIM_RIGHT 0
#define TRIM_LEFT 0
#define ROT_DELAY 1000
#define TRIG_PIN 4
#define ECHO_PIN 6
#define DIST_BOUND 30

#include <Servo.h>
#include "DistanceSensor.hpp"

enum DIRECTION {RIGHT, LEFT};


class Robot
{
    private: 
        Servo right, left, tail;
        DistanceSensor distSense;

        //Turns the robot 90 degrees in either the left or right direction
        void turn90(DIRECTION dir);

        //Sets the angle of the robots tail
        void setTailPos(int angle);

    public:
        Robot();

        void drive();
        

};
