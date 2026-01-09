/* TMC5160 SPI example

This code demonstrates the usage of a Trinamic TMC5160 stepper driver in SPI mode.

Hardware setup :
Connect the following lines between the microcontroller board and the TMC5160 driver
(Tested with a Teensy 3.2 and a TMC5160-BOB)

  MOSI (Teensy : 11)  <=> SDI
  MISO (Teensy : 12)  <=> SDO
  SCK (Teensy : 13)   <=> SCK
  37                   <=> CSN
  41                   <=> DRV_ENN (optional, tie to GND if not used)
  GND                 <=> GND
  3.3V/5V             <=> VCC_IO (depending on the processor voltage)

The TMC5160 VS pin must also be powered.
Tie CLK16 to GND to use the TMC5160 internal clock.
Tie SPI_MODE to VCC_IO, SD_MODE to GND.

Please run the Config Wizard for power stage fine tuning / motor calibration (this code uses the default parameters and auto calibration).

Copyright (c) 2020 Tom Magnier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <SPI.h>
#include <TMC5160.h>

// Teensy 4.1 SPI pins: MOSI0 = 11, MISO0 = 12, CS0 = 10, SCK0 = 13

const uint8_t SPI_DRV_ENN_1 = 5;  // DRV_ENN pin in SPI mode
//const uint8_t SPI_DRV_ENN_2 = 40;  // DRV_ENN pin in SPI mode
//const uint8_t SPI_DRV_ENN_3 = 16;  // DRV_ENN pin in SPI mode
//const uint8_t SPI_DRV_ENN_4 = 17;  // DRV_ENN pin in SPI mode

const uint8_t SPI_CS_1 = 10;  // CS pin in SPI mode (was 33)
//const uint8_t SPI_CS_2 = 36;  // CS pin in SPI mode
//const uint8_t SPI_CS_3 = 15;  // CS pin in SPI mode
//const uint8_t SPI_CS_4 = 6;  // CS pin in SPI mode

TMC5160_SPI motor_1 = TMC5160_SPI(SPI_CS_1);  //Use default SPI peripheral and SPI settings.
//TMC5160_SPI motor_2 = TMC5160_SPI(SPI_CS_2);  //Use default SPI peripheral and SPI settings.
//TMC5160_SPI motor_3 = TMC5160_SPI(SPI_CS_3);  //Use default SPI peripheral and SPI settings.
//TMC5160_SPI motor_4 = TMC5160_SPI(SPI_CS_4);  //Use default SPI peripheral and SPI settings.


void setup() {
  // USB/debug serial coms
  Serial.begin(115200);

  pinMode(SPI_DRV_ENN_1, OUTPUT);
  digitalWrite(SPI_DRV_ENN_1, LOW);  // Active low
  
  //pinMode(SPI_DRV_ENN_2, OUTPUT);
  //digitalWrite(SPI_DRV_ENN_2, LOW);  // Active low
  
  //pinMode(SPI_DRV_ENN_3, OUTPUT);
  //digitalWrite(SPI_DRV_ENN_3, LOW);  // Active low
  
  //pinMode(SPI_DRV_ENN_4, OUTPUT);
  //digitalWrite(SPI_DRV_ENN_4, LOW);  // Active low

  SPI.begin();

  delay(100); // let SPI settle

  // This sets the motor & driver parameters /!\ run the configWizard for your driver and motor for fine tuning !
  TMC5160::PowerStageParameters powerStageParams;  // defaults.
  TMC5160::MotorParameters motorParams;
  motorParams.globalScaler = 200;  // Adapt to your driver and motor (check TMC5160 datasheet - "Selecting sense resistors")
  motorParams.irun = 31;
  motorParams.ihold = 16;

  motor_1.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
  //motor_2.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
  //motor_3.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);
  //motor_4.begin(powerStageParams, motorParams, TMC5160::NORMAL_MOTOR_DIRECTION);

  // ramp definition
  motor_1.setRampMode(TMC5160::POSITIONING_MODE);
  motor_1.setMaxSpeed(400);
  motor_1.setAcceleration(500);

  //motor_2.setRampMode(TMC5160::POSITIONING_MODE);
  //motor_2.setMaxSpeed(400);
  //motor_2.setAcceleration(500);

  //motor_3.setRampMode(TMC5160::POSITIONING_MODE);
  //motor_3.setMaxSpeed(400);
  //motor_3.setAcceleration(500);

  //motor_4.setRampMode(TMC5160::POSITIONING_MODE);
  //motor_4.setMaxSpeed(400);
  //motor_4.setAcceleration(500);

  Serial.println("starting up");

  delay(500);  // Standstill for automatic tuning
}

unsigned long t_dirchange = 0;
unsigned long t_echo = 0;
bool dir = false;

void loop() {
  unsigned long now = millis();

  // every n seconds or so...
  if (now - t_dirchange > 2000) {
    t_dirchange = now;

    // reverse direction
    dir = !dir;

    motor_1.setCurrentPosition(0);
    //motor_2.setCurrentPosition(0);
    //motor_3.setCurrentPosition(0);
    //motor_4.setCurrentPosition(0);

    //motor_1.setTargetPosition(2000);  // 1 full rotation = 200s/rev
    //motor_2.setTargetPosition(2000);  // 1 full rotation = 200s/rev
    //motor_3.setTargetPosition(2000);  // 1 full rotation = 200s/rev
    //motor_4.setTargetPosition(2000);  // 1 full rotation = 200s/rev

    motor_1.setTargetPosition(dir ? 200 : 0);  // 1 full rotation = 200s/rev
    //motor_2.setTargetPosition(dir ? 200 : 0);  // 1 full rotation = 200s/rev
    //motor_3.setTargetPosition(dir ? 200 : 0);  // 1 full rotation = 200s/rev
    //motor_4.setTargetPosition(dir ? 200 : 0);  // 1 full rotation = 200s/rev
  }

  // print out current position
  if (now - t_echo > 100) {
    t_echo = now;

    // get the current target position

    float xactual = motor_1.getCurrentPosition();
    float vactual = motor_1.getCurrentSpeed();

    Serial.print("Motor current position : ");
    Serial.print(xactual);
    Serial.print("\tcurrent speed : ");
    Serial.println(vactual);

    TMC5160::DriverStatus status = motor_1.getDriverStatus();

    Serial.print(">>> DriverStatus as enum: 0x");
    Serial.println(status, HEX);



    const char* status_as_english = motor_1.getDriverStatusDescription(status);



    Serial.print(">>> DriverStatus as english: ");
    Serial.println(status_as_english);
  }
}
