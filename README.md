# ESP32-P4 Voice Assistant Satellite (Ethernet)

This repository contains a high-performance ESPHome configuration for the **Waveshare ESP32-P4-WiFi6** development board, optimized for use as a Home Assistant Voice Assistant satellite.

## Features

- ⚡ **Ultra-Fast Response**: Optimized I2S handover delays and strict hardware release rules to prevent bus locking.
- 🎙️ **Micro Wake Word**: Supports on-device wake word detection ("Hey Jarvis").
- 🔊 **Standalone Audio Pipeline**: Avoids `ESP_ERR_NO_MEM` crashes by skipping heavy MP3/WAV decoders. Instead, uses an optimized, built-in algorithmic sine-wave generator in C++ for notification sounds.
- 🌐 **Ethernet Optimized**: Uses the onboard IP101 PHY for reliable, low-latency connectivity (Wi-Fi is disabled to save resources) and includes a custom UDP "Wakeup Ping" to keep LAN Powerline adapters active.
- ⚙️ **Plug & Play Configuration**: 100% self-contained in a single YAML file with an easy-to-use `substitutions` block at the top. No external files needed!
- 🎛️ **Runtime Controls**: Volume, Microphone Gain, and Noise Suppression levels are exposed as entities in Home Assistant.

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
Standard ESPHome `play_file` actions for MP3 or WAV often cause `ESP_ERR_NO_MEM` due to the large buffers required by decoders. This project solves this by synthesizing the Voice Assistant notification 'Pling' mathmetically in real-time. This saves over 32KB of flash memory and makes the configuration fully standalone without requiring external sound files.

## How to use

1. Copy `esp32-p4-devkit.yaml` to your ESPHome configuration directory. 
2. Update the variables in the `substitutions` block at the very top of the file to match your `secrets.yaml` (e.g., your Home Assistant IP, OTA password, and Encryption key).
3. Flash the firmware to your Waveshare ESP32-P4.
4. In Home Assistant, go to **Settings > Voice Assistants** and ensure your entities are **Exposed** to the satellite.

## License

MIT
