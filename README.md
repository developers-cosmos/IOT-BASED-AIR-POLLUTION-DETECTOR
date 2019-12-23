# IOT-BASED-AIR-POLLUTION-DETECTOR
Air pollution is a common problem nowadays. Different particles in the air like led, carbon dioxide, sulfur dioxide, pollen, and mold spores are making so much air pollution. Air pollution can bring lots of diseases.  It is essential today to use a mechanism to measure air pollution in an area. Research on IOT projects brings a solution. Newly discovered IOT devices can monitor air pollution and save data to the web servers.

## AIM:
This device is intended to provide the user with a cost-efficient means of determining air quality. Our sensor focuses on the five components of the Environmental Protection Agency's Air Quality Index: ozone, particulate matter, carbon monoxide, sulfur dioxide, and nitrous oxide. This device detects all of these pollutants except sulfur dioxide. The device also includes a town gas sensor to alert the user to gas leaks or the presence of flammable gases. Furthermore, a temperature and humidity sensor is included as these conditions can impact the performance of the gas sensors.

We have yet to fully calibrate our device, but we have extracted data from sensor data sheets to make some preliminary estimations. The sensors used are relatively inexpensive and vary greatly from component to component so they need to be calibrated with known concentrations of the target gasses. We have not yet had the opportunity do so.

# MATERIALS:

## Control and Power
Arduino Uno
5V power supply
RGB 16x2 LCD shield
## Sensors
Shinyei PPD42 Particulate Matter Detector
MQ-2 Gas Sensor
MQ-9 Gas Sensor
MiCS-2714 Gas Sensor (NO2)
MiSC-2614 Gas Sensor (Ozone)
Keyes DHT11 Temperature and Humidity Sensor

## Box and Assembly
Access to 3D printer
Solder Board
5V fan
10 to 15 wires of gauge 24

## CIRCUIT
![CIRCUIT DIAGRAM](https://cdn.instructables.com/FJF/4TV1/I9OSX6JD/FJF4TV1I9OSX6JD.LARGE.jpg?auto=webp&fit=bounds)






















## Interpreting the Data
We are in the process of determining how to transform raw sensor values into meaningful outputs. Calibration against known pollution sources will eventually be necessary to ensure accuracy. In meantime we have used sensor data sheets and prior research to make approximations.

To estimate particulate matter concentrations we use information from a research paper by David Holstius. The paper correlates the Shinyei PPD42 dust sensor outputs with EPA measurements. The graphs in the appendix show best fit lines for the data. We used the graphs to approximate PM2.5 concentration in micrograms per cubic meter as:

PM2.5 = 5 + 5 * (small PM low-pulse occupancy percentage)

To estimate gas concentrations from MiCS gas sensors, we use the graphs in the datasheets (NO2 and O3) to extract functions relating sensor resistance to gas concentration.

For MQ sensors we use the graphs on the device datasheets to qualitatively assess the data. When the resistance value drops to less than half of the resistance in air, it is likely that the devise is detecting the target gasses. When resistance drops by a factor of 10, the levels of target gas are likely around 1000 ppm, close to the legal safety limit.

Once we obtain approximate concentrations of the target gasses, we defer to US government standards to interpret the data. We primarily use the EPA Technical Assistance Document for the Reporting of Daily Air Quality and a CDC information sheet on the hazards of propane.

Unfortunately our code that interprets the raw data is not yet fully functional. We hope to be able upload it at a later date.



