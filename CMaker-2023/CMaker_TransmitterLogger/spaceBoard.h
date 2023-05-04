//
// spaceBoard.h
//
#include <ArdusatSDK.h>

TemperatureTMP temp;
Luminosity lum;
UVLightML uv;
TemperatureMLX ir;
Acceleration acc;
Gyro gyro;
Magnetic mag;

void initSpaceBoard() {
  Serial.println("Initializing space board sensors...");
  temp.begin();
  lum.begin();
  uv.begin();
  ir.begin();
  acc.begin();
  gyro.begin();
  mag.begin();
}

void showTemp() {
  temp.read();
  Serial.print("temperature: ");
  Serial.print(temp.t*9/5+32);
  Serial.println(" ºF");
}

void showLum() {
  lum.read();
  Serial.print("luminosity: ");
  Serial.print(lum.lux);
  Serial.println(" lux");
}

void showUV() {
  uv.read();
  Serial.print("UV Index: ");
  Serial.print(uv.uvindex);
  Serial.println(" mW/cm2");
}

void showIR() {
  ir.read();
  Serial.print("IR temperature: ");
  Serial.print(ir.t*9/5+32);
  Serial.println(" ºF");
}

void showAcc() {
  acc.read();
  Serial.print("acceleration: ");
  Serial.print(acc.x);
  Serial.print(", ");
  Serial.print(acc.y);
  Serial.print(", ");
  Serial.print(acc.z);
  Serial.println(" m/s2");
}

void showGyro() {
  gyro.read();
  Serial.print("gyroscope: ");
  Serial.print(gyro.x);
  Serial.print(", ");
  Serial.print(gyro.y);
  Serial.print(", ");
  Serial.print(gyro.z);
  Serial.println(" rad/s2");  
}

void showMag() {
  mag.read();
  Serial.print("Magnetometer: ");
  Serial.print(mag.x);
  Serial.print(", ");
  Serial.print(mag.y);
  Serial.print(", ");
  Serial.print(mag.z);
  Serial.println(" μT");

}

void testSpaceBoard() {
//  Serial.println(mag.readToJSON("mag"));
  showTemp();
  showLum();
  showUV();
  showIR();
  showAcc();
  showGyro();
  showMag();
}

showSensorUnits() {
  Serial.println(temp.readToJSON("temp"));
  Serial.println( lum.readToJSON("lum "));
  Serial.println(  uv.readToJSON("uv  "));
  Serial.println(  ir.readToJSON("ir  "));
  Serial.println( acc.readToJSON("acc "));
  Serial.println( gyro.readToJSON("gyro"));
  Serial.println( mag.readToJSON("mag "));
  /*
  Sensor Units
    Temp  ºC      degrees Celsius
    Lux   lux     luminous flux
    UV    mW/cm2  milliwatts per square centimeter
    IR    ºC      degrees Celsius
    Acc   m/s2    meters per second squared
    Gyr   rad/s   radians per second
    Mag   μT      micro Teslas
  */
}

void readSensors() {
  // read all spaceboard sensors and run check for flightMode (pre-launch, launch, descent, landed)
  temp.read();
  lum.read();
  uv.read();
  ir.read();
  acc.read();
  gyro.read();
  mag.read();
  
}
