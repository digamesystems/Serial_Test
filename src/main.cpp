#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial mySerial2(2); // Testing on UART2 (pins 16, 17)

//********************************************************************************
void setup() {
  int baudRate = 2000000;

  Serial.begin(115200); // USB Serial port for debug msgs
  while (!Serial) {
    ; // wait for serial port to connect. 
  }

  mySerial2.begin(baudRate, SERIAL_8N1, 16, 17); // RX, TX -- 
                                                //  I put a loop back across these 
                                                //  pins to test the code
  while (!mySerial2) {
    ; // wait for serial port to connect. 
  } 

  Serial.println("Setup Complete. Baud Rate: " + String(baudRate));
}


//********************************************************************************
void loop() {

  delay(1000);

  String message = "hello."; 
  int messageLength = message.length() + 1 ; // +1 for null terminator
  Serial.println("message length: " + String(messageLength));

  // SEND
  int t0 = micros(); // start time

  mySerial2.print(message);
  mySerial2.flush();  // ******* Warning: This causes the program to wait unitl the TX buffer is empty.
                      // If you don't flush, the program will continue and the message will be sent 
                      // in the background. Making your send time look faster! We're doing this to be
                      // sure the data has been sent for timing measurements. ******
                      // 
                      // In real life, you might want not to flush but move on in the program flow and 
                      // and do other things while the device you are talking do does things and starts
                      // sending back data.  

  int t1 = micros(); // end time

  Serial.println("Time to send: " + String(t1 - t0) + " microseconds.");
  Serial.println("Time per character: " + String((t1 - t0) / messageLength) + " microseconds.");

  // RECEIVE
  t0 = micros(); // start time

  while (mySerial2.available()) {  // Data should be available since we just sent it through the loopback 
                                   // and the TX buffer got emptied. 
                                   // In real life you might want to spin until you get a response.
    char inChar = (char)mySerial2.read();
    // In real life, we'd do something with the incoming data here.
  }

  t1 = micros(); // end time

  Serial.println("Time to read: " + String(t1 - t0) + " microseconds.");
  Serial.println("Time per character: " + String((t1 - t0) / messageLength) + " microseconds.");
  Serial.print("\n\n");

}
