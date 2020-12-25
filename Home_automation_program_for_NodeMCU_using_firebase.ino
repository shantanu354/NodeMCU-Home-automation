#include <ArduinoJson.h>
#include <Firebase.h>  
#include <FirebaseArduino.h>  
#include <FirebaseCloudMessaging.h>  
#include <FirebaseError.h>  
#include <FirebaseHttpClient.h>  
#include <FirebaseObject.h>  
#include <ESP8266WiFi.h>  
#include <FirebaseArduino.h>  
  
// Set these to run example.  
#define FIREBASE_HOST "home-automation-401.firebaseio.com"  
#define FIREBASE_AUTH "R5TFNyTOP6xASytBVAsmujVNsUOteRpEugfgnfSV"  
#define WIFI_SSID "Anil Niwas"  
#define WIFI_PASSWORD "gitah3Giaghailie"  


int pin1 = 9; // Sets the value for Pin1 of Relay
int pin2 = 10; // Sets the value for Pin2 of Relay
int pin3 = 4; // Sets the value for Pin3 of Relay
int pin4 = 5;  // Sets the value for Pin4 of Relay



int ledPin = pin4;                // choose the pin for the LED
int inputPin = 14;               // choose the input pin (for PIR sensor)
int pirState = HIGH;             // we start, assuming no motion detected
int val = 0;  
  
void setup() {  
  
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, HIGH);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, HIGH);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, HIGH);
  pinMode(pin4, OUTPUT);
  digitalWrite(pin4, HIGH);
  Serial.begin(115200);
  delay(10);
 
  Serial.begin(9600);  
  
  // connect to wifi.  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  Serial.print("connecting");  
  while (WiFi.status() != WL_CONNECTED) {  
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print(".");  
    delay(500);  
  }  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(WiFi.localIP());  
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
}

void readFirebase(){
  if(Firebase.getString("NodeMCU/switch_status_phone") == "ON"){
    digitalWrite(pin1, LOW);
  }else digitalWrite(pin1, HIGH);
  if(Firebase.getString("NodeMCU/switch_status_office_fan") == "ON"){
    digitalWrite(pin2, LOW);
  }else digitalWrite(pin2, HIGH);
  if(Firebase.getString("NodeMCU/switch_status_office_light") == "ON"){
    digitalWrite(pin3, LOW);
  }else digitalWrite(pin3, HIGH);
}
  
void loop() {   
  Serial.println(val);
  val = digitalRead(inputPin);
  if (val == HIGH) {
    digitalWrite(ledPin, LOW);
    if (pirState == HIGH) {
      Serial.println("Motion detected!");
      pirState = LOW;
      Firebase.setString("NodeMCU/switch_status_office_fan", "ON");  
      digitalWrite(pin2, LOW);
      Firebase.setString("NodeMCU/switch_status_office_light", "ON");  
      digitalWrite(pin3, LOW);
      delay(300000);
    }
  } else {
    digitalWrite(ledPin, HIGH);
    if (pirState == LOW){
      Serial.println("Motion ended!");
      pirState = HIGH;
      Firebase.setString("NodeMCU/switch_status_office_fan", "OFF");  
      Firebase.setString("NodeMCU/switch_status_office_light", "OFF"); 
    }
  }
  readFirebase();
}  



/*
#include <Firebase.h>  
#include <FirebaseArduino.h>  
#include <FirebaseCloudMessaging.h>  
#include <FirebaseError.h>  
#include <FirebaseHttpClient.h>  
#include <FirebaseObject.h>  
  
//  
// Copyright 2015 Google Inc.  
//  
// Licensed under the Apache License, Version 2.0 (the "License");  
// you may not use this file except in compliance with the License.  
// You may obtain a copy of the License at  
//  
//     http://www.apache.org/licenses/LICENSE-2.0  
//  
// Unless required by applicable law or agreed to in writing, software  
// distributed under the License is distributed on an "AS IS" BASIS,  
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
// See the License for the specific language governing permissions and  
// limitations under the License.  
//  
  
// FirebaseDemo_ESP8266 is a sample that demo the different functions  
// of the FirebaseArduino API.  
  
#include <ESP8266WiFi.h>  
#include <FirebaseArduino.h>  
  
// Set these to run example.  
#define FIREBASE_HOST "nodemcu-demo-697d8.firebaseio.com"  
#define FIREBASE_AUTH "YOUR_FIREBASE_AUTH"  
#define WIFI_SSID "NETGEAR64"  
#define WIFI_PASSWORD "*JAVATPOINT#"  
  
void setup() {  
  Serial.begin(9600);  
  
  // connect to wifi.  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  Serial.print("connecting");  
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");  
    delay(500);  
  }  
  Serial.println();  
  Serial.print("connected: ");  
  Serial.println(WiFi.localIP());  
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
}  
  
int n = 0;  
  
void loop() {  
  // set value  
  Firebase.setFloat("number", 42.0);  
  // handle error  
  if (Firebase.failed()) {  
      Serial.print("setting /number failed:");  
      Serial.println(Firebase.error());    
      return;  
  }  
  delay(1000);  
    
  // update value  
  Firebase.setFloat("number", 43.0);  
  // handle error  
  if (Firebase.failed()) {  
      Serial.print("setting /number failed:");  
      Serial.println(Firebase.error());    
      return;  
  }  
  delay(1000);  
  
  // get value   
  Serial.print("number: ");  
  Serial.println(Firebase.getFloat("number"));  
  delay(1000);  
  
  // remove value  
  Firebase.remove("number");  
  delay(1000);  
  
  // set string value  
  Firebase.setString("message", "hello world");  
  // handle error  
  if (Firebase.failed()) {  
      Serial.print("setting /message failed:");  
      Serial.println(Firebase.error());    
      return;  
  }  
  delay(1000);  
    
  // set bool value  
  Firebase.setBool("truth", false);  
  // handle error  
  if (Firebase.failed()) {  
      Serial.print("setting /truth failed:");  
      Serial.println(Firebase.error());    
      return;  
  }  
  delay(1000);  
  
  // append a new value to /logs  
  String name = Firebase.pushInt("logs", n++);  
  // handle error  
  if (Firebase.failed()) {  
      Serial.print("pushing /logs failed:");  
      Serial.println(Firebase.error());    
      return;  
  }  
  Serial.print("pushed: /logs/");  
  Serial.println(name);  
  delay(1000);  
}  
*/
