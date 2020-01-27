#include <Keypad.h>
#include <Controllino.h> 
#include <Password.h>

Password password= Password("1234");

int inner_hall_door;
int outer_hall_door;
int E_Stop1;
int E_Stop2;
bool RUN_Signal;
int operator_safety;
int n_operator_safety;

  
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {CONTROLLINO_D14,CONTROLLINO_D9, CONTROLLINO_D10, CONTROLLINO_D12}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {CONTROLLINO_D13, CONTROLLINO_D15, CONTROLLINO_D11}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
//  pinMode(CONTROLLINO_D9, INPUT);  
//  pinMode(CONTROLLINO_D10, INPUT);
//  pinMode(CONTROLLINO_D11, INPUT);  
//  pinMode(CONTROLLINO_D12, INPUT);  
//  pinMode(CONTROLLINO_D13, INPUT);  
//  pinMode(CONTROLLINO_D14, INPUT);
//  pinMode(CONTROLLINO_D15, INPUT);
    pinMode(CONTROLLINO_R0, OUTPUT);
    pinMode(CONTROLLINO_R1, OUTPUT);
    pinMode(CONTROLLINO_R2, OUTPUT);
    pinMode(CONTROLLINO_R3, OUTPUT);
    pinMode(CONTROLLINO_R4, OUTPUT);
    pinMode(CONTROLLINO_R5, OUTPUT);
    pinMode(CONTROLLINO_R6, OUTPUT);
    pinMode(CONTROLLINO_R7, OUTPUT);
    pinMode(CONTROLLINO_R8, OUTPUT);
    pinMode(CONTROLLINO_R10,OUTPUT);
    pinMode(CONTROLLINO_R12,OUTPUT);
    
    
   keypad.addEventListener(keypadEvent); //add an event listener for this keypad
   keypad.setDebounceTime(250);
  
}
/////////////////////
void loop(){

char key = keypad.getKey();
EStop();
inner_hall_door= InterlockIn();
outer_hall_door= InterlockOut();
guessPassword();
//security function

Serial.println(inner_hall_door);
Serial.println(outer_hall_door);
//
//if (inner_hall_door > 0 || outer_hall_door > 0) { // if either x or y is greater than zero
//  // statements
//  Serial.println("Door Open Operator safety Started");//Print that the signal has been sent to stop the robot movement
//  digitalWrite(CONTROLLINO_R10,LOW);
//}
//else {
//  digitalWrite(CONTROLLINO_R10,HIGH);
//}



operator_safety=inner_hall_door ^ outer_hall_door;
n_operator_safety=!operator_safety;

Serial.println(n_operator_safety);
  switch (operator_safety) {
  case HIGH:
     //Turn off light, door closed
     digitalWrite(CONTROLLINO_R10,LOW);
     digitalWrite(CONTROLLINO_R12,LOW);
     digitalWrite(CONTROLLINO_R4,HIGH);
    break;
  case LOW:
    //Turn on light, door opened
    digitalWrite(CONTROLLINO_R10,HIGH);
    digitalWrite(CONTROLLINO_R12,HIGH);
    digitalWrite(CONTROLLINO_R4,LOW);
    break;
  default:
    //Turn on light, door opened
    digitalWrite(CONTROLLINO_R10,LOW);
    digitalWrite(CONTROLLINO_R12,LOW);
    digitalWrite(CONTROLLINO_R4,LOW);
    break;
  }


}



//  if (key != NO_KEY){
//    Serial.println(key);
//  }
//  
//Trigger an event if the keypad is used.
void keypadEvent(KeypadEvent eKey){
 switch (keypad.getState()){
   case PRESSED:
     Serial.print("Pressed: ");
     Serial.println(eKey);
     switch (eKey){
       case '#': guessPassword(); break;  
        default:
              password.append(eKey);
 }}
//////////////////////

}


//Password Function called in the main body 
void guessPassword(){
    Serial.print("Guessing password... ");
    if (password.evaluate()){
          //digitalWrite(CONTROLLINO_R14,HIGH); //activates relay/pin/or whatever
            delay(1000);                
            //digitalWrite(CONTROLLINO_R14,LOW); //turns off relay/pin/or whatever after .5 sec
          Serial.println("VALID PASSWORD "); //
             password.reset(); //resets password after correct entry
    }else{
          //digitalWrite(CONTROLLINO_R14,LOW);
          Serial.println("INVALID PASSWORD ");
             password.reset(); //resets password after INCORRECT entry
    }
} 

//////////////EMERGENCY STOP FUNCTION

//E stop Function for Laser and Robot
void EStop(){
  E_Stop1=digitalRead(CONTROLLINO_IN0);
  E_Stop2=digitalRead(CONTROLLINO_IN1);
   switch(E_Stop1){
    case HIGH:
      //E-STOP FOR LASER R0,R1 ON
      digitalWrite(CONTROLLINO_R0,HIGH);
      digitalWrite(CONTROLLINO_R1,HIGH);
       //E-STOP FOR ROBOT R2,R3 ON
      digitalWrite(CONTROLLINO_R2,HIGH);
      digitalWrite(CONTROLLINO_R3,HIGH);
      //LIGHTS
       //digitalWrite(CONTROLLINO_R4,LOW);
       digitalWrite(CONTROLLINO_R5,LOW);
       digitalWrite(CONTROLLINO_R6,LOW);
       digitalWrite(CONTROLLINO_R7,LOW);
       digitalWrite(CONTROLLINO_R8,LOW); 
       break;
    case LOW:
      //E-STOP FOR LASER R0,R1 OFF
      digitalWrite(CONTROLLINO_R0,LOW);
      digitalWrite(CONTROLLINO_R1,LOW);
      //E-STOP FOR ROBOT R2,R3 OFF
      digitalWrite(CONTROLLINO_R2,LOW);
      digitalWrite(CONTROLLINO_R3,LOW);
      //LIGHTS
       //digitalWrite(CONTROLLINO_R4,HIGH);
       digitalWrite(CONTROLLINO_R5,HIGH);
       digitalWrite(CONTROLLINO_R6,HIGH);
       digitalWrite(CONTROLLINO_R7,HIGH);
       digitalWrite(CONTROLLINO_R8,HIGH);
       break;
     default:
      //E Stops are NC
      digitalWrite(CONTROLLINO_R0,HIGH);
      digitalWrite(CONTROLLINO_R1,HIGH);
      digitalWrite(CONTROLLINO_R2,HIGH);
      digitalWrite(CONTROLLINO_R3,HIGH);
      //LIGHTS
       //digitalWrite(CONTROLLINO_R4,LOW);
       digitalWrite(CONTROLLINO_R5,LOW);
       digitalWrite(CONTROLLINO_R6,LOW);
       digitalWrite(CONTROLLINO_R7,LOW);
       digitalWrite(CONTROLLINO_R8,LOW);
       break;
  }
  
}

//////////////////////////////////////////////////////////////
/// Function for reading the Interlock from the INNER hall///

int InterlockIn(){

  //INTERLOCK READING
  inner_hall_door=digitalRead(CONTROLLINO_I17);
 switch (inner_hall_door) {
  case HIGH:
     //digitalWrite(CONTROLLINO_R4,LOW);//Turn off light, door closed
    break;
  case LOW:
    //digitalWrite(CONTROLLINO_R4,HIGH); //Turn on light, door opened
    break;
  default:
    //digitalWrite(CONTROLLINO_R4,HIGH); //Turn on light, door opened
    break;

    
}
return inner_hall_door;
}

/// Function for reading the Interlock from the OUTER hall
int InterlockOut(){
  //INTERLOCK READING  
  outer_hall_door=digitalRead(CONTROLLINO_I16);
switch (outer_hall_door) {
  case HIGH:
     //digitalWrite(CONTROLLINO_R4,LOW);//Turn off light, door closed
    break;
  case LOW:
    //digitalWrite(CONTROLLINO_R4,HIGH); //Turn on light, door opened
    break;
  default:
    //digitalWrite(CONTROLLINO_R4,HIGH); //Turn on light, door opened
    break;
}
 return outer_hall_door;
}


  
