#include <Wire.h>

#define MPL3115A2_ADDRESS 0x60 // Default I2C address for MPL3115A2

// MPL3115A2 Registers
#define CTRL_REG1 0x26
#define OUT_P_MSB 0x01 // Pressure/Altitude MSB register
#define OUT_T_MSB 0x04 // Temperature MSB register

void setup() {
    Wire.begin(); // Initialize I2C with SDA on GPIO 4, SCL on GPIO 5
    Serial.begin(115200);
     while (!Serial);{}
    // Initialize MPL3115A2 in Altimeter mode
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(CTRL_REG1);
    Wire.write(0xB8); // Altimeter mode, active, oversampling = 128
    Wire.endTransmission();
    delay(10); // Allow time for sensor to initialize

    Serial.println("MPL3115A2 Initialized!");
}

void loop() {
    //float altitude = readAltitude();
    float temperature = readTemperature();

   // Serial.print("Altitude: ");
   // Serial.print(altitude);
   // Serial.println(" meters");

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    delay(1000); // Read data every second
}
/*
float readAltitude() {
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(OUT_P_MSB);
    Wire.endTransmission(false);

    Wire.requestFrom(MPL3115A2_ADDRESS, 3); // Request 3 bytes
    if (Wire.available() == 3) {
        uint32_t alt = Wire.read(); // MSB
        alt <<= 8;
        alt |= Wire.read(); // CSB
        alt <<= 8;
        alt |= Wire.read(); // LSB
        alt >>= 4;          // Shift to 20-bit value

        return (float)alt / 16.0; // Convert to meters
    }
    return 0;
}
*/
float readTemperature() {
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(OUT_T_MSB);
    Wire.endTransmission(false);

    Wire.requestFrom(MPL3115A2_ADDRESS, 2); // Request 2 bytes
    if (Wire.available() == 2) {
        uint16_t temp = Wire.read() << 8 | Wire.read();
        Serial.print(temp);
        return (float)(temp >> 8) + ((temp & 0xFF) / 256.0);
    }
    return 0;
}

