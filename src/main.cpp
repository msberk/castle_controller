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
Error convert_input_to_number(const char* string_in, int& pct_throttle_out);
int percent_to_microseconds(const int percent);

// Define ESC "servo" globally so it can be used in both setup and loop.
Servo esc;
const uint8_t ESC_PIN = 9;

// Define a global string buffer length.
const uint8_t BUF_LEN = 200;

// put your setup code here, to run once:
void setup() {
  // Open up serial with computer
  Serial.begin(SERIAL_BAUD_RATE);
  // Start ESC control.
  esc.attach(ESC_PIN);
  // Initialize to zero throttle
  esc.writeMicroseconds(percent_to_microseconds(0));
}


// put your main code here, to run repeatedly:
void loop() {
  // Initialize buffer for read.
  char buf[BUF_LEN];
  // Read serial input if available.
  if (Serial.available() > 0) {
    // Read a line of input into buf (stop at newline);
    Serial.readBytesUntil('\n', buf, BUF_LEN);

    int pct_throttle;
    Error err = convert_input_to_number(buf, pct_throttle);

    // If no error detected during string conversion then write out to the servo.
    if (err == Error::NO_ERROR) {
      esc.writeMicroseconds(percent_to_microseconds(pct_throttle));
    }
  }
}

// Serial input read/checking function
Error convert_input_to_number(const char* string_in, int& pct_throttle_out) {
  char* end_ptr;
  
  // Read the throttle setting from the string.
  pct_throttle_out = strtol(string_in, &end_ptr, 10);
  
  // if end_ptr is at the start of the string then no number was read.
  if (end_ptr == string_in) {
    Serial.println("No number read! Killing throttle.");
    pct_throttle_out = 0;
    return Error::NO_NUMBER;
  } 
  // If below 0% or over 100%, invalid setting.
  else if (pct_throttle_out < 0 || pct_throttle_out > 100) {
    Serial.println("Please enter a number between 0 and 100");
    return Error::OUT_OF_RANGE;
  } 
  // Otherwise fine.
  else {
    return Error::NO_ERROR;
  }
}

// Function to convert percent throttle commands to PWM commands
int percent_to_microseconds(const int percent) {
  return map(percent, 0, 100, 1000, 2000);
}