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
![Block Diagram](./Photos/BlockDiagram.jpg)

### Subsystem Overview
We transfered over the subsystem overview from our RPA, each subsystem has pretty simple interaction and requirements.

### Subsystem Requirements
TBD

### Tolerance Analysis
Some simulating software will be used to prove the feasability of the model. Point to discuss at TA meeting.

### Ethics & Safety
Unknown Ethical and Safety concerns currently. Don't put your finger by the sharp probes. Ensure calibration/accuracy before delivery to user? Point to discuss at TA meeting.


# September 17, 2024

## TA Meeting

### Logistics
Currently planning on keeping the same time slot each week, will input availability to the PACE document. Normal office hours will be available, orange boxes are dissapearing

### Block Diagram
Certain connections need more labels, will update and correct. Potentially add the 

### Ethics and Safety
Few ideas and examples provided, mainly the FA22 Team 1 Proprosal. 

### High Level Requirements
Update to full complete sentences. Add a few qualitative sentences about the features. Be modest at first as anything we put down are going to be required for the final design.

### Github/Notebook
Single notebook being updated by multiple users if fine, we need to upload the link to the PACE My Project tab.

### Tolerance
Add error rates. Potentially use the datasheets, and various calculations. SPICE can be used, but more than likely will be too complex and need input. 

### PCB Design Review
Keep posted, we should keep in mind to ensure preparation. Shipping will likely take a week, we should keep that in mind to potentially have multiple PCB versions with alterations in case delays occur.

### Resources
Team gets $150 through my.ECE. Eshop has free stuff. Meeting is timed or something to log hours.

# September 24, 2024

## TA Meeting

### Proposal Feedback
High-level requirements good enough. Block Diagram looks fine, not much to change. Subsystem Overview looks good. Subsystem requirement 2.3.1.2 need more detail, should add the range of voltage 
but its difficult as theres no paper/data for sweep length. Can add note that it's a work in progress, and that theres no paper and will be figured out at a later step. May change from requirement to overview because there is no exact guideline. 2.3.4.1 needs to include a tolerance range +- 0.1V +- 10mA. Tolerance Analysis should have formulas added to it, calculations will be good to include in the section. Can do power cause it doesn't matter rubric wise, can spin it to the other subsystems because the power supply ripple will affect other components. Overall, pretty good proposal. Proposal regrade is next Friday.

### Design Document
Next week Thursday. Don't overpromise uncertain deliverables. Must have high-level requirements included. 

### Design Review and PCB Review
In two weeks. Will have to sign up for time slot. 

### Microcontroller
1 MB should be plenty. C/C++ is the main language, but it may have Python support. They have a few free STMs here, but the better versions will have to be purchased. 

# October 01, 2024

## TA Meeting

### Design Document
Can utilize Proposal for certain sections. Switching away from STM for development kit access. Schematic should be used for Supporting Material. 

### Design Review/Peer Review
Pick a peer review timeslot, can ask question. Be somewhat formal for our clothing, but doesn't have to be super strict. 
Modularity is that there needs to be different subsystems. 
Detailed Design is going in depth into the subsystem, why does it work well, why is it needed, why is it good.
Scope is adequetly complex and potentially feasible. 
Each person must answer at least one question, but some people 

# October 02, 2024

## Design Review Work

### Introduction
Largely remaining the same from the proposal document. We plan to switch from STM to ESP microprocessor because it has python. 

### Design
Block Diagram added a few labels/lines to indicate design changes. Added the 1.8V LDO and power rail because we need for our chips. Switched STM to ESP.
Schematics for all subsystems have been created in KiCAD and images added to the design document. These schematics can also be viewed in the models folder (10/4/2024 Update: A few components have been changed due to version conflicts these files have been updated).

#### Measurement Subsystem
Designs were largely chosen to mimic an actual four point probe. For that reason we plan to use 200 mA current source, apparently the higher current will provide better readings. We aren't completely sure on the voltage readings we are going to get from the probe head, however we currently anticipate between 20mV to 100mV readings. We plan to amplify these results in order to ensure that our ADC can properly measure the results.
#### User Input Subsystem
Chose buttons and capacitances to debounce switches for ~1ms.
#### Display Subsystem
Switched to the LCD at the ECE supply store to reduce project cost.
#### Power Regulation Subsystem
For higher efficency a buck was used for the initial 5V to 3.3V voltage stepdown because of its theoretical 100% efficiency. Chose TLV62568PDDCR because of its low cost and it met all specifications of other components requiring a 3.3V power rail. To make sure that the passives chosen would provide the desired output, the converter was simulated in LTSpice and this revealed that we were well within the specifications that the other components required as well as the specification that the buck feedback required. A LDO, TLV74018PDBVR, was chosen for the 3.3V to 1.8V stepdown for its low noise output. This powers the precision components that require a lower noise power supply to accurately measure.

### 

# October 08, 2024

## Design Review
Presented to professor. Feedback includes, costs missing 2.5x multiplier. Debugging time estimation may be low. Add background on what four-point probe is, wafers, doping, etc.

# October 13, 2024
## Layout
Designed the layout from our schematic. Placed components in a compact grouping that would be easy to solder and made the traces as direct as possible. Passed the DRC rules from the PCBWay website and defined the board geometry.

# October 15, 2024

## TA Meeting
Asked for Stencil. Asked for USB-UART bridge. Partner Evaluation due tomorrow. Eshop request form for parts was completed. Some of the pads for our first week PCB may not align with the parts we got from the eshop, we will discuss whether we simply order the parts or if we will adjust the pads on the PCB. 

# October 22, 2024

## TA Meeting
Four point probe arrived last night, haven't had time to brainstorm yet. Apparently the four point probe connector uses shielding and better wiring so we plan to use the connector for most of the connection. There are 5 points connectors, one unknown. PCB arrive this week (hopefully), design progress will resume when that happens. Part order is planned to happen tomorrow, should be easy we will use the eshoop form and the rest we will order from DigiKey. ESP prototyping/learning will happen during the next week.  

# October 27, 2024

## Group Meeting
Began filling out the ordering form for DigiKey and Eshop. Also looked up resources for programming an ESP as none of us are very experienced in that material. We so far have materials on SPI protocol, flashing, GPIO. Began to set up work space in Arduino/VS Code for programming. Jack began an initial test of the voltage measurements from the four-point probe using the lab bench power-supply. More details in the google drive, but so far the voltage levels are not what we anticipated. The voltage readings are higher than what we thought they would be, larger noise levels as well but we aren't sure if that's due to the long power supply/oscilloscope wires. Currently discussing changes to amplifier in order to account for this voltage change. Jack's measurements can be viewed in the seperate pdf folder.

# October 29, 2024

## Group Meeting
We placed the DigiKey and Eshop order requests. Eshop should be within a few days and DigiKey should be early next week. Might remove the amplifier from our design, the ADC is precise enough at the mV range we expect so the amplifier might just add noise. We plan to decide within the next week.

# November 3, 2024

## Updates
Parts arrived from DigiKey, Eshop, and PCB. DigiKey order was out of stock for the power protection diodes, also only have 1 current source by mistake. Eshop sent 0.01 uF capacitors instead of 0.1uF. Will need to order additional resistor/capacitor components from Eshop. PCB did not come with stencil, hand soldering will be done at some point.

# November 5, 2024

## TA Meeting
1 Board has been finished soldering, another currently being soldered. We have decided to tentatively remove the amplifier from our design and just see the voltage readings from the ADC. We can add it back later if we are having issues with voltage levels. Second Eshop form sent with additional part requests. Ordered an LCD display off of amazon, has touch-screen but we don't plan on using that part. Indidual Progress Report due tomorrow.

# November 7, 2024

## Updates
LCD display arrived. Second board has been soldered apart from current source.

# November 12, 2024

## TA Meeting 
Final Demo and Final Presentation sign up opens tomorrow. Mock Demo is next week, informal just need to show up for points. Will hopefully have at least most subsystems somewhat functional at that time.

# November 13, 2024

## Group Meeting 
Signed up for Demo and Presentation. Gave Dorian Dev Board so he can work at home.

# November 14, 2024

## ESP LCD Initial Testing
Began programming the ESP Dev Board. Downloaded and used a few of their examples (blink, hello_world) to familiarize myself with the environment. Also found an example lcd module which does work with our lcd_panel. Will try refactoring the code for our own menu. 

# November 15, 2024

## Group Meeting
Having issues with ESP32 chip. Not sure what the issue is but it doesn't seem to be communicating.

## ESP LCD
Can't get GPIO input to work consistently. Some of the pins can output others don't. Also LCD has a backlight level which our PCB does not include. We can probably just set to 3.3V constant and will function fine. Will discuss with team. Succesfully got half white/half black screen to show up. Pixel uses 16 bit color code. 0x0000 is white and 0xFFFF is black (I think because the colors got inverted).

# November 16, 2024

## Group Meeting 
Can't flash ESP32 for some reason. Troubleshooting, but flash is only successful like 1/100 times. Added generic ASCII printing to the LCD code so I can properly print things. Set up basic 
I/O, turns out the gpio config uses a bitmap rather than an actual number. Have basic screen cycling which goes "first screen." After pressing a button to goes to "next screen."

## 3D Printing
Designed the base to hold the LCD and button pad with clips and by friction fit. Added grooves too so that the back side can just be clipped in. Started print for the base.

# November 17, 2024

## 3D Printing
Designed the back side of the main casing as well as the plate for the wafer to be held. Increased size of the first printed piece. Printed the fixed base and wafer plate.

# November 18, 2024

## 3D Printing
Designed the probe head holder and printed it. Cantalever clip approach confimed to work well for holding the pieces together firmly without extra hardware required.

# November 19, 2024

## Mock Demo
Showed some results, honestly need to work more on the project.

## Group Meeting
Figured out the reason why flash wasn't working, one of the buttons was left floating and we had an extra resistor soldered. We are now able to reliably flash the device by manually setting the GPIO0 and EN pin to ground. It isn't great situation. Break is next week so everyone is gone from 11/22 - 12/1. 

# November 27, 2024

## 3D Printing
Increased the size of the back main casing and reprinted it to allow for more space for connection wires to sit.

# December 1, 2024

## Crunch Time
Turns out LCD display is vertical rather than horizontal, changed the code to reflect that. After some pin adjustments, got the LCD display working using the PCB. Problem is that backlighting needs a 3.3V pin that isn't present on the board. After some thinking, we have decided to repurpose one of our buttons to a dedicated 3.3V gpio slot. We have removed the corresponding capacitor from but_9 in order to isolate the pin. ADC isn't working. Using DevBoard we have found the MOSI, CLK, and CS all work properly. Measured on the PCB we found that CS and MOSI works well, but CLK is only at 1.8V and I can't figure out why. 

# December 2, 2024

## Crunch Time
Our ADC has the wrong voltage inputs. It is currently being set at 1.8V but needs to be 3.3V, we have added jumper wires and removed capacitors to make sure that it's powered correctly. Jumper wire is connected to a NC ESP32 pin which we set to 3.3V using GPIO. Killed the ADC, replaced it. We can read the internal registers, but not the data register. Data register always holds 0 for some reason, might be related to the MCLK pin. Not sure how to setup a CLK on a GPIO pin. We can set the configuration register in order to use the internal MCLK. Displayed on the oscilloscope looks good, still can't read data register though. Shutdown, Standby, and Conversion mode - think it's related to this. Set the ADC to conversion mode but still reading 0 not sure why. Found really old forum post that mentions something about the IRQ pin needing external pullup or high-z mode. Used GPIO to pullup and also set it to high-z mode. There's some setaup time stuff needed before the measurement actually happens. Got it working, turns out the IRQ needs configured before the ADC is put to conversion mode. Now are reading voltage levels on the ADC, looks really good to be honest, just need to get a baseline offset for 0V. Our four-point-probe connections are the most unreliable part of our system, they sometimes disconnect causing poor readings. We have disconnected the fourth pin of our probe, our readings have become more reliable with this change. This has been mathmatically compensated for in our compensation equation. All parts are working seperately when connected to the PCB, we just need to plug everything in tomorrow and fit it into the enclosure.

# December 3, 2024

## Final Demo Preparation
Today is the final demo. We have added the connectors for the LCD (used to be on breadboard) and it still functions. Fit everything into the enclosure. One minor change was that the buttons should've been active low instead of active high so we had to change the code for that. Got everything in the enclosure and it works. Only potential issues that I could see happening is the four-point probe connection potentailly becoming loose, or the poor probe head placement resulting in bad readings. Will update on how the Demo goes later.

## Final Demo
Demo went really well. We were able to answer the questions and I think accurately display the capabilities of our project. Our readings were slightly less accurate than we hoped for, I think just due to variance or the surface we measured on. Will begin prepping for presentation

# December 6, 2024

## Mock Presentation
Our presentation went okay. We agree that we all need more practice and that our slides need more refinement. Our public speaking skills were okay, just need to handle our nervousness.

## Team Contract
Just wrote this kinda quickly. We want to focus on the presentation. Everyone contributed meaningfully to the project, our roles changed as the semester went by. Everyone got along very well with each other.

# December 8, 2024

## Presentation Preparation
We practiced our slideshow a bit, and also adjusted a few of the slides. We are currently running at about 22 minutes for the whole presentation. We are kinda expecting the professor to ask questions in the middle of it so hopefully we don't go over time. 

# December 9, 2024

## Presentation Prep
Doing some practice this morning, just trying to shake out any nerves we might have. We did some last minute adjustments on the slide presentation order. Hopefully it flows better.

## Presentation
Presentation went well we think. We used 29 out of the 30 minutes, so a bit tight on time. Good job this semester proud of our progress.

# December 12, 2024

## Final Notes
Transferring this document to a different part of the folder. We have checkout today and the award ceremony. We won an award which is slightly surprising, I guess our hard work ended up paying off. Overall, good project proud of our work.


