//
// Created by pierre on 05/04/2020.
//

#ifndef NXT_INTERPRETER_CONSTANTS_H
#define NXT_INTERPRETER_CONSTANTS_H

#include "../utils/Vec4.h"

namespace MotorConstant
{
    const float powerToSpeedFactor = 10.f;
}

namespace RobotConstant {
    // width of the robot (from one wheel to the other):
    const float trackWidth = 0.825f*2.0f;

    // diameter of a wheel
    const float wheelDiameter = 0.4f;

    // Note: actual speed is `motorPower * powerToSpeedFactor / 360.f * M_PI * wheelDiameter` (in unit/second)
    // Note: distance is that, multiplied by the time.

    // 2D (x and z) dimensions of the robot
    const float4 standard2DBoundingBox[4] = {
            float4(1.0,    0.0f, -0.9f),
            float4(1.0,    0.0f,  0.9f),
            float4(-0.85f, 0.0f,  0.9f),
            float4(-0.85f, 0.0f, -0.9f)
    };

    // 2D dimensions of a sensor
    const float4 touchSensor2DBoundingBox[4] = {
            float4(0.45f,  0.0f, -0.125f),
            float4(0.45f,  0.0f,  0.125f),
            float4(0.375f, 0.0f,  0.125f),
            float4(0.375f, 0.0f, -0.125f)
    };

    // height of the robot
    const float boundingBox3DHeight = 1.1f;

    // sensor stuffs
    const float ultrasoundRange = 1000.0f;
    const float ultrasoundDistanceScale = 5.0f;
    const float noiseLevelScale = 10.0f;
    const float touchRange = 0.25f;
    const float lightRange = 100.0f;

    const float sensorYOffset = 0.2f;
    const float sensorXOffset = 1.15f;
}

#endif //NXT_INTERPRETER_CONSTANTS_H
