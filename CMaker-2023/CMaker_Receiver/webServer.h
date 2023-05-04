//
//  Web server should be setup assigning subroutines to handle request events.
//  Access is provided to the primary data struct 'telemetry' in telemetry.h
//  comments provided in groundStation.ino for locations to initialize and update web data.

// <define class instances for components here> ...

bool initWebServer() {
  // <initialize web server components here> ...


  return true;
}

// update web page parameters or referenced variables
void updateWebserverData() {
  
  // update web pages here if needed ...
  // could memcopy from telemetry struct to a copy here to avoid threading issues.
  // and have web requests dynamically constructed using the copy.
  
}
