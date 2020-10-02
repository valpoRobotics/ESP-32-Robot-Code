#include "../src/ESP32Servo/src/ESP32Servo.h"
#include "../src/esp32-ps3-develop/src/Ps3Controller.h"

#define QB_THROWER            5
#define TRIANGLE_THROW        175
#define CIRCLE_THROW          125
#define CROSS_THROW           108
#define SQUARE_THROW          102
#define RELOAD_THROW          80
int throwOffset = 0;                //used to adjust strength of cross and circle throws
Servo qbThrower;


void peripheralSetup(){
	qbThrower.attach(QB_THROWER, 1000, 2000);
	qbThrower.writeMicroseconds(1500);
}
  
void peripheral(Ps3Controller Ps3){
	if (Ps3.data.button.triangle)    qbThrower.write(TRIANGLE_THROW);
	else if (Ps3.data.button.circle) qbThrower.write(CIRCLE_THROW + throwOffset);
	else if (Ps3.data.button.cross)  qbThrower.write(CROSS_THROW + throwOffset);
	else if (Ps3.data.button.square) qbThrower.write(SQUARE_THROW + throwOffset) ;
	else if (Ps3.data.button.r1)
	{
		qbThrower.write(RELOAD_THROW);
		throwOffset = 0;
	}
	else qbThrower.writeMicroseconds(1500);
}