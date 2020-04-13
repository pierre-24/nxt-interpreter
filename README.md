**WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP ... WIP**

[I'm working on it](https://github.com/pierre-24/nxt-interpreter/issues/), stay tuned.

# NXT (Lego Mindstorm language) interpreter

Excerpt of [this simulator](http://schuelerlabor.informatik.rwth-aachen.de/roboter-simulator) (for which the source code is available [here](https://github.com/InfoSphereAC/RoboSim)) in order to get a functional interpreter of the NXT language, without the graphical and sound part.

Again, the goal here is to test some code [compiled from NBC](https://github.com/pierre-24/nbc-compiler) (Next Bytes Code) since it is the target language of my CS compiler project lecture.

**I do not plan on fixing bug or anything** ... But if you are a CS student in the University of Namur, you probably know where to find me for (some kind of) support, though... Lucky you ;)


## Compile that

You need [CMake](https://cmake.org/) to at least version 3.10 (I especially lower this requirement so that I can compile it on my "old" Ubuntu 18.04, so that should be good on your more recent distribution).

```bash
mkdir build
cd build
cmake ..
make -J 2
```

There is a lot of warning (deprecated C++14 stuffs herited from the original source code that I could clean up if I had a second life), but two executable should be found in the `build` directory: `nxt_interpreter` and `rxe_analyser`.

## Usage


`rxe_anlyser` is a disassambler of RXE file (excutable NXT format). It gives you the DSTOC section, the Dope Vectors one, the clumps (thread/subroutines) and finally the code.
Sample output:

```
$ ./rxe_analyser ../tests/simple.rxe 
-- Static data --
DSTOC Count:            10
Initial Size:           36
Static Size:            12
Default Data Size:      33
Dynamic Default Offset: 9
Dynamic Default Size:   24
Memory Manager Head:    1
Memory Manager Tail:    0
Dope Vector Offset:     16
Clump Count:            1
Code Word Count:        29

-- DSTOC --
entry	type	flags	desc	contents
0:  	sword	0		0		2000
1:  	ubyte	1		2		0
2:  	sbyte	0		3		96
3:  	sbyte	0		4		63
4:  	sbyte	0		5		100
5:  	sbyte	0		6		32
6:  	sbyte	1		7		0
7:  	sbyte	0		8		3
8:  	array	0		10		1
9:  	ubyte	0		0		208

-- Dope Vectors --
entry	size	count
0:		10		2
1:		1		2

-- Clump data --
entry	fc	cs	dependents
0:		0	0	

-- Code --
pc	word	Disassembled
0	0		OP_SETOUT 46  8 (array)  2 (sbyte)  4 (sbyte)  1 (ubyte)  7 (sbyte)  8 (array)  6 (sbyte)  6 (sbyte)  5 (sbyte)  7 (sbyte)  6 (sbyte)  5 (sbyte)  6 (sbyte)  10 (?????)  2 (sbyte)  11 (void)  5 (sbyte)  12 (?????)  5 (sbyte)  0 (sword)  3 (sbyte)  
1	23		OP_WAIT 65535 (NOT_A_DS_ID)  0 (sword)  
2	26		OP_FINCLUMP 65535  65535  
```

On the other hand, `nxt_interpreter` is an interpreter. 
Sample output (if you want to understand this strange output, check [the source code](tests/file.nbc)):

```
$ ./nxt_interpreter ../tests/file.rxe
this, this, and that 21
```

There is also a `-g` option if you want to get an idea on what you robot did (see [the source code](tests/simple.nbc) to understand what happened):

```
$ ./nxt_interpreter ../tests/simple.rxe -g
.note initial position of the robot is x=7.5;z=17.5;theta=0
-- start (at t=0 ms)
-- end (at t=2001 ms, nperiod=2086)
.note final position of the robot is x=14.4804;z=17.5;theta=0
```

And there is a `-m path/to/the/file/.map` if you want to change the robot environment.
For more information about the map, the robot, and its movement, check [there](doc/about_simulation.md).

## Note: dropped and added stuffs

I dropped:

+ [`Drawer`](https://github.com/InfoSphereAC/RoboSim/blob/master/Drawer.cpp), and basically the graphical part (rely on SDL) ;
+ [`RobotSpeaker`](https://github.com/InfoSphereAC/RoboSim/blob/master/RobotSpeaker.cpp), which use OpenAL for nothing much (we don't play sounds here).
+ [`Client`](https://github.com/InfoSphereAC/RoboSim/blob/master/Client.cpp) and [`Server`](https://github.com/InfoSphereAC/RoboSim/blob/master/Server.cpp), since I only need a single robot here. Thus, I'll use the [`Single` one](https://github.com/InfoSphereAC/RoboSim/blob/master/Single.h).

... And I added:

+ I implemented some of the few remaning opcodes (see what's left [here](src/Interpreter/InterpreterThread.h)).
+ A threading mechanism, so `op_finclump` really schedule other clumps (= threads) (see [there](tests/thread.nbc)).
+ A virtual file system, if you ever need that (see [there](tests/file.nbc)).
+ A special handler, `0` for the standard output, so you can `FileWrite` directly in it (it is always open and you can't close it anyway).

## Note: the project structure

I did a bit of cleaning by giving a little bit of structure in the files of the [`src/` folder](src/)

+ [`src/Execution` folder](src/Execution): related to the execution of the RXE byte code, with the [context](src/Execution/ExecutionContext.h) which serve as the main interface to execute a code and the [`RXEFile`](src/Execution/RXEFile.h) object, which opens the binary file. 
+ [`src/Interpreter` folder](src/Interpreter): contains the bytecode [`Interpreter`](src/Interpreter/Interpreter.h) (which sets up the threading mechanism).
+ [`src/System` folder](src/System): related to the virtual operating system of which the codes run. In particular, the [`VMMemory`](src/System/VMMemory.h) object (which manage the memory), the [`VFileSystem`](src/System/VFileSystem.h) object (which implement a in memory virtual flat file system), and the [`System`](src/System/System.h) object (which handle *syscalls* and other system stuffs).
+ [`src/Robot` folder](src/Robot): related to the [robot](src/Robot/Robot.h) itself, and its [motor](src/Robot/Motor.h). Also contains the (in)famous [`NetworkInterface`](src/Robot/NetworkInterface.h) abstract object to communicate with it.
+ [`src/Environement/` folder](src/Environment): related to the robot environment in which it moves, corresponding to a kind of "map". In particular, it deals with the light sensor and the collisions.
+ [`src/utils` folder](src/utils): the traditional `utils` folder (here [time](src/utils/Time.h), [byte manipulations](src/utils/ByteOrder.h) and [vectors](src/utils/Vec4.h)).
