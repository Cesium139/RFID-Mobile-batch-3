//PROMINI BATCH 3

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

//~~~~~~~~~~~~~~~~~~~READ RFID FROM ID01 SETUP~~~~~~~~~~~~~~~~//
void ReadRFID();

String inRFID = "";
String RFIDstring = "";
String RFIDstring2 = "";
String RFIDbaru = "";
String R2 = "";
String R3 = "";

bool RFIDcomplete1 = false;
bool RFIDcomplete2 = false;
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~~~SEND RFID TO LCD~~~~~~~~~~~~~~~~~~~~//
void SendRFIDtoLCD();
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~ READ LCD SETUP ~~~~~~~~~~~~~~~~~~~~//
bool cek = false;
bool LCDcomplete = false;
String lcd = "";
String DataLCD = "";
String DataLCD1 = "";
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~~~SEND RFID TO ESP32~~~~~~~~~~~~~~~~~~~~//
void SendRFIDtoESP32();
void SendRFIDtoESP32_1();
bool SendESP = false;
void ReadLCD();
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




//################################################################################################//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // Serial Monitor Arduino IDE
  Serial1.begin(115200);    // Serial 1 Promini to LCD (19pro --> Tx LCD & 18Pro --> Rx LCD)
  Serial2.begin(9600);      // Serial 2 Promini to ESP32 (17pro --> 17esp & 16pro --> 16esp)- read data ternak
  portOne.begin(9600);      // Promini to ESP32 for Send RFID
  portTwo.begin(115200);    // ID01 to Promini for Read RFID
}

void loop() {
  ReadLCD();
  ReadRFID();               //Read RFID from RFID Reader

  while (Serial2.available()) {
    inChar = Serial2.read();
    //Serial.println(inChar);
    ReadWifi();
    ReadTernak();
  }

   if (RFIDcomplete2) {
    Serial.println(R3);
    SendRFIDtoLCD();
    RFIDstring = "";
    RFIDbaru = "";
    R3 = "";
    RFIDcomplete2 = false;
  }

  if (LCDcomplete) {
    //Serial2.flush();
    Serial.println(DataLCD);
    SendRFIDtoESP32();
    lcd = "";
    DataLCD = "";
    LCDcomplete = false;
  }

  if (cek) {
    //Serial2.flush();
    Serial.println(DataLCD1);
    portOne.print(DataLCD1);
    lcd = "";
    DataLCD = "";
    cek = false;
  }

  //  if (TernakComplete && SendESP) {
  //    portOne.flush();
  //    Serial2.flush();
  //    RFIDcomplete = false;
  //    SendPendaftaranLCD();
  //    lcd = "";
  //    DataLCD = "";
  //    Serial2.flush();
  //    SendESP = false;
  //    TernakComplete = false;
  //  }

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
  while (portTwo.available()) {

    inRFID = String(portTwo.read(), HEX);
    delay(25);
    RFIDstring += inRFID;
    //    RFIDcomplete = true;

    if (RFIDstring.length() == 28 || RFIDstring.length() == 27 || RFIDstring.length() == 29) {
      RFIDbaru = RFIDstring;
      RFIDcomplete1 = true;
      //RFIDbaru = "";
      //Serial.print("");
    }
  }

  if (RFIDcomplete1) {
    char baca[100];
    RFIDbaru.toCharArray(baca, 64, 0);
    if (baca[1] == '0') {
      for (int c = 0; c < 22 ; c++) {
        //delay(15);
        R2 = (baca[c]);
        R3 += R2;
        Serial.print("");
      }
      RFIDcomplete1 = false;
      RFIDcomplete2 = true;
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
      LCDcomplete = true;
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
    int a = 0;
    for (int b = 0; b < sData.length(); b++) {
      if (sData[b] == '/') {
        a++;
        data[a] = "";
      }
      else {
        data[a] += sData[b];
      }
    }

    if (a == 3) {
      id_ternak1 = data[1];
      msg = data[2];
      TernakComplete = true;
    }
    //      Serial.println (data[q]);
    parsing2 = false;
    sData = "";
  }
}

//void SendPendaftaranLCD() {
//  Serial1.print("id.txt=\"" + id_ternak + "\"");
//  Serial1.write(0xff);
//  Serial1.write(0xff);
//  Serial1.write(0xff);
//}

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
