#include <TinyGPS++.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>

// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;
String responce = "";
double latitude, longitude;
#define gpspin 7
String Link = "";
//int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);
SoftwareSerial SIM800(10, 11); // RX, TX //it has priority beacuase it was called later. So now listen function is necessary

String SIM800_send(String incoming) //Function to communicate with SIM800 module 
{
    SIM800.println(incoming); delay(100); //Print what is being sent to GSM module 
    String result = "";
    while (SIM800.available()) //Wait for result 
    {
    char letter = SIM800.read();
    result = result + String(letter); //combine char to string to get result 
    }
    
return result; //return the result 
}

void setup()
{
  //POWERING THE GPS MODULE
  pinMode(gpspin,OUTPUT);
  digitalWrite(gpspin,HIGH);
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(9600);
  SIM800.begin(9600); //Software serial called SIM800 to speak with SIM800 Module

  responce = SIM800_send("ATH"); //Hand up the incoming call using ATH
  Serial.print ("Responce:"); Serial.println(responce); 
  delay(1000);
  gpsSerial.listen();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  boolean gpscoosOK = false;
  while (gpsSerial.available() > 0 && gpscoosOK == false){
    if (gps.encode(gpsSerial.read())){
      //displayInfo();
      if (gps.location.isValid())
  {
    /*Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters()); */
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    gpscoosOK = true;
  }
    }
  }
  SIM800.listen();
    if (SIM800.available()) {
     responce = SIM800_send("ATE0"); //Disable Echo
     delay (1000);
     
     prepare_message(); delay(1000); //use prepare_message funtion to prepare the link with the obtained LAT and LONG co-ordinates 

     SIM800.println("AT+CMGF=1"); //Set the module in SMS mode
     delay(1000);
     
     SIM800.println("AT+CMGS=\"+30\""); //Send SMS to this number 
     delay(1000);

     SIM800.println(Link); // we have send the string in variable Link 
     delay(1000);

     SIM800.println((char)26);// ASCII code of CTRL+Z - used to terminate the text message 
     delay(1000); 
    }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  //if (millis() > 11000 && gps.charsProcessed() < 10)
  //{
   // Serial.println("No GPS detected");
  //if 
    //while(true);
  //}
  
//PUTTING SIM800L TO SLEEP MODE
  responce = SIM800_send("AT+CSCLK=1");
  delay(500);
// PUTTING THE MCU TO SLEEP - THIS PRESUMABLY POWERS OFF THE GPS AS WELL-HAVE TO CHECK THAT SIMPLY BY VIEWING THE GPSs LED
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(500);
  sleep_cpu();
}

/* void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
} */

void prepare_message()
{
  //Sample Output for AT+CIPGSMLOC=1,1   ==> +CIPGSMLOC: 0,75.802460,26.848892,2019/04/23,08:32:35 //where 26.8488832 is Lattitude and 75.802460 is longitute 
  //int first_comma = responce.indexOf(','); //Find the position of 1st comma 
  //int second_comma = responce.indexOf(',', first_comma+1); //Find the position of 2nd comma 
  //int third_comma = responce.indexOf(',', second_comma+1); //Find the position of 3rd comma 
  //for(int i=first_comma+1; i<second_comma; i++) //Values form 1st comma to 2nd comma is Longitude 
   // {Longitude = Longitude + responce.charAt(i); }
//
  //for(int i=second_comma+1; i<third_comma; i++) //Values form 2nd comma to 3rd comma is Latitude 
    //{Latitude = Latitude + responce.charAt(i);} 

  //Serial.println(Latitude); Serial.println(Longitude); 
  Link = "http://www.google.com/maps/place/" + String(latitude,6) +"," + String(longitude,6); //Update the Link with latitude and Logitude values 
  Serial.println(Link);   //OR MAKE THEM ,5
}
