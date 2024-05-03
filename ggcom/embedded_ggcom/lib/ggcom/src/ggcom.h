#include <ggwave.h>

template <typename TSample = int16_t, size_t qmax = 512, size_t samples_per_frame = 128, size_t sample_rate = 6000>
class GGComRX {
public:
  GGComRX(uint8_t message_size) {
    ggwave.setLogFile(nullptr);
    

    p = GGWave::getDefaultParameters();
    p.payloadLength = message_size + 1;
    p.sampleRateInp   = sample_rate;
    p.sampleRateOut   = sample_rate;
    p.sampleRate      = sample_rate;
    p.samplesPerFrame = samples_per_frame;
    p.sampleFormatInp = GGWAVE_SAMPLE_FORMAT_I16;
    p.sampleFormatOut = GGWAVE_SAMPLE_FORMAT_U8;
    p.operatingMode   = GGWAVE_OPERATING_MODE_RX;

    GGWave::Protocols::tx().disableAll();
    GGWave::Protocols::rx().disableAll();
    GGWave::Protocols::rx().toggle(GGWAVE_PROTOCOL_DT_FASTEST, true);
    ggwave.prepare(p, true);
  }

  void loop() {
    while (qsize >= samples_per_frame) {

        // We have enough captured samples - try to decode any "ggwave" data:
        auto tStart = millis();

        ggwave.decode(sample_buffer + qhead, samples_per_frame*sizeof(TSample));
        qsize -= samples_per_frame;
        qhead += samples_per_frame;
        if (qhead >= qmax) {
            qhead = 0;
        }

        auto tEnd = millis();

        if (tEnd - tStart > 1000*(float(samples_per_frame)/sample_rate)) {
            Serial.println(F("Warning: decode() took too long to execute!"));
        }

        // Check if we have successfully decoded any data:
        GGWave::TxRxData result;
        ggwave.rxTakeData(result);
        if(result.size() == p.payloadLength) {
            // auto id_changed = id != prev_id;
            auto all_data_same = true;
            for(int i = 1; i < p.payloadLength; i++) {
                all_data_same = all_data_same and result.data()[i] == result.data()[1];
            }

            if (all_data_same) {
                auto id = result.data()[0];
                auto id_increased = id > prev_id;
                if (id_increased) {
                    if (not(id - 1 == prev_id) and prev_id != -1) {
                        Serial.print("Prev:");
                        Serial.print(prev_id);
                        Serial.print(", id: ");
                        Serial.print(id);
                        Serial.println(" ->probably lost packet!");
                    }
                    Serial.println((char)result.data()[1]);
                    prev_id = id;
                } else {
                    // Serial.println(F("ID is the same. Ignoring!!"));
                }
            }
        }



    }

    // This should never happen.
    // If it does - there is something wrong with the audio capturing callback.
    // For example, the microcontroller is not able to process the captured data in real-time.
    if (err > 0) {
        Serial.println(F("ERRROR"));
        Serial.println(err);
        err = 0;
    }
  }

    TSample *get_sample_buffer_for_samples(size_t n) {
        if(qsize + n > qmax) {
            // If you hit this error, try to increase qmax
            err += 10;

            qtail = 0;
            qhead = 0;
            qsize = 0;
        }

        auto buffer = sample_buffer + qtail;
        qtail += n;
        qsize += n;

        if(qtail > qmax){
            // If you hit this error, qmax is probably not a multiple of the recorded samples
            err += 1;
        }

        if(qtail >= qmax){
            qtail -= qmax;
        }
        return buffer;
    }


private:
    GGWave ggwave;
    TSample sample_buffer[qmax];
    GGWave::Parameters p;
    int prev_id = -1;

    volatile size_t qhead = 0;
    volatile size_t qtail = 0;
    volatile size_t qsize = 0;
    volatile int err = 0;
};