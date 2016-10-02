# Connected pins

Connected pins, is an IOT project.
This project control an Adafruit NeoPixel led  using ESP8266 Huzzah connected to a remote 3rd party websocket, the websocket simply send the RGB value of the led, that will in turn blink once with the value read.

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

Install the [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library, see project README.

Install the [JSON](https://github.com/bblanchon/ArduinoJson/wiki/Using%20the%20library%20with%20Arduino) library.

For Bluetooth LE, install the [BLE](https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/installing-ble-library) library.

## Tested hardware listing:

- [Huzzah Feather](http://www.exp-tech.de/en/adafruit-feather-huzzah-with-esp8266-wifi?___from_store=de) for WIFI mode
- [Feather M0 BLE](https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/pinouts?view=all) for Bluetooth LE mode with the [Bluefruit iOS app](https://learn.adafruit.com/bluefruit-le-connect-for-ios/library-and-config?view=all) in MQTT bridge mode
- [Lipo 1000mAh](http://www.exp-tech.de/en/polymer-lithium-ion-battery-1000mah)
- [Adafruit NeoPixel](http://www.exp-tech.de/adafruit-flora-rgb-smart-neo-pixel-version-2-pack-of-4)

## Battery life

First results without optimisation indicates that the 1000mAh battery should last at least 10 hours (but for sure not more than 15h).
Given the above result, the sketch will probably not try to put the board to sleep.

## Hardware connection

Example for the mentionned hardware above (Huzzah Feather + Flora NeoPixel v2):

Connect the led UP ARROW port to Huzzah Feather PIN 12.
  - Note: There are two _arrows pins_ on the led PCB, do not use the arrow going form the led to the pin, but the one from the pin to the led.
  - Note: You can use any other GPIO ports on the board, but don't forget to change the PIN number in the sketch

Connect the led (-) to Huzzah GND.
  - Note: There is only one GND port on my board, if you want to connect more hardware to the board you may want to not solder the wire directly.

Connect the led (+) to Huzzah VCC (see below note for Huzzah Feather)
  - Note: connecting to the 3.3V regulator works but the blue led will be very dimm, giving fake colors, and less overall brightness
  - Note: On the Huzzah Feather there is a specific port giving the LIPO direct power named BAT, this is the one you should use for mobile led pins project.
  - As written above, if you use your other esp8266 with your own power regulator, the direct power from a 1S LIPO gives good results with the Adaftruit Neopixel

![Connect led to pin 12](https://github.com/rawouter/connected_pins/blob/master/schema/wiring.png)

# MQTT bridge over bluetooth LE

curl -H "X-AIO-Key: myadafruitsecretkey" -H 'Content-Type: application/json' -X POST --data '{"value": {"color":[111,252,0]} }' "https://io.adafruit.com/api/feeds/borg-event-json/data.json"


