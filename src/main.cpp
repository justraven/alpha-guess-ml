// #define ENABLE_DEBUG_MODE       0x01    // Comment for disable debug mode
#define ALPHA_SESSION_SELECT    "Z"     // Change the alphabet to select the data stream session

#include <Arduino.h>
#include <Arduino_LSM6DS3.h>

#define GET_US_TICK(x) (micros() - elapsed_time) > x

static uint16_t elapsed_time = 0;
static uint16_t global_tick = 0;

/*
* IMU_data[0] --> X Acceleration
* IMU_data[1] --> Y Acceleration
* IMU_data[2] --> Z Acceleration
* IMU_data[3] --> X Gyroscope
* IMU_data[4] --> Y Gyroscope
* IMU_data[5] --> Z Gyroscope
*/
static float IMU_avg[6] = {0};
static float IMU_data[6] = {0};
static const String csv_label = "Alpha, X_Acc, Y_Acc, Z_Acc, X_Gyro, Y_Gyro, Z_Gyro"; 

void setup() {
    Serial.begin(115200);
    while(!Serial);

    if(!IMU.begin()){
#if ENABLE_DEBUG_MODE
        Serial.println("[IMU] initialization failed");
#endif
        while(1);
    }

#ifdef ENABLE_DEBUG_MODE
    pinMode(LED_BUILTIN, OUTPUT);
#else
    Serial.println(csv_label);
#endif
}

void loop() {
#ifdef ENABLE_DEBUG_MODE
    if(GET_US_TICK(1000)) {
        if(global_tick >= 500){
            Serial.println("[DATA] Session 500ms");
            digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);
            global_tick = 0;
        }else if(global_tick >= 50){
            if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
                IMU.readAcceleration(IMU_data[0], IMU_data[1], IMU_data[2]);
                IMU.readGyroscope(IMU_data[3], IMU_data[4], IMU_data[5]);
            }

            Serial.println(
                String("[IMU] ") + 
                String("Acceleration [X] ") + String(IMU_data[0]) + String(" [Y] ") + String(IMU_data[1]) + String(" [Z] ") +String(IMU_data[2]) +
                String(" | Gyroscope [X] ") + String(IMU_data[3]) + String(" [Y] ") + String(IMU_data[4]) + String(" [Z] ") +String(IMU_data[5])
            );
        }

        global_tick++;
        elapsed_time = micros();
    }
#else
    if(GET_US_TICK(1000)) {
        if(global_tick >= 50){
            // for(uint8_t x=0;x<6;x++)
            //     IMU_data[x] = IMU_avg[x] / 10;
            // for(uint8_t x=0;x<6;x++)
            //     IMU_avg[x] = 0;

            Serial.println(
                String(ALPHA_SESSION_SELECT) + "," + String(IMU_data[0]) + "," + String(IMU_data[1]) + "," + String(IMU_data[2]) +
                "," + String(IMU_data[3]) + "," + String(IMU_data[4]) + "," + String(IMU_data[5])
            );

            global_tick = 0;
        }else if(global_tick >= 5){
            if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable()){
                IMU.readAcceleration(IMU_avg[0], IMU_avg[1], IMU_avg[2]);
                IMU.readGyroscope(IMU_avg[3], IMU_avg[4], IMU_avg[5]);
            }

            for(uint8_t x=0;x<6;x++)
                IMU_data[x] = IMU_avg[x];
        }

        global_tick++;
        elapsed_time = micros();
    }
#endif
}