#include <Arduino.h>
#include <SlowSoftI2CMaster.h>

// Change to LDAC pin here
#define LDAC_PIN 2

// Define address here (0-7)
#define OLD_ADDRESS 4
#define NEW_ADDRESS 4

#ifdef CORE_TEENSY
#define SDA_PIN 17
#define SCL_PIN 16
#else  // Arduino
#define SDA_PIN A4
#define SCL_PIN A5
#endif

SlowSoftI2CMaster i2c = SlowSoftI2CMaster(A3, A2, true);

void setup() {
  Serial.begin(9600);
  pinMode(LDAC_PIN, OUTPUT);

  //i2c.i2c_init(SCL_PIN, SDA_PIN);
  delay(250);

  writeAddress(OLD_ADDRESS, NEW_ADDRESS, LDAC_PIN);
  delay(250);

  uint8_t addr = readAddress(LDAC_PIN);
  Serial.println("Address: ");
  Serial.println(addr);
  Serial.println("Done.");
}

void loop() {}

uint8_t readAddress(int LDACpin) {
  digitalWrite(LDACpin, HIGH);
  int ack1 = i2c.i2c_start(0B00000000);
  int ack2 = i2c.i2c_write(0B00001100);  // modified command for LDAC pin latch
  int ack3 = i2c.i2c_start(0B11000001);
  uint8_t address = i2c.i2c_read(true);
  i2c.i2c_stop();
  delay(100);
  digitalWrite(LDACpin, HIGH);
  Serial.println("");
  Serial.println(ack1);
  Serial.println(ack2);
  Serial.println(ack3);
  return address;
}

void writeAddress(int oldAddress, int newAddress, int LDACpin) {
  digitalWrite(LDACpin, HIGH);
  int ack1 = i2c.i2c_start(0B11000000 | (oldAddress << 1));
  int ack2 = i2c.i2c_write(0B01100001 | (oldAddress << 2));  // modified command for LDAC pin latch
  int ack3 = i2c.i2c_write(0B01100010 | (newAddress << 2));
  int ack4 = i2c.i2c_write(0B01100100 | (newAddress << 2));
  i2c.i2c_stop();
  delay(100);  // wait for eeprom write
  Serial.println("");
  Serial.println(ack1);
  Serial.println(ack2);
  Serial.println(ack3);
  Serial.println(ack4);
}