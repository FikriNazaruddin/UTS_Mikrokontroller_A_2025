#define MOTOR_PIN1  27
#define MOTOR_PIN2  26
#define ENABLE_PIN  12

int speedValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  
  analogWrite(ENABLE_PIN, 0);
  Serial.println("ESP32 siap menerima perintah (ON/OFF/REV/SPEED)");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "ON") {
      digitalWrite(MOTOR_PIN1, HIGH);
      digitalWrite(MOTOR_PIN2, LOW);
      analogWrite(ENABLE_PIN, speedValue);
      Serial.println("Motor ON (maju)");
    } 
    else if (command == "OFF") {
      analogWrite(ENABLE_PIN, 0);
      Serial.println("Motor OFF");
    } 
    else if (command == "REV") {
      digitalWrite(MOTOR_PIN1, LOW);
      digitalWrite(MOTOR_PIN2, HIGH);
      analogWrite(ENABLE_PIN, speedValue);
      Serial.println("Motor ON (mundur)");
    } 
    else if (command.startsWith("SPEED")) {
      int val = command.substring(6).toInt();
      speedValue = constrain(val, 0, 255);
      analogWrite(ENABLE_PIN, speedValue);
      Serial.print("Kecepatan diubah ke ");
      Serial.println(speedValue);
    } 
    else {
      Serial.println("Perintah tidak dikenali");
    }
  }
}
