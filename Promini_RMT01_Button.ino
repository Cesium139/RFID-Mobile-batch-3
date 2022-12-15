//PROMINI BATCH 3 (EL UHF RMT01)

//~~~~~~~~~~~~~~~~~~~~~~~LCD SETUP~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
#include <Nextion.h>
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~SOFTWARE SERIAL SETUP~~~~~~~~~~~~~~~~~~~~//
#include <SoftwareSerial.h>
// software serial #1: RX = digital pin 11, TX = digital pin 10
SoftwareSerial portOne(11, 10); // Promini to ESP32 (11pro --> 23esp & 10pro --> 22esp)

// saaaoftware serial #2: RX = digital pin 12, TX = digital pin 13
SoftwareSerial portTwo(12, 13); // ID01 to Promini (12pro --> Yellow & 13pro --> Green)
//************************************************************//

//~~~~~~~~~~~~~~~~~~~READ Wifi From ESP~~~~~~~~~~~~~~~~//
void ReadWifi();
bool wifi = false;
//String msg = "";
char inChar ;
//************************************************************//

//~~~~~~~~~~~~~~~~~~~READ RFID FROM RMT01 SETUP~~~~~~~~~~~~~~~~//
void ReadRFID();

String R = "";
String R1 = "";
String R2 = "";
String R3 = "";
String R4 = "";
String inRFID = "";
String RFIDstring = "";
String RFIDstring2 = "";
String RFIDbaru = "";

bool RFIDComplete = false;
bool RFIDComplete1 = false;
bool RFID = true;

int btn = 46;
int buttonState;

uint8_t singleRead[8] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~SEND RFID TO LCD~~~~~~~~~~~~~~~~~~~~//
void SendRFIDtoLCD();
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~ READ LCD SETUP ~~~~~~~~~~~~~~~~~~~~//
bool cek = false;
bool LCDcomplete = false;
bool LCDcomplete1 = false;
String lcd = "";
String DataLCD = "";
String DataLCD1 = "";
void ReadLCD();
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~SEND RFID TO ESP32~~~~~~~~~~~~~~~~~~~~//
void SendRFIDtoESP32();
void SendRFIDtoESP32_1();
bool SendESP = false;
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~READ DATA TERNAK FROM ESP32~~~~~~~~~~~~~~~~~~~~//
void ReadTernak();

bool parsing = false;
bool parsing2 = false;
String sData, data[11];

String id_ternak1 = "";
String msg = "";
String id_ternak = "";
String bangsa = "";
String jenis_kelamin = "";
String status_ternak = "";
String id_induk = "";
String id_pejantan = "";
String fase = "";
String kandang = "";
String berat = "";
String umur = "";

bool TernakComplete = false;
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~SEND DATA TERNAK TO LCD~~~~~~~~~~~~~~~~~~~~//
void SendTernaktoLCD();
void SendPendaftaranLCD();
//*******************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~BUZZER~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
int buzzer = 42;
void Buzzer();
//*******************************************************************//



//################################################################################################//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // Serial Monitor Arduino IDE
  Serial1.begin(115200);    // Serial 1 Promini to LCD (19pro --> Tx LCD & 18Pro --> Rx LCD)
  Serial2.begin(19200);      // Serial 2 Promini to ESP32 (17pro --> 17esp & 16pro --> 16esp)- read data ternak
  portOne.begin(19200);      // Promini to ESP32 for Send RFID
  portTwo.begin(115200);    // ID01 to Promini for Read RFID
  pinMode(buzzer, OUTPUT);
  pinMode(btn, INPUT);
  RFID = true;
  digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer, LOW);
  delay(50);
}

void loop() {

  if (RFID) {
    buttonState = digitalRead(btn);
    //Serial.println(buttonState);
  }

  if (buttonState == HIGH) {
    Serial.println("Push button OK");
    ReadRFID();
  }

  ReadLCD();

  while (Serial2.available()) {
    inChar = Serial2.read();
    //Serial.println(inChar);
    ReadWifi();
    ReadTernak();
  }

  if (RFIDComplete1) {
    if (R3.length() == 22) {
      Serial.println(R3);
      //      Buzzer();
      SendRFIDtoLCD();
    }
    R1 = "";
    R2 = "";
    R3 = "";
    RFIDComplete1 = false;
    RFID = true;
  }

  if (LCDcomplete1) {
    Serial.println(DataLCD);
    SendRFIDtoESP32();
    lcd = "";
    DataLCD = "";
    LCDcomplete1 = false;
  }

  if (LCDcomplete) {
    RFID = false;
    Serial.println(DataLCD);
    SendRFIDtoESP32();
    lcd = "";
    DataLCD = "";
    LCDcomplete = false;
  }

  if (cek) {
    RFID = false;
    Serial.println(DataLCD1);
    portOne.print(DataLCD1);
    lcd = "";
    DataLCD = "";
    cek = false;
  }

  if (TernakComplete) {
    Serial.println(id_ternak);
    Serial.println(bangsa);
    //    Serial.println(jenis_kelamin);
    //    Serial.println(status_ternak);
    //    Serial.println(id_induk);
    //    Serial.println(id_pejantan);
    //    Serial.println(fase);
    //    Serial.println(kandang);
    //    Serial.println(berat);
    //    Serial.println(umur);
    //SendPendaftaranLCD();
    SendTernaktoLCD();
    RFID = true;
    TernakComplete = false;
  }


}
//################################################################################################//




void ReadWifi() {
  //  while (Serial2.available()) {
  //    msg = Serial2.readString();
  //Serial.println(msg);
  //char inChar = Serial2.read();
  if ( inChar == '=') {
    String CONNECTED = "CONNECTED";
    String psn = "Berhasil Reconnect";
    Serial.println("berhasil");
    Serial1.print("r0.pco=831");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("r0.bco=831");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("wifi.txt=\"" + CONNECTED + "\"");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("wifi.pco=2886");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("psn.txt=\"" + psn + "\"");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }


  if ( inChar == '+') {
    String DISCONNECTED = "DISCONNECTED";
    String psn1 = "Berhasil Disconnect";
    Serial.println("berhasil lupakan jaringan");
    Serial1.print("r0.pco=63488");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("r0.bco=63488");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("wifi.txt=\"" + DISCONNECTED + "\"");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("wifi.pco=63488");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.print("psn.txt=\"" + psn1 + "\"");
    Serial1.write(0xff);
    Serial1.write(0xff);
    Serial1.write(0xff);
  }

}

void ReadRFID() {

  //  if (RFID) {
  //    for (int i = 0; i < 7 ; i++) {
  //      portTwo.write(singleRead[i]);
  //      //Serial.println(singleRead[i],HEX);
  //      delay(25);
  //    }
  //  }

  for (int i = 0; i < 7 ; i++) {
    portTwo.write(singleRead[i]);
    //Serial.println(singleRead[i],HEX);
    delay(25);
  }

  while (portTwo.available()) {
    //    Buzzer();
    R = String(portTwo.read(), HEX);
    delay(15);
    inRFID += R;
    //Serial.println(inRFID);
    if (R == "7e") {
      R1 = inRFID;
      RFIDComplete = true;
      inRFID = "";
      //break;
    }
  }

  if (RFIDComplete) {
    //    Serial.println("");
    //    Serial.println(R1);
    char baca[100];
    R1.toCharArray(baca, 64, 0);
    if (baca[10] == '3')
    {
      Buzzer();
      for (int a = 11; a < 33 ; a++) {
        delay(15);
        R2 = (baca[a]);
        R3 += R2;
        Serial.print("");
      }
      //R1 = "";
      RFIDComplete = false;
      RFIDComplete1 = true;
    }
  }

}

void SendRFIDtoLCD() {
  RFIDstring2 = R3;
  Serial1.print("t0.txt=\"" + (RFIDstring2) + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void ReadLCD() {
  while (Serial1.available()) {
    lcd = Serial1.readString();
    if (lcd == "kelahiran" || lcd == "Ternak Baru") {
      DataLCD = "";
      DataLCD += "/";
      DataLCD += RFIDstring2;
      DataLCD += "/";
      DataLCD += lcd;
      DataLCD += "/";
      DataLCD += "&";
      LCDcomplete = true;
      break;
    }

    if (lcd == "1" || lcd == "3") {
      DataLCD = "";
      DataLCD += "*";
      DataLCD += lcd;
      DataLCD += "*";
      DataLCD += "?";
      LCDcomplete1 = true;
      break;
    }

    if (lcd == "4") {
      RFIDComplete1 = true;
      break;
    }


    if (lcd == "2") {
      DataLCD1 = "";
      DataLCD1 += "#";
      DataLCD1 += RFIDstring2;
      DataLCD1 += "#";
      DataLCD1 += "$";
      cek = true;
      //portOne.print(DataLCD1);
      break;
    }
  }
}

void SendRFIDtoESP32() {
  portOne.print(DataLCD);
  //SendESP = true;
}

void SendRFIDtoESP32_1() {
  portOne.print(DataLCD1);
}

void ReadTernak() {
  //  while (Serial2.available()) {
  //    char inChar = Serial2.read();
  sData += inChar;
  if (inChar == '$') {
    parsing = true;
  }
  if (parsing) {
    int q = 0;
    for (int i = 0; i < sData.length(); i++) {
      if (sData[i] == '#') {
        q++;
        data[q] = "";
      }
      else {
        data[q] += sData[i];
      }
    }

    //    if (q == 2) {
    //      id_ternak1 = data[1];
    //      TernakComplete = true;
    //    }

    if (q == 11) {
      id_ternak = data[1];
      bangsa = data[2];
      jenis_kelamin = data[3];
      status_ternak = data[4];
      id_induk = data[5];
      id_pejantan = data[6];
      fase = data[7];
      kandang = data[8];
      berat = data[9];
      umur = data[10];
      //Serial.println (data[q]);
      TernakComplete = true;
    }
    //      Serial.println (data[q]);
    parsing = false;
    sData = "";
  }

  //------------ID Ternak--------------//
  if (inChar == '&') {
    parsing2 = true;
  }
  if (parsing2) {
    int c = 0;
    for (int b = 0; b < sData.length(); b++) {
      if (sData[b] == '/') {
        c++;
        data[c] = "";
      }
      else {
        data[c] += sData[b];
      }
    }

    if (c == 3) {
      id_ternak1 = data[1];
      msg = data[2];
      TernakComplete = true;
    }
    //      Serial.println (data[q]);
    parsing2 = false;
    sData = "";
  }
}

void SendTernaktoLCD() {
  Serial1.print("id.txt=\"" + id_ternak1 + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("msg.txt=\"" + msg + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("t1.txt=\"" + id_ternak + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("B.txt=\"" + bangsa + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("JK.txt=\"" + jenis_kelamin + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("ST.txt=\"" + status_ternak + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("II.txt=\"" + id_induk + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("IP.txt=\"" + id_pejantan + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("F.txt=\"" + fase + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("K.txt=\"" + kandang + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("BE.txt=\"" + berat + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("U.txt=\"" + umur + "\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void Buzzer() {
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
  delay(50);
}
