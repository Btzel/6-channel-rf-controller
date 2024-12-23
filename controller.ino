#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <RF24.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define CE_PIN 9
#define CSN_PIN 10

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "BTCOMM37";

const int leftJoyX = A1;
const int leftJoyY = A0;
const int leftJoyButton = 2;
const int rightJoyX = A3;
const int rightJoyY = A2;
const int rightJoyButton = 3;

const int menuItems = 2;
const int carSubMenuItems = 3;
int selectedItem = 0;
int subMenuSelectedItem = 0;
bool inMainMenu = true;
bool inSubMenu = false;
bool inCarMenu = false;



void setup() {
  Serial.begin(9600);
  pinMode(leftJoyButton, INPUT_PULLUP);
  pinMode(rightJoyButton, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 hatasi, baglantiyi kontrol edin"));
    for (;;);
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  randomSeed(analogRead(0)); // Rastgelelik için

  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}


void loop() {
  int leftY = analogRead(leftJoyY);
  int leftX = analogRead(leftJoyX);
  int rightY = analogRead(rightJoyY);
  int rightX = analogRead(rightJoyX);
  int leftButtonState = digitalRead(leftJoyButton);
  int rightButtonState = digitalRead(rightJoyButton);

  if (leftY < 400) {
    if (inMainMenu) {
      selectedItem++;
      if (selectedItem >= menuItems) selectedItem = 0;
    } else if (inSubMenu) {
      subMenuSelectedItem++;
      if (subMenuSelectedItem >= carSubMenuItems) subMenuSelectedItem = 0;
    }
    delay(200);
  } else if (leftY > 600) {
    if (inMainMenu) {
      selectedItem--;
      if (selectedItem < 0) selectedItem = menuItems - 1;
    } else if (inSubMenu) {
      subMenuSelectedItem--;
      if (subMenuSelectedItem < 0) subMenuSelectedItem = carSubMenuItems - 1;
    }
    delay(200);
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  if (inMainMenu) {
    display.println("Main Menu:");
    const char* menuNames[2] = {"Car", "Airplane"};
    for (int i = 0; i < menuItems; i++) {
      if (i == selectedItem) {
        display.print("> ");
      } else {
        display.print("  ");
      }
      display.println(menuNames[i]);
    }

    if (rightButtonState == LOW) {
      if (selectedItem == 0) {
        inMainMenu = false;
        inSubMenu = true;
        subMenuSelectedItem = 0;
      } else if (selectedItem == 1) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Airplane Menu:");
        display.println("Under Maintenance");
        display.display();
        delay(2000);
      }
      delay(200);
    }
  } else if (inSubMenu) {
    display.println("Car Menu:");
    const char* carMenuNames[3] = {"Free Control", "Line Follower", "Random Trip"};
    for (int i = 0; i < carSubMenuItems; i++) {
      if (i == subMenuSelectedItem) {
        display.print("> ");
      } else {
        display.print("  ");
      }
      display.println(carMenuNames[i]);
    }

    if (rightButtonState == LOW) {
      inCarMenu = true;
      inSubMenu = false;
      delay(200);
    }

    if (leftButtonState == LOW) {
      inMainMenu = true;
      inSubMenu = false;
      delay(200);
    }
  } else if (inCarMenu) {
    if (subMenuSelectedItem == 0) {
      freeControl();
    } else if (subMenuSelectedItem == 1) {
      lineFollower();
    } else if (subMenuSelectedItem == 2) {
      randomTrip();
    }
    delay(2000);
    inMainMenu = true;
    inCarMenu = false;
  }

  display.display();
}

void freeControl() {
  while (true) {
    int leftX = analogRead(leftJoyX);
    int leftY = analogRead(leftJoyY);
    int rightX = analogRead(rightJoyX);
    int rightY = analogRead(rightJoyY);
    int leftButtonState = digitalRead(leftJoyButton);

    // Create a data structure to send the joystick values
    struct {
      int lx;
      int ly;
      int rx;
      int ry;
    } joystickData;

    joystickData.lx = leftX;
    joystickData.ly = leftY;
    joystickData.rx = rightX;
    joystickData.ry = rightY;

    // Send the joystick values via NRF24L01
    radio.write(&joystickData, sizeof(joystickData));

    // Display joystick values on the screen
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Free Control:");
    display.print("Left X: "); display.println(leftX);
    display.print("Left Y: "); display.println(leftY);
    display.print("Right X: "); display.println(rightX);
    display.print("Right Y: "); display.println(rightY);
    display.display();

    if (leftButtonState == LOW) {
      break;  // Exit free control mode
    }
    delay(100);
  }
}


void lineFollower() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Car Option:");
  display.println("Line Follower");
  display.display();
}

void randomTrip() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Car Option:");
  display.println("Random Trip");
  display.display();
}
