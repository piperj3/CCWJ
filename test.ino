#include <Keypad.h>
#include <Controllino.h> 
#include <Password.h>

Password password= Password("1234");

int inner_hall_door;
int outer_hall_door;
  
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
   pinMode(CONTROLLINO_D16, OUTPUT);
   keypad.addEventListener(keypadEvent); //add an event listener for this keypad
   keypad.setDebounceTime(250);
  
}

void loop(){

  char key = keypad.getKey();

EStop();
InterlockIn();
InterlockOut();


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
          digitalWrite(CONTROLLINO_R5,HIGH); //activates relay/pin/or whatever
            delay(1000);                
            digitalWrite(CONTROLLINO_R5,LOW); //turns off relay/pin/or whatever after .5 sec
          Serial.println("VALID PASSWORD "); //
             password.reset(); //resets password after correct entry
    }else{
          digitalWrite(CONTROLLINO_R5,LOW);
          Serial.println("INVALID PASSWORD ");
             password.reset(); //resets password after INCORRECT entry
    }
} 

//////////////
//E stop Function for Laser and Robot
void EStop(){
 if (Serial.available()){
    int inByte=Serial.read();
   switch(inByte){
    case 'a':
      //E-STOP FOR LASER R0,R1 ON
      digitalWrite(CONTROLLINO_R0,HIGH);
      digitalWrite(CONTROLLINO_R1,HIGH);
    break;
    case 'b':
      //E-STOP FOR LASER R0,R1 OFF
      digitalWrite(CONTROLLINO_R0,LOW);
      digitalWrite(CONTROLLINO_R1,LOW);
      break;
    case 'c':
      //E-STOP FOR ROBOT R2,R3 ON
      digitalWrite(CONTROLLINO_R2,HIGH);
      digitalWrite(CONTROLLINO_R3,HIGH);
      break;
    case 'd':
      //E-STOP FOR ROBOT R2,R3 OFF
      digitalWrite(CONTROLLINO_R2,LOW);
      digitalWrite(CONTROLLINO_R3,LOW);
      break;
     default:
      //E Stops are NC
      digitalWrite(CONTROLLINO_R0,HIGH);
      digitalWrite(CONTROLLINO_R1,HIGH);
      digitalWrite(CONTROLLINO_R2,HIGH);
      digitalWrite(CONTROLLINO_R3,HIGH);
      break;
  }
  
}
}



/// Function for reading the Interlock from the INNER hall
void InterlockOut(){

  //INTERLOCK READING
  inner_hall_door=digitalRead(CONTROLLINO_I17);
 switch (inner_hall_door) {
  case HIGH:
     digitalWrite(CONTROLLINO_R4,LOW);
    break;
  case LOW:
    digitalWrite(CONTROLLINO_R4,HIGH);
    break;
  default:
    digitalWrite(CONTROLLINO_R4,HIGH);
    break;
}


}

/// Function for reading the Interlock from the OUTER hall
void InterlockIn(){
  //INTERLOCK READING  
  outer_hall_door=digitalRead(CONTROLLINO_I16);

switch (outer_hall_door) {
  case HIGH:
     digitalWrite(CONTROLLINO_R4,LOW);
    break;
  case LOW:
    digitalWrite(CONTROLLINO_R4,HIGH);
    break;
  default:
    digitalWrite(CONTROLLINO_R4,HIGH);
    break;
}


//  while (outer_hall_door==LOW){
//    digitalWrite(CONTROLLINO_R4,HIGH);
//  }
//   digitalWrite(CONTROLLINO_R4,LOW);


  
//  if (outer_hall_door==LOW)
//  {
//  Serial.println("Opened _outer_hall_door");
//  digitalWrite(CONTROLLINO_R4,HIGH);
//  }
//  else{
//  Serial.println("Closed _outer_hall_door");
//  
 
 // }

}


  
