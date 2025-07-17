#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// Global variables as mentioned in the problem statement
uint8_t G_DatalD = 0;
int32_t G_DataValue = 0;

// Data structure definition
typedef struct {
  uint8_t dataID;
  int32_t DataValue;
} Data_1;

// Task handles
TaskHandle_t TaskHandle_1 = NULL;
TaskHandle_t TaskHandle_2 = NULL;

// Queue handle
QueueHandle_t Queue1 = NULL;

// Task prototypes
void ExampleTask1(void *pV);
void ExampleTask2(void *pV);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial connection for printing

  // Create queue with size 5 and Data_1 element size
  Queue1 = xQueueCreate(5, sizeof(Data_1));
  if (Queue1 == NULL) {
    Serial.println("Queue creation failed!");
    while(1); // Halt if queue creation fails
  }

  // Create tasks
  xTaskCreate(
    ExampleTask1,    // Task function
    "ExampleTask1",  // Task name
    2048,           // Stack size (bytes)
    NULL,           // Parameter
    2,              // Priority (arbitrary choice)
    &TaskHandle_1   // Task handle
  );

  xTaskCreate(
    ExampleTask2,
    "ExampleTask2",
    2048,
    NULL,
    1,              // Lower priority than Task1
    &TaskHandle_2
  );

  // Delete the setup and loop tasks (not needed in FreeRTOS)
  vTaskDelete(NULL);
}

void loop() {
  // Not used - FreeRTOS scheduler takes over
}

void ExampleTask1(void *pV) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(500); // 500ms delay
  
  while (1) {
    // Create data structure to send
    Data_1 sendData;
    sendData.dataID = G_DatalD;
    sendData.DataValue = G_DataValue;

    // Send to queue
    if (xQueueSend(Queue1, &sendData, portMAX_DELAY) != pdPASS) {
      Serial.println("Failed to send to queue");
    }

    // Exact 500ms delay using vTaskDelayUntil
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void ExampleTask2(void *pV) {
  Data_1 receivedData;
  BaseType_t originalPriority = 1; // Store original priority
  bool priorityIncreased = false;  // Track if priority was increased

  while (1) {
    // Receive data from queue (block indefinitely until data arrives)
    if (xQueueReceive(Queue1, &receivedData, portMAX_DELAY) == pdPASS) {
      // Print received data as required
      Serial.print("Received data - ID: ");
      Serial.print(receivedData.dataID);
      Serial.print(", Value: ");
      Serial.println(receivedData.DataValue);

      // Process data according to conditions
      if (receivedData.dataID == 0) {
        Serial.println("Deleting ExampleTask2 (dataID == 0)");
        vTaskDelete(TaskHandle_2);
      }
      else if (receivedData.dataID == 1) {
        // Process DataValue member
        if (receivedData.DataValue == 0) {
          // Increase priority by 2
          vTaskPrioritySet(TaskHandle_2, originalPriority + 2);
          priorityIncreased = true;
          Serial.println("Increased ExampleTask2 priority by 2");
        }
        else if (receivedData.DataValue == 1 && priorityIncreased) {
          // Decrease priority if previously increased
          vTaskPrioritySet(TaskHandle_2, originalPriority);
          priorityIncreased = false;
          Serial.println("Decreased ExampleTask2 priority to original");
        }
        else if (receivedData.DataValue == 2) {
          Serial.println("Deleting ExampleTask2 (DataValue == 2)");
          vTaskDelete(TaskHandle_2);
        }
      }
    }
  }
}