#include<Arduino.h>
#include <PDM.h>
#include<ggcom.h>

GGComRX<> ggcom(3);

void onPDMdata() {
  auto available_bytes = PDM.available();
  auto available_samples = available_bytes / 2; // TODO: make this more generic
  auto buffer = ggcom.get_sample_buffer_for_samples(available_samples);
  PDM.read(buffer, available_bytes);
}

void setup() {
  Serial.begin(57600);
  while (!Serial);

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(1, 6000)) {
      Serial.println(F("Failed to start PDM!"));
      while (1);
  }
}

void loop() {
  ggcom.loop();
}