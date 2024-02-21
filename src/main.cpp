	
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
 

#define PN532_SS   (5) 
#define PN532_SCK  (18) 
#define PN532_MOSI (23) 
#define PN532_MISO (19) 
#define PN532_IRQ  (34)
 
Adafruit_PN532 nfc(PN532_SS, PN532_SCK, PN532_MISO, PN532_MOSI);
 
void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Hello!");
 
  nfc.begin();
 
 
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) 
  {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A card");
 
}
 
void loop(void) 
{
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
 
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
 
    for (uint8_t i=0; i < uidLength; i++) 
    {
    
      Serial.print(" 0x");
      Serial.print(uid[i], HEX); 
    }
    Serial.println("");
  // Wait 1 second before continuing
  delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a cardd
    Serial.println("Timed out waiting for a card");
  }
}
