/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLFH1spJK9"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "1i0XXdP0w8eW4ZAs1InXLW-TXDJzeL0R"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] = "Redmi Note 10 Pro";
char pass[] = "mithulmurali";

BlynkTimer timer;

int state=0;
uint8_t id;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // in Blynk app writes values to the Virtual Pin 1

  Serial.print("V1 Button value is: ");
  Serial.println(value);
  digitalWrite(33, value);
  state=value;
  
  // Update state
  Blynk.virtualWrite(V3, value==0?"DOOR LOCKED":"DOOR UNLOCKED");
}

//// This function sends Arduino's uptime every second to Virtual Pin 2.
//void myTimerEvent()
//{
//  // You can send any value at any time.
//  // Please don't send more that 10 values per second.
//  Blynk.virtualWrite(V2, millis() / 1000);
//}

void setup()
{
  // Debug console
  Serial.begin(57600);
  Serial1.begin(115200);
  pinMode(33, OUTPUT);
  Blynk.begin(auth, ssid, pass);


//FINGERPRINT

  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

}

void loop()
{
  Blynk.run();
 
  
  getFingerprintID();
//  timer.run();
}
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");

    if(state==0){
      digitalWrite(33, HIGH);
      // Update state
      Blynk.virtualWrite(V0, 1);
      Blynk.virtualWrite(V3, "DOOR UNLOCKED");
      state=1;
    }
    else{
      digitalWrite(33, LOW);
      // Update state
      Blynk.virtualWrite(V0, 0);
       Blynk.virtualWrite(V3,"DOOR LOCKED");
      state=0;
    }
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  Blynk.virtualWrite(V1,"SCANNED BY ",getname(finger.fingerID));

  return finger.fingerID;
}
String getname(int id){
  switch(id){
    case 1:
    return "MITHUL Left";
    case 2:
    return "MITHUL Right";
    case 3:
    return "AMITH";
    default:
    return "Unknown person";
   }
}
//blynkbutton to enroll for new id
