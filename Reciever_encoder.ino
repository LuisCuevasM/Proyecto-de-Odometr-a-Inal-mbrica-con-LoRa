#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  

//########################### OLED ##############################
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//#########################################################


#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6


String rssi = "RSSI --";
String packSize = "--";
String packet ;


void setup() {

  Serial.begin(115200);

  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");
 
  //########################## OLED ###############################
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
//#########################################################
  
  delay(1500);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Read the packet
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    // Extract _LeftEncoderTicks from the received data
    int delimiterIndex = receivedData.indexOf(',');
    if (delimiterIndex != -1) {
      String leftEncoderTicksStr = receivedData.substring(0, delimiterIndex);
      long leftEncoderTicks = leftEncoderTicksStr.toInt();
      Serial.println(leftEncoderTicks);

       String batteryLevelStr = receivedData.substring(delimiterIndex + 1);
      int batteryLevel = batteryLevelStr.toInt();

      // Display the values
      display.clearDisplay();
      display.setTextSize(1);      // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE); // Draw white text
      display.setCursor(0, 8);     // Start at top-left corner
      display.print("Battery Level: ");
      display.println(batteryLevel);
      display.println("%");
    

      display.setCursor(0, 16);     // Start at top-left corner
      display.print("Ticks: ");
      display.println(leftEncoderTicks);
      
      display.display();
    }
  }
  delay(50);
}
