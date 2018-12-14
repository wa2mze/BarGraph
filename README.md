# BarGraph
atmega16 driving a GL-112T9
The GL-112T9 bargraph is a 12 segment LED bar graph with 4 Green, 4 Yellow, and 4 Red segments.
Four of the LEDs are discrete having their own anode and cathode pins (1st and 4th green, 1st and 4th red).
The other eight LEDs are pairs of series connected diodes with a single cathode pin, a pin for the shared cathode and anode,
and an anode pin. (The four YELLOW LEDs are wired as two series connected pairs).
The LED's are driven by the MicroController with negative logic, IE: the anodes are connected to +5v and the cathodes are 
driven low though a current limiting resistor.
To drive the series connected LED's the common cathode-anode pin is driven low though a limiting resistor and a diode with the
anode connected to the LED.  When driving BOTH segements of a series connected LED's, the common (cathode/anode) pin is NOT
driven and is left floating.  The PORTS are configured as outputs, with no pullups (open drain).
330 ohm limiting resistors are used for the single GREEN LEDS, 150 ohm resistors and a 1N4148 diode are used for the
series GREEN and YELLOW LEDs.  510 ohm resistors are used for the single RED LEDs, and 220 ohm resistors and a 1N4148 diode are
are used for the series RED LED's.  (The RED LEDs have a lower forward drop, so they required higher value resistors to give
the same brightness).

Analog channel 0 is read and the result is divided into 13 levels to display on the bar graph.  The first level
is (of course) zero, or less than the first level, no LEDs are lit.

Untested code is provided to display 25 levels by blinking the MS bar when showing the ODD numbered levels. This option is
commented out (#DEFINE DOUBLE_STEPS).

I have not looked at scaling the output to show in LOG format, it's just linear from 0-5v right now.
