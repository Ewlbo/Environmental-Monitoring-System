<p align="center" style="text-align: center;">
<a href="https://ewlbo.github.io/Environmental-Monitoring-System/Website/index.html" target="_blank" ><img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/LOGO.png" width="80%"></a>
</p>

Overview
===============

A weather station based on Atmel ATmega328P microcontroller

Raw data is sent to base station via 2.4GHz RF transceiver, then it is sent to a server where the raw data gets processed and published 

[Go to website](https://ewlbo.github.io/Environmental-Monitoring-System/Website/index.html)

Release date 01.06.2017

Sensors
--------------

* [Telecontrolli](https://github.com/Telecontrolli) capacitive rain sensor
* Universal gas sensor
* [Bosch Sensortec](https://github.com/boschsensortec) Humidity, temperature and barometric pressure sensor
* Wind speed (IR RPM sensor)
* Wind direction (Reed-switch w/ voltage divider)
* Light sensor using intrumentation amplifier and Wheatstone bridge

More sensors to be used in future


Concept
--------------

<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/concept.jpg" width="40%">
</p>

Main board
--------------

<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/mainboard.jpg" width="50%">
</p>

Features:

* ATmega328P microcontroller
* AMS1117 3.3V and 5V voltage regulator
* 74HC4051 analog multiplexer
* INA126P instrumentation amplifier and Wheatstone bridge
* 16MHz external oscillator 

RF board
--------------



Features:

* [Nordic Semiconductor](http://www.nordicsemi.com/) nRF24L01+ 2.4GHz RF module
* For testing period PCB trace antenna is used, will be switched to external antenna and different frequency for greater range and less interference in 2.4GHz band


Main sensor board
--------------

<p align="center" style="text-align: center;">
<img src="https://github.com/Ewlbo/Environmental-Monitoring-System/blob/master/Images/sensorboard.jpg" width="40%">
</p>

Features:

* BME280 humidity, temperature and barometric pressure sensor
* HS-135 gas sensor

Wind direction
--------------



Features:

* Reed switch and voltage divider
* Determine wind direction depending on output voltage 

Wind speed
--------------



Features:

* IR emitter and IR transistor


