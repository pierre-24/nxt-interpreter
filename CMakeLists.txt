cmake_minimum_required(VERSION 3.15)
project(nxt_interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(nxt_interpreter
        src/NXTInterpeterMain.cpp
        # utils
        src/utils/Time.cpp
        src/utils/Vec4.cpp
        # execution context
        src/Execution/ExecutionContext.cpp
        src/Execution/RXEFile.cpp
        # virtual memory and system
        src/System/VMMemory.cpp
        src/System/System.cpp
        # robot stuffs
        src/Robot/Motor.cpp
        src/Robot/Robot.cpp
        src/Robot/NetworkInterface.cpp
        src/Robot/SingleRobotNetworkInterface.cpp
        # environment
        src/Environment/Environment.cpp
        src/Environment/Simulation.cpp
        # interpreter
        src/Interpreter/Interpreter.cpp
        src/Interpreter/Interpreter_CompareInstructions.cpp
        src/Interpreter/Interpreter_ControlFlowInstructions.cpp
        src/Interpreter/Interpreter_DataManipulationInstructions.cpp
        src/Interpreter/Interpreter_LogicalInstructions.cpp
        src/Interpreter/Interpreter_MathInstructions.cpp
        src/Interpreter/Interpreter_SystemIOInstructions.cpp)

add_executable(rxe_analyser
        src/RXEAnalyzerMain.cpp
        # file
        src/Execution/RXEFile.cpp
        # virtual memory
        src/System/VMMemory.cpp
        # opcode name
        src/Interpreter/OpcodeName.cpp)