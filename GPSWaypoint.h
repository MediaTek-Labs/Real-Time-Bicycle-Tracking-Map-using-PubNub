#pragma once
class GPSWaypoint
{
public:
	float longitude;
	float latitude;
    char* n_or_s;
    char* e_or_w;
	float speed;
	char date;
	int has_fix;
	
	GPSWaypoint();
	~GPSWaypoint();
};
