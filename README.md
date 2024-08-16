# ATTENDANCE MANAGEMENT SYSTEM 
## Objectives
Interface a 16x2 LCD Display with an ATmega32 Microcontroller: Display characters and messages on the LCD based on user input from a keypad.
Interface a 4x4 Keypad with an ATmega32 Microcontroller: Read user inputs from the keypad to interact with the system.
Use a Potentiometer to Adjust LCD Contrast: Implement a method to adjust the contrast of the LCD using a potentiometer.
Combine the LCD and Keypad Interfacing: Develop a system that uses both the LCD and keypad to display characters pressed on the keypad.


## Methodology
### Initial Setup:
- Configure the hardware components by connecting the LCD, keypad, and potentiometer to the ATmega32 microcontroller.
- Set up the development environment for programming the ATmega32, using tools such as AVR Studio or Atmel Studio.

### LCD Initialization and Control:
- Write code to initialize the LCD in 8-bit mode.
- Implement functions to send commands and data to the LCD.
- Create a function to display strings on the LCD.

### Keypad Initialization and Reading:
- Write code to initialize the 4x4 keypad.
- Implement a function to detect key presses and map them to their corresponding characters.

### Potentiometer for LCD Contrast:
- Connect the potentiometer to the LCD to control its contrast.
- Adjust the potentiometer value to set the desired contrast level.


## Diagram
### Here is a connection diagram created for the project:
- ATmega32 Microcontroller:
#### LCD Connections:
- RS (Register Select) -> PB0
- EN (Enable) -> PB2
- Data Pins (D4-D7) -> PD4-PD7
- VSS -> GND
- VDD -> +5V
- V0 (Contrast) -> Middle pin of Potentiometer
#### Keypad Connections:
- Rows (R1-R4) -> PA4-PA7
- Columns (C1-C4) -> PA0-PA3
####  Potentiometer:
- One end to GND
- Other end to +5V
- Wiper (middle pin) to V0 of LCD


## Methodology
The project begins with configuring the hardware components, including the ATmega32 microcontroller, a 16x2 LCD display, a 4x4 keypad, and a potentiometer. The LCD is initialized in 8-bit mode, with functions implemented to send commands and display data. The keypad is initialized to read user inputs, mapped to their respective characters. The potentiometer is connected to adjust the LCD contrast. Integration of these components involves writing embedded C code to handle LCD commands, read keypad inputs, and adjust contrast, followed by extensive testing and debugging to ensure functionality. The final step involves validating the system by displaying the characters pressed on the keypad onto the LCD, demonstrating successful integration and operation of all components.


## Conclusion 
This project successfully demonstrates the integration of an ATmega32 microcontroller with a 16x2 LCD display, a 4x4 keypad, and a potentiometer. The developed system efficiently reads user inputs from the keypad, adjusts the LCD contrast, and displays the input characters on the LCD. The project highlights the practical application of embedded systems, showcasing the seamless interaction between hardware components and software, and provides a solid foundation for more advanced microcontroller-based projects.




