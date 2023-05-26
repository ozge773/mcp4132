/*
   MCP4132 Dual 100K Digital Pot Demo
   Based on example at https://github.com/GadgetReboot/MCP42100_Demo
   This code is the pin and resitance implementation for the MCP4132

   pot 1 controls the op amp gain
   pot 0 controls the offset (wiper should be near mid point for full swing)

   Arduino Uno SPI connections:

   Uno            MCP42100
   -------------------------
   D2            CS  (pin 1)
   D11 (MOSI)    SDI  (pin 3)
   D13 (SCK)     SCK (pin 2)

   Gadget Reboot
*/

#include <SPI.h>

const byte csPin           = 2;       // MCP4132 chip select pin
const int  maxPositions    = 128;     // wiper can move from 0 to 125 =126 positions
const long rAB             = 100000;  // 100k pot resistance between terminals A and B
const byte rWiper          = 75;     // 75 ohms pot wiper resistance from datasheet
const byte pot            = 0x11;    //


void setup() {

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();

}

void loop() {

  setPotWiper(pot, 64);                  // op amp gain approx. -1, where  (-Rb/Ra) = (-50k/50k)

  delay(500);

  // moving the wiper 
  for (int i = 65; i < 110; i++) {
    setPotWiper(pot, i);
    delay(50);
  }

  for (int i = 110; i > 15; i--) {
    setPotWiper(pot, i);
    delay(50);
  }

}

void setPotWiper(int addr, int pos) {

  pos = constrain(pos, 0, 125);            // limit wiper setting to range of 0 to 125

  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance wiper to B terminal: ");
  Serial.print(resistanceWB);
  Serial.println(" ohms");

}
