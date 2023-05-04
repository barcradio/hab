
// telemetry packet identifier: "CM" for Cache Makers
#define CACHE_MAKERS_ID 0x434D

// Data to be transmitted... struct designed to avoid padding,
// but care should be taken if receiving on a big endian cpu.
// 4 bytes for header: ID (2) + packet type (1) + payload status (1).
// 4 bytes per float * 7 values + checksum value = 32 bytes
// Total: 52 bytes
struct Telemetry {
  uint16_t id = CACHE_MAKERS_ID;
  uint8_t packetType = 0x01;
  uint8_t payloadStatus;
  float gpslat, gpslon, gpsalt, gpsspeed;
  float temperature;
  float luminosity;
  float uvlight;
  float vBatt;
  float tempBatt;
  float tempInt;
  float tempExt;
  float checksum;
} telemetry;



/* Add up all the values in the Telemetry data structure to get a "checksum" value
   that can later be compared with the checksum value inside the telemetry struct. If the
   checksum values are equal, then we have a high level of confidence that the data
   we received over the radio reflects accurate sensor readings.
*/
float calculateTelemetryChecksum(Telemetry* telemetry) {
  return
    telemetry->gpslat +
    telemetry->gpslon +
    telemetry->gpsalt +
    telemetry->gpsspeed +
    telemetry->temperature +
    telemetry->luminosity +
    telemetry->uvlight +
    telemetry->vBatt +
    telemetry->tempBatt +
    telemetry->tempInt +
    telemetry->tempExt;
}

void debugTelemetryStruct() {
  char buf[80];
  Serial.println("----TELEMETRY STRUCT----");
  snprintf(buf, sizeof(buf), "sizeof(Telemetry) = %ld", sizeof(Telemetry));
  Serial.println(buf);
  snprintf(buf, sizeof(buf), "%ld - telemetry", &telemetry);
  Serial.println(buf);
//  snprintf(buf, sizeof(buf), "%ld -     header", &telemetry.header);
//  Serial.println(buf);
  snprintf(buf, sizeof(buf), "%ld -     id", &telemetry.id);
  Serial.println(buf);
  snprintf(buf, sizeof(buf), "%ld -     packetType", &telemetry.packetType);
  Serial.println(buf);
  snprintf(buf, sizeof(buf), "%ld -     payloadStatus", &telemetry.payloadStatus);
  Serial.println(buf);
}
