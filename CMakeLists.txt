cmake_minimum_required(VERSION 3.15)
project(nxt_interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(nxt_interpreter
        main.cpp
        # execution context
        ExecutionContext.cpp
        # RXE file
        RXEFile.cpp
        # virtual memory
        VMMemory.cpp
        # system
        System.cpp
        Time.c
        # robot stuffs
        Motor.cpp
        Robot.cpp
        Vec4.cpp
        # network interface
        NetworkInterface.cpp
        # environment
        Environment.cpp
        Simulation.cpp
        # interpreter
        Interpreter.cpp
        Interpreter_CompareInstructions.cpp
        Interpreter_ControlFlowInstructions.cpp
        Interpreter_DataManipulationInstructions.cpp
        Interpreter_LogicalInstructions.cpp
        Interpreter_MathInstructions.cpp
        Interpreter_SystemIOInstructions.cpp)

add_executable(rxe_analyser
        RXEAnalyzerMain.cpp
        # execution context
        ExecutionContext.cpp
        # RXE file
        RXEFile.cpp
        # virtual memory
        VMMemory.cpp
        # system
        System.cpp
        Time.c
        # robot stuffs
        Motor.cpp
        Robot.cpp
        Vec4.cpp
        # network interface
        NetworkInterface.cpp
        # environment
        Environment.cpp
        Simulation.cpp
        # interpreter
        Interpreter.cpp
        Interpreter_CompareInstructions.cpp
        Interpreter_ControlFlowInstructions.cpp
        Interpreter_DataManipulationInstructions.cpp
        Interpreter_LogicalInstructions.cpp
        Interpreter_MathInstructions.cpp
        Interpreter_SystemIOInstructions.cpp)