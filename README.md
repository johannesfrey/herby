# Project Overview

  The project "Herby" was realized in the context of the lecture "Embedded Systems" during winter semester 14/15.
  "Herby" pursues the idea of an automatic garden robot. "Herby" is driven by the micro-controller Arduino. Additionaly
  there is a "RaspberryPi", that acts as a web server, which exposes a REST API to control the Arduino.
  Furthermore "Herby" works fully automatic, when not controlled via the Web-Interface.

# Class Initialization
  
  "Herby" is written in C++ and is currently split up into following classes:
  
  * Herby
    
    Main controller, that controlls the movement of the axis-motors and the general behaviour of "Herby".

  * HerbyHead

    Controller that incorporates different sensors and working-sets like switching leds on/off, pouring water, checking humidity etc.

  * HerbyProtocolParser

    Handles I2C communication between the RaspberryPi and the Arduino. Implements the HerbyCommunicationProtocol.

  Additionally there are two header/config-files:

  * HerbyConfig

    Defines several constants like pins, speed of motors, steps per motor etc.

  * HerbyCommunicationProtocol

    Custom I2C protocol for sending commands and transfering data.

## Initialization process

  In order to initialize an instance of herby we first have to create an instance of HerbyHead and an instance of HerbyProtocolParser.
  Precisely the HerbyProtocolPraser gets created implicitly when initializing Herby, while the instance of HerbyHead has to be passed
  explicitly as method argument to Herby by calling Herby::addHead.

  When calling the default contructors of Herby and HerbyHead the required default values are retrieved from the HerbyConfig header-file.
  Custom values can be passed by explicitely calling the provided create<Component> and set<Value> methods. Basically they overwrite the
  default values.

  After the intialization process we have to call the begin methods of both HerbyHead and Herby in order to start them.

## Internal mechanisms

  "Herby" incorporates some functionality and techniques due to working in the Arduino/Atmel328P environment,
  which will be explained in this section.

### State Machines

  Due to the fact that the Atmel328P is a micro-controller, the Arduino library merely exposes a loop method (which is called by the
  main method in an infinite loop) in which all the work is done.

  To guarantee that all working-steps of "Herby" are done in an controlled, sequential way, there are two state machines involved.
  The primary state machine, which is implemented inside Herby.cpp is responsible for coordinating the sequential ordering between moving along
  the X and Y axes (automatic movement along all grid cells is the default behaviour) and letting the HerbyHead perform its work
  (measuring humidity, pouring water, switch-on lights etc). The secondary state machine is implemented inside HerbyHead. It is
  responsible for the guaranteed sequential ordering of the work that has to be performed by HerbyHead.

  Basic process:

  After moving along the axes, the primary state machine switches into WORK mode and calls doWork of HerbyHead. As long as doWork returns
  false (Meaning: the seconday state machine inside HerbyHead steps through its working sets) it stays in WORK mode. As soon as doWork returns
  true (Meaning: the work of HerbyHead is done) the primary state machine inside Herby switches into the next state.

  A special state inside the primary state machine is the state INTERRUPTED. In this state a special flag is checked. If true this flag signalizes
  that there was a command (grid coordinates + work-tasks) sent over from the web-interface via I2C. This leads to a prioritized execution of
  the sent commands. After finishing these, Herby automatically continues on the grid position where it left off.

### Timers and Interrupts

  Herby relies heavily on Timer1 of the Atmel32P. Timer1 is used to control the correct movement of the stepper motors by calling the run methods
  of the steppers inside a corresponding ISR. To avoid blocking the loop method, all working-sets and the movement of the DC motors are executed
  over specific time periods (these are checked in every loop cycle by calling millis).
  
  Also the used Arduino Wire I2C library relies on interrupts under the hood.

### I2C communication & HerbyCommunicationProtocol

  For sending commands and transfering data we defined a custom protocol that aligns with the underlying I2C protocol. While I2C theoretically
  allows an unlimited stream of bytes, the Arduino I2C buffer is limited to a maximum of 32 bytes.

  The HerbyCommunicationProtocol is defined as follows:

  * Byte 1: Start Pattern (0xc5)
  * Byte 2: Length
  * Byte 3: Command
  * Byte 4: Data (low byte: data & 0xff) 
  * Byte 5: Data (high byte: (data >> 8) & 0xff ) 
  * Byte 6: Unused 
  * Byte 7: Unused 
  * Byte 8: Parity Byte

  In order to reduce communication overhead, the payload block(byte 3, 4, 5) can be repeated, in order to be able to send several commands/data in one stream.
  Because of the restriction to 32 bytes and the five mandatory bytes of the HerbyProtocol (start pattern, length, unused, unused, parity byte) the maximum
  pay-load block is limited to 27 bytes. This means 9 x (command, data, data) blocks.

  // The first to bytes have to be these two:

  * Byte 1: Start Pattern (0xc5)
  * Byte 2: Length

  // These bytes form the payload block, which can be concatenated (max: 9 times)

  * Byte 3: Command
  * Byte 4: Data (low byte: data & 0xff) 
  * Byte 5: Data (high byte: (data >> 8) & 0xff ) 

  // The protocol has to end with the following bytes:

  * Byte 6: Unused 
  * Byte 7: Unused 
  * Byte 8: Parity Byte

### Todo & Next steps

  * Workaround for the I2C communication between Arduino & RaspberryPi (RaspberryPi interrupts)
  * Refactor and extend HerbyProtocolParser
  * Refactor shared state between HerbyProtocolParser and Herby
  * Utility library for removing repeated use of period calculation
  * New concept for components for HerbyHead
  * Module-Architecture
  * Extended configuration options for the Web-UI
  * ...
