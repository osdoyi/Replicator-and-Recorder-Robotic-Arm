// Prepared by Osman Dogan Yirmibesoglu & Arjen Kılıc

// include servo library
#include <Servo.h>

//define relay on and off situations
#define RELAY_ON 0
#define RELAY_OFF 1

//define boolean for if condition
boolean flag = false;

// Potantio meter pin assaignments
const int roll = 0;
const int pitch1 = 1;
const int pitch2 = 2;
const int relay = 3;

// Relay pin assaignment
const int relayPin = 12;

// Servo Objects
Servo rollServo;
Servo pitch1Servo;
Servo pitch2Servo;

// Potantio meter & servo & pinState values
int rollVal;
int pitch1Val;
int pitch2Val;
int relayVal;

int rollPos;
int pitch1Pos;
int pitch2Pos;

int tempRollPos;
int tempPitch1Pos;
int tempPitch2Pos;

int keepState;
int recordState;

// Switch Pin assaignments
const int recordPin = 9;
const int keepPin = 8;

//array for record
int recordArray[300];
int i = 0;
int j = 0;

void setup() {
  // Starting serial
  Serial.begin(9600);

  //atttaching servo Objects
  rollServo.attach(5);
  pitch1Servo.attach(3);
  pitch2Servo.attach(6);

  //pinMode
  pinMode(recordPin, INPUT);
  pinMode(keepPin, INPUT);
  pinMode(relayPin, INPUT);

}

void loop() {
  
  // Reading potantio meters
  rollVal = analogRead(roll);
  pitch1Val = analogRead(pitch1);
  pitch2Val = analogRead(pitch2);
  relayVal = analogRead(relay);

  // Reading switch states
  keepState = digitalRead(keepPin);   // read the input pin
  recordState = digitalRead(recordPin);   // read the input pin


  // change between switch states

  if (keepState == 1) {

    fanState();
    deleteArray();

    if (flag == false) {
      
      //getting temporary servo values
      tempRollPos = map(analogRead(roll), 0, 1023, 0, 180);
      tempPitch1Pos = map(analogRead(pitch1), 0, 1023, 0, 180);
      tempPitch2Pos = (180 - map(analogRead(pitch2), 0, 1023, 0, 180));

      //writing temporary values to servos
      rollServo.write(tempRollPos);
      pitch1Servo.write(tempPitch1Pos);
      pitch2Servo.write(tempPitch2Pos);

      flag = true;
    }

  } else if (recordState == 1 ) {
    flag = false;
    fanState();

    if (recordArray[299] == 0) {

      for (i = 0; i <= 300; i += 3) {

        recordArray[i] = map(analogRead(roll), 0, 1023, 0, 180);
        rollServo.write(map(analogRead(roll), 0, 1023, 0, 180));
        
        recordArray[i + 1] = map(analogRead(pitch1), 0, 1023, 0, 180);
        pitch1Servo.write(map(analogRead(pitch1), 0, 1023, 0, 180));
        
        recordArray[i + 2] = (180 - map(analogRead(pitch2), 0, 1023, 0, 180));
        pitch2Servo.write(180 - map(analogRead(pitch2), 0, 1023, 0, 180));
        

        // change this delay to arrange your recording time
        delay(100);
      }
    }


    i = 0;
    while (i <= 300) {

      rollPos = recordArray[i];
      rollServo.write(rollPos);

      pitch1Pos = recordArray[i + 1];
      pitch1Servo.write(pitch1Pos);

      pitch2Pos = recordArray[i + 2];
      pitch2Servo.write(pitch2Pos);

      i += 3;
      delay(100);
    }


  } else {
     // make flag false
  flag = false;
  // open or close fan
  fanState();
  // delete record array
  deleteArray();
  //map values between potantiometer and servo
  mapValues();
 
  //writing values to servos
  rollServo.write(rollPos);
  pitch1Servo.write(pitch1Pos);
  pitch2Servo.write(180 - pitch2Pos);

  }


}

// mapping potantiometer values to servo value range
void mapValues() {

  rollPos = map(rollVal, 0, 1023, 0, 180);
  pitch1Pos = map(pitch1Val, 0, 1023, 0, 180);
  pitch2Pos = map(pitch2Val, 0, 1023, 0, 180);

  // Printing values of servos for debugging
  //  Serial.print(rollPos);
  //  Serial.print(",");
  //  Serial.print(pitch1Pos);
  //  Serial.print(",");
  //  Serial.println(pitch2Pos);
}

// open and close fan
void fanState() {

  if (relayVal >= 500) {
    digitalWrite(relayPin, RELAY_OFF);
  } else {
    digitalWrite(relayPin, RELAY_ON);
  }
}

// making record array have all zeros
void deleteArray() {
  for (i = 0; i <= 299; i++) {
    recordArray[i] = 0;
  }
}




