#include <TinyGPS++.h>        // GPS
#include <SoftwareSerial.h>
#include "U8glib.h"           // Oled
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT22
#define LED_VERDE 5
SoftwareSerial mySerial(2, 3);
//******************************************Para GPS******************************************
TinyGPSPlus            gps;
static const int       RXPin = 7, TXPin = 6;
static const uint32_t  GPSBaud = 9600;
float                  latitud, longitud;
int                    year;
byte                   month, day, hour, minute, second, centisecond;

SoftwareSerial ss(RXPin, TXPin);

TinyGPSCustom pdop(gps, "GNGLL", 1); // 1er elemento (LAT) de la trama $GNGLL
TinyGPSCustom hdop(gps, "GNGLL", 3); // 3er elemento (LONG) de la trama $GNGLL
//******************************************Para OLED******************************************
U8GLIB_SH1106_128X64   u8g(U8G_I2C_OPT_NONE);

//******************************************Para DHT22******************************************
DHT dht(DHTPIN, DHTTYPE);

float temperatura = 0.0, humedad = 0.0;
//******************************************SETUP******************************************

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  ss.begin(GPSBaud);
  dht.begin();
  pinMode(LED_VERDE, OUTPUT);
}

void loop() {

  get_gps();

  Serial.println();

  get_tem_hum();
  
  u8g.firstPage();  
  do{
    draw();
  }while( u8g.nextPage() );

  mySerial.print("T: ");
  mySerial.println(temperatura);  ///lo mando al otro lora
  mySerial.print("H: ");
  mySerial.println(humedad);
  mySerial.print("LAT: ");
  mySerial.println(latitud);
  mySerial.print("LON: ");
  mySerial.println(longitud);
  mySerial.print("FECHA: ");
  mySerial.print(day);
  mySerial.print("/");
  mySerial.print(month);
  mySerial.print("/");
  mySerial.println(year);
  mySerial.print("HORA: ");
  mySerial.print(hour);
  mySerial.print(":");
  mySerial.print(minute);
  mySerial.print(":");
  mySerial.print(second);
  mySerial.println();
  
  digitalWrite(LED_VERDE, HIGH);

  delay(1000);
  
  digitalWrite(LED_VERDE, LOW);

  delay(1100);

}

void get_gps(void){

  while (ss.available() > 0)
    gps.encode(ss.read());

  // ****************LONGITUD Y LATITUD*********************
  latitud = atof(pdop.value())/100;
  longitud = atof(hdop.value())/100;
  Serial.print(F(" LATITUD: "))  ; Serial.print(latitud,7);     // Valor de latitud con 7 decimales
  Serial.print(F("\tLONGITUD: ")); Serial.println(longitud,7);  // Valor de longitud con 7 decimales
  delay(100);

  // ****************Fecha y Hora*********************
  Serial.print(F("  Día/Hora: "));
  if (gps.date.isValid()){
    month = gps.date.month();
    day = gps.date.day();
    year = gps.date.year();
    Serial.print(month);
    Serial.print(F("/"));
    Serial.print(day);
    Serial.print(F("/"));
    Serial.print(year);
  }else
    Serial.print(F("INVALID"));
  

  Serial.print(F(" "));
  if (gps.time.isValid()){
    hour = gps.time.hour();
    if (hour < 10) Serial.print(F("0"));
    Serial.print(hour);
    Serial.print(F(":"));
    minute = gps.time.minute();
    if (minute < 10) Serial.print(F("0"));
    Serial.print(minute);
    Serial.print(F(":"));
    second = gps.time.second();
    if (second < 10) Serial.print(F("0"));
    Serial.print(second);
    Serial.print(F("."));
    centisecond = gps.time.centisecond();
    if (centisecond < 10) Serial.print(F("0"));
    Serial.print(centisecond);
  }else
    Serial.print(F("INVALID"));

}

void get_tem_hum(void){

  temperatura = dht.readTemperature();
  humedad     = dht.readHumidity();
  
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" *C ");
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" %t ");
}

void draw(void) {
//Imprimimos en pantalla latitud, longitud
u8g.setFont(u8g_font_unifont);
u8g.setPrintPos(0, 20); 
u8g.print("LATI: ");
u8g.print(latitud,7);
u8g.setPrintPos(0, 31);
u8g.print("LONG: ");
u8g.print(longitud,7);
//Temperatura
u8g.setPrintPos(0, 42);
u8g.print("T: ");
u8g.print(temperatura, 1);
u8g.print(" H: ");
u8g.print(humedad, 1);
//Fecha y hora
u8g.setPrintPos(0, 53);
u8g.print("Fecha: ");
u8g.print(day, DEC);
u8g.print("/");
u8g.print(month, DEC);
u8g.print("/");
u8g.print(year);
u8g.setPrintPos(0, 64);
u8g.print("Hora: ");
u8g.print(hour, DEC);
u8g.print(":");
u8g.print(minute, DEC);
u8g.print(":");
u8g.print(second, DEC);
 }
