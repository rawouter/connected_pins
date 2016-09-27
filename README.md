# Connected pins

Connected pins, an internet of things project.
This project control a NeoPixel led using ESP2866 Huzzah connected to a remote 3rd party websocket, the websocket simply send the RGB value of the led, that will in turn blink once with the value read.

## Usage

The current code only read the color from the Serial port.
Just type any color in the form of a char, in: r, g, b, y, c, m, w

## Hardware connection

![Connect led to pin 12](https://github.com/rawouter/connected_pins/blob/master/schema/wiring.png)

