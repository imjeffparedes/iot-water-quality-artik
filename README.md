# Water Quality Monitoring Using MKR1000 and ARTIK Cloud

Arduino code for use in MKR1000 and Samsung ARTIK Cloud to monitor the pH and temperature levels of swimming pools.
* Version v1.0.0 | By Jeff Paredes

## Getting Started

These instructions will show you how to use the code

### Prerequisite Libraries

FlashStorage - used to save the offset of pH calibration
ArduinoThread - used it to read sensors in a separate thread.
ArduinoJson - we'll be using this to send JSON data to ARTIK Cloud
WiFi101 - used to enable wifi connection with mkr1000
ArduinoHttpClient - host for connecting to API
OneWire - needed to read digital input from Temperature sensor
DallasTemperature - Dallas Temperature sensor required library


### Installing

Please follow these step by step series of examples that tell you have to use this codes

1. Install prerequisite libraries

2. Add the attached **HexFont** folder to your Arduino libraries.

3. Open **simple_weather_display.ino** with your Arduino IDE

4. Change your own ARTIK Cloud device id and token.
```
String deviceID = "artik cloud device id"; // put your device id here created from tutorial
String deviceToken = "artik cloud device token"; // put your device token here created from tutorial
```
5. Change your own wifi ssid/name and password.

```
#define WIFI_AP "your wifi ssid"
#define WIFI_PWD "wifi password"
```

6. Then Upload the Software Code to MKR1000 and start monitoring.

## Deployment

Flash Your MKR1000 using the code

Please visit [Hardware Instruction Page](https://www.instructables.com/id/Water-Quality-Monitoring-Using-MKR1000-and-ARTIK-C/) on Instructables.com

## Built With

* [Arduino IDE](https://www.arduino.cc/en/Main/Software) - Programming IDE Used

## Contributing

Please read [CONTRIBUTING.md](https://github.com/imjeffparedes/) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Jefferson Paredes** - *Developer* - [imjeffparedes](https://github.com/imjeffparedes/)

## License

This project is licensed under the Attribution-NonCommercial-ShareAlike 2.5 License - see the [LICENSE.md](https://github.com/imjeffparedes/) file for details

## Acknowledgments

* Publishing Page - [Instructables](https://www.instructables.com/id/Water-Quality-Monitoring-Using-MKR1000-and-ARTIK-C/)
* Sponsors - [Hackster.io](https://hackster.io)
