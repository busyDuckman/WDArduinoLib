#include "CrossCompile.h"

//-------------------------------------------------------------------------------------------------
// Mocking Serial ports
//-------------------------------------------------------------------------------------------------
#ifdef __NOT_AN_ARDUINO_BUILD__
HardwareSerial Serial;
#endif



