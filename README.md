# NXT (Lego Mindstorm language) interpreter

Excerpt of [this simulator](http://schuelerlabor.informatik.rwth-aachen.de/roboter-simulator) (for which the source code is available [here](https://github.com/InfoSphereAC/RoboSim)) in order to get a functional interpreter of the NXT language, without the graphical part.

Again, the goal here is to test some code [compiled from NBC](https://github.com/pierre-24/nbc-compiler) (Next Bytes Code) since it is the target language of my CS compiler project lecture.

**I do not plan on fixing bug or anything** ... But if you are a CS student in the University of Namur, you probably know where to find me for (some kind of) support, though.


That being said, lets go.


## Note: dropped stuffs

+ [`Drawer`](https://github.com/InfoSphereAC/RoboSim/blob/master/Drawer.cpp), and basically the graphical part (rely on SDL) ;
+ [`RobotSpeaker`](https://github.com/InfoSphereAC/RoboSim/blob/master/RobotSpeaker.cpp), which use OpenAL for nothing much.