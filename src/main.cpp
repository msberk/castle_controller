// Include libraries necessary to run.
#include <Arduino.h>
#include <Servo.h>

// Define error cases.
enum class Error {
  NO_ERROR,
  NO_NUMBER,
  OUT_OF_RANGE
};

// Forward declare functions.
Error convert_input_to_number(String string_in, int& pct_throttle_out);
inline int percent_to_microseconds(const int percent);

// Define ESC "servo" globally so it can be used in both setup and loop.
Servo esc;
const uint8_t ESC_PIN = 9;

// Allocate a buffer
String buf;

// put your setup code here, to run once:
void setup() {
  // Open up serial with computer
  Serial.begin(SERIAL_BAUD_RATE);

  // Start ESC control.
  esc.attach(ESC_PIN);

  // Initialize to zero throttle
  esc.writeMicroseconds(percent_to_microseconds(0));
  
  // Allocate space to the string buffer.
  buf.reserve(20);
}


// put your main code here, to run repeatedly:
void loop() {
  // Read serial input if available.
  if (Serial.available() > 0) {
    // Read a line of input into buf (stop at newline);
    buf = Serial.readStringUntil('\n');

    int pct_throttle;
    Error err = convert_input_to_number(buf, pct_throttle);

    // If no error detected during string conversion then write out to the servo.
    if (err == Error::NO_ERROR) {
      esc.writeMicroseconds(percent_to_microseconds(pct_throttle));
    }
  }
}

// Serial input read/checking function
Error convert_input_to_number(const String string_in, int& pct_throttle_out)
{
  // Save initial throttle setting in case an out-of-bounds input given.
  const auto prev_pct_throttle = pct_throttle_out;

  // Read the throttle setting from the string. If no number detected, returns 0.
  pct_throttle_out = string_in.toInt();
  
  // If it's set to zero AND the string doesn't start with 0, then non-number sent.
  if (pct_throttle_out == 0 && !string_in.startsWith("0")) {
    Serial.println("No number read! Killing throttle.");
    pct_throttle_out = 0;
    return Error::NO_NUMBER;
  } 
  // If below 0% or over 100%, invalid setting.
  else if (pct_throttle_out < 0 || pct_throttle_out > 100) {
    Serial.println("Please enter a number between 0 and 100.");
    pct_throttle_out = prev_pct_throttle; // Restore original input.
    return Error::OUT_OF_RANGE;
  } 
  // Otherwise fine.
  else {
    Serial.print("Set throttle to ");
    Serial.print(pct_throttle_out);
    Serial.println("%.");
    return Error::NO_ERROR;
  }
}

// Function to convert percent throttle commands to PWM commands
inline int percent_to_microseconds(const int percent) {
  return map(percent, 0, 100, 1000, 2000);
}
