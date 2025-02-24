#include <NimBLEDevice.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int buttonPin = 18; // Define button pin (replace with your actual pin)

unsigned long lastButtonPressTime = 0; // Declare globally for loop access
bool buttonState = false;             // Declare globally for loop access

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize NimBLE BLE
  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create characteristic with read/write properties
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);

  // Set initial value for the characteristic
  pCharacteristic->setValue("Hello World says Neil");

  // Start the service
  pService->start();

  // Configure BLE Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMaxPreferred(0x12);

  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // Set button pin as input
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();
  static unsigned long lastDebounceTime = 0; // Use a static variable
  static bool lastButtonState = false;      // Use a static variable

  buttonState = digitalRead(buttonPin);

  // Debounce logic with edge detection
  if (buttonState != lastButtonState && (currentMillis - lastDebounceTime) > 300) {
    lastDebounceTime = currentMillis;
    if (!buttonState) { // Button pressed (low due to pull-up)
      Serial.println("Button pressed!");
    }
  }

  lastButtonState = buttonState; // bounce chek
}
