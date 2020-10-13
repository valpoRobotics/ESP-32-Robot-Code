#include "../src/ESP32Servo/src/ESP32Servo.h"
#include "../src/esp32-ps3-develop/src/Ps3Controller.h"

#define TARGET_PIN_ONE        13     // the ball release servo is wired to pin 5
#define TARGET_PIN_TWO        12     // the ball release servo is wired to pin 5
#define TARGET_RAISED_POSITION   0   // these are the angles between 0 and 180 to set servo for releasing and holding the ball
#define TARGET_LOWERED_POSITION     90   
#define REVERSE_SERVO_ONE 
#define SPEED     1  
Servo TARGET_SERVO_ONE;                // define servo object for ball release 
Servo TARGET_SERVO_TWO;                // define servo object for ball release 

int position;//what it should be
int servo_position;//what it actually is


void peripheralSetup(){
	TARGET_SERVO_ONE.attach(TARGET_PIN_ONE);   // attach ball release servo to its pin
	TARGET_SERVO_ONE.write(TARGET_LOWERED_POSITION);

	TARGET_SERVO_TWO.attach(TARGET_PIN_TWO);   // attach ball release servo to its pin
	TARGET_SERVO_TWO.write(TARGET_LOWERED_POSITION);
    Serial.println("ATTACKED" );

	position=servo_position=TARGET_LOWERED_POSITION;
}
  
void peripheral(Ps3Controller Ps3){
	if (Ps3.data.button.triangle) position = TARGET_RAISED_POSITION;
	else if (Ps3.data.button.cross) position = TARGET_LOWERED_POSITION;
    
	if (position > servo_position) servo_position++;
    else if (position < servo_position) servo_position--;
    
	TARGET_SERVO_ONE.write(servo_position);
    TARGET_SERVO_TWO.write(servo_position);

    Serial.println(servo_position);
}
