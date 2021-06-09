// Basic demo for readings from Adafruit SCD30
#include <Adafruit_SCD30.h>

Adafruit_SCD30  scd30;

double temperatureInC = 0;
double relativeHumidity = 0;
double co2ppm = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit SCD30 test!");

  // Try to initialize!
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");


  // if (!scd30.setMeasurementInterval(10)){
  //   Serial.println("Failed to set measurement interval");
  //   while(1){ delay(10);}
  // }
  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");

  Particle.variable("temperature", &temperatureInC, DOUBLE);
  Particle.variable("humidity", &relativeHumidity, DOUBLE);
  Particle.variable("co2", &co2ppm, DOUBLE);
}

void loop() {
  if (scd30.dataReady()){
    //Turn off  the led
    RGB.control(true);
    RGB.brightness(0);

    Serial.println("Data available!");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

    temperatureInC = scd30.temperature;
    relativeHumidity = scd30.relative_humidity;
    co2ppm = scd30.CO2;

    Serial.print("Temperature: ");
    Serial.print(temperatureInC);
    Serial.println(" degrees C");
    
    Serial.print("Relative Humidity: ");
    Serial.print(relativeHumidity);
    Serial.println(" %");
    
    Serial.print("CO2: ");
    Serial.print(co2ppm, 2);
    Serial.println(" ppm");
    Serial.println("");

    if (co2ppm == 0) {
      //Still initialing the sensor, dropping the data
      delay(1000);
    } else {
      String data = String::format(
        "{"
          "\"temperatureInC\":%.2f,"
          "\"humidityPercentage\":%.2f,"
          "\"co2ppm\":%.2f"
        "}",
        temperatureInC,
        relativeHumidity,
        co2ppm
      );
      Particle.publish("Sensor", data, 60, PRIVATE);
      delay(60*1000);
    }
  } else {
    Serial.println("No data");
    // Particle.publish("Log", "No data");
    RGB.color(255, 0, 0);
    RGB.brightness(255);
    delay(100);
  }
}
