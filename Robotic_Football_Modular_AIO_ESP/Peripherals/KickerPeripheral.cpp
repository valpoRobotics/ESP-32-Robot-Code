#include "../src/ESP32Servo/src/ESP32Servo.h"
#include "../src/esp32-ps3-develop/src/Ps3Controller.h"

#define KICKER_MOTOR          5     	// Kicker motor is wired to pin 5
										//these are the speeds for kicking and reload the kicker foot
#define KICKER_POWER          2000   
#define KICKER_RELOAD         1000
Servo kicker;                       	// Define motor object for the kicker motor

void peripheralSetup(){
	kicker.attach(KICKER_MOTOR);
	kicker.writeMicroseconds(1500);
}
  
void peripheral(Ps3Controller Ps3){
	if (Ps3.data.button.cross){
		kicker.writeMicroseconds(KICKER_POWER);
	}else if (Ps3.data.button.triangle){
		kicker.writeMicroseconds(KICKER_RELOAD);
	}else{
		kicker.writeMicroseconds(1500);
	}
}