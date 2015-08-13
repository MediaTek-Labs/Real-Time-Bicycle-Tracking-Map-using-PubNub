#include <LFlash.h> // Verify the lib path
#include <LSD.h> // Verify the lib path
#include <LStorage.h> // Verify the lib path



#include "GPSWaypoint.h"


static unsigned char getComma(unsigned char num, const char *str);
static double getDoubleNumber(const char *s);
static int getIntNumber(const char *s);  //see getDoubleNumber - this is same, but uses INT not DOUBLE
static float getFloatNumber(const char *s);
static char* getCharString(const char *s);

void parseGPGGA(const char* GPGGAstr, GPSWaypoint* wayPoint);
boolean printGPGGA(char* str, char* GPS_formatted, GPSWaypoint* wayPoint);
const char *nextToken(char* src, char* buf);
void getGPSData(gpsSentenceInfoStruct &g_info, char* GPS_formatted, GPSWaypoint* positionData);
//void convertCoords(const char* latitude, const char* longitude, char* lat_return, char* lon_return, int buff_length);
void convertCoords(float latitude, float longitude, const char* n_or_s, const char* e_or_w, float &lat_return, float &lon_return);
int arrayToInt(const char* char_array);
float arrayToFloat(const char* char_array);
void readSpeed(char* str, GPSWaypoint* wayPoint);
