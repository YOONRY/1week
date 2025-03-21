#include <Arduino.h>
#include <TaskScheduler.h>
// LED 핀 정의
#define RED_LED 3
#define YELLOW_LED 9
#define GREEN_LED 5
// 버튼 핀 정의
#define BTN_EMERGENCY 6
#define BTN_BLINKING 7
#define BTN_ONOFF 8
// 가변저항 핀 정의
#define POTENTIOMETER_PIN A0
// LED 밝기 및 시스템 상태 변수
int ledBrightness = 255; // LED 기본 밝기
bool emergencyMode = false; // 비상 모드 활성화 여부
bool blinkingMode = false; // 블링크 모드 활성화 여부
bool systemOn = true; // 시스템 작동 여부
// 버튼 디바운싱 관련 변수
unsigned long lastDebounceTimeEmergency = 0;
unsigned long lastDebounceTimeBlinking = 0;
unsigned long lastDebounceTimeOnOff = 0;
const unsigned long debounceDelay = 50;
// 신호등 상태 관리 변수
int trafficState = 0;
unsigned long lastStateChangeTime = 0;
unsigned long trafficDelays[] = {2000, 500, 2000, 200, 130, 200, 130, 200, 130, 500};

Scheduler runner;
// 함수 선언
void blinkLEDs();
void toggleLEDs();
void emergencyModeOn();
void sendTrafficLightState();
void checkButtons();
// TaskScheduler 객체 생성
Task taskBlinking(500, TASK_FOREVER, &blinkLEDs, &runner, false);
Task taskToggleLEDs(100, TASK_FOREVER, &toggleLEDs, &runner, true);
Task taskEmergency(100, TASK_FOREVER, &emergencyModeOn, &runner, false);
Task taskSendTrafficState(500, TASK_FOREVER, &sendTrafficLightState, &runner, true);

#define RED_LED 3
#define YELLOW_LED 9
#define GREEN_LED 5
#define POTENTIOMETER_PIN A0

int previousBrightness = -1;

// LED 밝기 업데이트 함수
void updateLEDBrightness() {
    int sensorValue = analogRead(POTENTIOMETER_PIN);
    ledBrightness = map(sensorValue, 0, 1023, 0, 255);
}

String lastSentState = "";

// 신호등 상태를 시리얼로 전송하는 함수
void sendTrafficLightState() {
    String state;

    if (!systemOn) state = "OFF";
    else if (emergencyMode) state = "EMERGENCY"; // 🚨 이머전시 모드일 때 상태 전송
    else if (blinkingMode) state = "BLINKING";   // 🚨 블링크 모드일 때 상태 전송
    else {
        if (trafficState == 0) state = "RED";
        else if (trafficState == 1 || trafficState == 9) state = "YELLOW";
        else if (trafficState == 2) state = "GREEN";
        else if (trafficState >= 3 && trafficState <= 8) state = "GREEN_BLINK";
    }

    if (state != lastSentState) {
        Serial.print("<" + state + ">");
        lastSentState = state;
    }

    if (ledBrightness != previousBrightness) {
        Serial.print("[BRIGHTNESS:" + String(ledBrightness) + "]");
        previousBrightness = ledBrightness;
    }
}
// 신호등 LED 제어 함수
void toggleLEDs() {
    unsigned long now = millis();
    checkButtons();
    updateLEDBrightness();

    if (now - lastStateChangeTime >= trafficDelays[trafficState]) {
        lastStateChangeTime = now;
        trafficState = (trafficState + 1) % 10;

        analogWrite(RED_LED, (trafficState == 0) ? ledBrightness : 0);
        analogWrite(YELLOW_LED, (trafficState == 1 || trafficState == 9) ? ledBrightness : 0);
        analogWrite(GREEN_LED, (trafficState == 2 || trafficState == 3 || trafficState == 5 || trafficState == 7) ? ledBrightness : 0);
    }
}
// 비상 모드 실행 함수
void emergencyModeOn() {
    checkButtons();
    updateLEDBrightness();
    analogWrite(RED_LED, ledBrightness);
    analogWrite(YELLOW_LED, 0);
    analogWrite(GREEN_LED, 0);
}
// 블링크 모드 실행 함수
void blinkLEDs() {
    static unsigned long lastBlinkTime = 0;
    unsigned long now = millis();
    
    // 블링크 모드일 때만 실행
    if (!blinkingMode) return;  

    if (now - lastBlinkTime > 500) {
        static bool state = false;
        state = !state;  // LED 상태 토글

        int brightness = state ? ledBrightness : 0;
        analogWrite(RED_LED, brightness);
        analogWrite(YELLOW_LED, brightness);
        analogWrite(GREEN_LED, brightness);

        lastBlinkTime = now;
    }

    // 🔥 블링크 중에도 웹에 상태 전송
    sendTrafficLightState();
}

// 버튼 입력 감지 및 처리 함수
void checkButtons() {
    unsigned long currentMillis = millis();

   if (digitalRead(BTN_EMERGENCY) == LOW && (currentMillis - lastDebounceTimeEmergency > debounceDelay)) {
    emergencyMode = !emergencyMode;

    if (emergencyMode) {
        taskEmergency.enable();
        taskBlinking.disable();
        taskToggleLEDs.disable();
        taskSendTrafficState.enable();  // ⭐️ Emergency 상태 전송

        Serial.println("<EMERGENCY MODE ENABLED>");
    } else {
        taskEmergency.disable();
        taskToggleLEDs.enable();
        taskSendTrafficState.enable();

        Serial.println("<EMERGENCY MODE DISABLED>");
    }
    lastDebounceTimeEmergency = currentMillis;
        while (digitalRead(BTN_EMERGENCY) == LOW);
    }

     if (digitalRead(BTN_BLINKING) == LOW && (currentMillis - lastDebounceTimeBlinking > debounceDelay)) {
        blinkingMode = !blinkingMode;
        if (blinkingMode) {
            taskBlinking.enable();
            taskToggleLEDs.disable(); // 🚨 신호등 작동 중지
            Serial.println("<BLINKING MODE ENABLED>");
        } else {
            taskBlinking.disable();
            taskToggleLEDs.enable(); // 🚦 신호등 다시 작동
            Serial.println("<BLINKING MODE DISABLED>");
        }
        lastDebounceTimeBlinking = currentMillis;
        while (digitalRead(BTN_BLINKING) == LOW);
    }

    if (digitalRead(BTN_ONOFF) == LOW && (currentMillis - lastDebounceTimeOnOff > debounceDelay)) {
        systemOn = !systemOn;
        if (systemOn) {
            taskToggleLEDs.enable();
            taskSendTrafficState.enable();
        } else {
            taskToggleLEDs.disable();
            taskBlinking.disable();
            taskEmergency.disable();
            taskSendTrafficState.disable();
            digitalWrite(RED_LED, LOW);
            digitalWrite(YELLOW_LED, LOW);
            digitalWrite(GREEN_LED, LOW);
            Serial.print("<OFF>");
        }
        lastDebounceTimeOnOff = currentMillis;
        while (digitalRead(BTN_ONOFF) == LOW);
    }
}

// 아두이노 초기 설정 함수
void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BTN_EMERGENCY, INPUT_PULLUP);
    pinMode(BTN_BLINKING, INPUT_PULLUP);
    pinMode(BTN_ONOFF, INPUT_PULLUP);

    Serial.begin(115200);

    runner.addTask(taskToggleLEDs);
    runner.addTask(taskSendTrafficState);
    taskToggleLEDs.enable();
    taskSendTrafficState.enable();
}

// 메인 루프 함수
void loop() {
    checkButtons();
    if (systemOn) {
        runner.execute();
    }

    //시리얼 수신 처리 부분 
    if (Serial.available() > 0) {
        String received = Serial.readStringUntil('>');
        int redDuration, yellowDuration, greenDuration;

        if (!blinkingMode && sscanf(received.c_str(), "<SET,R:%d,Y:%d,G:%d", &redDuration, &yellowDuration, &greenDuration) == 3) {
            trafficDelays[0] = redDuration;
            trafficDelays[1] = yellowDuration;
            trafficDelays[2] = greenDuration;

            Serial.println("Timings Updated");
        } else if (blinkingMode) {
            Serial.println("Ignored: Blinking Mode Active");
        }
    }
}