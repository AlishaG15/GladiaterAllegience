#include <Arduino.h>

// ── Pin assignments ───────────────────────────────────────────────────────────
#define EYE_PIN    5
#define NOSE_PIN   6
#define HB_PIN     13

#define BUTTON_PIN 2
#define POT_EYE    A0
#define POT_NOSE   A1

// ── Limits ────────────────────────────────────────────────────────────────────
#define ADC_MAX 1023
#define PWM_MAX 255

// ── Timing (ms) ───────────────────────────────────────────────────────────────
#define EYE_TIME   5000UL
#define NOSE_TIME  10000UL
// Half-period bounds for nose blink in milliseconds.
// Smaller half-period = faster blink rate.
#define BLINK_MIN  50UL    // shortest half-period (~10 Hz, fastest blink)
#define BLINK_MAX  100UL   // longest  half-period (~5  Hz, slowest blink)
#define HB_HALF    500UL

// ── Function prototypes ───────────────────────────────────────────────────────
void buttonISR();
void readInputs(int &eye_raw, int &nose_raw);
void computeParams(int eye_raw, int nose_raw);
void updateHeartbeat();

// ── State machine ─────────────────────────────────────────────────────────────
// IDLE:        waiting for button press; all LEDs off
// ANALOG_READ: read potentiometers and compute parameters; transition to LED_OUTPUT
// LED_OUTPUT:  drive eye and nose LEDs per spec; return to IDLE when done
enum State { IDLE, ANALOG_READ, LED_OUTPUT };
volatile State state = IDLE;

// ── Global variables ──────────────────────────────────────────────────────────
// eye_raw / nose_raw removed — they are now locals passed between readInputs() and computeParams()

int eye_brightness = 0;        // PWM value for eye LEDs (0–255)
unsigned long blink_half = 0;  // nose blink half-period (ms)

unsigned long start_time = 0;
unsigned long last_blink = 0;
unsigned long last_hb    = 0;

bool nose_on    = false;
bool entry_flag = false;  // set true when first entering LED_OUTPUT

void setup() {
  pinMode(EYE_PIN,    OUTPUT);
  pinMode(NOSE_PIN,   OUTPUT);
  pinMode(HB_PIN,     OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // button is active LOW

  // trigger ISR on falling edge (button press)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  updateHeartbeat();  // blink onboard LED at 1 Hz in all states

  switch (state) {

    case IDLE:
      // keep LEDs off while waiting for button press
      analogWrite(EYE_PIN,  0);
      analogWrite(NOSE_PIN, 0);
      break;

    case ANALOG_READ: {
      // read both potentiometers then compute brightness and blink rate;
      // raw values are local — no global intermediaries needed
      int eye_raw, nose_raw;
      readInputs(eye_raw, nose_raw);
      computeParams(eye_raw, nose_raw);
      entry_flag = true;   // signal LED_OUTPUT to initialise timing on first entry
      state = LED_OUTPUT;
      break;
    }

    case LED_OUTPUT: {
      // initialise timing variables once on entry (entry_flag replaces fragile millis() sentinel)
      if (entry_flag) {
        start_time = millis();
        last_blink = start_time;
        nose_on    = true;
        analogWrite(NOSE_PIN, PWM_MAX);  // nose LED on immediately
        entry_flag = false;
      }

      unsigned long now = millis();

      // eye LEDs on for EYE_TIME ms
      if (now - start_time < EYE_TIME) {
        analogWrite(EYE_PIN, eye_brightness);
      } else {
        analogWrite(EYE_PIN, 0);
      }

      // nose LED blinks for NOSE_TIME ms, then cycle ends
      if (now - start_time < NOSE_TIME) {
        if (now - last_blink >= blink_half) {
          nose_on = !nose_on;
          analogWrite(NOSE_PIN, nose_on ? PWM_MAX : 0);
          last_blink = now;
        }
      } else {
        analogWrite(NOSE_PIN, 0);
        state = IDLE;
      }
      break;
    }
  }
}

// ISR: transitions IDLE → ANALOG_READ on button press.
// state is stored as a single byte on AVR, so the read here is atomic.
void buttonISR() {
  if (state == IDLE) {
    state = ANALOG_READ;
  }
}

// Read both potentiometers into caller-provided references.
void readInputs(int &eye_raw, int &nose_raw) {
  eye_raw  = analogRead(POT_EYE);
  nose_raw = analogRead(POT_NOSE);
}

// Map raw ADC readings to PWM brightness and nose blink half-period.
// Nose mapping is inverted: higher pot value → shorter half-period → faster blink.
void computeParams(int eye_raw, int nose_raw) {
  eye_brightness = map(eye_raw,  0, ADC_MAX, 0,         PWM_MAX);
  blink_half     = map(nose_raw, 0, ADC_MAX, BLINK_MAX, BLINK_MIN);
}

// Toggle the onboard heartbeat LED every HB_HALF ms (1 Hz).
void updateHeartbeat() {
  unsigned long now = millis();
  if (now - last_hb >= HB_HALF) {
    digitalWrite(HB_PIN, !digitalRead(HB_PIN));
    last_hb = now;
  }
}