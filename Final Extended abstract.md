## **Quadcontrol 2.0**

A second attempt at making software pilot a Quadcopter.

*Date: 2016-12-09*


*Authors:*

* *Peter Kjellén, [pekj@kth.se](mailto:pekj@kth.se), 960130-4018*

* *Jesper Larsson, [jespel@kth.se](mailto:jespel@kth.se), 960722-3378*

## **Objective and Requirements**

Our main objective in this project was to write software that enabled us to interact with the hardware required for a quadcopter and utilize these software layers to implement some controller attempting stabilizing the craft in one dimension. We also had a second objective to implement a controller that managed to stabilize the craft in actual flight.

We formalized these objectives into a few milestones. The first milestone was to communicate with the hardware, i.e. reading gyroscopic- and acceleration data from our sensors and to control the four motors on the quadcopter. Next milestone required us to utilize this data to complete our main objective, i.e. attempting to stabilize the craft in one axis. The third milestone was to generalize our work from the second milestone into higher dimension. These milestones covers our objectives. We then added some further milestones relating to being able to control the craft as well as optimise and clean up the code.

Creating a fully fledged quadcopter is hard and besides interacting with the hardware has very little to do with the low level programming the course focuses on. Due to these facts we limited our requirements to milestone two.

## **Solution**

We built our program from the hardware up, and here is where we put the most focus of our work. We created general interfaces for hardware modules for the I2C module and the Output Compare module. On top of those interfaces we built more specialised interfaces layers to interact with the sensor and motors, allowing us to interact with the hardware in more abstract and useful ways.

We also built some utility libraries doing specialized tasks such as time keeping, PID controllers, and input/output to the buttons/switches/leds on the Basic I/O Shield.

We then chained these interfaces and libraries together in a main loop that works very close to how we described it in the draft version of the extended abstract. It polls data from the sensors, sends that data through the stabilizing controller, and finally outputs that data to the motors.

We have also added a separate debugging mode triggered by one of the switches that outputs sensor values to the screen instead of running the main loop. We also use one of the switches for deciding which startup sequence to send to the ESCs.

We only use the interrupt functionality for the timers due to simplifying the program logic.

## **Verification**

Since physics and control engineering isn’t rpart of the course, we decided to evaluate our ability to communicate with and control the motors and sensors of the craft instead of its calibration and ability to stabilize. We decided that the project would be considered a success if we could succeed with the following:

1. read information from the sensors (via I2C),

2. control the motors (via PWM), and

3. use the information from the sensors to control the motors (Control algorithms in C).

We succeed on all three of these points, and we verified them by doing the following:

1. Output sensor information to the screen on the I/O Shield, verify that the numbers change to reflect the orientation of the sensor and craft.

2. Input different PWM values to the motors, verify that their torque change to reflect the given value. This was done by creating a loop that incremented the PWM input by a constant value each loop, and outputted the value to the screen on the I/O Shield.

3. Use sensor information processed by the controller as input to the motors. Verify that the torque of the motors change to reflect the orientation of the sensor and craft.

## **Contributions**

We started the project working together — side by side — on all files. Later on we split up, working on different parts of the project. The distribution of contributions are roughly as follows:

**Jesper & Peter:**

Main loop and low level sensor I2C interface.

**Jesper:**

PWM library, ESC PWM interface, I2C hardware interface, high level sensor I2C interface, and time library.

**Peter:**

PID controller, input interface (switches and buttons), and middle level sensor I2C interface.

**F Lundevall & Axel Isaksson**

Display interface, quicksleep function, and general PIC32 setup including ISR trampoline

## **Reflections**

Since this is essentially a revival or restart of an old project, we knew a lot about the nature of the project before we began. We knew getting the controller functioning well would be hard and time-consuming, which is why we limited the scope by putting up milestones. During this project we have been surprisingly unsurprised, everything has been going according to plan. We expected to not fully finish the controller for all 6 axes, and thus we decided to be content if we managed to complete it for one axis of rotation. This has been completed, and the entire codebase leading up to it is generalized and abstracted enough to allow easy expansion of the project. It have been nice to have a project go according to plan for once.
