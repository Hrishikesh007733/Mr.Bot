#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <String.h>
#include<Servo.h>

const int rs = 13, en = 12, d4 = 8, d5 = 9, d6 = 10, d7 = 11;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial bluetoothSerial(6, 7); /* (Rx, Tx) */



int IN1 = 14;
int IN2 = 15;
int IN3 = 16;
int IN4 = 17;

int IR = 4;
int Buzzer=19;

#define trigPin 2
#define echoPin 3

#define servo 5

int Set= 40;
int distance_L, distance_F, distance_R;
int bt_data;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR, INPUT);
  pinMode(Buzzer, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servo, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(Buzzer, LOW);

  lcd.begin(16, 2);
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  Serial.print("DELIVERY ROBOT");
  lcd.clear();
  lcd.setCursor(0, 0);
  String msg = "HI I AM MR BOT USE ME FOR DELIVERY ";
  lcd.print(msg);
  delay(800);  //time waiting for lcd to display meassage on hold
  for (int pos = 0; pos < msg.length(); pos++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  distance_F = ultrasonic();
  Serial.println(distance_F);
  delay(10);
}


void loop() {
   
  if(digitalRead(IR)==HIGH)
  {
    Serial.println("$Object Placed inside the Box#");
    lcd.clear();
    lcd.print("PLZ PLACE OBJECT");
    lcd.setCursor(0,1);
    lcd.print("inside the Box");
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    
  }
  else{robot();}
}
void robot(){
  lcd.clear();
  lcd.print("PL SELECT OPTIONS");
  lcd.setCursor(0, 1);
  lcd.print("FROM THE APP");

  bluetoothSerial.begin(9600);
  if (bluetoothSerial.available() > 0) {  //if some date is sent, reads it and saves in state
    delay(10);                            //Delay added to make thing stable
    bt_data = bluetoothSerial.read();
  }
  Serial.print(bt_data);
  delay(10);
  int value = bt_data;
  switch (value){
    case 6: Serial.begin(9600);
            Serial.println("path 1 Selected");
            delay(10);
            CLASS1();
            break;
    case 7: Serial.begin(9600);
            Serial.println("path 2 Selected");
            delay(10);
            CLASS2();
            break;
    case 8 :Serial.begin(9600);
            delay(10);
            MANUAL_MODE_OPERATION();
            break;
    
    case 9: Serial.begin(9600);
            delay(10);
            class3();
            break;
    
    case 0: STOP();
            break;
    default: STOP();
  }
  
}


void MANUAL_MODE_OPERATION() {
  Serial.println("MANUAL MODE SELECTED.");
  lcd.clear();
  lcd.print("Waiting For ");
  lcd.setCursor(0, 1);
  lcd.print("Command Info");
  while (1) { 

    bluetoothSerial.begin(9600);
    if (bluetoothSerial.available() > 0) {  //if some date is sent, reads it and saves in state
      delay(10);                            //Delay added to make thing stable
      bt_data = bluetoothSerial.read();
    }
    Serial.println(bt_data);
    if (bt_data == 1) {
      Serial.begin(9600);
      Serial.println("$Forward#");
      lcd.clear();
      lcd.print("Forward");
      delay(10);  //Delay added to make thing stable
      FORWARD();

    }  // if the bt_data is '1' the DC motor will go forward

    else if (bt_data == 2) {
      Serial.begin(9600);
      Serial.println("$Backword#");
      lcd.clear();
      lcd.print("Backword");
      delay(10);  //Delay added to make thing stable
      REVERSE();
    }  // if the bt_data is '2' the motor will Reverse

    else if (bt_data == 3) {

      Serial.begin(9600);
      Serial.println("$left#");
      lcd.clear();
      lcd.print("LEFT");
      delay(10);  //Delay added to make thing stable
      LEFT();
    }  // if the bt_data is '3' the motor will turn left

    else if (bt_data == 4) {
      Serial.begin(9600);
      Serial.println("$RIGHT#");
      lcd.clear();
      lcd.print("RIGHT");
      delay(10);  //Delay added to make thing stable
      RIGHT();
    }  // if the bt_data is '4' the motor will turn right

    else if (bt_data == 5) {
      Serial.begin(9600);
      Serial.println("$STOP#");
      lcd.clear();
      lcd.print("STOP");
      delay(10);  //Delay added to make thing stable
      STOP();
    }  // if the bt_data '5' the motor will Stop

  }
}

void CLASS1() {
  lcd.clear();
  lcd.print("Moving to CLASS1");
  delay(1000);

   //staring point
  Serial.println(" to p1");
  for(int i =0;i<6;i++){
    FORWARD(); 
    dela(1000);
    sto();
  }
  
  STOP();  //point 1
  Serial.println("point 1");
  delay(3000);

    //to ponit 2
  Serial.println(" to p2");

  for(int i =0;i<6;i++){
    FORWARD();
    dela(1000);
    sto();
  }

  STOP();
  Serial.println(" r p2");
  delay(3000);

    //final point
  Serial.println(" to fp");
  for(int i =0;i<5;i++){
    FORWARD();
    dela(1000);
    sto();
  }
  
  STOP();
  Serial.println(" fianl p");
  delay(3000);


  RETURN_PATH1();
}
void dela(int time){
  delay(time);
}
void sto(){
 distance_F = ultrasonic();
  Serial.print("S=");
  Serial.println(distance_F);
  if (distance_F < Set){
   STOP();
   while(distance_F<40){
     distance_F = ultrasonic();
      Serial.println(distance_F);  
      STOP();
      lcd.print("Object Detected");
      digitalWrite(Buzzer,HIGH);
      delay(1000);
      digitalWrite(Buzzer,LOW);
      delay(10);
   }
  }  
}
void CLASS2() {
  lcd.clear();
  lcd.print("Moving to CLASS2");
  delay(1000);
  
  Serial.println(" to p1");  
  for(int i =0;i<6;i++){
    FORWARD(); 
    dela(1000);
    sto();
  }  //staring point
 
  
  STOP();  //point 1
  Serial.println("point 1");
  delay(3000);

  //to ponit 2
  Serial.println(" to p2");
 for(int i =0;i<4;i++){
    RIGHT();  
    dela(1000);
    sto();
  } 
  
  STOP();
  Serial.println(" r p2");
  delay(3000);

  //final point
  Serial.println(" to fp");
 for(int i =0;i<6;i++){
    FORWARD(); 
    dela(1000);
    sto();
  } 
  
  STOP();
  Serial.println(" fianl p");
  delay(3000);
  RETURN_PATH2();
}

void class3(){
  lcd.clear();
  lcd.print("Moving to CLASS 3");
  delay(10);
  distance_F = ultrasonic();
  Serial.print("S=");
  Serial.println(distance_F);
  if (distance_F > Set){
   Serial.println("Forward");
   FORWARD();
  }
  else{
    STOP();
    delay(100);    
    lcd.print("Object Detected");
    digitalWrite(Buzzer,HIGH);
    delay(1000);
    digitalWrite(Buzzer,LOW);
    delay(10);
    Check_side();
  }
}
int RETURN_PATH1() {
  lcd.clear();

  Serial.println("return path");
  lcd.print("RETURN PATH 1");
  delay(1000);

  REVERSE();  //staring point
  delay(5000);
  STOP();  //point 1
  Serial.println("point 1");
  delay(3000);

  REVERSE();  //to ponit 2
  delay(5000);
  STOP();
  Serial.println(" r p2");
  delay(3000);

  REVERSE();  //final point
  delay(5000);
  Serial.println(" fianl p");
  STOP();
  delay(1000);
  return bt_data = 0;
  
}

int RETURN_PATH2() {
  lcd.clear();

  Serial.println("return path");
  lcd.print("RETURN_PATH1");
  delay(1000);

  REVERSE();  //staring point
  delay(6000);
  STOP();  //point 1
  Serial.println("point 1");
  delay(3000);

  LEFT();  //to ponit 2
  delay(4000); 
  STOP();
  Serial.println(" r p2");
  delay(3000);

  REVERSE();  //final point
  delay(6000);
  Serial.println(" fianl p");
  STOP();
  delay(1000);
  return bt_data = 0;
  
}

long ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  long distance = pulseIn (echoPin, HIGH);
  return distance / 29 / 2;
  Serial.println(distance);
  //return d;
}

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}

void Check_side(){
    STOP();
    delay(100);
  for (int angle = 70; angle <= 120; angle += 3)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_R = ultrasonic();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 120; angle >= 0; angle -= 3)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = ultrasonic();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle =0; angle <= 70; angle += 3)  {
   servoPulse(servo, angle);  }
    delay(300);

    compareDistance();
}

void compareDistance(){
    if(distance_L > distance_R){
    REVERSE();
    delay(5000);
    LEFT();
    delay(5000);
    FORWARD();
    delay(2000);
    RIGHT();
    delay(5000);
    
  }
  else{
    REVERSE();
    delay(5000);
    RIGHT();
    delay(5000);
    FORWARD();
    delay(2000);
    LEFT();
    delay(5000);
    
  }
}

void FORWARD() {

  Serial.println("FORWARD.");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void REVERSE() {

  Serial.println("REVERSE.");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void LEFT() {
  Serial.println("LEFT.");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void RIGHT() {

  Serial.println("RIGHT.");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void STOP() {

  Serial.println("STOP.");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}