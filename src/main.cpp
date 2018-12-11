/*
 * This sketch is for smart air sensor SB001 designed by Tomas Lorinc
 * Sensor board is composed from SCD30(CO2, Temperature, Humidity) 
 * and GP2Y1010AU0F (Dust, Smoke) sensors. 
 * For more information, schematics and PCB layout visit:
 * https://github.com/lorot19/HA_SCD30_DUST
 *
 * Author: Tomas Lorinc, 2018
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Some parts of WiFi and MQTT stuff are copied from PubSubClient.h library
 * example distribued under MIT license
 * Source: https://github.com/knolleary/pubsubclient
 * 
 */


#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <SparkFun_SCD30_Arduino_Library.h>

/*
* Network related settings
*/
#define wifi_ssid "your_ssid"
#define wifi_password "your_password"
#define mqtt_server "192.168.1.106"

#define humidity_topic "sensor/scd30/1/humidity"
#define temperature_topic "sensor/scd30/1/temperature"
#define co2_topic "sensor/scd30/1/co2"
#define in_topic "sensor/scd30/1/fan"
#define dust_topic "sensor/scd30/1/dust"
#define dust_voltage_topic "sensor/scd30/1/dust_voltage"
#define NO_DUST_VOLTAGE 0 //No dust voltage is used for more accurate measurment of dust
#define measure_interval 2 //Measure interval in seconds


/*
* Hardware configuration
*/
#define FAN_EN D7
#define DUST_LED D5
#define DUST_S A0

long lastMsg = 0;
float dust_voltage = 0;


/*
* Function prototypes
*/
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
bool checkBound(float newValue, float prevValue, float maxDiff);
float getDust();

WiFiClient espClient;
PubSubClient client(espClient);
SCD30 airSensor;



/*
 * Setup loop
 */
void setup() {
  Serial.begin(115200);
  pinMode(FAN_EN, OUTPUT); 
  pinMode(DUST_LED,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Wire.begin(4,5);
  delay(500);
  airSensor.begin();
}

/*
 * Main loop
 * used to keep device connected and to trigger measurement every <measure_interval>
 */
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  

  long now = millis();
  if (now - lastMsg > measure_interval*1000) {
    
    lastMsg = now;
    if (airSensor.dataAvailable())
    {
      //getting datas
      float temp = airSensor.getTemperature();
      float humidity = airSensor.getHumidity();
      float co2 = airSensor.getCO2();
      float dust = getDust();
      //publishing datas
      client.publish(temperature_topic, String(temp).c_str(), true);
      client.publish(humidity_topic, String(humidity).c_str(), true);
      client.publish(co2_topic, String(co2).c_str(), true);
      client.publish(dust_topic, String(getDust()).c_str(), true);
      client.publish(dust_voltage_topic, String(dust_voltage).c_str(), true);
      //printing datas
      Serial.printf("%ld \t CO2: %.3f\t TEMPERATURE: %.3f\t HUMIDITY: %.3f\t DUST: %.3f\t DUST_VOLTAGE: %.3f\n",millis(),co2,temp,humidity,dust, dust_voltage);
    }
  }
}


/*
 * Wifi setup function
 * connects device to the selected network and inform client about status
 */
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 * Callback function
 * executed when subsribed msg arrives from MQTT broker
 * used to turn on and off dc fan
 */
void callback(char* topic, byte* payload, unsigned int length) {
  String data = "";
  for (uint i = 0; i < length; i++) {
    data += (char)payload[i];
  }
  if (data == "ON") {
    digitalWrite(FAN_EN, HIGH);
    Serial.println("FAN ENABLED");
  } 
  else if (data == "OFF") {
    digitalWrite(FAN_EN, LOW); 
    Serial.println("FAN DISABLED");
  }

}

/*
 * Reconnect function
 * connects device to the selected network and inform client about status
 */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("\nCONNECTED");
      client.subscribe(in_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


/*
* Dust measure function
* This function measures analog value on DUST sensor. 
* Measured value is not calibrated and should be improved in future.
*/

float getDust()
{
  float voltsMeasured = 0;
  float calcVoltage = 0;
  float dustDensity = 0;
  
  digitalWrite(DUST_LED,LOW); 
  delayMicroseconds(15);
  voltsMeasured = analogRead(DUST_S); 
  delayMicroseconds(10);
  digitalWrite(DUST_LED,HIGH);
  delayMicroseconds(9680);
  calcVoltage = voltsMeasured * (3.3 / 1024.0);
  dust_voltage = calcVoltage;
  if(calcVoltage >= NO_DUST_VOLTAGE)
  {
    calcVoltage -= NO_DUST_VOLTAGE;
    dustDensity = 0.2 * calcVoltage;
  }
  else
    dustDensity = 0;
  return dustDensity;
}




