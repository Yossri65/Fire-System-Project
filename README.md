# Fire-System-Project
This project consists of an application written in C language for a fire detection and alarm system. The system utilizes various components such as a microcontroller, LCD display, keypad, ADC module, and peripherals like LED, buzzer, and DC motor to detect and respond to fire incidents.
The main functionality of the system includes monitoring temperature and smoke levels using an ADC module. The temperature and smoke levels are converted to digital values and processed by the microcontroller. The system continuously checks the temperature and smoke levels to detect any abnormal conditions.
The user is presented with a menu on the LCD display, allowing them to perform different actions. They can either enter a new password to set up the system or enter an existing password to access the system. The passwords are stored in the EEPROM memory of the microcontroller.
If the system is armed with a valid password and the smoke level exceeds a threshold while the temperature is high, indicating a fire situation, the system activates an alarm. The alarm includes turning on a red LED, activating a DC motor, and sounding a buzzer. The system also sends a fire alert message via a UART communication interface.
To use the system, follow these steps:

Power on the system.
The LCD display will show a welcome message.
Choose an option from the menu using the keypad:
Press 1 to enter a new password.
Press 2 to enter an existing password.
Press '+' to restart the program in case of a fire alarm.
If you choose to enter a new password, follow the prompts on the LCD display and keypad to set up a new password. You can set a new password up to three times.
If you choose to enter an existing password, enter the password using the keypad. If the entered password matches the stored password, the system will grant access. Otherwise, an error message will be displayed.
Once access is granted, the system will continuously monitor the temperature and smoke levels.
If a fire situation is detected, the alarm will be activated, indicating the presence of a fire. The LCD display will show a fire alert message, the red LED will turn on, the DC motor will start rotating, and the buzzer will sound.
To restart the program after a fire alarm, press the '+' key.
Make sure to refer to the system's hardware documentation for proper connections and component configurations.

Note: This code is provided as a sample implementation and may require modifications to work with specific hardware configurations or additional functionalities.
