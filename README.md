# Introduction To Robotics

This repository includes a collection of homeworks and projects developed throughout the 'Introduction To Robotics' course taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest, each encompassing requirements, implementation instructions, code, and video/image files.

## Homework #2 - RGB

The first technical assignment of the semester was to light up an RGB LED by controlling each color with an individual potentiometer. This was realised using various electrical components (potentiometers, RGB LED, resistors, wires, breadboard), as well as the Arduino UNO microcontroller board. Using the Arduino IDE (and the code uploaded in the Homework2_RGB folder), I read the values from the potentiometers and assigned them to their respective LED in order to add colors gradually to the RGB LED.

![RGB and potentiometers setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/989c5a7c-4c28-4f13-bde4-d1f94d468b7b)

Video demonstration: https://youtube.com/shorts/Ug5oerTwDiM?si=9ch4H0Yc5zlcSfkJ

## Homework #3 - Elevator Simulation

The purpose of this task was to simulate the movement of an elevator between three floors. This was realised using a breadboard, three red LEDs to illustrate the floor on which the elevator is at, three pushbuttons used to "call" the elevator to a certain floor, a blue LED to indicate the movement of the elevator between floors and a passive buzzer which generates a simple tone of 800Hz when the doors of the elevator would close and a short one of 1000Hz when it "arrives" at its destination. The initial state of the elevator is on the first floor. To run the simulation, I used the Arduino UNO microcontroller board and the Arduino IDE (with the code uploaded in the Homework3_Elevator folder). The electrical scheme and physical setup are presented below: 

![Electrical scheme](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/b9e6e539-4987-4284-a60f-ff36dcfc030b)


![Elevator setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/64e88090-3c4e-47f8-880e-8d0adae78603)

Video demonstration: https://youtu.be/KS6gAQM7ZwA?si=tOfHyc0fKOgSSqSp

## Homework #4 - 7 Segment Display Drawing

This task involves drawing on a Seven Segment Display by using a joystick to navigate through the segments and light them up at will. In order to achieve this, I used the Arduino UNO microcontroller board, a breadboard, a 7 Segment Display, 220Ω resistors and wires as needed, as well as a joystick. The movement between segments is indicated in a given matrix (as shown below), and the simulation code (which is uploaded in the Homework4_7Segment folder) is run using the Arduino IDE.

![Movement Matrix](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/c9ec9435-af60-4907-b705-19e17e40d65e)

The starting position is on the decimal point segment, and the current segment is indicated through a blinking of its led. The joystick can also be used to reset the display segments to their initial state in order to begin drawing from the top.

![Electrical scheme](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/ba1a292b-e1f1-4e6e-978d-d04f9747e1d0)


![Project setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/a8dcdc2e-b927-4bde-bdbb-8901526d895e)

Video demonstration: https://youtube.com/shorts/D_TH6OmUh6c?si=rLmXkZZM5AAlxBmO 

## Homework #5 - Stopwatch 

This technical assignment consists of simulating a stopwatch using a Four Digit Seven Segment Display and the 74HC595 Shift Register (as well as other electrical components such as buttons, resistors, wires, breadboards and the Arduino UNO microcontroller board). This project has multiple functionalities, such as using a button to start or pause the stopwatch, one to memorise up to four laps and another button to reset the timer back to its initial state (000.0). The lap button can also be used to view the previously recorded laps. In developing this project, I also learned about multiplexing and making use of optical persistence when working with multiple LEDs.
The adjacent code can be found in the Homework5_Stopwatch folder.


![Project setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/86109616-6534-4fea-ba05-f326f3fb46ad)

Video demonstration: https://youtu.be/3zze2odnqu8?si=2NGQ0i-xjIaa2zk_

## Homework #6 - Environment Monitor and Loggger

This following projects is based on working with sensors, the serial monitor and RGB LED to monitor the light in the environment, to measure distances, offer feedback and log data. On the hardware side, I worked with the HC-SR04 Ultrasonic Sensor, the Light-Dependent Resistor, one RGB LED, the Arduino UNO board and other basic electrical components such as wires, resistors and a breadboard. On the coding side, I used the Arduino IDE and learned to work with EEPROM, as well as the flash memory (adjacent code in the Homework6_EnvironmentMonitor).
To access all the functionalities of the Monitor and Logger, the user interacts through a menu within the serial monitor. The menu has four main options, each with more detailed functionalities:

1. **Sensor Settings**: in here, the user can choose a sampling interval between 1 and 10 seconds to tell the system how often to collect data from the sensors; the user can also set a treshold for each sensor, establishing the smallest value they can retrieve from the environment before throwing an error.
2. **Reset Logger Data**: this option gives the opportunity of deleting the existing measurements and restarting from 0.
3. **System Status**: this third option is for checking the data that's being collected; in here, the user can see what the system is recording in real time, the last 10 measurements saved and the settings set for the sensors (either default or what was set at 1.)
4. **RGB LED Control**: finally, this slot is for controlling the colours the RGB LED uses to give feedback; the user is presented with two modes:
   - automatic, where by default green inidicates if the system is functioning within given parameters and red indicates that the recorded values are exceeding the tresholds 
   - manual, where the user can choose their own colour code for feedback.

![Project setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/61009d20-8d57-4b65-bd54-2d052b88b750)

Video demonstration: https://youtu.be/Y5vvGK696l0?si=BxaUvHymC53KD2CO

## Homework #7 - Mini Matrix Game

This assignment represents a mini-game on a 8 by 8 LED matrix in which the purpose is to clear the entire matrix by taking down walls. To achieve this, the player moves around the field using a joystick and places bombs that destroy the walls when pressing the joystick button. The challenge of the game is to avoid collision with the walls and not get caught in the explosions' radius.

Game components:
1. **Player**: the position of the player is represented on the matrix by a slowly blinking dot; it can move up, down, left and right, as well as "through the edges" of the matrix.
2. **Bombs**: the bombs, placed by the player wherever they choose on the walkable spaces, explode within three seconds of being placed; a player can place a maximum of three bombs at a time; bombs are represented through faster blinking dots.
3. **Walls**: the walls are randomly generated on the matrix at the start of the game or when choosing to play again; they are represented through LEDs that are lit up and not blinking.

If the player succeeds in breaking down all the walls and clearing the field, a smiling face will be displayed to indicate the game has been won. If the player runs into a wall or explodes with the bomb they placed, the game will be over as indicated by a sad face animation. In both cases, the player can enter 0 in the serial monitor if they want to try again, at which point a new field will be generated and the game will start again.

For this project, I used the Arduino Uno board, an 8x8 LED Matrix, the MAX7219 display driver, a joystick as well as two breadboards, resistors, capacitors and wires. The code can be found in the Homework7_MiniMatrixGame folder.

![Project setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/e2f32187-a6db-49e0-840e-beeae969fbea)

Video demonstration: https://youtu.be/kkz1Z_EdIL0?si=XYh3HSrdYYKS3_C8 


## Homework #8 - Matrix Checkpoint - LCD Menu

This assigment consisted of adding an LCD to create a game menu, which you can toggle using a joystick. The purpose of this is to add new functionalities to the game created at Homework #7, such as displaying an initial message for two seconds upon starting the program, the option to start the game from the joystick, to read information about the game and author and modify the LCD and Matrix brightnesses manually. Newly added electrical elements are the LCD, wires, a resistor and a potentiometer.

The menu structure so far is as follows:

1. **Start game**: press the joystick button on this option to begin playing on the matrix
2. **About**: display information such as game name, author and github link of the author by scrolling it on the LCD
3. **Settings**: enter settings menu

Settings menu:
1. **LCD Brightness**: alter LCD brightness by moving the joystick left or right
2. **Matrix Brightness**: alter matrix brightness by moving the joystick left or right
3. **Back**: go back to the main menu



![Project setup](https://github.com/vfranci/IntroductionToRobotics/assets/115077321/9abd74e7-7694-4aa2-9916-14bf960da96b)

Video demonstration: https://youtube.com/shorts/D-jKOg2Ve1Q?si=EBPlpDEz0um2BDDl


