#include <SPI.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <MFRC522.h>
#include <WiFiUdp.h>

#define BUZZER D2
#define LED D1

WiFiUDP wifiudp;
NTPClient timeClient(wifiudp, "pool.ntp.org");

MFRC522 mfrc522(D4, D3);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.setDebugOutput(false);
  Serial.println();
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  connectToNetwork();

  timeClient.begin();
  timeClient.setTimeOffset(16200);

  analogWriteRange(100);

  SPI.begin();           
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
  pinMode( LED, OUTPUT);
  pinMode( BUZZER, OUTPUT);

  digitalWrite(LED, LOW);
}

void connectToNetwork(){
  WiFi.begin("daryoosh", "34275767");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
  }
}

String getTime(){
  String formattedTime = timeClient.getFormattedTime();
  return formattedTime;
}
String attendanceTime;
void loop() {
  if (mfrc522.PICC_IsNewCardPresent()){
    if(mfrc522.PICC_ReadCardSerial()){
      Serial.println(">>RFID TAG INFORMATION:");

      mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
      Serial.print("\n");
      Serial.println("FINISHED");
      timeClient.update();
      attendanceTime = getTime();
      Serial.println(">>ATTENDANCE TIME:");
      Serial.println(attendanceTime);
      if (attendanceTime>= "01:11:00" && attendanceTime<= "02:22:00"){
          digitalWrite(LED, HIGH);
          delay(3000);
          digitalWrite(LED, LOW);
      }
      else{
        analogWrite(BUZZER, 50);
        delay(3000);
        analogWrite(BUZZER, 0);
      }
      Serial.println("---------");
    }
  }



}
