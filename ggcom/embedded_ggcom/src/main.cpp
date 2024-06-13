#include<Arduino.h>
#include <PDM.h>
#include<ggcom.h>
#include<ArduinoJson.h>

GGComRX<> ggcom(3);

void onPDMdata() {
  auto available_bytes = PDM.available();
  auto available_samples = available_bytes / 2; // TODO: make this more generic
  auto buffer = ggcom.get_sample_buffer_for_samples(available_samples);
  PDM.read(buffer, available_bytes);
}

void setup() {
  Serial.begin(57600);
  Serial1.begin(9600);
  while (!Serial);

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, 6000)) {
      Serial.println(F("Failed to start PDM!"));
      while (1);
  }
}
void sendLine(float x1, float y1, float x2, float y2) {
    // pack line into json
    JsonDocument doc;
    doc["x1"] = x1;
    doc["y1"] = y1;
    doc["x2"] = x2;
    doc["y2"] = y2;
    
    // serialize to Serial1
    serializeJson(doc, Serial1);
}

void loop() {
  sendLine(random(0, 50), random(0, 50), random(0, 50), random(0, 50));
  delay(1000);
}