# ESP32-P4 Voice Assistant Satellite (Ethernet)

This repository contains a high-performance ESPHome configuration for the **Waveshare ESP32-P4-WiFi6** development board, optimized for use as a Home Assistant Voice Assistant satellite.

## Features

- ⚡ **Ultra-Fast Response**: Optimized I2S handover delays (~450ms) for near-instant voice recognition after wake word detection.
- 🎙️ **Micro Wake Word**: Supports "Hey Jarvis" and "Okay Nabu" on-device wake word detection.
- 🔊 **Stable Audio Pipeline**: Uses a custom PCM-to-C-header embedding strategy to avoid `ESP_ERR_NO_MEM` crashes common with MP3/WAV decoders on ESP32.
- 🌐 **Ethernet Optimized**: Uses the onboard IP101 PHY for reliable, low-latency connectivity (Wi-Fi is disabled to save resources).
- 🎛️ **Runtime Controls**: Volume, Microphone Gain, and Noise Suppression levels are exposed as entities in Home Assistant.
- 🛠️ **Diagnostic Tools**: Built-in speaker test switch and system health diagnostics button.

## Hardware Configuration (Waveshare ESP32-P4)

| Component | Pin / Setting |
|-----------|---------------|
| **I2C SDA** | GPIO7 |
| **I2C SCL** | GPIO8 |
| **I2S MCLK** | GPIO13 |
| **I2S BCLK** | GPIO12 |
| **I2S LRCLK** | GPIO10 |
| **I2S DOUT** | GPIO9 (Speaker) |
| **I2S DIN** | GPIO11 (Microphone) |
| **Amp Enable** | GPIO53 |
| **Ethernet** | IP101 (PHY Addr 1) |

## Implementation Details

### I2S Bus Sharing
The ESP32-P4 uses a single I2S bus for both input (Mic) and output (Speaker). To prevent driver conflicts, this configuration implements a strict handover logic:
1. **Stop** the active component (e.g., `microphone.stop()`).
2. **Wait** (300ms) for the I2S hardware lock to be released.
3. **Prime** the new component (using a short silent PCM burst) to initialize the DMA pipeline.
4. **Start** the next component.

### Memory Optimization
Standard ESPHome `play_file` actions for MP3 or WAV often cause `ESP_ERR_NO_MEM` due to the large buffers required by decoders. This project solves this by:
1. Converting notification sounds to raw **16kHz 16-bit Mono PCM**.
2. Embedding the raw bytes into a C header file (`notification_pcm.h`) using `PROGMEM`.
3. Playing the raw bytes directly via `id(mixer).play()`, which consumes zero additional heap memory.

## How to use

1. Copy `esp32-p4-devkit.yaml` and `notification_pcm.h` to your ESPHome configuration directory.
2. Update the `!secret` values (encryption key, etc.) in your `secrets.yaml`.
3. Flash the firmware to your Waveshare ESP32-P4.
4. In Home Assistant, go to **Settings > Voice Assistants** and ensure your entities (Lights, etc.) are **Exposed** to the satellite.

## License

MIT
