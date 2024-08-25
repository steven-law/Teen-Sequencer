// Basic demo for configuring the MCP4728 4-Channel 12-bit I2C DAC
#include <Adafruit_MCP4728.h>
#include <Wire.h>

Adafruit_MCP4728 mcp;
Adafruit_MCP4728 mcp2;

void setup(void) {
  Wire1.begin();
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MCP4728 test!");

  // Try to initialize!
  if (!mcp.begin(0x64, &Wire1)) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }
    // Try to initialize!
  if (!mcp2.begin(0x67, &Wire1)) {
    Serial.println("Failed to find MCP4728 chip2");
    while (1) {
      delay(10);
    }
  }

  mcp.setChannelValue(MCP4728_CHANNEL_A, 4095);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 2048);
  mcp.setChannelValue(MCP4728_CHANNEL_C, 1024);
  mcp.setChannelValue(MCP4728_CHANNEL_D, 0);

  mcp2.setChannelValue(MCP4728_CHANNEL_A, 3575);
  mcp2.setChannelValue(MCP4728_CHANNEL_B, 1500);
  mcp2.setChannelValue(MCP4728_CHANNEL_C, 500);
  mcp2.setChannelValue(MCP4728_CHANNEL_D, 4095);
}

void loop() { delay(1000); }
