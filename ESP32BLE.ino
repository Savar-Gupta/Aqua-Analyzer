#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic; 
bool deviceConnected = false; 
int txValue = 0; 

// Define Service & TX UUID 
#define SERVICE_UUID "867879ce-8994-11ee-b9d1-0242ac120002"
#define CHARACTERISTIC_UUID_TX "86787cd0-8994-11ee-b9d1-0242ac120002"

// Sets deviceConnected to true when device connected
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};


void setup() {
  Serial.begin(9600);

  // Create BLE Device
  BLEDevice::init("MyESP32");
  
  // Create BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service [holds characterisitics]
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristic 
  pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX, 
        BLECharacteristic::PROPERTY_NOTIFY
        );


  // Start the Service 
  pService->start();

  // Start Advertising 
  pServer->getAdvertising()->start();
  Serial.println("Waiting for client connection...");
}

void loop() {
  if (deviceConnected) {
    txValue = random(-10,20);

    // Conversion of txValue
    char txString[8];
    dtostrf(txValue, 1, 2, txString);

    // Setting the Value to the Characteristic
    pCharacteristic->setValue(txString);

    // Notifying the Connected Client 
    Serial.println("Sent Value: " + String(txString)); 

    delay(500);
  }
}
