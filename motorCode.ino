#include <AccelStepper.h>
#include <MultiStepper.h>

#define FULL_RAIL_STEP 200 //200 steps to travel the entire rail
#define STEPS_PER_REV 200 //native to the nema17, steps per rev is useful cos theta is based off 2pi


enum motorPos {IDLE, MOVING, ESTOP} state, prevState;
AccelStepper thetaStepper (AccelStepper::DRIVER, 4, 3); //first argument is an enum type, we're using a4988 so just put driver, step, direciton
AccelStepper railStepper (AccelStepper::DRIVER, 6, 5);

void moveToPos(float r, float theta)
{
    thetaStepper.moveTo((theta/(2 * PI) * STEPS_PER_REV)); //converts theta radians to revolutions and multiples it by steps per rev to get steps
    railStepper.moveTo(r * FULL_RAIL_STEP); //scales r ratio to the steps for the full rail
    thetaStepper.setAcceleration(500);
    railStepper.setAcceleration(500);
    thetaStepper.run();
    railStepper.run();
}

void setup() {
    Serial.begin(115200);
    pinMode(6, OUTPUT); //rail step
    pinMode(5, OUTPUT); //rail dir
    pinMode(4, OUTPUT); //theta step
    pinMode(3, OUTPUT); //theta dir

    digitalWrite(5, HIGH); //set dir pin HIGH
    digitalWrite(3, HIGH); //set dir pin HIGH
    state = IDLE;
    prevState = ESTOP;
}

void loop() {
    moveToPos(.8, PI);
    
}