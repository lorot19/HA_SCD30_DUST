# MQTT Sensor for HVAC and Indoor Air Quality Monitoring
This sensor, called SB001, is build from ESP8266, SCD30 and GP2Y1010AU0F sensors.
It is capable to measure CO2, temperature, humidity, dust/smoke.
Thanks to small 2,5cm DC fan reading is very stable without accidental peaks
due to preassure changes.

Measured datas are published to the MQTT broker with this topics:

| Value        | Topic           |
|:-------------:|:-------------|
|Humidity     | "sensor/scd30/1/humidity"|
|Temperature     | "sensor/scd30/1/temperature"|
|CO2 | "sensor/scd30/1/co2"|
|Dust | "sensor/scd30/1/dust"|
|Dust voltage | "sensor/scd30/1/dust_voltage"|
|Fan control | "sensor/scd30/1/fan"|


<img src="/images/back.JPG" alt="drawing" width="49%"/></img>
<img src="/images/front.JPG" alt="drawing" width="49%"/></img>
<img src="/images/airflow.png" alt="drawing" width="100%"/></img>

# Data visualisation - Home Assistant
<img src="/images/hass.png" alt="drawing" width="100%"/></img>
<img src="/images/graph.png" alt="drawing" width="100%"/></img>

# SCD30 SENSIRION
CMOSens® Technology for IR detection enables highly accurate carbon dioxide measurement 
at a competitive price. Along with the NDIR measurement technology for CO2 detection, 
a best-in-class Sensirion humidity and temperature sensor is also integrated on the same 
sensor module. Ambient humidity and temperature can be outputted by Sensirion’s algorithm 
expertise through modeling and compensating of external heat sources without the requirement 
for any additional components.
<br><br>
More informations: https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors-co2/
<br><br>
<img src="/images/scd30.png" alt="drawing" width="40%"/></img>

# GP2Y1010AU0F SHARP
GP2Y1010AU0F is a dust sensor by optical sensing system.
An infrared emitting diode (IRED) and an phototran- sistor are diagonally arranged into this device.
It detects the reflected light of dust in air.
Especially, it is effective to detect very fine particle like the cigarette smoke.
In addition it can distinguish smoke from house dust by pulse pattern of output voltage.
<br><br>
More informations: https://www.sparkfun.com/datasheets/Sensors/gp2y1010au_e.pdf
<br><br>
<img src="/images/sharp.png" alt="drawing" width="60%"/></img>
<img src="/images/sharp2.png" alt="drawing" width="60%"/></img>

<img src="/images/block_diagram.png" alt="drawing" width="100%"/></img>

# Schematics
Schematics is created in EAGLE and located in SB001 folder
<img src="/images/schematics.png" alt="drawing" width="100%"/></img>

# PCB Layout
Layout is created in EAGLE and located in SB001 folder
<img src="/images/pcb.png" alt="drawing" width="60%"/></img>

# Licence
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

Some parts of WiFi and MQTT stuff are copied from PubSubClient.h library
example distribued under MIT license
Source: https://github.com/knolleary/pubsubclient

Schematics and PCB layout are distributed in the hope that they will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
