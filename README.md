<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/LOGO.png" width="80%">
</p>

Overview
===============

A weather station based on Atmel ATmega328P microcontroller

Raw data is sent to base station via 2.4GHz RF transceiver, then it is sent to a server where the raw data gets processed and published 


Sensors
--------------

* [Telecontrolli](https://github.com/Telecontrolli) capacitive rain sensor
* Gas sensor
* [BoschSensortec](https://github.com/boschsensortec) Humidity, temperature and barometric pressure sensor
* Wind speed (IR RPM sensor)
* Wind direction (Reed-switch w/ voltage divider)
* Light sensor using intrumentation amplifier and Wheatstone bridge

More sensors to be used in future

Main board
--------------

<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/mainboard.jpg" width="80%">
</p>

Features:

* ATmega328P microcontroller
* AMS1117 3.3V and 5V voltage regulator
* 74HC4051 analog multiplexer
* INA126P instrumentation amplifier and Wheatstone bridge
* 16MHz external oscillator 

Main sensor board
--------------

<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/sensorboard.jpg" width="80%">
</p>

Features:

* BME280 humidity, temperature and barometric pressure sensor
* HS-135 gas sensor
