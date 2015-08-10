
//Servo
#define servoPin      14

//Encoder
#define encoderPin    4 
#define encoderSteps  120

//Speed
#define fastFoward    180
#define fastBack      0
#define slowFoward    110
#define slowBack      55
#define stopIt        90

// Wheel tracking variables
int trayPosition    = 0;
int encoderPosition = 0;
int remotePosition  = 0;

//wheelDirection type => STATIC = Static, CW = Clockwise, AC = Anticlockwise
enum wheelDirection {
  STATIC,
  CW,
  AC
};

//Define initial direction as static
wheelDirection trayDirection = STATIC;

Servo myservo;

//************************************
// Setup functions
//************************************

// Trigger the encoder events
void listenEncoder(){
  attachInterrupt(encoderPin, getEncoder, FALLING);
}

// Initiate servo
void initServo(){
  myservo.attach(servoPin);
}

//************************************
// Movement functions
//************************************

// Track encoder events on interrupt input
void getEncoder(){
  // Increase when clockwise and decrease when anticlockwise
  if(trayDirection == CW){
    encoderPosition ++;
    
    if(encoderPosition > encoderSteps){
      encoderPosition = 1; 
    }
  }else if(trayDirection == AC){
    encoderPosition --;
    
    if(encoderPosition < 1){
      encoderPosition = encoderSteps;
    }
  }
  
    trayPosition = encoderPosition * 3;
  // Set the tray position proportionally to encoder
  // map function converts to the right angle
//  trayPosition = map(encoderPosition, 1, encoderSteps, 1, 360);

  if(inTheRange()){
    trayDirection = STATIC;
     myservo.write(90);
  }

  log("Encoder position: ");
  log (String(encoderPosition, DEC));
  log("   Tray position: ");
  log (String(trayPosition, DEC));
  log("   Remote position: ");
  logln (String(remotePosition, DEC));
}

// Define direction of the wheel
void getDirection(){
  if (inTheRange()){
      trayDirection == STATIC;
  }else{
    if (isBigger(remotePosition, trayPosition)) {
      if (remotePosition - trayPosition > 180){
        trayDirection = AC;
      }else{
        trayDirection = CW;     
      }
    }else{
      if (trayPosition - remotePosition > 180){
        trayDirection = CW;
      }else{      
        trayDirection = AC;
      }
    }
  }
}

void rotate(){
  getDirection();

    if (trayDirection == STATIC){
      myservo.write(stopIt);
    }else{      
      // Define the direction
      // 180 = full speed Clockwise; 0 = full speed Anticlockwise
      if(trayDirection == CW){
        myservo.write(slowFoward);
      }else{
        myservo.write(slowBack);
      }
    }
}


// Check if the remote position is in the range of the tray position
boolean inTheRange(){
  return remotePosition == constrain(remotePosition, trayPosition - 3, trayPosition);
}
