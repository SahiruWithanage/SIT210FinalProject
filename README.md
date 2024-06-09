# 🎵 Automated Volume Adjustment System (AVAS) 🎵

Welcome to my project! This project lets you stream music from a Raspberry Pi to an Arduino Nano 33 IoT and adjusts the volume of your speakers based on how close you are to them. It's a fun and interactive way to experience music!

## 📦 What You Need

### Hardware:
- **Arduino Nano 33 IoT**
- **Raspberry Pi** (for streaming audio)
- **Two Ultrasonic Sensors** (like the HC-SR04)
- **External DAC** (Digital-to-Analog Converter)
- **I2S Audio Interface**
- **Two Speakers** (Left and Right)

### Software:
- **Arduino IDE**: [Download here](https://www.arduino.cc/en/software)
- **Python 3**: Make sure it's installed on your Raspberry Pi
- **FFmpeg**: For audio processing on the Raspberry Pi
- **Librespot**: To stream Spotify music

## 🚀 Getting Started

### Step 1: Set Up Your Raspberry Pi

1. **Install FFmpeg**:
    ```bash
    sudo apt-get update
    sudo apt-get install ffmpeg
    ```

2. **Install Librespot**:
    Follow the instructions from the [Librespot GitHub repository](https://github.com/librespot-org/librespot).

3. **Clone This Repository**:
    ```bash
    git clone https://github.com/yourusername/your-repo.git
    cd your-repo
    ```

4. **Install Required Python Packages**:
    ```bash
    pip3 install fastapi uvicorn pydub
    ```

5. **Run the Server**:
    ```bash
    python3 spotify_streamer.py
    ```

### Step 2: Set Up Your Arduino Nano

1. **Connect the Hardware**:
    - Connect the ultrasonic sensors to the appropriate pins on the Arduino.
    - Connect the speakers to the DAC.
    - Connect the DAC to the I2S interface on the Arduino.
    - Connect the WiFi module to the Arduino.

2. **Upload the Arduino Sketch**:
    - Open the Arduino IDE.
    - Load the `ClientCode.ino` file.
    - Update the WiFi credentials and Raspberry Pi IP address in the code.
    - Upload the code to your Arduino Nano.

### Step 3: Enjoy Your Music!

1. **Start the Raspberry Pi Server**:
    Ensure the server is running on your Raspberry Pi by executing:
    ```bash
    python3 spotify_streamer.py
    ```

2. **Power Up the Arduino**:
    Ensure your Arduino is powered and connected to the same network as your Raspberry Pi.

3. **Experience Interactive Audio**:
    The Arduino will connect to the stream and play music through the speakers. Move closer to one speaker to decrease its volume and increase the volume of the opposite speaker. Move closer to the other speaker to reverse the effect. Enjoy the dynamic sound experience!

## 🔧 Troubleshooting

- **No Audio**: Check the DAC and I2S connections.
- **WiFi Issues**: Verify the SSID and password in the Arduino code.
- **Laggy Audio**: Try adjusting the buffer size in the Arduino code for better performance.

Happy Listening! 🎧
