#include "src/esp32-ps3-develop/src/Ps3Controller.h"
#include "src/ESP32Servo/src/ESP32Servo.h"

//===========Uncomment a LED===========================
#include "Leds/Leds.cpp"
//#include "Leds/OldLeds.cpp"

//===========Uncomment a drive train===================
#include "DriveTrains/BasicDrive.cpp"
//#include "DriveTrains/CenterDrive.cpp"
//#include "DriveTrains/SquareOmniDrive.cpp"

//===========Uncomment for tackle sensor===============
#define TACKLE

//===========Uncomment to choose a Position============
//#define WR
//#define Center
//#define QB
//#define Kicker

//===========Uncomment if not using bag motors=========
//#define OldMotors

//===========Uncomment for debug modes=================
//#define SHOW_CONTROLLER_INPUT
//#define SHOW_EXECUTION_TIME

//===================================
//Includes the right peripheral file for specified position
#ifdef WR
    #define PERIPHERAL
    #include "Peripherals/WRPeripheral.cpp"
#endif

#ifdef Center
    #define PERIPHERAL
    #include "Peripherals/CenterPeripheral.cpp"
#endif

#ifdef QB
    #define PERIPHERAL
    #include "Peripherals/QBPeripheral.cpp"
#endif

#ifdef Kicker
    #define PERIPHERAL
    #include "Peripherals/KickerPeripheral.cpp"
#endif

//This just enables and disables the old motors
#ifdef OldMotors
  int motorType = -1;
#else
  int motorType = 1;
#endif
//===================================

#define TACKLE_INPUT    35           // Tackle sensor is wired to pin 6
bool hasIndicated = false;
bool stayTackled = false;
int handicap = 3;
bool kidsMode = false;
int newconnect = 0;
int leftX, leftY, rightX, rightY;
ps3_cmd_t cmd;
float exeTime;

void setup() {// This is stuff for connecting the PS3 controller.
  Serial.begin(115200);       //Begin Serial Communications
  ledsSetup();          //Setup the leds
  flashLeds();
  
  // Wait for contoller to be connected to the esp
  Ps3.begin("00:1b:dc:0f:f3:59");
  while(!Ps3.isConnected()){
    Serial.println("Controller not connected");
    blue();
  }
  Serial.println("Controller is connected!");

  // Vibrates controller when you first connect
  if (newconnect == 0) {
    cmd.rumble_left_intensity = 0xff;
    cmd.rumble_right_intensity = 0xff;
    cmd.rumble_right_duration = 750;
    cmd.rumble_left_duration = 750;
    newconnect++;
    ps3Cmd(cmd);
  }

  //Setup the drive train, peripherals, tackle sensor, and changes leds to green once complete
  driveSetup(motorType);

  #ifdef PERIPHERAL
    peripheralSetup();
  #endif

  #ifdef TACKLE
    pinMode(TACKLE_INPUT, INPUT);
  #endif

  #ifdef TACKLE
      green();
  #else
      blue();
  #endif

  // Variable(boolean) for connection to ps3, also activates rumble
  int newconnect = 0;         
}


void loop() {
  // Run if the controller is connected
  if (Ps3.isConnected()) {
    #ifdef SHOW_EXECUTION_TIME
      exeTime = micros();
    #endif

    // !QUESTION! - What is the point of this code?
    // // Press the PS button to disconnect the controller
    // if (Ps3.data.button.ps && newconnect == 1) {
    //   newconnect = 0;
    // }

    //====================Get Controller Input=================================
    // Reads and maps joystick values from -90 to 90
    leftX = map(Ps3.data.analog.stick.lx, -128, 127, -90, 90);
    leftY = map(Ps3.data.analog.stick.ly, -128, 127, -90, 90);
    rightX = map(Ps3.data.analog.stick.rx, -128, 127, -90, 90);
    rightY = map(Ps3.data.analog.stick.ry, -128, 127, -90, 90);
    
    // Deals with stickness from joysticks
    if (abs(leftX) < 8) leftX = 0;
    if (abs(leftY) < 8) leftY = 0;
    if (abs(rightX) < 8) rightX = 0;
    if (abs(rightY) < 8) rightY = 0;
    
    #ifdef SHOW_CONTROLLER_INPUT
      Serial.print(leftX);    
      Serial.print("\t");
      Serial.print(leftY);    
      Serial.print("\t");
      Serial.print(rightX);    
      Serial.print("\t");
      Serial.print(rightY);    
      Serial.print("\t");
    #endif

    //====================Specify the handicap=================================
    //Toggle in and out of kidsmode
    if (Ps3.data.button.start) {
      if (kidsMode == true) {
        kidsMode = false;
        handicap = 3;
        //PS3.setLedRaw(1);               // ON OFF OFF ON
        //PS3.setRumbleOn(5, 255, 5, 255);// vibrate both, then left, then right
      } else if (kidsMode == false) {
        kidsMode = true;
        handicap = 7;
        //PS3.setLedRaw(9);               // OFF OFF OFF ON
        //PS3.setRumbleOn(5, 255, 5, 255);// vibrate both, then left, then right
      }
    }

    if (kidsMode == false) {
      // Press R2 to boost
      if (Ps3.data.button.r2) {
        handicap = 1;
      }
      // Press L2 to slow down
      else if (Ps3.data.button.l2) {
        handicap = 6;
      } 
      // Sets default handicap
      else {
        handicap = 3;
      }
    }

    //===============================Adjust motors=============================
    if (Ps3.data.button.left){
      correctMotor(1);
      Serial.println("Left button clicked");
    }
    if (Ps3.data.button.right){
      correctMotor(-1);
      Serial.println("right button clicked");
    }

    //=================================Tackle Sensor===========================
    #ifdef TACKLE
      // NORMAL OPERATION MODE
      // for the if statement for whether or not
      // tackle is enabled. cool stuff
      if (Ps3.data.button.left) {
        if (stayTackled == true) {
          stayTackled = false;
        } 
        else {
          stayTackled = true;
        }
        /*
          Ps3.data.cmd.rumble_right_duration(30);
          Ps3.data.cmd.rumble_right_intensity(255);
          Ps3.data.cmd.rumble_left_duration(30);
          Ps3.data.cmd.rumble_left_intensity(255);
        */
        //PS3.setRumbleOn(30, 255, 30, 255);
      }
      if (!digitalRead(TACKLE_INPUT))
      {
        red();
        if (!hasIndicated) {
          //PS3.setRumbleOn(10, 255, 10, 255);
          hasIndicated = true;
        }
      }
      else
      {
        if (stayTackled == false) {
          hasIndicated = false;
          green();
        }
      }
    #endif
    //===============================================================================================

    // Drives the robot according to joystick input
    driveCtrl(handicap, leftX, leftY, rightX, rightY);

    #ifdef PERIPHERAL
        peripheral(Ps3);//Call the peripheral
    #endif

    #ifdef SHOW_EXECUTION_TIME
      Serial.print("Exe exeTime: ");
      Serial.print(micros() - exeTime);
      Serial.print("\t");
    #endif

    Serial.println();
  }

  // If the controller is not connected, LEDs blue and stop robot
  if (!Ps3.isConnected()) {
    blue();
    driveStop();
  }
}
