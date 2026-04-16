#include <AccelStepper.h>
#include <MultiStepper.h>

#define FULL_RAIL_STEP 400 //400 steps to travel the entire rail
#define DEG_PER_STEP 0.9 //native to NEMA17, check datasheet, OMCSTEPPERONLINE is .9 deg per step
#define STEPPER_SPEED 1000
#define STEPPER_ACCEL 500

enum motorPos {IDLE, MOVING, ESTOP} state, prevState;
AccelStepper thetaStepper (AccelStepper::DRIVER, 4, 3); //first argument is an enum type, we're using a4988 so just put driver, step, direciton
AccelStepper railStepper (AccelStepper::DRIVER, 6, 5);

float r, theta;

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
    
    thetaStepper.setAcceleration(STEPPER_ACCEL); //sets accel (Steps/s^2)
    thetaStepper.setMaxSpeed(STEPPER_SPEED); //sets max speed (steps/s)
    railStepper.setAcceleration(STEPPER_ACCEL);
    railStepper.setMaxSpeed(STEPPER_SPEED);
}
void setup() {
    Serial.begin(115200);
    stepperInit();
    state = IDLE;
    prevState = ESTOP;
}

void loop() 
{  
    if (Serial.available())
        {
            String packet = Serial.readStringUntil('\n'); // every C string ends with a null terminator, single quotes will read the single character, but "" will read null terminator
            Serial.print("received: "); //completes handshake with packet as acknowledgement
            Serial.println(packet); 
            int spaceIndex = packet.indexOf(' '); //finds the whitespace index
            r = packet.substring(0, spaceIndex).toFloat(); //makes a substring from 0 of packet to the whitespace, then converts to float
            theta = packet.substring(spaceIndex + 1).toFloat(); //makes a substring from 1 past the whitespace, then converts to float
            moveToPos(r, theta); //moves to position
        }
}