/*
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;
String ok_rfid_1="ddfe4e1f";
int lock=7;
int buzzer=11;


/*
 * Initialize.
 */
void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card

    //Choose which lock below:
    pinMode(lock, OUTPUT);
    pinMode(buzzer, OUTPUT);
    
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}

void open_lock() {

  digitalWrite(lock, HIGH);
  delay(2000);
  digitalWrite(lock,LOW);
}

void beepyes() {
tone(buzzer, 1500, 500);
}

void beepno() {
tone(buzzer, 300, 150);
delay(200);
tone(buzzer, 300, 150);
delay(200);
tone(buzzer, 300, 150);
delay(200);
}

void loop() {

      // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    if (read_rfid==ok_rfid_1) {
      //ok, open the door.
      Serial.print("Valid: ");
      Serial.println(read_rfid);
      beepyes();
      open_lock();
    }else{
      Serial.print("Not Valid: ");
      Serial.println(read_rfid);
      beepno();
    }
      
}
