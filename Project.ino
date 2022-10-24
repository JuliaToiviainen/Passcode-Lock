#author @Julia Toiviainen

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Password.h>
#include <Servo.h>
#include <Wire.h>


const byte ROWS = 4;
const byte COLLUMS = 4; 
boolean noPassword = true;
int LOCKED = 0;
int UNLOCKED = 1;
int state = LOCKED;

//create servo object to control a servo
Servo motor;
int pos = 0; //position for the servo 

char keys[ROWS][COLLUMS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};

//need to connect these rows and colums to these specific places on UNO board
byte rowConnect[ROWS] = {9, 8, 7, 6};
byte colConnect[COLLUMS] = {5, 4, 3, 2};

char phrase[] = "4578";
Password password = Password(phrase);

Password oldPassword = Password(phrase);

char phrase2[4] = "";
Password newPassword = Password(phrase2);

//making the keyPad
Keypad myKeypad = Keypad(makeKeymap(keys), rowConnect, colConnect, ROWS, COLLUMS);

//assigning pins to corresponding led spots numbers
int LED1 = 10; //green light, if the password is correct
int LED2 = 12; //red light

boolean correct = false;
boolean allReds = false;
int count = 0;
boolean firstWrong = true;

//making the lcd board
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);


void setup() {

//lcd light and background
  lcd.backlight();
  lcd.init();
  
  Serial.begin(9600);

  //motor to the right place 
  motor.write(13);
  motor.attach(13);    

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}


void loop() {

// lcd board and the print 
 lcd.setCursor(0, 0);
 lcd.print("Enter password:");
 
  if (state == LOCKED){
    while (noPassword){
      getPassword();
    }
    state = UNLOCKED;
    openLock();
  }
  else{
    myKeypad.waitForKey(); 
    closeLock();
    state = LOCKED;
    digitalWrite(LED1, LOW);
    noPassword = true;
  }
} 


boolean getPassword(){

  do {
    char key1 = myKeypad.waitForKey();  
    password.append(key1);
    Serial.print(key1);
    lcd.clear();
    lcd.print(key1);

    char key2 = myKeypad.waitForKey();  
    password.append(key2);
    Serial.print(key2);
    lcd.print(key2);

    char key3 = myKeypad.waitForKey();  
    password.append(key3);
    Serial.print(key3);
    lcd.print(key3);

    char key4 = myKeypad.waitForKey();  
    password.append(key4);
    Serial.print(key4);
    lcd.print(key4);

    if (password.evaluate()){
      noPassword = false;
      digitalWrite(LED2, LOW);
 
      digitalWrite(LED1, HIGH);
      Serial.println(" Correct");
      lcd.clear();
      lcd.print("Correct");
      delay(100);
      password.reset();
      break;
    }

    if (key1 == '#'){
      changePassword();
      break;
    }
  
    else{
      count++;
      Serial.println(" Incorrect");
      lcd.clear();
      lcd.print("Incorrect, "); 
      lcd.setCursor(0,1);
      lcd.print("2 tries left");
      digitalWrite(LED2, HIGH);

     if (count == 2){
        lcd.clear();
        lcd.print("Incorrect, ");
        lcd.setCursor(0,1);
        lcd.print("1 try left");
        digitalWrite(LED2, LOW);
        delay(500);
        digitalWrite(LED2, HIGH);
      }
      if (count == 3){
        lcd.clear();
        lcd.print("Incorrect, ");
        lcd.setCursor(0,1);
        lcd.print("status: locked");
        digitalWrite(LED2, LOW);
        delay(500);
        digitalWrite(LED2, HIGH);
        allReds = true; 
      }
     
      //if the users has used all it attemps, the program stops for a while
      if (allReds){
        digitalWrite(LED2, HIGH);
        //make the program stop for some time
        delay(50000);
        digitalWrite(LED2, LOW);
      }
    }
    password.reset();
  }
  while (password.evaluate() == false);
}


boolean openLock(){ 
  for (pos = 10; pos <= 180; pos += 13) { // goes from 0 degrees to 180 degrees
    motor.write(pos);         
    delay(150);                       
  }                   
}


boolean closeLock(){
  for (pos = 180; pos >= 10; pos -= 13) { // goes from 0 degrees to 180 degrees
    motor.write(pos);            
    delay(150);                       
  }                   
}


void changePassword(){

  password.reset();
  Serial.println();
  Serial.println("Enter old password: ");
  lcd.clear();
  lcd.print("Enter old ");
  lcd.setCursor(0,1);
  lcd.print("password: ");
  delay(1000);

  char key1x = myKeypad.waitForKey();  
  oldPassword.append(key1x);
  Serial.print(key1x);
  lcd.clear();
  lcd.print(key1x);

  char key2x = myKeypad.waitForKey();  
  oldPassword.append(key2x);
  Serial.print(key2x);
  lcd.print(key2x);

  char key3x = myKeypad.waitForKey();  
  oldPassword.append(key3x);
  Serial.print(key3x);
  lcd.print(key3x);

  char key4x = myKeypad.waitForKey();  
  oldPassword.append(key4x);
  Serial.print(key4x);
  lcd.print(key4x);
  
  if (oldPassword.evaluate()){
    Serial.println();
    Serial.println("Enter new password: ");
    lcd.clear();
    lcd.print("Enter new ");
    lcd.setCursor(0,1);
    lcd.print("password: ");

    char key1y = myKeypad.waitForKey();  
    Serial.print(key1y);
    lcd.clear();
    lcd.print(key1y);
    phrase[0] = key1y;

    char key2y = myKeypad.waitForKey();  
    Serial.print(key2y);
    lcd.print(key2y);
    phrase[1] = key2y;

    char key3y = myKeypad.waitForKey();  
    Serial.print(key3y);
    lcd.print(key3y);
    phrase[2] = key3y;

    char key4y = myKeypad.waitForKey();  
    Serial.print(key4y);
    lcd.print(key4y);
    phrase[3] = key4y;
    
    Serial.println();
    Serial.println("New password is set!");
    Serial.println();
    
    lcd.clear();
    lcd.print("New password ");
    lcd.setCursor(0,1);
    lcd.print("is set!");
    
  }
}
