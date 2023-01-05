#include <Servo.h>

#define trigger 6
#define echo 12


// Declaring all servos
Servo servoA;
Servo servoB;
Servo servoC;
Servo servoD;


bool Ready = false;

// for storing the colors
int shape;


void setup() {

  pinMode(12,INPUT); // for the sensor
  pinMode(6,OUTPUT); // for the sensor 
  
  // Attaching and initializing all servos
  servoA.attach(7);
  servoB.attach(8);
  servoC.attach(9);
  servoD.attach(10);

  // Setting up the ARM
  servoA.write(10);
  servoB.write(90);
  servoC.write(10);
  servoD.write(90);

  //Beging thr serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // For Assuring communication with BT device
  Serial.println(2);
  Serial.println(2);

}

void loop() {

  //Waiting for a signal to start
  
  if(Ready){
    
    
      grip(true); // open the grip
      moveArmTo(158,53,19,10,7,7);
      grip(false); // close the grip
      moveArmTo(158,20,70,0,7,7);
      delay(300);
      
      //placing the color in its appropriat place
      PlaceShapeAs(shape);
      delay(100);
      grip(true);
      delay(400);

      //restore the default position
      restorePosition();
      Ready = false;
    }
}



void moveArmTo(int b,int c,int d,int t1,int t2,int t3){

          writeTo(servoB,b,t1); // rotate the arm
          delay(400);
          writeTo(servoD,d,t3); // move up/down
          delay(400);
          writeTo(servoC,c,t2); // move forward/backward
          delay(400);
          
}

void grip(bool g){

  if(g) writeTo(servoA,100,0);
  else  writeTo(servoA,0,6);
  
  
}


void PlaceShapeAs(int c){

switch(c){

  //NOTE: the values entered in the functions 
  //      must be changed after testing
  
  case 3: moveArmTo(120,20,59,10,7,7);//square
          break;

  case 4: moveArmTo(35,8,56,10,7,7);//trapozoidal
          break;

  case 5: moveArmTo(63,15,41,10,7,7);//pentagon
          break;        

  case 8: moveArmTo(92,42,83,10,7,7);//circle
          break; 
          
  }
  
  grip(true); // for releasing the object
  
}

void restorePosition(){
  
     writeTo(servoB,90,5); // rotate the arm
          
     writeTo(servoC,10,5); // move forward/backward
          
     writeTo(servoD,90,5); // move up/down

  }



// alternative function to write to servo

void writeTo(Servo sv,int angle,int t){
  int p = sv.read();
  if((p-angle)< 0){

    for(int i=p;i<= angle; i++){
    sv.write(i);
    delay(t);}
  }
  else{
    
    for(int i=p ;i>=angle; i--){
    sv.write(i);
    delay(t);}
    
    }
  }

float CheckForObject(){
  
  digitalWrite(trigger, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
   
  // distance in centimeters
  float distance = pulseIn(echo, HIGH) / 58;
  return distance;
  }

void serialEvent() {
  
  if(Serial.available()>0){
  
  int inByte = (int) Serial.read();

  if(inByte == 3) { shape = 3 ; Ready = true ;}//square
  if(inByte == 4) { shape = 4 ; Ready = true ;}//trapozoidal
  if(inByte == 5) { shape = 5 ; Ready = true ;}//pentagone
  if(inByte == 8) { shape = 8 ; Ready = true ;}//circle
  }
 }