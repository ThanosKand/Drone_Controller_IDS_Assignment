#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TelloDrone.h>
#include <string.h>
#include <iostream>
#include <sstream> 

TelloDrone drone; //create an instance of TelloDrone

const int red_LED=23;
const int green_LED =18;

const int up_btn=22;
const int down_btn=19;
const int cw_btn= 4;       
const int ccw_btn= 5;

const int speed_potent=33;

const int VRx = 34; 
const int VRy= 35;  
const int SW = 15; 

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

  pinMode(red_LED, OUTPUT);
  pinMode(green_LED, OUTPUT);

  digitalWrite(red_LED, HIGH);
  digitalWrite(green_LED, LOW);

  pinMode(up_btn, INPUT_PULLUP);
  pinMode(down_btn, INPUT_PULLUP);
  pinMode(cw_btn, INPUT_PULLUP);
  pinMode(ccw_btn, INPUT_PULLUP);

  pinMode(speed_potent, INPUT);

  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  drone.connect("TELLO-FE2FC4", "");
  delay(5000);

  sendCommand("speed 20");
  flying=false;
}

void loop(){

  if(drone.connected){

      digitalWrite(green_LED, HIGH); 
      digitalWrite(red_LED, LOW); 

      //Joystick:
      joystick_x = analogRead(VRx);
      joystick_y = analogRead(VRy);
      joystick_btn = digitalRead(SW);

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

      //Potentiometer:
      potent_speed=analogRead(speed_potent);
      mapped_potent_speed=map(potent_speed, 0, 4095, 20, 100); 
  
      if(mapped_potent_speed!=drone_speed){
          drone_speed=mapped_potent_speed;
          String speedy="speed ";
          speedy=speedy + drone_speed;
          sendCommand(speedy.c_str());
          delay(200); 
        } 

      //Buttons:
      if(digitalRead(up_btn)==LOW){
        sendCommand("up 20");
        delay(150);
      }

      if(digitalRead(down_btn)==LOW){
        sendCommand("down 20");
        delay(150);
      }

      if(digitalRead(cw_btn)==LOW){
        sendCommand("cw 40");
        delay(150);
      }

      if(digitalRead(ccw_btn)==LOW){
        sendCommand("ccw 40");
        delay(150);
      }


        /*if(!((joystick_x>1800 && joystick_x<1900) && (joystick_y>1700 && joystick_y<1800))){
          String com="rc ";
          com= com + map_x + " " + map_y + " 0 0";
          sendCommand(com.c_str());
        } */
       

     /* if((joystick_x>= 1700 && joystick_x<=2000) && (joystick_y<10)){
        sendCommand("forward 50");
        delay(150);
      }
      
      if((joystick_x>= 1700 && joystick_x<=2000) && joystick_y>=4000){
        sendCommand("back 50");
        delay(150);
      }
      

      if(joystick_x<10  && (joystick_y>1600 && joystick_y<2000)){
        sendCommand("left 50");
        delay(150);
      }

      if(joystick_x>=3900 && (joystick_y>1600 && joystick_y<2000)){
        sendCommand("right 50");
        delay(150);
      }  
      */

      
      /*if(digitalRead(upbtn)==LOW){
        sendCommand("left 100");
        delay(1000);
      } */

     /* if(digitalRead(landbtn)==LOW){
      sendCommand("land");
      flying=false;
      delay(1000);
      }  */
      



      /* if(map_x<512 && map_y==512){
        sendCommand("right 100");
        delay(1000);
      }

      if(map_x>512 && map_y==512){
        sendCommand("left 100");
        delay(1000);
      }
      */
      
      
      
      
      
      
      
      
      
      /*if(btn.pressed==true){
        sendCommand("land");
      } 

       if(digitalRead(btn)==LOW){
        locked=!locked;
        delay(500);
      } */
    






    /*sendCommand("up 100");
    delay(2000);

    sendCommand("down 100");
    delay(5000);
    */

    
    //sendCommand("command");
  /* if(btn.pressed){
      sendCommand("land");
       delay(5000);
   } */

   
 // Serial.println("...repeating in 5 sec..."); 
  //drone.getResponse();
  delay(200);
  
  }
  else {
    digitalWrite(red_LED, HIGH);
    digitalWrite(green_LED, LOW);
  }
} 

//drone.commandResponse("battery?");
      //drone.commandResponse("time?");
      
      
      

     /* while(drone.commandResponse != "ok"){
        Serial.println("Waiting..");
      }
      */
      //btn.loop;

     /* if(btn.pressed){
        sendCommand("takeoff");
        delay(500);
      } */
      

      /* if(btn.pressed){
        sendCommand("land");
        delay(500);
      } */
    

     /* if(millis()-previous_time > WAIT_TIME)
    {
      Serial.println("Waiting......");
    } */

     /* if(analogRead(speed_potent
    )!=-1){

        int val = analogRead(speed_potent
      );
        //Serial.println(val);
        val = map(val, 0, 4095, 10, 100);
        //string s="speed '" + val;
        sendCommand("speed '" + val);
      }
      */
      //"Sending command '" + message + "'"

      
      // This works!