# A few note on the simulation

**Note:** there are a few useful macros [here](tests/commons.inc).

By default, the robot is dropped at a random position (that may change in the future) in a 25x25 map surrounded by walls.

The bot itself is about 2x2. It may not go outside the map since the simulator checks for collisions.
The interpreter assume that `OUT_A` is the left motor and `OUT_B` the right one.

The constants may be adjusted before compilation in [`src/Robot/Constants.h`](src/Robot/Constants.h)

## Moving

Thus, in NBC, moving the bot forward is basically (see [there](tests/simple.nbc)):

```c
OnFwd(OUT_AB,100) // p = 100
wait 2000 // t = 2s
```

The following formula help to compute the distance:

+ `v(p) = p * f * D * π / 360` where `p` is the power (given to the motor as second parameter of `OnFwd()`), `f` is the power to speed ratio (10 is the current code) and `D` is the diameter of a wheel (0.4 in the current code). This give you the **speed** of the robot depending on the power.
+ `x(p,t) = v(p) * t`: this give you the **distance** that the robot will travel in a given `t` time (in second).
+ Thus, `t(p) = 1 / v(p)` gives you the time required to move by one unit of distance.


## Turning

**Turning** is a bit tricky (!! the `RotateMotor()` macro is bugged !!), but one way to do it is to give a reverse power value to both motors (see [there](tests/simpleturn.nbc)):

```c
OnFwd(OUT_A,20) // p=20
OnFwd(OUT_B,-20) // works as well with `OnRev(OUT_B,20)`
wait 1856 // t=1.856 s
```

By setting the left motor forward and right motor backward, one turn right.

Here, time is the key ingredient to arrive to the right angle, `a`. It is given by the following formula : `t(a, p) = |a| * H / (p * f * D) * 2`, where `H` is the half width of the robot (currently 0.825).
Sadly, due to the precision of the hardware and the values (only integer values), the angle is not exactly 90°. Except if you move veeeeeeeery slowly.

But if you want to play, there is a way to do better by using the rotation counter: [see there](https://sheldenrobotics.com/tutorials/Detailed_Turning_Tutorial.pdf) in the meantime.