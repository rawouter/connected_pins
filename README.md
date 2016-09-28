# Connected pins

Connected pins, is an IOT project.
This project control a NeoPixel led using ESP2866 Huzzah connected to a remote 3rd party websocket, the websocket simply send the RGB value of the led, that will in turn blink once with the value read.

## Usage

Install the dependencies (see below).

Edit the `connected_pins.ino` project and setup the configuration constants accordingly, you should look at:

```
#define SSID "yourssid"
#define SSIDPASS "yourssidpassword"
#define SSOCKETURL "your websocket URL"

#define FREQ 1             // Number of blink(s) during BLINK_TIME
#define BLINK_TIME 2       // Total blinking time
#define MAXBRIGHTNESS 255  // 0 to 255, overall brigthness of the leds
#define PIN 12             // Pin number of the NeoPixel bus
#define NUM_PIX 1          // Number of NoePixel leds
```

Note: the websocket connection will by default be on port 443, over SSL.

The websocket must send the following JSON messages:
```
{"color": [<r>,<g>,<b>]}
```
Where r,g and b are integer between 0 and 255.

## Dependencies

Install the [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets) library, download zip and use the arduino IDE `Sketch -> Include Library -> Add .ZIP Library`.
Note: I renamed the installed directory in '~/Documents/Arduino/Libraries' to remove the -master, not sure this is needed.

Install the [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel), see project README.

## Hardware connection

Connect the led UP ARROW port to Huzzah PIN 12.
  - Note: There are two _arrows pins_ on the led PCB, do not use the arrow going form the led to the pin, but the one from the pin to the led)
  - Note: You can use any other GPIO ports on the board, but don't forget to change the PIN number in the sketch

Connect the led (-) to Huzzah GND.
  - Note: There is only one GND port on my board, if you want to connect more hardware to the board you may want to not solder the wire directly.

Connect the led (+) to Huzzah VCC (see below note for Huzzah Feather)
  - Note: connecting to the 3.3V regulator works but the blue led will be very dimm, giving fake colors, and less overall brightness
  - Note: On the Huzzah Feather there is a specific port giving the LIPO direct power named BAT, this is the one you should use for mobile led pins project.
  - As written above, if you use your other esp2866 with your own power regulator, the direct power from a 1S LIPO gives good results with the Adaftruit Neopixel

![Connect led to pin 12](https://github.com/rawouter/connected_pins/blob/master/schema/wiring.png)

