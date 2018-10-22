//IMGdata object
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include "myo/myo.hpp"
#include "Kalman.h"

using namespace std;

class IMGdata
{
public:
	IMGdata(){//constructor->initial angle
		for(int i=0;i<3;i++){
			gyroData[i]=0.0f;
			//gyroAng[i] = 0.0f;
			accAng[i]=0.0f;
			euler[i]=0.0f;
			//euler_tmp[i]=0.0f;
			clock_tmp = 0;
		}
		init=1;
	}
	void initial(float z){
		yaw_init=z;
	}
	
	void setGyroData(float x,float y,float z){
		gyroData[0]=x;
		gyroData[1]=y;
		gyroData[2]=z;

	}
	void setAccData(float x,float y,float z,double delta_t){
		accAng[0]=(atan2(y,sqrt(pow(x,2)+pow(z,2)))/M_PI)*180;
		accAng[1]=(atan2(-x,sqrt(pow(y,2)+pow(z,2)))/M_PI)*180;
		accAng[2] = (atan2(sqrt(pow(y, 2) + pow(x, 2)), z) / M_PI) * 180;
	}
	void setQuat(float x,float y,float z,float w){
		quaternion[0]=x;
		quaternion[1]=y;
		quaternion[2]=z;
		quaternion[3]=w;
	}
	void setEuler(float yaw, clock_t clock_now){
		/*for (int i = 0;i < 3;i++) {
			gyroAng[i] += gyroData[i] * (clock_now - clock_tmp)*0.001;
		}*/
		//kalman filter
		euler[0] = euler_f[0].getAngle(accAng[0], gyroData[0], clock_now - clock_tmp);
		euler[1] = euler_f[1].getAngle(accAng[1], gyroData[1], clock_now - clock_tmp);
		euler[2] = yaw - yaw_init;
		clock_tmp = clock_now;
		if (euler[2] > 180)
			euler[2] -= 360;
		//orientation in euler angles
		euler2[0] = atan2(2.0f * (quaternion[3] * quaternion[0] + quaternion[1] * quaternion[2]),
			1.0f - 2.0f * (quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]));
		euler2[1] = asin(max(-1.0f, min(1.0f, 2.0f * (quaternion[3] * quaternion[1] - quaternion[2] * quaternion[0]))));
		euler2[2] = atan2(2.0f * (quaternion[3] * quaternion[2] + quaternion[0] * quaternion[1]),
			1.0f - 2.0f * (quaternion[1] * quaternion[1] + quaternion[2] * quaternion[2]));
		for (int i = 0;i < 3;i++) {
			euler2[i] = euler2[i] * 180 / M_PI;
		}
		//convert to rotation matrix 
		//method-get rotation matrix by using euler[i]
		/*position2[0]=(cos(euler[1]*M_PI/180)*cos(euler[2]*M_PI/180))*1;
		position2[1]=(cos(euler[2]*M_PI/180)*sin(euler[1]*M_PI/180)*sin(euler[0]*M_PI/180)+cos(euler[0]*M_PI/180)*sin(euler[2]*M_PI/180))*1;
		position2[2]=(-cos(euler[0]*M_PI/180)*cos(euler[2]*M_PI/180)*sin(euler[1]*M_PI/180)+sin(euler[0]*M_PI/180)*sin(euler[2]*M_PI/180))*1;
		*/
	}
//data
	float gyroData[3];
	//float gyroAng[3];
	float accAng[3];
	float quaternion[4];
	//euler[0],[1] [2] represent roll,pitch,yaw respectively
	float euler[3];
	float euler2[3];
	//float euler_tmp[3];
	//float position2[3];
	float row_init;
	float pitch_init;
	float yaw_init;
	//float position2[3];
	bool init;
	clock_t clock_tmp;
	//kalman filter for roll, pitch, yaw
	Kalman euler_f[3];
};