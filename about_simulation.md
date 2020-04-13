# A few note on the simulation

**Note:** there are a few useful macros [here](tests/commons.inc).


```
+----------------------> x
|
|
|   --> (initial orientation of the robot)
|
|
z

```

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

The following formula help to compute the distance. This is the formula giving you the speed, ![](http://www.sciweavers.org/tex2img.php?eq=v&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0), of the robot, depending on the power, ![](http://www.sciweavers.org/tex2img.php?eq=p&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

![](http://www.sciweavers.org/tex2img.php?eq=v%28p%29%20%3D%20p%5C%2Cf%20%5C%2CD%5C%2C%5Cfrac%7B%5Cpi%7D%7B360%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

where ![](http://www.sciweavers.org/tex2img.php?eq=f&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) is the power to speed ratio (10 is the current code) and ![](http://www.sciweavers.org/tex2img.php?eq=D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) is the diameter of a wheel (0.4 in the current code).

Thus,

![](http://www.sciweavers.org/tex2img.php?eq=x%28p%2Ct%29%3Dv%28p%29%5Ctimes%20t&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

gives you the **distance** that the robot will travel in a given ![](http://www.sciweavers.org/tex2img.php?eq=t&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) time (in second).

And therefore,

![](http://www.sciweavers.org/tex2img.php?eq=t%28p%29%20%3D%20%5Cfrac%7B1%7D%7Bv%28p%29%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

gives you the time required to move by one unit of distance at the given power.


## Turning

**Turning** is a bit tricky.
 
One way to do it is to give a reverse power value to both motors (see [there](tests/simpleturn.nbc)):

```c
OnFwd(OUT_A,20) // p=20
OnFwd(OUT_B,-20) // works as well with `OnRev(OUT_B,20)`
wait 1856 // t=1.856 s
```

By setting the left motor forward and right motor backward, one turn right (so the rotation angle gets negative).

Here, time, ![](http://www.sciweavers.org/tex2img.php?eq=t&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0), is the key ingredient to arrive to the right angle, ![](http://www.sciweavers.org/tex2img.php?eq=\\alpha_{w}&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) (given in degree). It is given by the following formula : 

![](http://www.sciweavers.org/tex2img.php?eq=t%28%5Calpha%2C%20p%29%20%3D%202%5C%2C%7C%5Calpha%7C%5C%2C%5Cfrac%7B%20H%7D%7Bp%5C%2Cf%5C%2CD%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)


where ![](http://www.sciweavers.org/tex2img.php?eq=H&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) is the half width of the robot (currently 0.825).
Sadly, due to the precision of the hardware and the values, the angle is not exactly what you expect. Except if you move veeeeeeeery slowly.

Another way is to set an expected rotation of the wheel and wait for the wheel to actually get to that point.
That can be achieved with the `RotateMotorEx()` macro (see [there](tests/simpleturn2.nbc)):

```
RotateMotorEx(OUT_AB, 100 /* output power */, 371 /* wheel rotation */, -100 /*-100 for a right rotation, +100 for left */, 1, 1)
```

Of course, the wheel rotation ![](http://www.sciweavers.org/tex2img.php?eq=\\alpha_{w}&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) does not correspond to the robot rotation. Again, there is a formula, being 

![equation](http://www.sciweavers.org/tex2img.php?eq=\\alpha_{w}(\\alpha)%20%3D%202\\,\\alpha\\,\\frac{H}{D}&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)

This way seems a little bit more precise (in fact, it adjusts the value by doing a few control and error cycles).
