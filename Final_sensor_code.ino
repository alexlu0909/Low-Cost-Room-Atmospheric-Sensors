#include <Wire.h>

#define MPL3115A2_ADDRESS 0x60
#define LIGHT_SENSOR_ADDRESS 0x23
#define ATMOSPHERIC_SENSOR_ADDRESS 0x60
#define REGISTER_ADDRESS 0x68 
#define CTRL_REG1 0x26
#define OUT_T_MSB 0x04 //temperature data
#define OUT_P_MSB 0x01 //pressure data
void setup() {
    Wire.begin();
    Serial.begin(115200);

    // initialize MPL3115A2
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(CTRL_REG1);
    Wire.write(0x38); // ALT=0（pressure mode），SBYB=1（active mode）
    Wire.endTransmission();
    delay(10);

    Wire.beginTransmission(LIGHT_SENSOR_ADDRESS);
    Wire.write(0x01);
    if (Wire.endTransmission() != 0) {
    Serial.println("Error: Unable to communicate with I2C device.");
    delay(1000);
    return;
  }  
  Wire.beginTransmission(LIGHT_SENSOR_ADDRESS);
    Wire.write(0x10);
    Wire.endTransmission();
    delay(100);

    Serial.println("MPL3115A2 Initialized for Pressure and Temperature Measurement");



     pinMode(LED_BUILTIN, OUTPUT); //notification

}

void loop() {
    // single detection
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(CTRL_REG1);
    Wire.write(0x39); // OST=1，single detection
    Wire.endTransmission();
    delay(100); // waiting


    // read detected data
    Serial.print("Received Light level data: ");
    Serial.println(readLight());

    readTemperature();
    readPressure();


    delay(1000);
}


uint16_t readLight(){
  Wire.requestFrom(LIGHT_SENSOR_ADDRESS, 2, true);
  uint8_t msb = Wire.read(); // Most significant byte
  uint8_t lsb = Wire.read(); // Least significant byte
  return (msb << 8) | lsb;
}

void readTemperature(){
  Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(OUT_T_MSB);
    Wire.endTransmission(false);

    Wire.requestFrom(MPL3115A2_ADDRESS, 2);
    if (Wire.available() == 2) {
        uint16_t temp = Wire.read() << 8 | Wire.read(); // 合併高低字節
        Serial.print("Raw Temp: ");
        Serial.println(temp, HEX);
        float temperature = (float)((temp >> 8) + ((temp & 0xFF) / 256.0)) * 1.8 + 32.0; // transfer unit
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °F");
    } else {
        Serial.println("Failed to read temperature data");
    }
}

void readPressure(){
  Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(OUT_P_MSB); // read from OUT_P_MSB 
    Wire.endTransmission(false);

    Wire.requestFrom(MPL3115A2_ADDRESS, 3); // ask for 3 words data
    if (Wire.available() == 3) {
      uint32_t pressure = Wire.read() << 16; // MSB
      pressure |= Wire.read() << 8;          // CSB
      pressure |= Wire.read() & 0xF0;        // LSB 高 4 位有效
      pressure >>= 4;                        // shift right 4 bits (20 bits data)
      Serial.print("Raw Pressure: ");
      Serial.println(pressure);
      if (pressure > 0) {
        Serial.print("Pressure: ");
        Serial.print((pressure / 4.0)/3386.39); //transfer unit
        Serial.println(" inHg");
      } else {
        Serial.println("Failed to read pressure data");
      }
    }
}

/*void setNotify(data){
  if(data>=1200 && ){

  }
}*/






