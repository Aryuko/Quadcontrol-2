## **Quadcontrol 2.0**

A second attempt at making software pilot a Quadcopter.

*Date: 2016-11-05*


*Authors: *

* *Peter Kjellén, [pekj@kth.se](mailto:pekj@kth.se)*

* *Jesper Larsson, [jespel@kth.se](mailto:jespel@kth.se)*

## **Background**

We, together with Viktor Landemoo, constructed a quadcopter and attempted to build software to pilot it as an exam project in high school. The hardware turned out great (thanks to V. Landemoo) and while the software looked promising, in the end the low level programming, the inter-communication between modules and the physics of quadcopters simultaneously proved too challenging at the time.

Now we have a much more intimate understanding of how to utilise and design for microprocessor and inter-communication between components. With this knowledge we will make a second attempt at the software piloting the quadcopter.

## **Objective and Requirements**

While we have better understanding about the software side of things, we know pretty much the same as last time about the physics. Therefore, the main objective of this project will be to build software that can interact with all circuitry on our quadcopter and try to pilot it. The focus will be on the software challenges (e.g. communications between connected circuits, utilising the hardware and implementing control algorithms) and not understanding the physics required for the quadcopter to fly.

Since implementing a fully functioning quadcopter is an enormous task, we have broken it down into smaller milestones. Our first milestone is to achieve automatic stabilization in one axis. This is considered our minimum, and we will be satisfied as long as at least this milestone is reached. If this should succeed and we have time to continue, we will attempt automatic stabilization in 3 axes. Following that we’ll attempt to get some kind of wireless controller input to control the aircraft.

We believe this project to be considered an advanced project, and that it fits the description given in the first example of an advanced project:

*"You create some project with external electronic components that are communicating over SPI, I²C, or some other non-trivial data protocol. Your application has a non-trivial logic."*

## **Solution**

As described above our planned solution is divided into milestones, or more precisely four of them. First we need to communicate with our sensors and motors. This is the foundation that will allow us to build the rest. To achieve this we need to read and understand the manual about the communication protocols and then implement them. After that we want to tackle a similar but simpler problem. Therefore, for our second part we are to implement stabilization in one axis. We plan to isolate a single axis of freedom by mounting our hardware to an axle of some sort. With the the axis isolated we can implement a stabilization algorithm, e.g. a PID algorithm, that works for the reduced problem. When the problem of stabilization for one axis is solved we try to extend that solution into first two dimension of free variables (i.e. two axis of freedom) and then three. To extend our solution we should simply need to extend our stabilization algorithm to more dimensions. If we have time in the end we would like to be implement some way of steering our quadcopter through external input.

We mean to use the ChipKit together with the Basic I/O Shield. Primarily because it provides the I²C-bus correctly wired, which is important since that is the bus that our sensors will use. Our plan for the program is to have quite simple architecture. We intend for the main loop to be quite simple and step through i) polling our sensors, ii) stepping through our stabilization algorithm, and iii) updating the speed of our motors. We also mean to use interrupts connected to the buttons on the I/O board to power on/off the craft and start/stop the polling.

## **Verification**

Stabilization is a subject where there isn’t really one correct solution. A system could be considered stable if it eventually corrects itself, regardless of how long that takes. There is however a distinct difference between the following outcomes:

1. under correcting and not improving the stability much,

2. over correcting and making the stability worse, xor

3. eventually correcting, even if it takes multiple oscillations or just one.

Good automatic stabilization is hard to achieve, and while we will aim for outcome 3 mentioned above, it’s not something we can guarantee. Since physics and automatic control engineering isn’t exactly part of the course, we will evaluate our ability to communicate with and control the motors and sensors of the craft instead of its calibration and ability to stabilize. The project will be considered a success if we can succeed with the following:

1. read information from the sensors (via I²C),

2. control the motors (via PWM), and

3. use the information from the sensors to control the motors (Control algorithms in C).

## **Contributions**

We intend to start the project by working together, side by side. As the project moves on we will likely find areas we can work on simultaneously by splitting up. The actual division will be explained in the final extended abstract.

## **Reflections**

In the final abstract, we will discuss and reflect on the project’s progress, successes and shortcomings.
