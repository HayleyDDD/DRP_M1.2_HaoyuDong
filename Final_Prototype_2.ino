// (c) 2019 Haoyu Dong
// for Design Research Project M1.2 prototype
// (Include User-test and Final-Demo Part)
// Industrial Design, TU Eindhoven
// personal website: http://haoyudong.wixsite.com/projects
/******************************************************************************/

#include <Adafruit_NeoPixel.h>
#define leds 16 //set the number of the LEDs 
#define ledPin 6 //LED ring attaching to  Pin6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(leds, ledPin);

uint8_t  eye = 15; // Position of eyes (uint8_t: a type of unsigned integer of length 8 bits)
uint32_t color  = 0xE8F5FF; // Start bright white light

#include <Servo.h>
// create servo objects to control a servo
Servo myservo1;
Servo myservo2;
int pos = 0;  // variable to store the servo position

#define softpotPin A0 // Pin connected to softpot wiper
int temporaryVal = 0; //the temporary value read from soft potmeter
int releasedVal = 100; //the threshold for legit input
int softpotVal = 0; //initialize the soft potmeter's value as 0
boolean touched = false; //initialize the softpot as untouched
int status = 0; //initialize the status as 0

void setup() {
  //setup for softpot
  Serial.begin(9600);//Initialize serial communications at a 9600 baud rate
  pinMode(softpotPin, INPUT); //set softpotPin as input
  //setup for LED ring
  pinMode(ledPin, OUTPUT);
  pixels.begin();
  pixels.setBrightness(150); // 1/3 brightness
  pixels.fill(0);
  pixels.show();
  //setup for servos
  myservo1.attach(9);  // attaches the servos on pin 9 and 10
  myservo2.attach(10);
  myservo1.write(0); //set the start position 0
  myservo2.write(0);
}

//custom function to get the data from soft potmeter
void softpot() {
  temporaryVal = analogRead(softpotPin);  // read the softpot temporary value
  //checking if value is a legit input
  if (temporaryVal <= releasedVal) {
    //when not touching the sensor, do nothing
    //the volume will stay as last defined legit volume
  } else {
    //when touching, value is legit, so softpot value get new input
    softpotVal = temporaryVal;
  }
  Serial.println(softpotVal); //send the value to Processing (100-850)
  delay(50);//wait 50 milliseconds
}

//custom function to generate the static pattern
void staticpattern() {
  pixels.fill(0xFF03C9); //rosa color
  pixels.show();
  delay(50);//wait 50 milliseconds
}
//custom function to generate the dynamic pattern
void dynamicpattern() {
  uint8_t  i;
  for (i = 0; i < leds; i++) {
    uint32_t c = 0;
    if (((eye + i) & 7) < 2) {
      c = color; // 4 pixels on...
    }
    pixels.setPixelColor(i, c); // the eye
  }
  pixels.show();
  eye--;
  delay(50);
}
//custom function to generate the shape-change
void shapechange() {
  pixels.fill(0); //reset the LED ring
  pixels.show();

  for (pos = 0; pos <= 60; pos += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos); // set servos to the position 'pos'
    myservo2.write(pos);
    delay(50); //wait 50 milliseconds
  }
  myservo1.write(pos); //keep the servos in the position 'pos’
  myservo2.write(pos);
}

void loop() {
  softpot();
  if (Serial.available() > 0) { //Checking if Processing is sending serial value
    char command = Serial.read(); //Reading the value and store it in command
    if (command == '1') { //If 1 was received, NON-FEEDFORWARD
      status = 1;
      pixels.fill(0); //reset the LED
      pixels.show();
      myservo1.write(0); //set servos to the 0 position
      myservo2.write(0);
    } else if (command == '2') { //If 2 was received, STATIC FEEDFORWARD
      status = 2;
      myservo1.write(0);//set servos to the 0 position
      myservo2.write(0);
    } else if (command == '3') { //If 3 was received, DYNAMIC FEEDFORWARD
      status = 3;
      myservo1.write(0);//set servos to the 0 position
      myservo2.write(0);
    } else if (command == '4') { //If 4 was received, SHAPE-CHANGE
      pixels.fill(0);//reset the LED
      pixels.show();
      status = 4;
      shapechange();
    } else if ( command == '5') { // DEMO1_ SHAPE-CHANGE + FOLLOWING DOT
      pixels.fill(0);//reset the LED
      pixels.show();
      status = 5;
      shapechange();
    } else if (command == '6') { //DEMO@_ SHAPE-CHANGE + DYNAMIC PATTERN
      pixels.fill(0);//reset the LED
      pixels.show();
      status = 6;
      shapechange();
    }
  }
  if (status == 1) {
    pixels.fill(0);//reset the LED
    pixels.show();
  }
  if (status == 2) {
    staticpattern();
  }
  if (status == 3) {
    dynamicpattern();
  }
  if (status == 4) {
    pixels.fill(0);
    pixels.show();
  }
  if (status == 5) {
    pixels.fill(0);//reset the LED
    pixels.show();
    //generate following dot display
    //map the softpot value to 15-0
    //then find the closest integer of n between 0-15
    //which represent the closest LED in the LED ring
    float n = map(softpotVal, 100, 850, 15, 0);
    uint8_t intn = round(n);
    pixels.setPixelColor(intn, 0x03FF32);
    pixels.show();
    delay(50);//wait 50 milliseconds
  }
  if (status == 6) {
    dynamicpattern();
  }
}
