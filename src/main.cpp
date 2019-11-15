#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TelloDrone.h>
#include <string.h>
#include <iostream>
#include <sstream> 

TelloDrone drone; //create an instance of the class 'TelloDrone'

//Allocate the controller's components to Esp32's GPIO pins

//LEDs:
const int red_LED=23;
const int green_LED =18;

//Buttons:
const int up_btn=22;
const int down_btn=19;
const int cw_btn= 4;       
const int ccw_btn= 5;

//Potentiometer:
const int speed_potent=33;

//Joystick:
const int VRx = 34; 
const int VRy= 35;  
const int SW = 15; 

//Define, initialize variables which are used in the program
int joystick_x = 0;
int joystick_y = 0;
int joystick_btn = 0;

int map_x = 0;
int map_y = 0;

int potent_speed;
int mapped_potent_speed;
int drone_speed=20;

boolean flying;
boolean stable;

void sendCommand(string cmd){

  Serial.print("Executing command: ");
  Serial.println(cmd.c_str());
  drone.sendCommand(cmd);   
}

void setup(){
 
  Serial.begin(9600);

  //Assign functioning mode to pins(INPUT/ OUTPUT)
  
  //LEDs:
  pinMode(red_LED, OUTPUT);
  pinMode(green_LED, OUTPUT);
  digitalWrite(red_LED, HIGH); // red LED is set ON
  digitalWrite(green_LED, LOW); // green LED is set OFF

  //Buttons:
  pinMode(up_btn, INPUT_PULLUP);
  pinMode(down_btn, INPUT_PULLUP);
  pinMode(cw_btn, INPUT_PULLUP);
  pinMode(ccw_btn, INPUT_PULLUP);

  //Potentiometer:
  pinMode(speed_potent, INPUT);

  //Joystick:
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  drone.connect("TELLO-FE2FC4", ""); //Achieve connection between Esp32 board and Tello drone
  delay(5000);
  
  sendCommand("speed 20"); //Initialize drone's speed to 20cm/s
  flying=false;
}

void loop(){

  if(drone.connected){ //This part is executed only if a connection is achieved

      digitalWrite(green_LED, HIGH); 
      digitalWrite(red_LED, LOW); 

      //Joystick (forward/ back/ left/ right/ takeoff/ land):
      joystick_x = analogRead(VRx);
      joystick_y = analogRead(VRy);
      joystick_btn = digitalRead(SW);

      //Map the acquired analogue values to an accepted range by the drone    
      map_x= map(joystick_x, 0, 4095, -50, 50); 
      map_y= map(joystick_y, 0, 4095, -50, 50);

      if(((map_x<10 && map_x>-20) && (map_y>-30 && map_y<30))==false){
          String com="rc ";
          int mX= map_x;
          int mY= - map_y;
          com= com + mX + " " + mY + " 0 0";
          sendCommand(com.c_str());
          delay(100);
          stable=false;
        }  

      //If the Joystick is back to the 'resting' position and the drone is moving -> send 'rc 0 0 0 0' command 
      //-> make it stop moving and stay in the same position
      if(((map_x<10 && map_x>-20) && (map_y>-30 && map_y<30))==true && stable==false){ 
          String com="rc ";
          com= com + 0 + " " + 0 + " 0 0";
          sendCommand(com.c_str());
          delay(100);
          stable=true;
        }  

      if(digitalRead(SW)==LOW && flying==false){
        sendCommand("takeoff");
        flying=true;
        delay(200);
     } 
      
      if(digitalRead(SW)==LOW && flying==true){
        sendCommand("land");
        flying=false;
        delay(200);
      }

      //Potentiometer (set speed):
      potent_speed=analogRead(speed_potent);

      //Map the acquired analogue values to an accepted range by the drone 
      mapped_potent_speed=map(potent_speed, 0, 4095, 20, 100);
  
      if(mapped_potent_speed!=drone_speed){ 
          drone_speed=mapped_potent_speed; 
          String speedy="speed ";
          speedy=speedy + drone_speed;
          sendCommand(speedy.c_str());
          delay(200); 
        } 

      //Buttons (up/ down/ cw/ ccw):
      if(digitalRead(up_btn)==LOW){
        sendCommand("up 30");
        delay(150);
      }

      if(digitalRead(down_btn)==LOW){
        sendCommand("down 30");
        delay(150);
      }

      if(digitalRead(cw_btn)==LOW){
        sendCommand("cw 23");
        delay(150);
      }

      if(digitalRead(ccw_btn)==LOW){
        sendCommand("ccw 23");
        delay(150);
      }  
   
  delay(200);
  
  }
  else {
    //In case the connection is lost:
    digitalWrite(red_LED, HIGH);
    digitalWrite(green_LED, LOW);
  }
} 
