// Name: Alwin George Christie
// Date: 18 August 2021
// Description: The project is based on a password door lock system that includes Arduino UNO R3,
//              Keypad, LCD, and Servo motor. The circuit will also include a NeoPixel LED strip 
//              so that the user will be able to understand if he entered the correct or 
//              incorrect key.


// References
// https://www.tinkercad.com/things/6nGD6BHf17z-copy-of-using-keypad-4x4-with-arduino/editel
// https://www.tinkercad.com/things/jR8DHye4dlk-copy-of-neopixel-strip/editel?tenant=circuits

#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

#include <Adafruit_NeoPixel.h>
#define PIN        A3
#define NUMPIXELS 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino
byte rowPins[numRows] = {9,8,7,6}; //Rows 1 to 4
byte colPins[numCols]= {5,4,3,2}; //Columns 1 to 4

//Initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//Initialise the Serial LCD.
LiquidCrystal lcd(A1,A2,13,12,11,10); //These pin numbers are hard coded in on the serial backpack board.

Servo myservo;

//variables

// #########################################

//The password should be 4 character alphanumeric consisting of digits,letters (A,B,C,D) and symbols (*,#)

char* password= "A123"; // The password for opening the door

// #########################################



int currentposition = 0;
int x =10;//variable used to set cursor in lcd
int y;// variable used to keep the count of characters pressed in keypad
int trials;//variable used to keep the count for number of times wrong password was entered




void setup(){
  
  lcd.begin (16,2); //Initialize the LCD.
  
  Serial.begin(9600);
  
  myservo.attach(A0); // Servo attached to port A0
  
  pixels.begin();//Initialize the LED Strip.
}



void loop(){
  
  myservo.write(180);//Set micro servo to lock door.  
  if(y == 0){
    //Asking to enter the password
    lcd.setCursor(1,0);
    lcd.print("Enter Password");
    lcd.setCursor(2,1);
    lcd.print("TO OPEN DOOR");
    //Setting LED Strip to red color
    ledstripOFF();
  }
 
  //If key is pressed, this key is stored in 'keypressed' variable
  //If key is not equal to 'NO_KEY', then this key is printed out in the LCD
  
  char keypressed = myKeypad.getKey();
  
  if (keypressed != NO_KEY){
    if(y == 0){  
      lcd.clear();
    }
    
    lcd.setCursor(0,0);
    lcd.print("Password: ");
    lcd.setCursor(x,0);
    lcd.print("*");
    x++;
    if(x == 14){
      x=10;
      lcd.setCursor(x,0);
    }
    
    y++;
  }
  
  //Checking if the keypressed is same as in password
  if(keypressed == password[currentposition]){
    
    currentposition++;
    
    ledstripON();
    
    opendoor();
    
  }else if(y==4){
    
    invalidpassword();
    
    ledstripOFF();
    
    delay(3000);
    lcd.clear();
    currentposition = 0;
    y=0;
  }

}

void opendoor(){//function to open door
  if(currentposition == 4){
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Access Granted");
        lcd.setCursor(4,1);
        lcd.print("WELCOME!");
        currentposition=0;
        
        myservo.write(90);
        delay(5000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("THE DOOR CLOSES");       
        for(int l=10;l>0;l--){
          lcd.setCursor(0,2);
          lcd.print("             ");
          lcd.setCursor(0,2);
          lcd.print("IN " + String(l) + " SECONDS");
          delay(1000);
        } 
        myservo.write(180);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("DOOR  LOCKED");
        ledstripOFF();
        delay(3000);
    
        lcd.clear();
        y=0;
        trials=0;
      }
}

void invalidpassword(){//Checking how many times the incorrect key is entered
  trials++;
      lcd.clear();
      if(trials == 1){
        lcd.setCursor(0,0);
        lcd.print("Wrong Password");
        lcd.setCursor(0,1);
        lcd.print("Only 2 Try Left");
      }else if(trials == 2){
        lcd.setCursor(0,0);
        lcd.print("Wrong Password");
        lcd.setCursor(0,1);
        lcd.print("Only 1 Try Left");
      }else if(trials == 3){
        lcd.setCursor(0,0);
        lcd.print("Authorized");
        lcd.setCursor(0,1);
        lcd.print("Personnel Only");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Try Again in");
        lcd.setCursor(0,1);
        lcd.print("5 Minutes");//The wait time is not correct as it will take more time in real life.
        delay(5000);
        lcd.setCursor(0,1);
        lcd.print("3 Minutes");
        delay(5000);
        lcd.setCursor(0,1);
        lcd.print("1 Minutes");
        delay(5000);
        trials=0;
      }
}

void ledstripON(){//To set green color on LED strip while correct password is entered.
  if(currentposition == 4){
    for(int k=0;k<4;k++){
    pixels.setPixelColor(k, pixels.Color(0, 255, 0));
    pixels.show();
    }
  }
}    

void ledstripOFF(){//To set LED strip color to red after locking the door
  for(int k=0;k<4;k++){
    pixels.setPixelColor(k, pixels.Color(255, 0, 0));
    pixels.show();
  }
}