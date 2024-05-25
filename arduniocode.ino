void setup() {
  // put your setup code here, to run once:

}

void loop() {


  // put your main code here, to run repeatedly:

}

bool checkPassword() {
  if (input_password == password) {
    attempt_count = 0; // reset attempt count after successful login
    return true;
  } else {
    return false;
    attempt_count++;
    if (attempt_count >= max_attempts) {
      lockout_time = millis() + lockout_duration;
      attempt_count = 0; // reset attempt count after lockout
    }
  }
}
