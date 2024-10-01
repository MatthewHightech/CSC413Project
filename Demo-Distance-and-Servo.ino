#define ledPin 9
#define button 4
#define trigPin 12
#define echoPin 2

#include <Servo.h>

float cm;
float duration;
long startTime;
long echoStartTime;
long triggerStartTime;


int previousBtnState = LOW;
int debounceTime = 50;
bool powerOn = false;

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int withinDistanceCounter = 0; // number of cycles the subject is in range for

#define OUT_OF_RANGE 1
#define IN_RANGE 2
#define IN_PROGRESS 3
#define DONE 4

int photoStatus = OUT_OF_RANGE;

// Timer Display Pins
int a=45; 
int b=43; 
int c=41; 
int d=53; 
int e=51; 
int f=47; 
int g=49; 

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  
  pinMode(button, INPUT);
  pinMode(ledPin, OUTPUT);

  myservo.attach(7); 
  myservo.write(pos);              // tell servo to go to position in variable 'pos'


  startTime = millis();
  echoStartTime = micros();
  triggerStartTime = micros();

  int i;
  for(i=41;i<=53;i+=2) {
      Serial.println(i);
      pinMode(i,OUTPUT);
  }
}

void loop()
{

  long currentTime = millis();
  int buttonState = digitalRead(button);
  
  if (buttonState == HIGH && previousBtnState != HIGH && currentTime - startTime > debounceTime) {
    int state = powerOn ? LOW : HIGH;
    powerOn = !powerOn;
    digitalWrite(ledPin, state);
    startTime = millis();
    if (powerOn) {
      attachInterrupt(0, EchoPinISR, CHANGE);
    } else {
      detachInterrupt(0);
    }
  }
  previousBtnState = buttonState;


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // If subject is within the appropriate distance for at least 1 second, the photo process begins
  if (withinDistanceCounter >= 6) {
    photoStatus = IN_RANGE;
    TakePhoto();
  } else {
    photoStatus = OUT_OF_RANGE;
  }

  delay(100);
}

void EchoPinISR() {

  if (digitalRead(echoPin) == HIGH) {
    echoStartTime = micros();
  } else {
    long duration = micros() - echoStartTime;


    //Sound travels at 340 meters per second which is 29.412 microseconds.
    // To account for the round trip you divide by 2 for a one way trip 
    long speedOfSoundMicroSecPerCm = 29; // Speed of light in microseconds per cm
    cm  = duration / speedOfSoundMicroSecPerCm / 2;
    //Serial.print("cm: ");
    //Serial.println(cm);

    // counts iterations where the subject is within the specified range of the sensor
    if (cm > 80 && cm < 90) {
      withinDistanceCounter++;
    } else {
      withinDistanceCounter = 0;
    }
    Serial.println(withinDistanceCounter);
    Serial.println(cm);

  }
}

void TakePhoto() {
  Serial.println("Take Photo");

  photoStatus = IN_PROGRESS;
  CountdownTimer();
  TriggerShutter();

  photoStatus = DONE;
  delay(1000);

}

// activates the servo to trigger the physical shutter button on the camera
void TriggerShutter() {
    for (pos = 0; pos <= 40; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 40; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

// Counts from 3 down to 1 and then clears 7-segment screen
void CountdownTimer() {
  clearDisplay();
  display3();
  delay(1000);
  clearDisplay();
  display2();
  delay(1000);
  clearDisplay();
  display1();
  delay(1000);
  clearDisplay();
}


//display number 1
void display1(void) 
{  
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
} 
//display number2
void  display2(void) 
{
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(g,HIGH);
  digitalWrite(e,HIGH);
  digitalWrite(d,HIGH);
}  
// display number3
void display3(void) 
{ 
  digitalWrite(a,HIGH);
  digitalWrite(b,HIGH);
  digitalWrite(c,HIGH);
  digitalWrite(d,HIGH);
  digitalWrite(g,HIGH);
} 

void clearDisplay(void) 
{ 
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(g,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);  
  digitalWrite(e,LOW);  
  digitalWrite(f,LOW);  
} 
