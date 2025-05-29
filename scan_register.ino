#include <Wire.h>

#define MPL3115A2_ADDRESS 0x60 // 默認 I2C 地址
#define CTRL_REG1 0x26
#define OUT_P_MSB 0x01

void setup() {
    Wire.begin(); // 初始化 I2C，SDA = GPIO 4，SCL = GPIO 5
    Serial.begin(115200);

    // 初始化傳感器
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(CTRL_REG1);
    Wire.write(0xB8); // ALT=1 (高度計模式)，SBYB=1 (啟動模式)，OSR=128
    Wire.endTransmission();
    delay(10); // 等待傳感器初始化完成

    // 啟動單次測量
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(CTRL_REG1);
    Wire.write(0xB9); // OST=1，啟動單次測量
    Wire.endTransmission();
    delay(100); // 等待測量完成
}

void loop() {
    // 讀取 CTRL_REG1 的值
    byte ctrl_reg1_value = readRegister(CTRL_REG1);
    Serial.print("CTRL_REG1 Value: 0x");
    Serial.println(ctrl_reg1_value, HEX);

    // 讀取 OUT_P_MSB 的值
    byte out_p_msb_value = readRegister(OUT_P_MSB);
    Serial.print("OUT_P_MSB Value: 0x");
    Serial.println(out_p_msb_value, HEX);

    delay(1000); // 每秒讀取一次
}

byte readRegister(byte reg) {
    Wire.beginTransmission(MPL3115A2_ADDRESS);
    Wire.write(reg); // 指定寄存器
    Wire.endTransmission(false); // 重啟條件

    Wire.requestFrom(MPL3115A2_ADDRESS, 1); // 請求 1 字節數據
    if (Wire.available()) {
        return Wire.read(); // 返回數據
    }
    return 0; // 如果讀取失敗，返回 0
}

