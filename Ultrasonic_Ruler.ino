
// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //
//Include LCD and Keypad library
#include <LiquidCrystal.h>
#include <Keypad.h>


#define echoPin 10 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 9 //attach pin D3 Arduino to pin Trig of HC-SR04

//Defines the layout of keypad

String currentInput;
String finalInput;
char key;

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {19, 18, 17, 16}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {15, 14, 6, 7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
//Defines the pin for the buzzer
const int buzzPin = 8;

//Defines the key for the mode button
const char modeKey = 'D';
//Defines the new function key 
const char newFunctionKey = 'C';
//Defines the maximium mode integer
const int maxMode = 3;

//Reference to the mode of the device
int currentMode;

//References to measured distance booleans
bool distanceMeasured1;
bool distanceMeasured2;
//Reference to custom function program boolean
bool doNewFunction;

//References to measurements
int distance1;
int distance2;

//References to timer counts for future use once I replace delays
//unsigned long currentTime; 
//unsigned long previousTime;
//unsigned long passedTime;

//References to distances per timeframe
int distancePerSecond;
int distancePerMinute;
int distancePerHour;

//Reference to button pressed boolean 
bool buttonPressed;

//Reference to distance loop
void distanceLoop();

//Reference to custom function
void customFunction();

//Reference to resetDistance
void resetDistance();

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
  //Sets up all the variables by setting them to a default value
  doNewFunction = false;
  currentInput = "EMPTY";
  finalInput = "EMPTY";
  distance = 0;
  distanceMeasured1 = false;
  distanceMeasured2 = false;
  distance1 = 0;
  distance2 = 0;
  //currentTime = 0;
  //previousTime = 0;
  //passedTime = 0;K
  currentMode = 1;
  pinMode(13, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");

   // set up the LCD's number of columns and rows: 
  //lcd.begin(16, 2);
  //lcd.setCursor(0, 2);
  //lcd.print("Arduino ultrasonic distance sensor");
}
void loop() {
  key = keypad.getKey();
  if(key != NO_KEY){
  Serial.println(key);
  }
  delay(500);
  //currentTime = millis();
  //Cycles through the modes of the ruler
  if(buttonPressed){
  digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  if(key == modeKey && buttonPressed == false){
    if(currentMode < maxMode)
    {
      buttonPressed = true;
      currentMode++;
    }else
     {
      currentMode = 1; 
    }
  }else
   {
    buttonPressed = false;
   }
  
  //Print fun messages to 2nd line of LCD
 
  //lcd.setCursor(0, 2);
  //lcd.print("Ultrasonic Ruler");
  //delay(625);
  distanceLoop();
  //delay(625);
  //lcd.setCursor(0,2);
  //lcd.print("by Logan Kessler");
  //delay(625);
  //distanceLoop();
  //delay(625);
  //lcd.setCursor(0, 2);
  //lcd.print("#Team Metric!   ");
  //delay(500);
  //distanceLoop();
  //delay(500);
  //lcd.setCursor(0, 2);
  //lcd.print("Made 8/18/2020   ");
  //waits a second before looping program
  //delay(500);
  //distanceLoop();
  //delay(500); 
}

void distanceLoop(){
  
// Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  abs(distance);
  // Displays the distance on the Serial Monitor
  if(currentMode == 1){
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println("cm");

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //Resets cursor
  resetDistance();
  //Print distance to first line of LCD
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("cm");
}else if(currentMode == 2)
{
  //Not currently needed due to the way I setup delays, may be useful in the future
  //lcd.setCursor(0, 0);
  //passedTime = currentTime - previousTime;
  //if(passedTime >= 500){
  //previousTime = currentTime;
  //passedTime = 0;
    if(distanceMeasured1 == false){
    distance1 = distance;
    distanceMeasured1 = true;
  }else if(distanceMeasured2 == false){
    distance2 = distance;
    distanceMeasured2 = true;
  }else if(distanceMeasured1 && distanceMeasured2){
    distancePerSecond = distance2 - distance1;
    //distancePerMinute = distance2 - distance1 * 60;
    //distancePerHour = distance2 - distance1 * 3600;

    resetDistance();
    
    lcd.print("DPS: "); lcd.print(abs(distancePerSecond)); lcd.print("cm "); //lcd.print("DPM: "); lcd.print(distancePerMinute); lcd.print("cm "); lcd.print("DPH: "); lcd.print(distancePerHour); lcd.print("cm");

    distanceMeasured1 = false;
    distanceMeasured2 = false; 
  }
 //}
  }else if(currentMode == 3){
    customFunction();
  }
}

void resetDistance(){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
}

void customFunction(){
if(currentInput == "EMPTY" && finalInput == "EMPTY"){
  resetDistance();
  lcd.print("Function Mode");
}

  if(key == 'C'){
    doNewFunction = true;
    finalInput = "EMPTY";  
  }
if(doNewFunction == true && currentInput != "EMPTY"){
  resetDistance();
  lcd.print("CI: ");
  lcd.print(currentInput);
  lcd.print("CMs");
}
Serial.print("doNewFunction: ");
Serial.println(doNewFunction);
Serial.print("currentInput: ");
Serial.println(currentInput);
Serial.print("finalInput: ");
Serial.println(finalInput);
if(key != NO_KEY && key != '#' && key!= '*' && key!= 'A' && key!= 'B' && key!= 'C' && key!= 'D' && doNewFunction == true){
  if(currentInput == "EMPTY"){
    currentInput = "";
  }
  currentInput += key;
}else if(key == '#' && doNewFunction == true){
  finalInput = currentInput;
  currentInput = "EMPTY";
  doNewFunction = false;
}else if(key == '*'){
  currentInput = "EMPTY";
}

if(finalInput != "EMPTY"){
  if(distance < finalInput.toInt()){
    resetDistance();
    lcd.print("DBM: ");
    lcd.print(finalInput.toInt() - distance);
    lcd.print(" CMs");
    delay(1500);
    resetDistance();
    lcd.print("GET BACK!");
    digitalWrite(buzzPin, HIGH);
  }else{
    resetDistance();
    //DFM = Distance from minimium
    lcd.print("DFM: ");
    lcd.print(distance - finalInput.toInt());
    lcd.print(" CMs");
    digitalWrite(buzzPin, LOW);
    }
  }
}
