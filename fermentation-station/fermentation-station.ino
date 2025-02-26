#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define HUMIDIFIER_PIN 3
#define HEATER_PIN 4
#define TEMP_UP_BTN 10
#define TEMP_DOWN_BTN 11
#define HUM_UP_BTN 12
#define HUM_DOWN_BTN 13

// Parametri PID per umidità
double humiditySetpoint = 55.0;
double humidityInput, humidityOutput;
double humidityKp = 25, humidityKi = 5, humidityKd = 2;
PID humidityPID(&humidityInput, &humidityOutput, &humiditySetpoint, humidityKp, humidityKi, humidityKd, DIRECT);

// Parametri PID per temperatura
double temperatureSetpoint = 27.0;
double temperatureInput, temperatureOutput;
double temperatureKp = 43, temperatureKi = 5, temperatureKd = 1;
PID temperaturePID(&temperatureInput, &temperatureOutput, &temperatureSetpoint, temperatureKp, temperatureKi, temperatureKd, DIRECT);

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

bool humidifierOn = false;
bool heaterOn = false;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 2000;

bool tempUpBtnState = false;
bool tempDownBtnState = false;
bool humUpBtnState = false;
bool humDownBtnState = false;
bool prevTempUpBtnState = false;
bool prevTempDownBtnState = false;
bool prevHumUpBtnState = false;
bool prevHumDownBtnState = false;
unsigned long lastButtonCheckTime = 0;
const unsigned long buttonDebounceTime = 200;

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();
  updateDisplay(0, 0);
  
  // Configurazione pin per attuatori
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HUMIDIFIER_PIN, LOW);
  digitalWrite(HEATER_PIN, LOW);
  
  // Configurazione pin per pulsanti
  pinMode(TEMP_UP_BTN, INPUT);
  pinMode(TEMP_DOWN_BTN, INPUT);
  pinMode(HUM_UP_BTN, INPUT);
  pinMode(HUM_DOWN_BTN, INPUT);
  
  // Configurazione PID umidità
  humidityPID.SetMode(AUTOMATIC);
  humidityPID.SetOutputLimits(0, 100);
  
  // Configurazione PID temperatura
  temperaturePID.SetMode(AUTOMATIC);
  temperaturePID.SetOutputLimits(0, 100);
}

void loop() {
  unsigned long currentTime = millis();

  checkButtons();
  
  // Leggi i sensori e aggiorna ogni updateInterval millisecondi
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    // Controllo errori di lettura
    if (isnan(humidity) || isnan(temperature)) {
      lcd.setCursor(0, 0);
      lcd.print("Errore sensore!");
      return;
    }
    
    // Aggiorna i valori di input per i PID
    humidityInput = humidity;
    temperatureInput = temperature;
    
    // Calcola l'output dei controller PID
    humidityPID.Compute();
    temperaturePID.Compute();
    
    updateDisplay(temperature, humidity);
    
    // Controlla gli attuatori
    controlHumidifier();
    controlHeater();
  }
}

void checkButtons() {
  unsigned long currentTime = millis();
  
  // Leggi lo stato dei pulsanti (con controllo debounce)
  if (currentTime - lastButtonCheckTime >= buttonDebounceTime) {
    tempUpBtnState = digitalRead(TEMP_UP_BTN);
    tempDownBtnState = digitalRead(TEMP_DOWN_BTN);
    humUpBtnState = digitalRead(HUM_UP_BTN);
    humDownBtnState = digitalRead(HUM_DOWN_BTN);
    
    if (tempUpBtnState && !prevTempUpBtnState) {
      temperatureSetpoint += 0.5;
      updateSetpoints();
    }
    
    if (tempDownBtnState && !prevTempDownBtnState) {
      temperatureSetpoint -= 0.5;
      updateSetpoints();
    }
    
    if (humUpBtnState && !prevHumUpBtnState) {
      humiditySetpoint += 1.0;
      updateSetpoints();
    }
    
    if (humDownBtnState && !prevHumDownBtnState) {
      humiditySetpoint -= 1.0;
      updateSetpoints();
    }
    
    // Salva lo stato precedente dei pulsanti
    prevTempUpBtnState = tempUpBtnState;
    prevTempDownBtnState = tempDownBtnState;
    prevHumUpBtnState = humUpBtnState;
    prevHumDownBtnState = humDownBtnState;
    
    lastButtonCheckTime = currentTime;
  }
}

void updateSetpoints() {
  //Aggiorna i setpoint sul display
  lcd.setCursor(10, 0);
  lcd.print("    ");
  lcd.setCursor(10, 0);
  lcd.print(temperatureSetpoint, 1);
  
  lcd.setCursor(10, 1);
  lcd.print("    ");
  lcd.setCursor(10, 1);
  lcd.print(humiditySetpoint, 1);
}

void updateDisplay(float temperature, float humidity) {
  // Riga 1
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C ");
  lcd.setCursor(8, 0);
  lcd.print("S:");
  lcd.print(temperatureSetpoint, 1);
  lcd.print("C");
  
  // Riga 2
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 1);
  lcd.print("% ");
  lcd.setCursor(8, 1);
  lcd.print("S:");
  lcd.print(humiditySetpoint, 1);
  lcd.print("%");
}

void controlHumidifier() {
  // Logica PWM "simulata" per l'umidificatore basata sull'output PID
  if (humidityOutput > 50) {
    if (!humidifierOn) {
      digitalWrite(HUMIDIFIER_PIN, HIGH);
      humidifierOn = true;
    }
  } else {
    if (humidifierOn) {
      digitalWrite(HUMIDIFIER_PIN, LOW);
      humidifierOn = false;
    }
  }
}

void controlHeater() {
  // Logica PWM "simulata" per il riscaldatore basata sull'output PID
  if (temperatureOutput > 50) {
    if (!heaterOn) {
      digitalWrite(HEATER_PIN, HIGH);
      heaterOn = true;
    }
  } else {
    if (heaterOn) {
      digitalWrite(HEATER_PIN, LOW);
      heaterOn = false;
    }
  }
}