**WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP**

[I'm working on it](https://github.com/pierre-24/nxt-interpreter/issues/), stay tuned.

# NXT (Lego Mindstorm language) interpreter

Excerpt of [this simulator](http://schuelerlabor.informatik.rwth-aachen.de/roboter-simulator) (for which the source code is available [here](https://github.com/InfoSphereAC/RoboSim)) in order to get a functional interpreter of the NXT language, without the graphical and sound part.

Again, the goal here is to test some code [compiled from NBC](https://github.com/pierre-24/nbc-compiler) (Next Bytes Code) since it is the target language of my CS compiler project lecture.

**I do not plan on fixing bug or anything** ... But if you are a CS student in the University of Namur, you probably know where to find me for (some kind of) support, though... Lucky you ;)


## Compile that

You need `CMake` ... 

... And I need to understand how to properly compile a release binary in a place that is not hidden (`cmake .; cd build; make` ... something like that would be nice).

## Usage

*blablabla input file ...*

Check a few things [there](about_simulation.md).

## Note: dropped and added stuffs

I dropped:

+ [`Drawer`](https://github.com/InfoSphereAC/RoboSim/blob/master/Drawer.cpp), and basically the graphical part (rely on SDL) ;
+ [`RobotSpeaker`](https://github.com/InfoSphereAC/RoboSim/blob/master/RobotSpeaker.cpp), which use OpenAL for nothing much (we don't play sounds here).
+ [`Client`](https://github.com/InfoSphereAC/RoboSim/blob/master/Client.cpp) and [`Server`](https://github.com/InfoSphereAC/RoboSim/blob/master/Server.cpp), since I only need a single robot here. Thus, I'll use the [`Single` one](https://github.com/InfoSphereAC/RoboSim/blob/master/Single.h).

... And I added:

+ I implemented some of the few remaning opcodes.
+ A threading mechanism, so `op_finclump` really schedule other clumps (= threads).
+ A virtual file system.

## Note: the project structure

I did a bit of cleaning by giving a little bit of structure in the files of the [`src/` folder](src/)

+ [`src/Execution` folder](src/Execution): related to the execution of the RXE byte code, with the [context](src/Execution/ExecutionContext.h) which serve as the main interface to execute a code and the [`RXEFile`](src/Execution/RXEFile.h) object, which opens the binary file. 
+ [`src/Interpreter` folder](src/Interpreter): contains the bytecode [`Interpreter`](src/Interpreter/Interpreter.h) (which sets up the threading mechanism).
+ [`src/System` folder](src/System): related to the virtual operating system of which the codes run. In particular, the [`VMMemory`](src/System/VMMemory.h) object (which manage the memory), the [`VFileSystem`](src/System/VFileSystem.h) object (which implement a in memory virtual flat file system), and the [`System`](src/System/System.h) object (which handle *syscalls* and other system stuffs).
+ [`src/Robot` folder](src/Robot): related to the [robot](src/Robot/Robot.h) itself, and its [motor](src/Robot/Motor.h). Also contains the (in)famous [`NetworkInterface`](src/Robot/NetworkInterface.h) abstract object to communicate with it.
+ [`src/Environement/` folder](src/Environment): related to the robot environment in which it moves, corresponding to a kind of "map". In particular, it deals with the light sensor and the collisions.
+ [`src/utils` folder](src/utils): the traditional `utils` folder (here [time](src/utils/Time.h), [byte manipulations](src/utils/ByteOrder.h) and [vectors](src/utils/Vec4.h)).
