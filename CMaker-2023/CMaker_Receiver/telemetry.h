// telemetry packet identifier: "CM" for Cache Makers
#define CACHE_MAKERS_ID 0x434D

// Balloon Payload Telemetry (56 bytes)

// High Altitude Balloon Project 2023
// Cache Makers and Bridgerland Amateur Radio Club

float checksum;
int packetCount = 0;

// NOTE: When using the TTGO board, we receive 4 bytes of preamble or padding.
// We should research further but for now, we skip the first 4 bytes.
struct Telemetry {
  long preamble; // see note above
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
