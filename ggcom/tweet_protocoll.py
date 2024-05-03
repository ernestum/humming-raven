import pyaudio
import ggwave

class Tweeter:
    def __init__(self, message_size: int):
        self.message_counter = 0
        self.audio = pyaudio.PyAudio()
        self.message_size = message_size
        params = ggwave.getDefaultParameters()
        params['payloadLength'] = message_size*3 + 1
        ggwave.init(params)

    def tweet(self, message: str):
        if len(message) != self.message_size:
            raise ValueError(f"Message size must be {self.message_size} characters long")
        message_and_counter = chr(self.message_counter) + message * 3
        waveform = ggwave.encode(message_and_counter, protocolId=8, volume=100)
        stream = self.audio.open(format=pyaudio.paFloat32, channels=1, rate=48000, output=True, frames_per_buffer=4096)
        stream.write(waveform, len(waveform)//4)
        stream.stop_stream()
        stream.close()
        self.message_counter += 1
        if self.message_counter > 255:
            self.message_counter = 0

    def __del__(self):
        self.audio.terminate()
