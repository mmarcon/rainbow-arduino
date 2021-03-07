struct State {
  bool on;
  int r;
  int g;
  int b;
  int x;
  int y;
} state;

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int red_pin = 6; //analog pin connected to LED red
const int green_pin = 5; //analog pin connected to LED green
const int blue_pin = 3; //analog pin connected to LED blue

char buff[50];

void setup() {
  state.on = false;
  state.r = 127;
  state.g = 127;
  state.b = 127;

  // RGB LED
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  // Joystick switch
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  // Serial for printing
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(SW_pin) == 0) {
    state.on = !state.on;
    delay(500);
  }
  if (state.on) {
    if (analogRead(Y_pin) > 520) {
      state.g = state.g < 255 ? state.g + 1 : state.g;
    } else if (analogRead(Y_pin) < 480) {
      state.g = state.g > 0 ? state.g - 1 : state.g;
    }

    if (analogRead(X_pin) > 520) {
      state.b = state.b < 255 ? state.b + 1 : state.b;
      state.r = state.r > 0 ? state.r - 1 : state.r;
    } else if (analogRead(X_pin) < 480) {
      state.b = state.b > 0 ? state.b - 1 : state.b;
      state.r = state.r < 255 ? state.r + 1 : state.r;
    }
    set_lights(state);
  } else {
    switch_lights_off();
    state.r = 127;
    state.g = 127;
    state.b = 127;
  }
  
  sprintf(buff, "%d|%d,%d,%d", state.on, state.r, state.g, state.b); // Protocol: on|r,g,b
  Serial.println(buff);
}

void set_lights(State state) {
  analogWrite(red_pin, state.r);
  analogWrite(green_pin, state.g);
  analogWrite(blue_pin, state.b);
}

void switch_lights_off() {
  digitalWrite(red_pin, LOW);
  digitalWrite(green_pin, LOW);
  digitalWrite(blue_pin, LOW);
}
