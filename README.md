# NXT (Lego Mindstorm language) interpreter

Excerpt of [this simulator](http://schuelerlabor.informatik.rwth-aachen.de/roboter-simulator) (for which the source code is available [here](https://github.com/InfoSphereAC/RoboSim)) in order to get a functional interpreter of the NXT language, without the graphical and sound part.

Again, the goal here is to test some code [compiled from NBC](https://github.com/pierre-24/nbc-compiler) (Next Bytes Code) since it is the target language of my CS compiler project lecture.

**I do not plan on fixing bug or anything** ... But if you are a CS student in the University of Namur, you probably know where to find me for (some kind of) support, though... Lucky you ;)


## Compile that

You need `CMake` ... 

... And I need to understand how to properly compile a release binary in a place that is not hidden (`cmake .; cd build; make` ... something like that would be nice).

## Note: the bot and its movement

By default, the bot is dropped at a random position (that may change in the future) in a 25x25 map surrounded by wall.
The bot itself is about 2x2 (see its exact dimension in [`src/Robot/Constants.h`](src/Robot/Constants.h)).
It may not go outside the map since the simulator checks for collisions.

Basically, the interpreter assume that `OUT_A` is the left motor and `OUT_B` the right one.
Thus, in NBC, moving the bot forward is basically (see [there](tests/simple.nbc)):

```c
OnFwd(OUT_AB,100) // p = 100
wait 2000 // t = 2s
```

The following formula help to compute the distance:

+ `v(p) = p * f * D * π / 360` where `p` is the power (given to the motor as second parameter of `OnFwd()`), `f` is the power to speed ratio (10 is the current code) and `D` is the diameter of a wheel (0.4 in the current code). This give you the **speed** of the robot depending on the power.
+ `x(p,t) = v(p) * t`: this give you the **distance** that the robot will travel in a given `t` time (in second).
+ Thus, `t(p) = 1 / v(p)` gives you the time required to move by one unit of distance.

**Turning** is a bit tricky, but one way to do it is to give a reverse power value to both motors (see [there](tests/simpleturn.nbc)):

```c
OnFwd(OUT_A,20) // p=20
OnFwd(OUT_B,-20) // works as well with `OnRev(OUT_B,20)`
wait 1856 // t=1.856 s
```

By setting the left motor forward and right motor backward, one turn right.

Here, time is the key ingredient to arrive to the right angle. It is given by the following formula : `t(p) = D' / (p * f * D) * 180`, where `D'` is the half width of the robot (currently 0.825).
Sadly, due to the precision of the hardware and the values (only integer values), the angle is not exactly 90°. Except if you move veeeeeeeery slowly.

## Note: the project structure

I did a bit of cleaning by giving a little bit of structure in the files of the [`src/` folder](src/)

+ [`src/Execution` folder](src/Execution): related to the execution of the RXE byte code, with the [context](src/Execution/ExecutionContext.h) which serve as the main interface to execute a code and the [`RXEFile`](src/Execution/RXEFile.h) object, which opens the binary file. 
+ [`src/Interpreter` folder](src/Interpreter): contains the bytecode [`Intepreter`](src/Interpreter/Interpreter.h).
+ [`src/System` folder](src/System): related to the virtual operating system of which the codes run. In particular, the [`VMMemory`](src/System/VMMemory.h) object (which manage the memory) and the [`System`](src/System/System.h) object (which handle *syscalls* and other system stuffs).
+ [`src/Robot` folder](src/Robot): related to the [robot](src/Robot/Robot.h) itself, and its [motor](src/Robot/Motor.h). Also contains the (in)famous [`NetworkInterface`](src/Robot/NetworkInterface.h) abstract object to communicate with it.
+ [`src/Environement/` folder](src/Environment): related to the robot environment in which it moves, corresponding to a kind of "map". In particular, it deals with the light sensor and the collisions.
+ [`src/utils` folder](src/utils): the traditional `utils` folder (here [time](src/utils/Time.h), [byte manipulations](src/utils/ByteOrder.h) and [vectors](src/utils/Vec4.h)).

Note that the robot position is updated out of the movement of the motors. Took me some time to figure it out :)

## Note: dropped stuffs from the original simulator

+ [`Drawer`](https://github.com/InfoSphereAC/RoboSim/blob/master/Drawer.cpp), and basically the graphical part (rely on SDL) ;
+ [`RobotSpeaker`](https://github.com/InfoSphereAC/RoboSim/blob/master/RobotSpeaker.cpp), which use OpenAL for nothing much (we don't play sounds here).
+ [`Client`](https://github.com/InfoSphereAC/RoboSim/blob/master/Client.cpp) and [`Server`](https://github.com/InfoSphereAC/RoboSim/blob/master/Server.cpp), since I only need a single robot here. Thus, I'll use the [`Single` one](https://github.com/InfoSphereAC/RoboSim/blob/master/Single.h).