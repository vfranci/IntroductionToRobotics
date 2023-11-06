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







