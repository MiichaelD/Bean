
#include <ArduinoJson.h>

#define PIN_A0  A0
#define PIN_A1  A1
#define PIN_0   0
#define PIN_1   1
#define PIN_2   2
#define PIN_3   3
#define PIN_4   4
#define PIN_5   5
#define NUM_DIG_PINS 6

const String COLOR = "color";
const String TEMP = "temperature";
const String BATTERY = "battery";
const String SECONDS = "secs";
const String LAST_TRANSMITION = "last_msg";

uint32_t m_counter = 0;
bool m_transmit = false;

long m_lastUpdate;

// EOM code to explicitly tell when we end a message
#define EOM_MSG_CODE "|"
#define SEND_EOM_CODE Serial.println(EOM_MSG_CODE);

void setup() {
//  UUID: A495____-C5B1-4B44-B512-1370F02D74DE
  Bean.enableWakeOnConnect(true);
  Serial.begin(57600);
  Serial.setTimeout(25);
  randomSeed(0);
  Bean.setLed(0, 0, 0);
  m_counter = 0;
  //  pinMode(PIN_A0, OUTPUT);
  //  for (int i = 0; i < NUM_DIG_PINS; ++i) {
  //    pinMode(PIN_0 + i, OUTPUT);
  //  }

  Bean.enableWakeOnConnect(true);
  m_lastUpdate = millis();
}


void sendLectures() {
  if (Serial.available()) {
    char data = 'q';
    while (Serial.available()) {
      // if message is empty or ends with 'q', stop transmition.
      m_transmit = (data != 'q');
      data = Serial.read();
      Serial.print(data);
    }
    SEND_EOM_CODE
  }
  
  if (!m_transmit){
    Serial.print("Not transmitting.");
    SEND_EOM_CODE
    return;
  }

  StaticJsonBuffer<200> m_jsonBuffer;
  JsonObject &root = m_jsonBuffer.createObject();
  JsonArray &colors = root.createNestedArray("color");
  
  int8_t currTemp = Bean.getTemperature();
  int8_t battery = Bean.getBatteryLevel();
  int r = random(256), g = random(256), b = random(256);
//  Bean.setLed(r, g, b);
  
  colors.add(r);
  colors.add(g);
  colors.add(b);
  root[TEMP] = currTemp;
  root[SECONDS] = ++m_counter;
  root[BATTERY] = battery;
  root[LAST_TRANSMITION] = millis() - m_lastUpdate;
  m_lastUpdate = millis();
  root.printTo(Serial);
//  root.prettyPrintTo(Serial);
  SEND_EOM_CODE
}

void loop() {
  //  digitalRead - digitlaWrite
  //  Serial.read()
  if (Bean.getConnectionState()) {
    sendLectures();
    Bean.sleep(1000);
  } else {
    m_transmit = false;
    Bean.setLed(0, 0, 0);
    Bean.sleep(0xFFFFFFFF);
  }
}


void updatePinOutput() {
  int pinOn = m_counter % 6;
  for (int i = 0; i < NUM_DIG_PINS; ++i) {
    digitalWrite(PIN_0 + i, i == pinOn ? HIGH : LOW);
  }
}
