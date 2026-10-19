#define BLYNK_TEMPLATE_ID "TMPL3_LLld3zu"
#define BLYNK_TEMPLATE_NAME "Smart Doctor"
#define BLYNK_AUTH_TOKEN "HbO7qHC4k0j4fAVNI9wSSxfhK-r5zLxZ"

#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials (for Wokwi simulation)
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define POT_HR 34
#define POT_BP 35
#define GREEN_LED 27
#define RED_LED 26

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;
bool lastAlertState = false;

void sendSensorData() {
  int hrRaw = analogRead(POT_HR);
  int bpRaw = analogRead(POT_BP);

  int heartRate = map(hrRaw, 0, 4095, 40, 140);
  int systolicBP = map(bpRaw, 0, 4095, 70, 180);
  float roomTemp = dht.readTemperature();

  bool heartAlert = (heartRate < 50 || heartRate > 120);
  bool bpAlert = (systolicBP < 90 || systolicBP > 140);
  bool alert = heartAlert || bpAlert;

  // Update LEDs
  digitalWrite(RED_LED, alert ? HIGH : LOW);
  digitalWrite(GREEN_LED, alert ? LOW : HIGH);

  // LCD Line 1
  lcd.setCursor(0, 0);
  lcd.print("HR:");
  lcd.print(heartRate);
  lcd.print(" BP:");
  lcd.print(systolicBP);
  lcd.print("   ");

  // LCD Line 2
  lcd.setCursor(0, 1);
  if (isnan(roomTemp)) {
    lcd.print("DHT error       ");
  } else {
    lcd.print("T:");
    lcd.print(roomTemp, 1);
    lcd.print("C ");
    lcd.print(alert ? "ALERT " : "NORMAL");
  }

  // --- Send Data to Blynk Virtual Pins ---
  Blynk.virtualWrite(V0, heartRate);
  Blynk.virtualWrite(V1, systolicBP);
  if (!isnan(roomTemp)) {
    Blynk.virtualWrite(V2, roomTemp);
  }
  Blynk.virtualWrite(V3, alert ? "ALERT" : "NORMAL");

  // --- Trigger Blynk Event when alert state changes ---
  if (alert && !lastAlertState) {
    Blynk.logEvent("health_alert", "Warning: Critical health metric detected!");
  }
  lastAlertState = alert;

  // Serial Monitor Output
  Serial.println("\n--- SMART DOCTOR ---");
  Serial.printf("Heart rate: %d BPM\n", heartRate);
  Serial.printf("Systolic BP: %d mmHg\n", systolicBP);
  if (!isnan(roomTemp)) {
    Serial.printf("Room Temp: %.1f C\n", roomTemp);
  }
  Serial.print("Status: ");
  Serial.println(alert ? "ALERT" : "NORMAL");
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  analogReadResolution(12);
  dht.begin();
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SMART DOCTOR");
  lcd.setCursor(0, 1);
  lcd.print("Connecting Blynk");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART DOCTOR");
  lcd.setCursor(0, 1);
  lcd.print("Fog Monitoring");
  delay(2000);
  lcd.clear();

  // Call sendSensorData every 2 seconds
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}