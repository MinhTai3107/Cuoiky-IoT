#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>

#define DATABASE_URL "https://loll-269bb-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyB_eWMlxzMhPELBK2HxylLU88pYOIf1GnA"
#define USER_EMAIL "tigiahb1234@gmail.com"
#define USER_PASSWORD "01269677090"
#define DHT_PIN 5  
DHT dht(DHT_PIN, DHT11);
FirebaseAuth auth;
FirebaseConfig config;

#define D1 22//SCL
#define D2 21//SDA

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define WIFI_SSID "IoT Lab-5G"
#define WIFI_PASSWORD "IoT@123456"
FirebaseData fbdo;

void setup() {
  Serial.begin(9600);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Can't connect to wifi");
    delay(500);
  }

  dht.begin();
  Wire.begin();
  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0); 

  Serial.println("");
  Serial.println("Da ket noi Wifi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(&config, &auth);
}

void loop() {
  String path = "/TT_IoT/";
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.printf("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Nhiet do: ");
  Serial.print(t); 
  Serial.println("*C ");
  Serial.print("Do am: ");
  Serial.print(h); 
  Serial.println("% ");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.print(t);
  display.println(" *C");
  display.print("Humidity: ");
  display.print(h);
  display.println("%");
  display.display();

  Firebase.RTDB.setFloat(&fbdo, path + "/temperature", t);
  Firebase.RTDB.setFloat(&fbdo, path + "/humidity", h);

  delay(2000); 
}