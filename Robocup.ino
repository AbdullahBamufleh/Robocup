#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

int motorA1 = 13;
int motorA2 = 12;
int motorB1 = 14;
int motorB2 = 27;
int Enable1 = 26;
int Enable2 = 25;


void setup() {
  Serial.begin(115200);
  BP32.setup(&onConnectedController, &onDisconnectedController); // Critical!
  BP32.forgetBluetoothKeys(); // Helps with reconnection issues

  pinMode(motorA1,OUTPUT);
  pinMode(motorA2,OUTPUT);
  pinMode(motorB1,OUTPUT);
  pinMode(motorB2,OUTPUT);
  pinMode(Enable1, OUTPUT);
  pinMode(Enable2, OUTPUT);
}

void loop() {
    analogWrite(Enable2,80);
    analogWrite(Enable1,80);
        BP32.update(); // Must be called regularly
    
    // Process connected controllers
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected() && ctl->hasData()) {
            processGamepad(ctl);
            // if (ctl->throttle() > 300) {
            //     Serial.println("throttle");
            //     forward();
            // }
            // if (ctl->axisY() < -400 ) {
            //     Serial.println("LY -");
                    // forward();
            // }
            // if (ctl->axisY() > 400) {
            //     Serial.println("LY +");
                  //   backward();
            // }
            // if (ctl->axisRX() < -400) {
            //     Serial.println("RX -");
                  // left();
            // }
            // if (ctl->axisRX() > 400) {
            //     Serial.println("RX +");
            // right();
            // }
    }
    }
    delay(20); // Prevent serial flooding
}

void stop()
{
  dw(motorA1,LOW);
  dw(motorA2,LOW);
  dw(motorB1,LOW);
  dw(motorB2,LOW);
}

void dw(int motor, boolean statement){
  digitalWrite(motor,statement);
}

void d(int latency)
{
  delay(latency);
}

void backward()
{
  dw(motorB1,HIGH);
  dw(motorB2,LOW);

  dw(motorA1,LOW);
  dw(motorA2,HIGH);
  d(100);
  stop();
}

void forward()
{
  dw(motorB1,LOW);
  dw(motorB2,HIGH);

  dw(motorA1,HIGH);
  dw(motorA2,LOW);
  d(100);
  stop();
}

void left()
{
  dw(motorB1,HIGH);
  dw(motorB2,LOW);

  dw(motorA1,HIGH);
  dw(motorA2,LOW);
  d(100);
  stop();
}

void right()
{
  dw(motorB1,LOW);
  dw(motorB2,HIGH);

  dw(motorA1,LOW);
  dw(motorA2,HIGH);
  d(100);
  stop();
}

// Required connection callbacks
void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            myControllers[i] = ctl;
            Serial.printf("Controller connected at index %d\n", i);
            break;
        }
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            myControllers[i] = nullptr;
            Serial.printf("Controller disconnected from index %d\n", i);
            break;
        }
    }
}

void processGamepad(ControllerPtr ctl) {
    // Only read axes and throttle
    Serial.printf("LX:%4d LY:%4d RX:%4d RY:%4d TH:%4d\n",
                 ctl->axisX(), ctl->axisY(), 
                 ctl->axisRX(), ctl->axisRY(), 
                 ctl->throttle());
}