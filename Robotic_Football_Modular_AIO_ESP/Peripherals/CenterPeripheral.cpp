#include "../src/ESP32Servo/src/ESP32Servo.h"
#include "../src/esp32-ps3-develop/src/Ps3Controller.h"

#define CENTER_RELEASE        5     // the ball release servo is wired to pin 5
#define CENTER_RELEASE_DOWN   120   // these are the angles between 0 and 180 to set servo for releasing and holding the ball
#define CENTER_RELEASE_UP     70    
Servo centerRelease;                // define servo object for ball release 
  
void peripheralSetup(){
	centerRelease.attach(CENTER_RELEASE);   // attach ball release servo to its pin
	centerRelease.write(CENTER_RELEASE_UP);
}
  
void peripheral(Ps3Controller Ps3){
	if (Ps3.data.button.triangle) {
		centerRelease.write(CENTER_RELEASE_UP);
	} else if (Ps3.data.button.cross) {
		centerRelease.write(CENTER_RELEASE_DOWN);
	}
}