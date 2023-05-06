# HAB Pre-flight Checklist for ESP32 CAM Modules

The electronics and programming team has included three extra 2MP cameras in the payload to capture additional footage during the flight.
 - A 75 degree field-of-view towards the horizon.
 - A 180 degree field-of-view skyward facing
 - A 200 degree field-of-view ground facing

### Step 1: Enable Mobile HotSpot
Instruct mentor Russ (KE7VFI) to power on the mobile hotspot.  This must be running when the cameras are powered to synchronize the date/time on each camera.  This will provide the correct date encoding for the video files.
 - If this is missed, then it can be fixed in a later step.

### Step 2: Remove camera protection and lens caps
Remove the blue tape covering each camera module.

### Step 3: Power up camera modules via main system board
The camera modules will have a solid red LED visible inside the payload.
  - [ ] Verify CMakerCamSky is powered
  - [ ] Verify CMakerCamHrz is powered
  - [ ] Verify CMakerCamGnd is powered
 
 *If the red led is blinking SOS in Morse code then a fatal error has ocurred, report this to a mentor.

```
  S   O   S
 ... --- ...
```

### Step 4: Verify Camera Access Points are running
Using mobile computers or phones, verify that the following WiFi access points can be found.

```
AP: CMakerCamSky_C4
PW: 16C4jz60

AP: CMakerCamGnd_7C
PW: 167Cjz60

AP: CMakerCamHrz_84
PW: 6384jz60
```

### Step 5: Connect to camera WiFi and open webserver
Connect to each Access Point and browse to:
>http://192.168.4.1

A web page should open with a header like this example:
```
CMakerCam
ESP32-CAM Video Recorder Junior v60.4.7
Fri May 5 23:01:50 2023

Used / Total SD Space 7903 MB / 29568 MB, Rssi 0
Filename: /CMakerCam47.001.avi
Framesize 13, Quality 10, Frame 219
Record Interval 100ms, Stream Interval 0ms
Avg framesize 170722, fps 9.7
Time left in current video 1777 seconds
```

### Step 6: Verify the date/time
Verify that the date/time listed in the header is correct.
- If the time is incorrect, press the link labeled: **Send time to camera (in AP Mode)**
 - Press back in the browser to return to the webserver page.

### Step 7: Verify image stream is running
On the web page that opens click the button labled **Stream 81**.  This will start a live video stream in the browser.

### Step 8: Verify image focus
The lens adjustment rings have been glued into position and should not be able to adjusted.

If the image looks blurry and out of focus, report this to a mentor.

### Step 9: Go for flight
If no issues are found, the camera modules are recording and ready for flight.

## Notes:
The camera flashes and piezo speaker are intended to be emitting flashes and sound anytime the payload is below 6000ft.  This is a test to see if this will aid visibility of the vehicle during launch, landing, and recovery.