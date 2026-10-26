#include "USB.h"
#include "USBHIDKeyboard.h"

// Instantiate the HID keyboard object
USBHIDKeyboard Keyboard;

// Most ESP32-S3 development boards map the built-in 'BOOT' button to GPIO 0
const int BUTTON_PIN = 0; 
bool lastButtonState = HIGH;

void setup() {
  // Set up the built-in button with an internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize the USB peripheral and the Keyboard driver
  // It is important to initialize Keyboard before starting USB
  Keyboard.begin();
  USB.begin();
  
  delay(1000); // Give the host computer a second to recognize the device
}

void triggerMacro() {
  // Example Macro: Open Windows Run dialog (GUI/Windows Key + R)
  Keyboard.press(KEY_LEFT_GUI); 
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll(); // Always release keys to prevent stuck inputs
  
  delay(500); // Wait for the Run window to appear
  
  // Type 'notepad' and hit Enter
  Keyboard.print("notepad");
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  
  delay(1000); // Wait for Notepad to open
  
  // Type a simulated message across the USB bus
  Keyboard.println("Hello World from ESP32-S3!");
  Keyboard.println("This text was sent via native USB HID emulation.");
}

void loop() {
  // Read the current state of the button (LOW = pressed)
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // Detect state transition (Edge detection for a single click)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    delay(50); // Simple software debouncing
    if (digitalRead(BUTTON_PIN) == LOW) {
      triggerMacro();
    }
  }
  
  lastButtonState = currentButtonState;
  delay(10); // Small pause to lower CPU utilization
}
