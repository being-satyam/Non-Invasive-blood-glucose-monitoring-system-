#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


const char *ssid = "";
const char *password = "";

// Twilio API Credentials
const char *twilioAccountSID = " ";
const char *twilioAuthToken = " ";
const char *twilioPhoneNumber = " ";
const char *recipientPhoneNumber = " ";

// Pin Definitions
const int glucosePin = 34;   // Analog pin for glucose sensor
const int monitorPin = 4;    // Digital pin 4 for finger detection

// Regression Coefficients (m and c) from Calibration(average of the four slopes and intercepts)
const float slope = 62.7315; 
const float intercept = -117.5;  


volatile bool fingerDetected = false;

// Task Handles
TaskHandle_t MonitorTaskHandle = NULL;


void TaskReadGlucose(void *parameter);
void TaskMonitorPin(void *parameter);
void IRAM_ATTR onPinChange();  // Interrupt Service Routine
void sendSMS(float glucoseValue);

void setup() {
  Serial.begin(115200);

  
  WiFi.begin(ssid, password);
  pinMode(monitorPin, INPUT);   // Set D4 as input for finger detection

  // Attach Interrupt to Pin 4
  attachInterrupt(digitalPinToInterrupt(monitorPin), onPinChange, CHANGE);

  // Create Tasks
  xTaskCreate(TaskReadGlucose, "Read Glucose", 4096, NULL, 1, NULL);
  xTaskCreate(TaskMonitorPin, "Monitor Pin", 2048, NULL, 2, &MonitorTaskHandle);
}

void loop() {
  // Empty loop - Tasks run independently
}

// Interrupt Service Routine for Pin Change
void IRAM_ATTR onPinChange() {
  fingerDetected = digitalRead(monitorPin);  // Update finger state
  if (MonitorTaskHandle != NULL) {
    xTaskResumeFromISR(MonitorTaskHandle);  // Resume Task 2
  }
}

// Task 1: Simulate Glucose Value and Apply Calibration
void TaskReadGlucose(void *parameter) {
  while (true) {
    if (fingerDetected) {
      int rawValue = analogRead(glucosePin);  
      float glucoseValue = rawValue*slope+intercept;

      Serial.println("Readings Taken");
      Serial.print("Glucose Value (mg/dL): ");
      Serial.println(glucoseValue);

      if (glucoseValue > 95) {
        sendSMS(glucoseValue);
      }

      Serial.println("Remove your finger");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 1 second
  }
}

// Task 2: Monitor Finger Detection
void TaskMonitorPin(void *parameter) {
  static bool previousState = false;  // Tracks the last finger detection state
  while (true) {
    if (fingerDetected && !previousState) {
      // Finger just detected
      Serial.println("Finger detected.");
      previousState = true;  // Update state
    } else if (!fingerDetected && previousState) {
      // Finger just removed
      Serial.println("Finger not detected.");
      previousState = false;  // Update state
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Periodic check every 100ms
  }
}

// Function to Send SMS via Twilio API
void sendSMS(float glucoseValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String message = "Alert! Glucose level is high: " + String(glucoseValue) + " mg/dL";

    // Construct Twilio API URL
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + String(twilioAccountSID) + "/Messages.json";

    // Construct HTTP POST body
    String body = "To=" + String(recipientPhoneNumber) +
                  "&From=" + String(twilioPhoneNumber) +
                  "&Body=" + message;

    // Set HTTP headers
    http.begin(url);
    http.setAuthorization(twilioAccountSID, twilioAuthToken);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send HTTP POST request
    int httpResponseCode = http.POST(body);
    //error handling
    if (httpResponseCode > 0) {
     // Serial.print("SMS sent successfully. Response code: ");
      //Serial.println(httpResponseCode);
    } else {
     // Serial.print("Failed to send SMS. Error code: ");
     // Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected. Unable to send SMS.");
  }
}