/***************************************************************************************************
 * This code is designed to control two Thermoelectric Generator (TEG) modules (referred to here as 
 * TEG_L and TEG_R). Each TEG can be driven in two directions:
 *  - "forward" for heating (current flows in one direction)
 *  - "backward" for cooling (current flows in the reverse direction)
 *
 * The code uses PWM pins to control the magnitude of the current (and thus the level of heating or 
 * cooling) by adjusting the duty cycle from 0 (off) to 255 (full power).
 *
 * How it works:
 * 1. A TEG object is created by specifying:
 *     - an enable pin (for PWM output),
 *     - two digital pins (for direction control).
 * 2. In the main loop, commands are read from the Serial interface:
 *     - 'a', 'd' to control the left TEG (TEG_L) for heating or cooling,
 *     - 'w', 'x' to control the right TEG (TEG_R) for heating or cooling,
 *     - 's' to stop both TEGs.
 * 3. Each TEG can vary power by adjusting the PWM value (0 ~ 255), thus controlling the intensity 
 *    of heating or cooling.
 * 4. The user can enter commands via the Serial Monitor to dynamically set heating/cooling states 
 *    and power levels.
 *
 * This approach offers straightforward, bidirectional control over TEG modules for applications
 * such as small-scale heating/cooling experiments or prototypes involving thermoelectric modules.
 ***************************************************************************************************/

// TEG Motor Drive Control Class
class TEG {
public:
    int enable_pin;  // PWM enable pin for controlling output power
    int pin1;        // Input Pin 1 to set direction (HIGH/LOW)
    int pin2;        // Input Pin 2 to set direction (LOW/HIGH)

    /***********************************************************************************************
     * Constructor: Initializes the TEG with the specified pins.
     * @param enablePin: The PWM pin used to control the power level (0 ~ 255).
     * @param inputPin1: The first digital pin to determine current direction.
     * @param inputPin2: The second digital pin to determine current direction.
     ***********************************************************************************************/
    TEG(int enablePin, int inputPin1, int inputPin2) {
        enable_pin = enablePin;
        pin1 = inputPin1;
        pin2 = inputPin2;
    }

    /***********************************************************************************************
     * forward(): Drives the TEG in the "forward" direction to produce heating.
     * @param pwm: The PWM duty cycle (0 for off, up to 255 for maximum power).
     ***********************************************************************************************/
    void forward(int pwm) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        analogWrite(enable_pin, pwm);
    }

    /***********************************************************************************************
     * backward(): Drives the TEG in the "backward" direction to produce cooling.
     * @param pwm: The PWM duty cycle (0 for off, up to 255 for maximum power).
     ***********************************************************************************************/
    void backward(int pwm) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        analogWrite(enable_pin, pwm);
    }

    /***********************************************************************************************
     * stop(): Stops any current flow through the TEG by setting both direction pins LOW 
     *         and the PWM output to 0.
     ***********************************************************************************************/
    void stop() {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        analogWrite(enable_pin, 0);
    }
};

// Pin definitions for both TEG modules
int ena = 2;  // TEG_L enable pin
int in1 = 3;  // TEG_L input pin 1
int in2 = 4;  // TEG_L input pin 2
int in3 = 5;  // TEG_R input pin 1
int in4 = 6;  // TEG_R input pin 2
int enb = 7;  // TEG_R enable pin

// Create two TEG objects to control the left and right TEGs respectively
TEG TEG_L(ena, in1, in2);
TEG TEG_R(enb, in3, in4);

void setup() {
    // Configure all pins as outputs for TEG control
    pinMode(ena, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enb, OUTPUT);

    // Initialize Serial communication at 9600 baud
    Serial.begin(9600);

    // Wait until there is data available in the serial buffer
    // (prevents unintended behavior before sending commands)
    while (!Serial.available()) {
        // Busy-wait; once data is available, we'll proceed
    }

    // Print a message to indicate that commands can now be sent
    Serial.println("Begin command....");
}

void loop() {
    // Check if there is a new command available on the serial port
    if (Serial.available() > 0) {
        // Read a single character as the command
        char command = Serial.read();

        // Interpret the received command and act accordingly
        if (command == 'a') {
            // 'a' indicates left TEG heating
            // TO DO: Adjust the PWM (0~255) as needed for desired heating power
            TEG_L.forward(255);  
            Serial.println("a");
        } else if (command == 's') {
            // 's' stops both TEGs
            TEG_L.stop();
            TEG_R.stop();
            Serial.println("s");
        } else if (command == 'd') {
            // 'd' indicates left TEG cooling
            // TO DO: Adjust the PWM (0~255) as needed for desired cooling power
            TEG_L.backward(255);
            Serial.println("d");
        } else if (command == 'w') {
            // 'w' indicates right TEG heating
            // TO DO: Adjust the PWM (0~255) as needed for desired heating power
            TEG_R.forward(135);
            Serial.println("w");
        } else if (command == 'x') {
            // 'x' indicates right TEG cooling
            // TO DO: Adjust the PWM (0~255) as needed for desired cooling power
            TEG_R.backward(135);
            Serial.println("x");
        }
    }

    // Add a small delay to prevent overwhelming the serial buffer with reads
    delay(100);
}
