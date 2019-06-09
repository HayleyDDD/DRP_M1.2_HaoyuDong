// (c) 2019 Haoyu Dong
// for Design Research Project M1.2 prototype 
// (Include User-test and Final-Demo Part)
// Industrial Design, TU Eindhoven
// personal website: http://haoyudong.wixsite.com/projects
/******************************************************************************/

import processing.serial.*; 
import processing.sound.*;
import controlP5.*;
Serial myPort; //create object from Serial class
SoundFile soundfile;//Create objetc from SoundFile class
ControlP5 controlP5; //Create objetc from ControlP5 class

String val; //data received from the serial port
float value; //value trimmed from val
float amplitude = 0.0; //volume of the soundfile
float theAngle; //angel of the graphical indicator for the volume
PFont font ; //declare a new font

void setup() {
  size(900, 900);
  background(255);
  font = createFont("Montserrat-Light.ttf", 15); //create the font and the default size

  String portName=Serial.list()[0];
  myPort=new Serial(this, portName, 9600); //declare the new port at the rate 9600
  controlP5 = new ControlP5(this); //create a top level control manager
  controlP5.setColorBackground(200); //declare the color of controlP5 controlers
  controlP5.setFont(font); //declare the font in the controlP5 controlers
  
  //add mode buttons for user-test
  controlP5.addButton("non_feedforward", 1, 600, 200, 200, 50);
  controlP5.addButton("static_pattern", 2, 600, 300, 200, 50);
  controlP5.addButton("dynamic_pattern", 3, 600, 400, 200, 50);
  controlP5.addButton("shape_change", 4, 600, 500, 200, 50);
  //add mode buttons for final-demo
  controlP5.addButton("Demo1", 5, 600, 600, 200, 50); // shape-change + following dot
  controlP5.addButton("Demo2", 6, 600, 700, 200, 50); // shape-change + dynamic pattern

  // Load a soundfile from the data folder of the sketch 
  // declare the volume of the soundfile and play it in loop
  soundfile = new SoundFile(this, "music1_01.aif");
  soundfile.amp(amplitude);
  soundfile.loop();

  //add the tile for the graphical interface
  textFont(font);
  textSize(40);
  fill(0);
  text("Select One Mode", 100, 100);
}

//if certain button is pressed
//sending the button value out of the serial port to Arduino
void non_feedforward (float buttonvalue) { 
  print(buttonvalue); 
  myPort.write('1'); 
}
void static_pattern(float buttonvalue) {
  print(buttonvalue);
  myPort.write('2'); 
  
void dynamic_pattern(float buttonvalue) {
  print(buttonvalue);
  myPort.write('3');
}
void shape_change(float buttonvalue) {
  print(buttonvalue);
  myPort.write('4');
}
void Demo1(float buttonvalue) {
  print(buttonvalue);
  myPort.write('5');
}
void Demo2(float buttonvalue) {
  print(buttonvalue);
  myPort.write('6');
}

void draw() {
  textFont(font);
  textSize(70);
  noStroke();
  indicator(); //show the indicator of the volume
  changevolume(); //adjust the volume of the soundfile
}

//custom function to design a graphical indicator to indicate the volume
void indicator() {
  if (myPort.available()>0) { //if data from Arduino is available
    println("someone is adjusting the volume"); 
    val = myPort.readStringUntil('\n'); //read the data until \n and store it in val

    if (val != null&&val.length()>3) {
      val = val.substring(0, 4); //convert val
      value = Float.parseFloat(val.trim()); //convert string val to float value
      println(value); //100 - 850

      fill(color(#B7B7B7));
      ellipse(200, height/2, 200, 200);
      theAngle = map (value, 100, 850, 2.5 * PI, PI /2);
      fill(color(#366DFF));
      arc(200, height/2, 200, 200, theAngle - PI/18, theAngle, PIE);
    }
  }
}
//custome function to change the volume of the soundfile
void changevolume() {
  amplitude = map(value, 100, 850, 1.0, 0.0);
  soundfile.amp(amplitude);
}
