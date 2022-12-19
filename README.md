# RFID-Mobile-batch-3
Pembuatan RFID Mobile menggunakan RFID Reader EL UHF RMT01

Untuk pembuatan RFID Mobile ini, menggunakan 2 mikrokontroller yaitu Arduino Mega 2560 Promini dan ESP32

#Untuk Arduino Mega 2560 Promini 
1.Mikrokontroller ini digunakan untuk membaca tag RFID dengan memberikan perintah kepada RFID Reader RMT01 dengan trigger dari button
2.Selanjutnya ID RFID yang terbaca akan dikirimkan atau ditampilkan pada LCD Nextion
3.ID RFID dikirimkan ke ESP32 untuk dikirimkan ke database dengan respon detail data ternak (Menu Cek Ternak)
4.ID RFID,Jenis ternak dan Token dikirimkan ke ESP32 untuk dikirimkan ke ESP32 dengan respon ID ternak (Menu Pendaftaran ID RFID)
5.Respon dari ESP32 berupa ID ternak atau Detail ternak ditampilkan pada LCD.

#Untuk ESP2
1.Menerima ID RFID dari Arduino Mega 2560 dan mengirimkannnya ke database dan mengambil response nya kemudian dikirimkan lagi ke Arduino
