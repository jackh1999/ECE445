# Table of Contents

[Sep 16, 2024](#September-16,-2024)

# September 16, 2024

## Respository Setup

We setup the notebooks repository, will be adding updates and supporting documents as they come by in future meetings

## Proposal Preperation
Currently using Google Docs for the proposal for collaborative environment, may transition to LaTeX later TBD.

### Problem
Problem was inspired by ECE 444 Class: Theory and Fabrication of Integrated Circuits. Semiconductors are difficult to test and testing equipment is expensive because of that. We decided to tackle the four-point probe setup due to it being used in the lab and its lower complexity.

### Solution
We plan to design and create our own four-point probe capable of measuring the sheet resistance of wafers. We don't really have the capability to create probe-heads on our own, so we plan to order some. Four-point probes utilize a current-source and a voltage-measurement, the outer two probes supply current while the inner two probes measure the voltage. For user interface, we plan to use a keyboard and display. 

### Visual Aid
Key components will include the probe measurement setup, the analog to digital converter, the microcontroller for calculations, the display, and the keyboard. The probe measurements will interact with the ADC. The Microcontroller will interact with the ADC, Display, and Keyboard to manage proper I/O.

### High-Level Requirements
Discussion on High-level requirements. We are thinking that measurement precision is important, perhaps within ±50 mV. Wafers come in different sizes and shapes (?) so we would like to make sure that our system will work for a variety of different shapes, we think that 5 shapes is a good benchmark. Current four-point probes use a varying current source value for wafers, we are not sure why (will look into it? guess right now perhaps to slowly achieve an accruate value). We would like to use cost as a requirement, but will discuss with TA during meeting to make sure it's valid.

### Block Diagram
We have 3 subsystems within our design: Measurment, User Input, Display. Measurement subsystem will supply the current across the two outer probes and measure the voltage across the two inner probes. User Input is self explanatory, we will need to come up with a menu for interaction. Display is a bit ambiguous, but will likely include the various calculations used to determine what is being measured and the sheet resistance including the correction factor. 
Added a 4th subsystem, Power Supply subsystem as we will need to step down the 5V input to 3.3V (and potentially later 1.8V) for the other subsystems.
![Block Diagram](Photos/Block-Diagram.pdf)

### Subsystem Overview


### Tolerance Analysis
Some simulating software will be used to prove the feasability of the model. 
