/***********************************************************************************************
 * This code periodically samples the analog voltage from pin A0 at a fixed interval (50 ms),
 * applies an exponential smoothing filter (also known as an exponential moving average) to
 * the raw data, and prints out a timestamp along with the filtered voltage to the Serial Monitor.
 *
 * How it works:
 * 1. 'millis()' is used to track elapsed time, ensuring consistent sampling every 50 ms.
 * 2. Each new reading from 'analogRead(A0)' is combined with the previous smoothed value using:
 *         smoothedValue = alpha * last_val + (1 - alpha) * currentReading
 *    This formula helps reduce high-frequency noise in the signal.
 * 3. The result is converted to a voltage by multiplying with (5.0 / 1024.0) since Arduino ADC
 *    resolution is 10-bit (0–1023) over a 5V reference.
 * 4. 'last_val' stores the most recent smoothed value for the next loop iteration.
 * 5. The final filtered voltage and timestamp are sent via Serial.print() for real-time monitoring.
 *
 * This approach helps stabilize sensor readings by smoothing out random fluctuations, which is
 * particularly useful in scenarios involving noisy input signals. The combination of timed
 * sampling and exponential smoothing provides both consistency and responsiveness.
 ***********************************************************************************************/

const int analogPin = A0;   // Analog input pin where the sensor/output signal is connected
float analogValue = 0;      // Variable to store the current (or filtered) analog reading
unsigned long previousMillis = 0;  // Timestamp of the last sensor read in milliseconds
const unsigned long interval = 50; // Fixed sampling interval in milliseconds (50 ms)
float alpha = 0.9;          // Weight factor for exponential smoothing (high alpha => stronger smoothing)
float last_val = -1.0;      // Stores the previous smoothed value (initialized to negative for first-check logic)

void setup() {
  // Initialize Serial communication at 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // Record the current time in milliseconds since the Arduino started running
  unsigned long currentMillis = millis();
  
  // Check if the desired sampling interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    // Update the timestamp for the last read
    previousMillis = currentMillis;
    
    // Read the raw ADC value from pin A0 (range: 0 to 1023)
    analogValue = (float)analogRead(analogPin);

    // Apply exponential smoothing only if a valid previous value exists
    if (last_val > 0) {
      analogValue = alpha * last_val + (1 - alpha) * analogValue;
    }

    // Update the last_val for the next iteration
    last_val = analogValue;

    // Convert the ADC reading to a voltage (assuming a 5V reference)
    float voltage = analogValue * 5.00 / 1024.0;

    // Print out the current time (ms) and the filtered voltage
    Serial.print(currentMillis);
    Serial.print(",");
    Serial.println(voltage);
  }
}
