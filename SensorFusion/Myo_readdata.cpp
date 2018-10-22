//2017 Myo-IMU system

#define _USE_MATH_DEFINES
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <cmath>
#include <iomanip>
//#include <algorithm>
//#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <time.h>
#include <Windows.h>
#include <conio.h>

#include "myo/myo.hpp"
#include "IMGdata.h"

using namespace std;

SOCKET sConnect; //operating if a connection was found
SOCKADDR_IN addr;
int addrlen;

// The files we are logging to
    ofstream gyroFile;
    /*ofstream orientationFile;
    ofstream EulerFile;
	ofstream EulerFile2;
    ofstream accFile;
	ofstream accAngFile;*/

	ofstream armAngFile;


class DataCollector : public myo::DeviceListener {  
public:  
    DataCollector()
    {
        openFiles();
		uparm=1;
		downarm=0;
		init=1;
		for (size_t i = 0;i < 3;i++) {
			angle[i] = 0.0f;
			//pos_t[i] = 0.0f;
		}
		angle[3] = 0.0f;
    }

	void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
    {
        // Print out the MAC address of the armband we paired with.
        // The pointer address we get for a Myo is unique - in other words, it's safe to compare two Myo pointers to
        // see if they're referring to the same Myo.
        // Add the Myo pointer to our list of known Myo devices. This list is used to implement identifyMyo() below so
        // that we can give each Myo a nice short identifier.
        knownMyos.push_back(myo);
		myo->unlock(myo::Myo::unlockHold);
		IMGdata myodata;
		data.push_back(myodata);
        // Now that we've added it to our list, get our short ID for it and print it out.
        cout << "Paired with " << identifyMyo(myo) << "." << endl;
    }

    void openFiles() {
        time_t timestamp =time(0);
		clock_begin=clock();
		clock_tmp=clock();
		
        // Open file for gyroscope data
        if (gyroFile.is_open()) {
            gyroFile.close();
        }
        gyroFile.open("gyro.csv", ios::out|ios::trunc);
        gyroFile << "timestamp,gyro_x,gyro_y,gyro_z" << endl;

		//open file for accAng
		/*if (accAngFile.is_open()) {
            accAngFile.close();
        }
        accAngFile.open("accAng.csv", ios::out|ios::trunc);
        accAngFile << "timestamp,acc_x,acc_y" << endl;
		
        // Open file for quaternion
        if (orientationFile.is_open()) {
            orientationFile.close();
        }
        orientationFile.open("quaternion.csv", ios::out|ios::trunc);
        orientationFile << "timestamp,x,y,z,w" << endl;
		
		//Open file for accelerometer data
		if (accFile.is_open()) {
			accFile.close();
		}
		accFile.open("acc.csv", ios::out | ios::trunc);
		accFile << "timestamp,x,y,z" << endl;

        // Open file for Euler angles
        if (EulerFile.is_open()) {
            EulerFile.close();
        }
        EulerFile.open("euler.csv", ios::out|ios::trunc);
        EulerFile << "timestamp,roll,pitch,yaw" << endl;

		if(knownMyos.size()==2){
			if (EulerFile2.is_open()) {
	            EulerFile2.close();
		    }
			EulerFile2.open("euler2.csv", ios::out|ios::trunc);
			EulerFile2 << "timestamp,roll,pitch,yaw" << endl;
		}
		*/

		//open file for armAng
		if (armAngFile.is_open()) {
			armAngFile.close();
		}
		armAngFile.open("armAng.csv", ios::out | ios::trunc);
		armAngFile << "timestamp,ang1,ang2,ang3,ang4" << endl;

	}

	// onOrientationData is called whenever new orientation data is provided
	void onOrientationData(myo::Myo *myo, uint64_t timestamp, const myo::Quaternion< float > &rotation) {
		clock_now = clock();
		size_t myonum = identifyMyo(myo) - 1;
		data[myonum].setQuat(rotation.x(), rotation.y(), rotation.z(), rotation.w());

		//Calculate Euler angles from quaternion.
		//initial roll, pitch, and yaw because complementary filter can't get initail value
		if (data[myonum].init) {
			float yaw_init = atan2(2.0f * (rotation.w() * rotation.z() + rotation.x() * rotation.y()),
				1.0f - 2.0f * (rotation.y() * rotation.y() + rotation.z() * rotation.z()));
			yaw_init = (yaw_init / M_PI) * 180;

			data[myonum].initial(yaw_init);
			data[myonum].init = 0;
		}

		double delta_t = clock_now - data[myonum].clock_tmp;
		float yaw = atan2(2.0f * (rotation.w() * rotation.z() + rotation.x() * rotation.y()),
			1.0f - 2.0f * (rotation.y() * rotation.y() + rotation.z() * rotation.z()));
		yaw = (yaw / M_PI) * 180;

		clock_t clock_now = clock();
		if (myonum == downarm) {
			data[myonum].setEuler(yaw, clock_now);
		}
		else if (myonum == uparm) {
			data[myonum].setEuler(yaw, clock_now);
			//calculate armAng
			float a = -(data[uparm]).euler[2] * M_PI / 180;
			float b = (data[uparm]).euler[1] * M_PI / 180;
			float r = -(data[uparm]).euler[0] * M_PI / 180;

			angle[1] = -a * 180 / M_PI; //pitch
			angle[2] = b * 180 / M_PI; //yaw
			angle[0] = r * 180 / M_PI; //roll

			/*float a2 = -(data[downarm]).euler[2] * M_PI / 180;
			float b2 = (data[downarm]).euler[1] * M_PI / 180;
			float r2 = -(data[downarm]).euler[0] * M_PI / 180;
			//posFile << data[downarm].position2[0] << ',' << data[downarm].position2[1] << ',' << data[downarm].position2[2] << endl;
			//mapping position in downarm frame to uparm frame
			pos_t[0] = cos(a2)*cos(b2)*data[downarm].position2[0] + (cos(a2)*sin(b2)*sin(r2) - sin(a2)*cos(r2))*data[downarm].position2[1]
				+ (cos(a2)*sin(b2)*cos(r2) + sin(a2)*sin(r2))*data[downarm].position2[2];
			pos_t[1] = sin(a2)*sin(b2)*data[downarm].position2[0] + (sin(a2)*sin(b2)*sin(r2) + cos(a2)*cos(r2))*data[downarm].position2[1] +
				(sin(a2)*sin(b2)*cos(r2) - cos(a2)*sin(r2))*data[downarm].position2[2];
			pos_t[2] = -sin(b2)*data[downarm].position2[0] + cos(b2)*sin(r2)*data[downarm].position2[1] +
				cos(b2)*cos(r2)*data[downarm].position2[2];

			posFile << pos_t[0] << ',' << pos_t[1] << ',' << pos_t[2] << endl;

			angle[3] = 190 - (acos((data[uparm].position2[0]) * pos_t[0]
				+ (data[uparm].position2[1]) * pos_t[1]
				+ (data[uparm].position2[2]) * pos_t[2]) * 180 / M_PI);
			*/

			angle[3] = 200 - abs(data[uparm].euler[1] - data[downarm].euler[1])
				- abs(data[uparm].euler[2] - data[downarm].euler[2]);
			if (angle[3] > 180) {
				angle[3] = 178 + ((angle[3] - 180) / 90);
			}
		}



		myo->unlock(myo::Myo::unlockHold);
		data[myonum].clock_tmp = clock_now;
	}

	// onAccelerometerData is called whenever new acceleromenter data is provided
	void onAccelerometerData(myo::Myo *myo, uint64_t timestamp, const myo::Vector3< float > &acc) {
		clock_now = clock();
		data[identifyMyo(myo) - 1].setAccData(acc[0], acc[1], acc[2], clock_now - clock_begin);
	}

	// onGyroscopeData is called whenever new gyroscope data is provided
	void onGyroscopeData(myo::Myo *myo, uint64_t timestamp, const myo::Vector3< float > &gyro) {
		data[identifyMyo(myo) - 1].setGyroData(gyro[0], gyro[1], gyro[2]);
    }

    void onConnect(myo::Myo *myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
        //Reneable streaming
        openFiles();
    }

    // print accelerometer and gyroscope vectors
    void printVector(std::ofstream &file, uint64_t timestamp, const myo::Vector3< float > &vector) {
        file << timestamp << ',' << vector.x() << ',' << vector.y() << ',' << vector.z() << endl;
    }

	// This is a utility function implemented for this sample that maps a myo::Myo* to a unique ID starting at 1.
    // It does so by looking for the Myo pointer in knownMyos, which onPair() adds each Myo into as it is paired.
    size_t identifyMyo(myo::Myo* myo) {
        // Walk through the list of Myo devices that we've seen pairing events for.
        for (size_t i = 0; i < knownMyos.size(); ++i) {
            // If two Myo pointers compare equal, they refer to the same Myo device.
            if (knownMyos[i] == myo) {
                return i + 1;
            }
        }
        return 0;
    }
	void print() {
		//output angle.
		/*accAngFile << clock_now - clock_begin << ',' << data[uparm].accAng[0] << ','
			<< data[uparm].accAng[1] << ',' << data[uparm].accAng[2] << endl;
		gyroFile << clock_now - clock_begin << ',' << data[uparm].gyroAng[0] << ','
			<< data[uparm].gyroAng[1] << ',' << data[uparm].gyroAng[2] << endl;*/
		float angle2 = 200 - abs(data[uparm].euler2[1] - data[downarm].euler2[1])
			- abs(data[uparm].euler2[2] - data[downarm].euler2[2]);
		if (angle2 > 180) {
			angle2 = 178 + ((angle2 - 180) / 90);
		}
		gyroFile << clock_now - clock_begin << ',' << data[uparm].euler2[0] << ','
			<< data[uparm].euler2[1] << ',' << data[uparm].euler2[2] <<','
			 << angle2 << endl;
		cout << '\r';
		armAngFile << clock_now - clock_begin << ',';
		for (size_t j = 0;j<4;j++) {
			cout << fixed << setprecision(4) << angle[j] << "   ";
			if (j == 3) {
				armAngFile << fixed << setprecision(4) << angle[j];
			}
			else {
				armAngFile << fixed << setprecision(4) << angle[j] << ',';
			}
		}
		armAngFile << '\r';

		stringstream ss1, ss2, ss3, ss4;
		ss1 << (int)angle[0];
		ss2 << (int)angle[1];
		ss3 << (int)angle[2];
		ss4 << 180 - (int)angle[3];
		string str = ss1.str() + "," + ss2.str() + "," + ss3.str() + "," + ss4.str();
		char sendb[100];
		strcpy(sendb, str.c_str());
		//cout << sendb << endl;
		char* sendbuf;
		sendbuf = sendb;
		int fail = sendto(sConnect, sendbuf, (int)strlen(sendbuf), 0, (sockaddr*)&addr, addrlen);
		if (fail == SOCKET_ERROR) {
			cout << "send fail" << endl;
		}
	}
    // We store each Myo pointer that we pair with in this list, so that we can keep track of the order we've seen
    // each Myo and give it a unique short identifier (see onPair() and identifyMyo() above).
    vector<myo::Myo*> knownMyos;

	//variable
	vector<IMGdata> data;
	clock_t clock_begin, clock_now, clock_tmp;
	float angle[4];
	//float pos_t[3];
	size_t uparm,downarm;
	bool init;
};

int main(int argc, char** argv)  
{
	// We catch any exceptions that might occur below -- see the catch statement for more details.
    try {

    // First, we create a Hub with our application identifier. Be sure not to use the com.example namespace when
    // publishing your application. The Hub provides access to one or more Myos.
    myo::Hub hub("com.undercoveryeti.myo-data-capture");

    cout << "Attempting to find a Myo..." << std::endl;

    // Next, we attempt to find a Myo to use. If a Myo is already paired in Myo Connect, this will return that Myo
    // immediately.
    // waitForMyo() takes a timeout value in milliseconds. In this case we will try to find a Myo for 10 seconds, and
    // if that fails, the function will return a null pointer.
    /*myo::Myo* myo = hub.waitForMyo(10000);

    // If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
    if (!myo) {
        throw runtime_error("Unable to find a Myo!");
    }*/

    // We've found a Myo.
    //cout << "Connected to a Myo armband! Logging to the file system." << std::endl << std::endl;*/
	
	//client
	WSAData wsaData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 2);
	int r = WSAStartup(DLLVersion, &wsaData);
	if (r != 0) {
		cout << "WinSock initialization fail" << endl;
		getchar();
		return 1;
	}

	//construct socket(client)
	sConnect = socket(AF_INET, SOCK_DGRAM, 0);

	//set addt data
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addrlen = sizeof(addr);

	DataCollector collector;

	// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
	// Hub::run() to send events to all registered device listeners.
	hub.addListener(&collector);

	// Finally we enter our main loop.
	while (1) {
		//run every 200ms
		//but the sample frequency maintain 50Hz
		hub.run(100);
		//send data to server
		collector.print();
	}

	
    // If a standard exception occurred, we print out its message and exit.
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << std::endl;
        cerr << "Press enter to continue.";
        cin.ignore();
        return 1;
    }
}
