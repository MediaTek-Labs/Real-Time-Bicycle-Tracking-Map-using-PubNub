#include <LStorage\LFlash.h> // Verify the lib path
#include <LStorage\LSD.h> // Verify the lib path
#include <LStorage\LStorage.h> //Verify the lib path

#include <LGPS.h>
#include "GPS_functions.h"
#include "GPSWaypoint.h"



gpsSentenceInfoStruct g_info;

#define Drv LFlash

char g_GPS_buff[256];

const char *nextToken(const char* src, char* buf)
{
  int i = 0;
  while (src[i] != 0 && src[i] != ',')
    i++;
  if (buf)
  {
    strncpy(buf, src, i);
    buf[i] = 0;
  }
  if (src[i])
    i++;
  return src + i;
}



void parseGPGGA(const char* GPGGAstr, GPSWaypoint* wayPoint){
	/* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
	* Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
	* Where:
	*  GGA          Global Positioning System Fix Data
	*  123519       Fix taken at 12:35:19 UTC
	*  4807.038,N   Latitude 48 deg 07.038' N
	*  01131.000,E  Longitude 11 deg 31.000' E
	*  1            Fix quality: 0 = invalid
	*                            1 = GPS fix (SPS)
	*                            2 = DGPS fix
	*                            3 = PPS fix
	*                            4 = Real Time Kinematic
	*                            5 = Float RTK
	*                            6 = estimated (dead reckoning) (2.3 feature)
	*                            7 = Manual input mode
	*                            8 = Simulation mode
	*  08           Number of satellites being tracked
	*  0.9          Horizontal dilution of position
	*  545.4,M      Altitude, Meters, above mean sea level
	*  46.9,M       Height of geoid (mean sea level) above WGS84
	*                   ellipsoid
	*  (empty field) time in seconds since last DGPS update
	*  (empty field) DGPS station ID number
	*  *47          the checksum data, always begins with *
	*/
    Serial.println(GPGGAstr);

	if (GPGGAstr[0] == '$'){
		int tmp;
		tmp = getComma(1, GPGGAstr);
		/*MyGPSPos.hour = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
		MyGPSPos.minute = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
		MyGPSPos.second = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');*/

		//get time
		//sprintf(buff, "UTC timer %2d-%2d-%2d", MyGPSPos.hour, MyGPSPos.minute, MyGPSPos.second);
		 
		//get lat/lon coordinates
		float latitudetmp;
		float longitudetmp;
		tmp = getComma(2, GPGGAstr);
		latitudetmp = getFloatNumber(&GPGGAstr[tmp]);

        tmp = getComma(3, GPGGAstr);
        //char* n_or_s = getCharString(&GPGGAstr[tmp]);
        Serial.print("Temp:");
        Serial.println(tmp);
        char n_or_s = GPGGAstr[tmp];
        Serial.print("North or South ");
        Serial.println(n_or_s);

		tmp = getComma(4, GPGGAstr);
		longitudetmp = getFloatNumber(&GPGGAstr[tmp]);

        tmp = getComma(5, GPGGAstr);
        char e_or_w = GPGGAstr[tmp];
        Serial.print("East or West ");
		Serial.println(e_or_w);
		
		// need to convert format
		convertCoords(latitudetmp, longitudetmp, &n_or_s, &e_or_w, wayPoint->latitude, wayPoint->longitude);

		//get lat/lon direction
		tmp = getComma(3, GPGGAstr);
		//MyGPSPos.latitude_dir = (GPGGAstr[tmp]);
		tmp = getComma(5, GPGGAstr);
		//MyGPSPos.longitude_dir = (GPGGAstr[tmp]);

		//sprintf(buff, "latitude = %10.4f-%c, longitude = %10.4f-%c", MyGPSPos.latitude, MyGPSPos.latitude_dir, MyGPSPos.longitude, MyGPSPos.longitude_dir);
		//Serial.println(buff); 

		//get GPS fix quality
		tmp = getComma(6, GPGGAstr);
		wayPoint->has_fix = getIntNumber(&GPGGAstr[tmp]);
		//sprintf(buff, "  -  GPS fix quality = %d", MyGPSPos.fix);
		//Serial.print(buff);
		//get satellites in view
		tmp = getComma(7, GPGGAstr);
		//MyGPSPos.num = getIntNumber(&GPGGAstr[tmp]);
		//sprintf(buff, "  -  %d satellites", MyGPSPos.num);
		//Serial.println(buff);
	}
	else{
		Serial.println("No GPS data");
	}
}



boolean printGPGGA(char* str, char* GPS_formatted, GPSWaypoint* wayPoint)
{
  char SMScontent[160];
  char latitude[20];
  char lat_direction[1];
  char longitude[20];
  char lon_direction[1];
  char buf[20];
  char time[30];
  const char* p = str;
  Serial.println("float");

  Serial.print("GPGGA Value:");
  Serial.println(p);

  p = nextToken(p, 0); // GGA
  p = nextToken(p, time); // Time
  p = nextToken(p, latitude); // Latitude
  p = nextToken(p, lat_direction); // N or S?
  p = nextToken(p, longitude); // Longitude
  p = nextToken(p, lon_direction); // E or W?

  Serial.print("latitude:");
  Serial.println(latitude);

  p = nextToken(p, buf); // fix quality
  
  float fLatitude = getFloatNumber(latitude);
  float fLongitude = getFloatNumber(longitude);

  float fLatitude_fixed;
  float fLongitude_fixed;

  convertCoords(fLatitude, fLongitude,lat_direction,lon_direction, wayPoint->latitude, wayPoint->longitude);

 

  if (buf[0] == '1')
  {
	  Serial.println("float numbers tada");
	  //p = nextToken(p, buf); // number of satellites
	 
	  //Serial.print(atoi(buf));
	  //Serial.println(" satellite(s) found!");
	 





	/*  LFile f = Drv.open("Position.txt", FILE_WRITE);
	  if (f){
		  char text_write_buffer[50];
		  sprintf(text_write_buffer, "\n%s", str);

		  f.println(text_write_buffer);
		  f.close();
	  }*/

    // GPS fix
 
    
    
    const int coord_size = 8;
    char lat_fixed[coord_size],lon_fixed[coord_size];

	float fLatitude = getFloatNumber(latitude);
	float fLongitude = getFloatNumber(longitude);
	Serial.println("float numbers");
	float fLatitude_fixed;
	float fLongitude_fixed;

    convertCoords(fLatitude, fLongitude, lat_direction, lon_direction, fLatitude_fixed, fLongitude_fixed);

	Serial.println("converted float numbers");
    //convertCoords(latitude,longitude,lat_fixed, lon_fixed,coord_size);
    

	wayPoint->latitude = fLatitude_fixed;
	wayPoint->longitude = fLongitude_fixed;
	
    Serial.print("Latitude:");
  //  Serial.println(lat_fixed);
	Serial.println(wayPoint->latitude);

    Serial.println(lat_direction);
    //strcat(SMScontent, "\nLatitude: ");
    //strcat(SMScontent,lat_fixed);
    //strcat(SMScontent,lat_direction);
    
    Serial.print("Longitude:");
    //Serial.println(lon_fixed);
	Serial.println(wayPoint->longitude);
    Serial.println(lon_direction);
   
    return true;
  }
  else
  {
    Serial.println("GPS is not fixed yet.");
    //Serial.println(p);
    return false; 
  }
}


static float getFloatNumber(const char *s){
	char buf[10];
	unsigned char i;
	float rev;

	i = getComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = atof(buf);
	return rev;
}

static unsigned char getComma(unsigned char num, const char *str){
	unsigned char i, j = 0;
	int len = strlen(str);
	for (i = 0; i < len; i++){
		if (str[i] == ',')
			j++;
		if (j == num)
			return i + 1;
	}
	return 0;
}

static int getIntNumber(const char *s){
	char buf[10];
	unsigned char i;
	float rev;

	i = getComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev = atoi(buf);
	return rev;
}

static char* getCharString(const char *s){
    char buf[1];
    unsigned char i;
    float rev;

    i = getComma(1, s);
    i = i - 1;
    strncpy(buf, s, i);
 
    return buf;
}


void readSpeed(char* str,  GPSWaypoint* wayPoint){
	//GPVTG
	/*
	example string. $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K

	054.7,T      True track made good
	034.4,M      Magnetic track made good
	005.5,N      Ground speed, knots
	010.2,K      Ground speed, Kilometers per hour

	eg3. $GPVTG,t,T,,,s.ss,N,s.ss,K*hh
	1    = Track made good
	2    = Fixed text 'T' indicates that track made good is relative to true north
	3    = not used
	4    = not used
	5    = Speed over ground in knots
	6    = Fixed text 'N' indicates that speed over ground in in knots
	7    = Speed over ground in kilometers/hour
	8    = Fixed text 'K' indicates that speed over ground is in kilometers/hour
	9    = Checksum
	*/

	Serial.print(str);
	Serial.println("->readSpeed");
	
    
    float speed_temp;
    int tmp = getComma(7, str);
    speed_temp = getFloatNumber(&str[tmp]);
    speed_temp *= 0.621371; // convert to miles per hour

    Serial.println(speed_temp);

    wayPoint->speed = speed_temp;
	
}


void getGPSData(gpsSentenceInfoStruct &g_info, char* GPS_formatted, GPSWaypoint* positionData)
{
  
  boolean GPS_fix = false;

  int loopCounter = 0;

  while (!GPS_fix)
  {
		LGPS.getData(&g_info);    //get the data from the GPS and store it in 'g_info'
		parseGPGGA((char*)g_info.GPGGA, positionData);
		
		if (positionData->has_fix > 0){
			GPS_fix = true;
		}
		else if (loopCounter == 25){
			GPS_fix = true;
		}

		if (GPS_fix){
			readSpeed((char*)g_info.GPVTG, positionData);
		}
        delay(500);
		loopCounter++;
  }

  //test

  //char gpgga[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  //char gpgtv[] = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K";

  //GPS_fix = printGPGGA((char*)gpgga, GPS_formatted, positionData);    //printGPGGA returns TRUE if the GPGGA string returned confirms a GPS fix.
  //if (GPS_fix){
	 // Serial.println("Gps fixed!");
	 // readSpeed((char*)gpgtv, positionData);
  //}

}



void convertCoords(float latitude, float longitude, const char* n_or_s, const char* e_or_w, float &lat_return, float &lon_return){

    /*
    Latitude	5213.2930,N	-->	52d 13.2930' N
    52 degrees 13.2930 minutes NORTH
    52 + (13.2930 / 60) = 52.22155
    Because it is north of the equator, the number remains positive.
    +52.22155
    */

    int lat_deg_int = int(latitude / 100);		//extract the first 2 chars to get the latitudinal degrees
    float latitude_float = latitude - (lat_deg_int * 100); //remove the degrees part of the coordinates - so we are left with only minutes-seconds part of the coordinates
    lat_return = lat_deg_int + latitude_float / 60;			//add back on the degrees part, so it is decimal degrees

    //Check if it is N or S, S will turn the value negative
     
    if (*n_or_s == 'S'){
        Serial.println("is South");
        lat_return *= -1;
    }

    /*
    Longitude	00004.5337,W	-->	00d 04.5337' W
    00 degrees 4.5337 minutes WEST
    00 +  (4.5337 / 60)  = 0.0755616
    Because it is West, the number becomes negative.
    -0.0755616
    */

    int lon_deg_int = int(longitude / 100);		
    float longitude_float = longitude - lon_deg_int * 100;
    lon_return = lon_deg_int + longitude_float / 60;
    if (*e_or_w == 'W'){
        Serial.println("is West");
        lon_return *= -1;
    }
}



int arrayToInt(const char* char_array)
{
    int temp;
    sscanf(char_array, "%d", &temp);
    return temp;
}

float arrayToFloat(const char* char_array)
{
    float temp;
    sscanf(char_array, "%f", &temp);
    return temp;
}
