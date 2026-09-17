/*
 * Laser Security System in Crop Protection from Wild Animals
 * Team: ANishanth, Dashami C, H Mounesh Reddy, Prerana Hegde
 * Course: BCSK307 (VTU ECE 3rd Sem)
 */

const int LDR_PIN = A0;           // LDR sensor pin
const int BUZZER_PIN = 8;         // Piezo speaker pin
const int RED_LED = 9;            // Intrusion alert LED
const int GREEN_LED = 10;         // Safe state LED
const int NE555_TRIGGER_PIN = 7;  // Signal pin for 555 ultrasonic generator

const int LIGHT_THRESHOLD = 400;  // Threshold level for laser beam
const int INTRUSION_DELAY = 100;  // Minimum break time (ms) to trigger alert

bool intrusionDetected = false;
unsigned long beamBreakStartTime = 0;
unsigned long intrusionCount = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("=== LASER SECURITY SYSTEM INITIALIZING ==="));

  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(NE555_TRIGGER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(NE555_TRIGGER_PIN, LOW);

  Serial.println(F("[SYSTEM READY] Monitoring laser barrier."));
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);

  if (ldrValue < LIGHT_THRESHOLD) {
    // Laser beam interrupted
    if (beamBreakStartTime == 0) {
      beamBreakStartTime = millis();
    }

    if ((millis() - beamBreakStartTime) >= INTRUSION_DELAY) {
      if (!intrusionDetected) {
        intrusionDetected = true;
        intrusionCount++;
        Serial.print(F("[ALERT] INTRUSION DETECTED! Count: "));
        Serial.println(intrusionCount);
      }
      triggerAlarm();
    }
  } else {
    // Laser beam intact
    beamBreakStartTime = 0;
    if (intrusionDetected) {
      intrusionDetected = false;
      Serial.println(F("[STATUS] Laser beam restored. Safe."));
      stopAlarm();
    }
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(NE555_TRIGGER_PIN, LOW);
    noTone(BUZZER_PIN);
  }
  delay(50);
}

void triggerAlarm() {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(NE555_TRIGGER_PIN, HIGH);

  for (int hz = 1000; hz <= 3500; hz += 100) {
    tone(BUZZER_PIN, hz);
    delay(5);
  }
}

void stopAlarm() {
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(NE555_TRIGGER_PIN, LOW);
}