// Fastio.h is licensed GPL2 from github.com/MarlinFirmware/Mariln
#include "fastio.h"

// see https://www.pjrc.com/teensy/td_libs_TimerOne.html
// for Timer1/Timer3 documentation
// Timer3 is licensed Creative Commons Attribution
// Authored by Paul Stoffregen
#include <TimerThree.h>

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG;  cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START

// x motor
// fastio pin numbers (see https://labitat.dk/wiki/Panelolu_and_Printrboard_the_easy_way )
#define MOTOR_0_STEP_PIN 0
#define MOTOR_0_DIR_PIN 1
#define MOTOR_0_EN_PIN 39


void setup() {
  SET_OUTPUT(MOTOR_0_STEP_PIN);
  SET_OUTPUT(MOTOR_0_DIR_PIN);
  SET_OUTPUT(MOTOR_0_EN_PIN);
  WRITE(MOTOR_0_STEP_PIN, LOW);
  WRITE(MOTOR_0_DIR_PIN, LOW);
  WRITE(MOTOR_0_EN_PIN, LOW);

  // setup Timer
#define STEP_INTERRUPT_FREQUENCY 10000
  Timer3.initialize(1000000/STEP_INTERRUPT_FREQUENCY); // microseconds
  Timer3.attachInterrupt(stepInterrupt);
}     

int xspeed = 0; // signed # of steps/sec
long xpos = 0; // used to find when to step

#define X_SPEED_INCREMENT 100
int xspeed_increment_demo = X_SPEED_INCREMENT;
int xspeed_increment_demo_max =  20000;
int xspeed_increment_demo_min = -20000;
void loop() {
  xspeed += xspeed_increment_demo;
  if (xspeed > xspeed_increment_demo_max) {
    xspeed_increment_demo = - X_SPEED_INCREMENT;
  }
  if (xspeed < xspeed_increment_demo_min) {
    xspeed_increment_demo = X_SPEED_INCREMENT;
  }
  delay(10);
}

void stepInterrupt(void) {
  if (xspeed > 0) {
    WRITE(MOTOR_0_DIR_PIN, LOW);
  } else {
    WRITE(MOTOR_0_DIR_PIN, HIGH);
  }

  xpos += xspeed;
  int step = 0;
  while (xpos > STEP_INTERRUPT_FREQUENCY) {
    xpos -= STEP_INTERRUPT_FREQUENCY;
    step += 1;
  }
  while (xpos < -STEP_INTERRUPT_FREQUENCY) {
    xpos += STEP_INTERRUPT_FREQUENCY;
    step += 1;
  }

  while (step--) {
    WRITE(MOTOR_0_STEP_PIN,LOW);
    // looking for over 1usec step time, delay some:
    __asm__ __volatile__ ("nop\n\t");
    __asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    // __asm__ __volatile__ ("nop\n\t");
    //__asm__ __volatile__ ("nop\n\t");
    WRITE(MOTOR_0_STEP_PIN,HIGH);
  }
}
