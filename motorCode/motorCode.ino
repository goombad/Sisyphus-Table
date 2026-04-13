#include <AccelStepper.h>
#include <MultiStepper.h>
#include <FastAccelStepper.h>

#define FULL_RAIL_STEP 200 //200 steps to travel the entire rail
#define DEG_PER_STEP 1.8 //native to NEMA17, check datasheet, mine is 1.8 deg per step
#define STEPPER_SPEED 1500
#define STEPPER_ACCEL 500

enum motorPos {IDLE, MOVING, ESTOP} state, prevState;
AccelStepper thetaStepper (AccelStepper::DRIVER, 4, 3); //first argument is an enum type, we're using a4988 so just put driver, step, direciton
AccelStepper railStepper (AccelStepper::DRIVER, 6, 5);

void moveToPos(float r, float theta)
{
    thetaStepper.moveTo(theta / DEG_PER_STEP); //converts theta deg to steps by dividing deg per step
    while (thetaStepper.distanceToGo())
    {
        thetaStepper.run();
    }

    railStepper.moveTo(r * FULL_RAIL_STEP); //scales r ratio to the steps for the full rail
    while (railStepper.distanceToGo())
    {
        railStepper.run();
    }
}
void stepperInit()
{
    pinMode(6, OUTPUT); //rail step
    pinMode(5, OUTPUT); //rail dir
    pinMode(4, OUTPUT); //theta step
    pinMode(3, OUTPUT); //theta dir

    digitalWrite(5, HIGH); //set dir pin HIGH
    digitalWrite(3, HIGH); //set dir pin HIGH
    
    thetaStepper.setAcceleration(500); //sets accel (Steps/s^2)
    thetaStepper.setMaxSpeed(1500); //sets max speed (steps/s)
    railStepper.setAcceleration(500);
    railStepper.setMaxSpeed(1500);
}
void setup() {
    Serial.begin(115200);
    stepperInit();
    state = IDLE;
    prevState = ESTOP;
}

void loop() {
    if (Serial.available())
    {
        
    }
    moveToPos(.8, 180);
}