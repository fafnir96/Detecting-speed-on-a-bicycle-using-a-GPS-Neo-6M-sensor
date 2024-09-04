//---------------------------------------- Include Library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
//----------------------------------------

//---------------------------------------- Configure GPS MODULE
#define GPS_BAUDRATE 9600  // The default baudrate of NEO-6M is 9600
#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
TinyGPSPlus gps;  // the TinyGPS++ object
float latitude, longitude, speedKm;
//----------------------------------------

//---------------------------------------- Configure OLED screen size in pixels
#define SCREEN_WIDTH 128 //--> OLED display width, in pixels
#define SCREEN_HEIGHT 64 //--> OLED display height, in pixels
//----------------------------------------

//---------------------------------------- Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//----------------------------------------

//---------------------------------------- Configure WIFI
const char* ssid = "SSID WIFI";
const char* password = "PASSWORD WIFI";

// +international_country_code + phone number
// Example: Portugal +351, +351912345678
String phoneNumber = "Phone Number";
String apiKey = "API Key";

//---------------------------------------- Function to Send WhatsApp Message
void sendMessage(String message) {
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    int httpResponseCode = http.POST(url);
    if (httpResponseCode == 200) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

//---------------------------------------- Function to Read GPS Data
void GPS() {
    if (neogps.available() > 0) {
        if (gps.encode(neogps.read())) {
            if (gps.location.isValid() && gps.location.isUpdated()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                // speedKm = gps.speed.kmph();
                Serial.print(F("- latitude: "));
                Serial.println(latitude);

                Serial.print(F("- longitude: "));
                Serial.println(longitude);

                Serial.print(F("- altitude: "));
                if (gps.altitude.isValid())
                    Serial.println(gps.altitude.meters());
                else
                    Serial.println(F("INVALID"));
                    
                Serial.print(F("- Speed km/h: "));
                Serial.println(speedKm);
            } else {
                Serial.println(F("- location: INVALID"));
            }notification(); // Mengirimkan pesan ke WA
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}

void notification(){
    // Send warning message if speed exceeds limit
    if (speedKm > 25) {
      String message = "WARNING!\n";
      message += "Speed over " + String(speedKm) + "km/h\n";
      message += "Location ";
      message += "https://maps.google.com/maps?&z=15&mrt=yp&t=k&q=" + String(latitude, 6) + "+" + String(longitude, 6);
      sendMessage(message);
      Serial.print(message);
    }
}

//---------------------------------------- Function to Display Data on OLED
void OLED() {
    display.clearDisplay(); // Clear display before showing data

    if (speedKm > 25) {
        // Warning Display
        display.setTextSize(2); // Large text size for warning
        display.setCursor(10, 10);
        display.setTextColor(WHITE, BLACK); // White text on black background
        display.println("WARNING!");

        display.setTextSize(1); // Normal text size for additional message
        display.setCursor(0, 30);
        display.println("Speed over 25 km/h");
        display.setCursor(0, 40);
        display.println("Slow Down!");
    } else {
        // Normal Display
        display.setTextSize(1); // Set small text size to fit the screen
        display.setCursor(0, 0);
        display.print("Lat: ");
        display.setCursor(40, 0);
        display.print(latitude, 6); // Display latitude

        display.setCursor(0, 10);
        display.print("Lon: ");
        display.setCursor(40, 10);
        display.print(longitude, 6); // Display longitude

        display.setCursor(0, 20);
        display.print("Alt: ");
        if (gps.altitude.isValid()) {
            display.setCursor(40, 20);
            display.print(gps.altitude.meters()); // Display altitude
            display.print(" m");
        } else {
            display.setCursor(40, 20);
            display.print("INVALID");
        }

        display.setCursor(0, 30);
        display.print("Speed: ");
        display.setCursor(40, 30);
        display.print(speedKm); // Display speed
        display.print(" km/h");
    }

    display.display(); // Show data on screen
}

void setup() {
    Serial.begin(115200); // Initialize Serial Communication at 115200 baudrate
    neogps.begin(GPS_BAUDRATE, SERIAL_8N1, RXD2, TXD2); // Begin GPS communication
    Serial.println();
    delay(2000);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    display.clearDisplay(); 
    display.setTextColor(WHITE);
    display.setTextSize(2); 
    display.setCursor(15, 0); 
    display.print("Portable");
    display.setCursor(10, 20); 
    display.print("Speed");
    display.setCursor(0, 40); 
    display.print("Detection");
    display.display();
    delay(2000);
    display.clearDisplay(); 

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi with IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    GPS(); // Membaca data GPS
    OLED(); // Menampilkan data pada OLED
    delay(1000); // Menunggu 1 detik sebelum memperbarui tampilan
}
