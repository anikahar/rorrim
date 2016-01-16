# Rorrim
Arduino code for the Rorrim interactive sculpture

Rorrim is an interactive sculpture installation initially comissioned for the 'Makers: Real Time' exhibition, a part of the '24/7' exhibitions at the Hertzliya Musem Of Contemporary Art.
The two sided sculpture lets visitors interact with it using hand gestures which cause changes in the lighting and sounds coming out of the object. When two people on both sides of the sculpture come into movement synchronization, the sculpture puts on a little 'show' we call the Orgasm. 

# Hardware setup
Interaction code for the sculpture was coded on OpenFrameworks using ofxKinect which outputs MIDI that is routed to both Ableton Live for sounds and to two Arduino Dues for controlling the LEDs. The code in this repository is the exact code running on both microcontrollers.
The reason for using more than one microcontroller is the ability to update the amount of individually addressable LEDs involved in the project - more than 2300 LEDs / pixels. One DUE is able to output only about 16 FPS while maintainig high responsivity to MIDI.

Original project participators:
Matan Berkowitz
Liron Tsabari
Zohar Levi
Roi Stephansky

All code in this repository by HAR

See included license in LICENSE.md

Feel free to open an issue or contact me directly if you have any questions or comments
