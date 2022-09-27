# Arduino-Nano-33-IoT-WiFi-Webserver-XML-Communication-Example-Share
It's a WIP project. I'm sharing this hoping it can help others to build their WiFi remote control more easily.

1. Set SSID and password in arduino_secrets.h, or set the WiFi hotspot SSID and password to "Arduino" & "12345678".
2. When connected, the onboard LED (D13 yellow) will light up
3. Check the IP and type in the IP in your browser.
4. The home page:
  Blink: LED goes off for 1 second
  Test: retrieve time from arduino with "requestXMLtoID" function
  arduino run time is update every 333ms by "setInterval" function
  Calibration: go to calibration page
5. The calibration page:
  WIP, just ignore it.

That's it. I don't have time to make it pretty ;D
