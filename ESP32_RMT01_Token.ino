//ESP32_Wemos Batch 3 (EL UHF RMT01)

//~~~~~~~~~~~~~~~~~~~~~WIFI ESP32 SETUP~~~~~~~~~~~~~~~~~~~~~~//
#include <WiFiManager.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <String.h>

void connectWifi();

WiFiManager wm;
//**********************************************************//

//~~~~~~~~~~~~~~~~~~~~SOFTWARE SERIAL SETUP~~~~~~~~~~~~~~~~~~~~//
#include <SoftwareSerial.h>
// software serial #1: RX = digital pin 22, TX = digital pin 23
SoftwareSerial espSerial1(22, 23); // Promini to ESP32 (11pro --> 23esp & 10pro --> 22esp)

// software serial #1: RX = digital pin 16, TX = digital pin 17
SoftwareSerial espSerial2(16, 17); // Promini to ESP32 (17pro --> 17esp & 16pro --> 16esp)
//************************************************************//

//~~~~~~~~~~~~~~~~~~~READ RFID FROM ID01 SETUP~~~~~~~~~~~~~~~~//
String jenis_ternak = "";
String RFIDstring1 = "";

void ReadRFIDfromMini();
char inRFID;
String RFIDstring = "";
String RFIDstring2 = "";
bool RFIDcomplete = false;
bool RFIDcomplete1 = false;

bool parsing = false;
bool parsing2 = false;
String rData, data[3];
//************************************************************//

//~~~~~~~~~~~~~~~~~~~Reconnect dan lupakan jaringan Setup~~~~~~~~~~~~~~~~~~~~~~~~~~//
bool gagal = false;
bool reconnectComplete = false;
bool reconnectComplete1 = false;
bool parsing1 = false;
String reconnect = "";
//************************************************************//

//~~~~~~~~~~~~~~~~~~~POST & GET RESPONSE SETUP~~~~~~~~~~~~~~~~//
String id_ternak = "";
String bangsa = "";
String jenis_kelamin = "";
String id_induk = "";
String id_pejantan = "";
String kandang = "";
String berat = "";
String kesehatan = "";
String penyakit = "";
String umur = "";
void postHttp();
void postHttp1();
bool TernakComplete = false;
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~SEND DATA TERNAK SETUP~~~~~~~~~~~~~~~~~~~//
//void SendDataTernakToMini();
String DataTernak = "";
String DataTernak1 = "";
//************************************************************//

//~~~~~~~~~~~~~~~~~~~~ ID PETRNAKAN SETUP ~~~~~~~~~~~~~~~~~~~//
String token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZF9wZXRlcm5ha2FuIjoxLCJpYXQiOjE2NzAyOTY3OTF9.j5o0BQOrheYtw05LZOK8Mhg6YWXWutfWCAC_rOhSoDk";
//************************************************************//


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  espSerial1.begin(19200);
  espSerial2.begin(19200);
  connectWifi();
}

void loop() {

  if (reconnectComplete1) {        //Lupakan Jaringan Manual
    wm.resetSettings();
    espSerial2.print("+");
    reconnect = "";
    reconnectComplete1 = false;
  }

  if (reconnectComplete) {        //Reconnect Manual
    connectWifi();
    reconnect = "";
    reconnectComplete = false;
  }

  ReadRFIDfromMini();               //Read RFID from RFID Reader
  if (RFIDcomplete1) {
    Serial.println("~~~~~~~~~~~~~~~~~~~~FROM MINI~~~~~~~~~~~~~~~~~~~~");
    //Serial.print("token                : "); Serial.println(token);
    Serial.print("Jenis Ternak         : "); Serial.println(jenis_ternak);
    Serial.print("RFID                 : "); Serial.println(RFIDstring2);
    //    Serial.println(id_peternakan);
    //    Serial.println(RFIDstring1);
    postHttp();
    RFIDstring2 = "";
    jenis_ternak = "";
    RFIDcomplete1 = false;
  }

  if (RFIDcomplete) {
    Serial.println("~~~~~~~~~~~~~~~~~~~~FROM MINI~~~~~~~~~~~~~~~~~~~~");
    Serial.print("RFID                 : "); Serial.println(RFIDstring1);
    //    Serial.println(id_peternakan);
    //    Serial.println(RFIDstring1);
    postHttp1();
    RFIDstring1 = "";
    RFIDcomplete = false;
  }

  if (TernakComplete) {           //Kirim data ternak ke Arduino
    //SendDataTernakToMini();
    TernakComplete = false;
  }

}
//##################################################################################################//


void connectWifi() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("RFID MOBILE READER 1", "12345678"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connect");
    espSerial2.print("=");
  }
}


//Read RFID dan data dari LCD Nextion
void ReadRFIDfromMini() {
  while (espSerial1.available()) {

    inRFID = char(espSerial1.read());
    //Serial.println(inRFID);
    delay(5);
    rData += inRFID;

    //----------------Baca data Reconnect------------//
    if (inRFID == '?') {
      parsing1 = true;
    }
    if (parsing1) {
      int x = 0;
      for (int y = 0; y < rData.length(); y++) {
        if (rData[y] == '*') {
          x++;
          data[x] = "";
        }
        else {
          data[x] += rData[y];
        }
      }

      if (x == 2) {
        reconnect = data[1];
        if (reconnect == "1") {
          reconnectComplete = true;
        }
        if (reconnect == "3") {
          reconnectComplete1 = true;
        }
      }
      parsing1 = false;
      rData = "";
    }

    //------------Baca RFID--------------//
    if (inRFID == '$') {
      parsing = true;
    }
    if (parsing) {
      int q = 0;
      for (int i = 0; i < rData.length(); i++) {
        if (rData[i] == '#') {
          q++;
          data[q] = "";
        }
        else {
          data[q] += rData[i];
        }
      }

      if (q == 2) {
        RFIDstring1  = data[1];
        RFIDcomplete = true;
      }
      parsing = false;
      rData = "";
    }

    //------------Baca RFID & Jenis Ternak--------------//
    if (inRFID == '&') {
      parsing2 = true;
    }
    if (parsing2) {
      int a = 0;
      for (int b = 0; b < rData.length(); b++) {
        if (rData[b] == '/') {
          a++;
          data[a] = "";
        }
        else {
          data[a] += rData[b];
        }
      }

      if (a == 3) {
        RFIDstring2  = data[1];
        jenis_ternak = data[2];
        //Serial.println (data[q]);
        RFIDcomplete1 = true;
      }
      //      Serial.println (data[q]);
      parsing2 = false;
      rData = "";
    }

  }
}

void postHttp()
{
  Serial.println ("Posting....");
  String url = "http://be.gembala.sembadafarm.com/api/rfid/add-data";
  HTTPClient http;
  String response;

  StaticJsonDocument<256> buff;
  String jsonParams;
  buff["rf_id"] = RFIDstring2;
  buff["token"] = token;
  buff["jenis_ternak_baru"] = jenis_ternak;

  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int statusCode = http.POST(jsonParams);
  response = http.getString();
  //Serial.println(response);

  StaticJsonDocument<192> doc;
  deserializeJson(doc, response);
  JsonObject obj = doc.as<JsonObject>();
  String data = obj[String("data")];
  //Serial.println(data);

  StaticJsonDocument<128> doc1;
  deserializeJson(doc1, data);
  JsonObject obj1 = doc1.as<JsonObject>();
  String msg = obj1[String("message")];
  String id_ternak = obj1[String("id_ternak")];

  Serial.println(" ");

  Serial.println("~~~~~~~~~~~~~~~~~~~~PENDAFTARAN TERNAK~~~~~~~~~~~~~~~~~~~~");

  Serial.print("ID Ternak         : "); Serial.println(id_ternak);

  DataTernak1 = "";
  DataTernak1 += "/";
  DataTernak1 += id_ternak;
  DataTernak1 += "/";
  DataTernak1 += msg;
  DataTernak1 += "/";
  DataTernak1 += "&";

  Serial.println(DataTernak1);
  espSerial2.println(DataTernak1);
  //  if(statusCode == 200){
  //    Serial.println("Berhasil");
  //    }
  //  else{
  //    Serial.println(statusCode);
  //    Serial.println("Gagal");}
  //TernakComplete = true;
}

void postHttp1()
{
  String url = "http://be.gembala.sembadafarm.com/api/rfid/get-data";
  HTTPClient http;
  String response1;

  StaticJsonDocument<100> buff;
  String jsonParams;
  buff["rf_id"] = RFIDstring1;

  serializeJson(buff, jsonParams);
  Serial.println(jsonParams);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int statusCode = http.POST(jsonParams);
  response1 = http.getString();
  //Serial.println(response);

  StaticJsonDocument<1536> doc2;
  deserializeJson(doc2, response1);
  JsonObject obj2 = doc2.as<JsonObject>();
  String data = obj2[String("data")];
  //  Serial.println(data);

  StaticJsonDocument<1536> doc3;
  deserializeJson(doc3, data);
  JsonObject obj3 = doc3.as<JsonObject>();
  String id_ternak = obj3[String("id_ternak")];
  String bangsa1 = obj3[String("bangsa")];
  String jenis_kelamin = obj3[String("jenis_kelamin")];
  String id_dam = obj3[String("id_dam")];
  String id_sire = obj3[String("id_sire")];
  String kandang1 = obj3[String("kandang")];
  String berat = obj3[String("berat")];
  String fase = obj3[String("fase")];
  String status_ternak1 = obj3[String("status_ternak")]; //ubah jadi "status_ternak"
  String umur = obj3[String("umur")];
  //

  StaticJsonDocument<64> doc4;
  deserializeJson(doc4, bangsa1);
  JsonObject obj4 = doc4.as<JsonObject>();
  String bangsa2 = obj4[String("bangsa")];

  StaticJsonDocument<64> doc5;
  deserializeJson(doc5, kandang1);
  JsonObject obj5 = doc5.as<JsonObject>();
  String kandang2 = obj5[String("kode_kandang")];

  StaticJsonDocument<100> doc6;
  deserializeJson(doc6, status_ternak1);
  JsonObject obj6 = doc6.as<JsonObject>();
  String status_ternak2 = obj6[String("status_ternak")];

  StaticJsonDocument<100> doc7;
  deserializeJson(doc7, fase);
  JsonObject obj7 = doc7.as<JsonObject>();
  String fase1 = obj7[String("fase")];

  Serial.println(" ");

  Serial.println("~~~~~~~~~~~~~~~~~~~~DATA TERNAK~~~~~~~~~~~~~~~~~~~~");

  Serial.print("ID Ternak         : "); Serial.println(id_ternak);
  Serial.print("Bangsa            : "); Serial.println(bangsa2);
  Serial.print("Jenis Kelamin     : "); Serial.println(jenis_kelamin);
  Serial.print("Status ternak     : "); Serial.println(status_ternak2);
  Serial.print("ID Dam            : "); Serial.println(id_dam);
  Serial.print("ID Sire           : "); Serial.println(id_sire);
  Serial.print("Fase              : "); Serial.println(fase1);
  Serial.print("kandang           : "); Serial.println(kandang2);
  Serial.print("Berat Berkala     : "); Serial.println(berat);
  //Serial.print("Kesehatan         : "); Serial.println(kesehatan);
  //  Serial.print("penyakit          : "); Serial.println(penyakit);
  Serial.print("Umur              : "); Serial.println(umur);
  ////
  DataTernak = "";
  DataTernak += "#";
  DataTernak += id_ternak;
  DataTernak += "#";
  DataTernak += bangsa2;
  DataTernak += "#";
  DataTernak += jenis_kelamin;
  DataTernak += "#";
  DataTernak += status_ternak2;
  DataTernak += "#";
  DataTernak += id_dam;
  DataTernak += "#";
  DataTernak += id_sire;
  DataTernak += "#";
  DataTernak += fase1;
  DataTernak += "#";
  DataTernak += kandang2;
  DataTernak += "#";
  DataTernak += berat;
  DataTernak += "#";
  DataTernak += umur;
  DataTernak += "#";
  DataTernak += "$";
  ////
  Serial.println(DataTernak);
  espSerial2.println(DataTernak);
}
