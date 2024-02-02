
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <mySD.h>
#include <LoRa.h>

//########################### LORA ##############################
#define SCK     5    // GPIO5  -- SCK
#define MISO    19   // GPIO19 -- MISO
#define MOSI    27   // GPIO27 -- MOSI
#define SS      18   // GPIO18 -- CS
#define RST     23   // GPIO14 -- RESET (If Lora does not work, replace it with GPIO14)
#define DI0     26   // GPIO26 -- IRQ(Interrupt Request)
#define BAND    868E6


String rssi = "RSSI --";
String packSize = "--";
String packet ;
//#########################################################


//########################### OLED ##############################
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//#########################################################

//########################## Encoder ###############################
#define c_LeftEncoderPinA 34
#define c_LeftEncoderPinB 12  
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
long _LeftEncoderTicks = 0;
String Dato = "";
//#########################################################


//########################## Voltaje ###############################
// Definir el pin ADC que se utilizará
const int batteryPin = 39; // Utiliza un pin ADC como GPIO
// Constantes para el cálculo del voltaje
const float adcResolution = 4095.0;
const float referenceVoltage = 3.3;
const float voltageDividerRatio = 0.41; // Ajustar según tu divisor de voltaje
// Constantes para el mapeo del porcentaje de la batería
const float minBatteryVoltage = 3.3; // Voltaje mínimo de la batería considerada vacía
const float maxBatteryVoltage = 8.0; // Voltaje máximo de la batería considerada llena
const int numReadings = 10; 
//#########################################################


//########################## SD ###############################
ext::File myFile;
const int chipSelect = 13;
//#########################################################

void setup() {

  Serial.begin(115200);

//########################### LORA ##############################
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
//#########################################################

//########################## OLED ###############################
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
//#########################################################

//########################## ENCODER ###############################
  pinMode(c_LeftEncoderPinA, INPUT_PULLUP); // sets pin A as input with pull-up
  pinMode(c_LeftEncoderPinB, INPUT_PULLUP); // sets pin B as input with pull-up
  attachInterrupt(digitalPinToInterrupt(c_LeftEncoderPinA), HandleLeftMotorInterruptA, RISING);
//#########################################################

//########################## SD ###############################
//Open serial communications and wait for port to open:
// Estado de la tarjeta SD
/*
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 8);
  display.println("Initializing SD card");
  display.display();      // Show initial text
  delay(1000);

  pinMode(SS, OUTPUT);

  if (!SD.begin(13, 15, 2, 14)) {  //definición de pines CS,MOSI,MISO,SCK
    Serial.println("initialization failed!");
    return;
  }
  */
//#########################################################
}



void loop() {
//######################### LORA ################################

  LoRa.beginPacket();
  LoRa.print(_LeftEncoderTicks);
  LoRa.print(",");
  LoRa.print(getBatteryLevel());
  LoRa.endPacket();

//#########################################################

//########################## OLED ###############################
  display.clearDisplay();
  bool loraConnected = isLoRaConnected(); // Retorna true si LoRa está conectado

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,24);
  display.print("ticks: ");
  display.print(_LeftEncoderTicks);

  // Muestra el nivel de batería
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,8);
  display.print("Bateria: ");
  display.print(getBatteryLevel());
  

  // Estado de la conexión LoRa
  display.setCursor(0, 16);
  display.print("LoRa: ");
  display.println(loraConnected ? "transmitiendo" : "...");

//#########################################################

//########################## SD ###############################
  /*
  long now = millis(); // Tiempo actual en milisegundos
  int horas = (now / (1000*60*60)) % 24; // Horas
  int minutos = (now / (1000*60)) % 60; // Minutos
  int segundos = (now / 1000) % 60; // Segundos
  int miliSegundos = now % 1000; // Milisegundos

  
  myFile = SD.open("prueba.csv", FILE_WRITE);
  if (myFile) {

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.print("SD: Writing");

    myFile.print("\""); // Comienzo de la primera celda
    if (horas < 10) myFile.print("0");
    myFile.print(horas);
    myFile.print(":");
    if (minutos < 10) myFile.print("0");
    myFile.print(minutos);
    myFile.print(":");
    if (segundos < 10) myFile.print("0");
    myFile.print(segundos);
    myFile.print(".");
    if (miliSegundos < 100) myFile.print("0");
    if (miliSegundos < 10) myFile.print("0");
    myFile.print(miliSegundos);
    myFile.print("\";\""); // Fin de la primera celda y comienzo de la segunda

    myFile.print(_LeftEncoderTicks, 2); // Imprime la distancia con dos decimales
    myFile.println("\";"); // Fin de la segunda celda y salto de línea para la siguiente fila
    myFile.close();
    */
    //#########################################################

  display.display();





}



String getBatteryLevel() {
  long sum = 0;
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(batteryPin);
    delay(10);
  }
  int average = sum / numReadings;

  float voltage = (average / 4095.0) * 3.3 / 0.41;

  int batteryPercentage = map(voltage * 100, minBatteryVoltage * 100, maxBatteryVoltage * 100, 0, 100);
  batteryPercentage = constrain(batteryPercentage, 0, 100);

  if (batteryPercentage == 0) {
    return "Bateria baja!";
  } else {
    return String(batteryPercentage) + "%";
  }
}


bool isLoRaConnected() {
  // Aquí va tu código para verificar la conexión LoRa
  return LoRa.beginPacket(); // Ejemplo
}



void serialEvent() {
  String input = "";
  while (Serial.available()) {
    char character = Serial.read();                    
    if (character != '@') {
      input += character;
      delay(1);
    } else {
      Dato = input.substring(5, input.length()); // separa la magnitud de la data y deja el valor                                                
      if (input.substring(0, 5) == "reset") { // le asigna un nuevo valor a _LeftEncoderTicks
        _LeftEncoderTicks = round(Dato.toFloat() / 0.000111846);
      } 
      input = "";                         
    }     
  } 
}


void HandleLeftMotorInterruptA() {
  _LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);   // read the input pin
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #endif
}
