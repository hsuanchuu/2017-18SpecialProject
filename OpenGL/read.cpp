//read stl file
//input for ASCII file
//input_binary for binary file
#include "stdafx.h"
#include "read.h"

using namespace std;

void read::get_maxsize(vec3d point, double & x_min, double & x_max,
	double & y_min, double & y_max, double & z_min, double & z_max)
{
	if (point.x < x_min) x_min = point.x;
	else if (point.x > x_max) x_max = point.x;
	if (point.y < y_min) y_min = point.y;
	else if (point.y > y_max) y_max = point.y;
	if (point.z < z_min) z_min = point.z;
	else if (point.z > z_max) z_max = point.z;


};

int read::input(vector<triangle> & facet, string filename,
	double & x_min, double & x_max,
	double & y_min, double & y_max,
	double & z_min, double & z_max)
{
	//string filename = "LOWPOLYHUAMN_2.stl";
	// declare a (input) file object
	ifstream InputFile;

	// open the STL file by using the full path and the name
	// specify that the file is opened in "read-only" mode
	InputFile.open(filename.c_str(), ifstream::in);
	if (InputFile.fail())
	{
		cout << "ERROR: Input STL file could not be opened!" << endl;
		return 1; // error
	}

	// read in the contents line by line until the file ends

	// initialize counter for counting the number of lines in this file
	int triangle_number = 0;

	// declare an object "tri" of type triangle (see above for the definition of the triangle struct)
	triangle tri;

	// declare some string objects
	string junk;
	string string1, string2;

	// read in the first line (until the /n delimiter) and store it in the string object "line"
	getline(InputFile, junk);

	// initialize parameters that will be used to store the minimum and maximum extents of the geometry
	// described by the STL file
	x_min = 1e+30, x_max = -1e+30;
	y_min = 1e+30, y_max = -1e+30;
	z_min = 1e+30, z_max = -1e+30;

	// begin loop to read the rest of the file until the file ends
	while (true)
	{
		// read the components of the normal vector
		InputFile >> string1 >> string2 >> tri.normal.x >> tri.normal.y >> tri.normal.z;        //  1
																									 // continue reading this line until the \n delimiter
		getline(InputFile, junk);                                                                //  1

																									  // read the next line until the \n delimiter
		getline(InputFile, junk);                                                                //  2

																									  // read the (x,y,z) coordinates of vertex 1            
		InputFile >> string1 >> tri.point[0].x >> tri.point[0].y >> tri.point[0].z;             //  3
		getline(InputFile, junk);                                                                //  3

																									  // read the (x,y,z) coordinates of vertex 2            
		InputFile >> string1 >> tri.point[1].x >> tri.point[1].y >> tri.point[1].z;             //  4
		getline(InputFile, junk);                                                                //  4

																									  // read the (x,y,z) coordinates of vertex 3            
		InputFile >> string1 >> tri.point[2].x >> tri.point[2].y >> tri.point[2].z;             //  5
		getline(InputFile, junk);                                                                //  5

																									  // read some more junk
		getline(InputFile, junk);                                                                //  6
		getline(InputFile, junk);                                                                //  7

		for (int i = 0;i < 3;i++) {
			//cout << tri.point[i].x << endl;
			get_maxsize(tri.point[i],x_min,x_max,y_min,y_max,z_min,z_max);
			//cout << "x_min:" << x_min << " x_max:" << x_max << endl;
		}

		// break out of the while loop if "end-of-file" becomes true
		if (InputFile.eof()) break;
		
		// increment the triangle number
		triangle_number++;

		// add data for this triangle to the "facet" vector
		facet.push_back(tri);
	}
	cout << "break loop" << endl;
	// end while loop

	// explicitly close the output file
	InputFile.close();

	return 0;
}

int read::input_binary(vector<triangle> & facet, string filename,
	double & x_min, double & x_max, double & y_min, double & y_max, double & z_min, double & z_max)
{
	// specify the location of STL files on this computer
	//string filename = "forearm.stl";

	// declare an (input) file object
	ifstream binaryInputFile;

	// open the STL file by using the full path and the name
	// specify that the file is opened in "read-only" mode
	// specify that the file is opened in binary format
	binaryInputFile.open(filename.c_str(), ifstream::in | ifstream::binary);

	// check whether the file was opened successfully
	// if yes then continue otherwise terminate program execution
	if (binaryInputFile.fail())
	{
		cout << "ERROR: Input STL file could not be opened!" << std::endl;
		return (1);
	}

	// position the pointer to byte number 80
	binaryInputFile.seekg(80);

	// read the number of facets (triangles) in the STL geometry
	int numberOfTriangles;
	binaryInputFile.read((char*)&numberOfTriangles, sizeof(int));

	// declare an object "tri" of type triangle (see main.h for the definition of the triangle class)
	triangle tri;

	// storage space for the "unused bytes" 
	char unused_bytes[2];

	// initialize parameters that will be used to store the minimum and maximum extents of the geometry
	// described by the STL file
	x_min = 1e+30, x_max = -1e+30;
	y_min = 1e+30, y_max = -1e+30;
	z_min = 1e+30, z_max = -1e+30;

	// temporary floating point variable
	float temp_float_var;

	for (int count = 0;count<numberOfTriangles;count++)
	{
		// read the three components of the normal vector
		binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.x = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.y = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.normal.z = (double)temp_float_var;

		// read the three coordinates of vertex 1 
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].x = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].y = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[0].z = (double)temp_float_var;

		// read the three coordinates of vertex 2 
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].x = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].y = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[1].z = (double)temp_float_var;

		// read the three coordinates of vertex 3
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].x = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].y = (double)temp_float_var;
		binaryInputFile.read((char*)&temp_float_var, 4); tri.point[2].z = (double)temp_float_var;

		// read the 2 unused bytes
		binaryInputFile.read(unused_bytes, 2);

		for (int i = 0;i < 3;i++) {
			//cout << tri.point[i].x << endl;
			get_maxsize(tri.point[i], x_min, x_max, y_min, y_max, z_min, z_max);
			//cout << "x_min:" << x_min << " x_max:" << x_max << endl;
		}

		// add data for this triangle to the "facet" vector
		facet.push_back(tri);
	}

	// explicitly close the connection to the input STL file
	binaryInputFile.close();

	return (0);  // all is well
}

void read::createVertexArray(std::vector<triangle> & facet, float* vertices)
{
	const int NUM_FACETS = facet.size();
	int count = 0;
	for (int facet_index = 0; facet_index < NUM_FACETS; facet_index++)
	{
		// vertex 0
		{
			// x-y-z coordinates of the point
			vertices[count + 0] = facet[facet_index].point[0].x;
			vertices[count + 1] = facet[facet_index].point[0].y;
			vertices[count + 2] = facet[facet_index].point[0].z;
			// x-y-z coordinates of the normal
			vertices[count + 3] = facet[facet_index].normal.x;
			vertices[count + 4] = facet[facet_index].normal.y;
			vertices[count + 5] = facet[facet_index].normal.z;
			// (R,G,B,A) color values 
			vertices[count + 6] = fabs(facet[facet_index].normal.x);
			vertices[count + 7] = fabs(facet[facet_index].normal.y);
			vertices[count + 8] = fabs(facet[facet_index].normal.z);
			vertices[count + 9] = 1.0;
		}

		// vertex 1
		{
			// x-y-z coordinates of the point
			vertices[count + 10] = facet[facet_index].point[1].x;
			vertices[count + 11] = facet[facet_index].point[1].y;
			vertices[count + 12] = facet[facet_index].point[1].z;
			// x-y-z coordinates of the normal
			vertices[count + 13] = facet[facet_index].normal.x;
			vertices[count + 14] = facet[facet_index].normal.y;
			vertices[count + 15] = facet[facet_index].normal.z;
			// (R,G,B,A) color values 
			vertices[count + 16] = fabs(facet[facet_index].normal.x);
			vertices[count + 17] = fabs(facet[facet_index].normal.y);
			vertices[count + 18] = fabs(facet[facet_index].normal.z);
			vertices[count + 19] = 1.0;
		}

		// vertex 2
		{
			// x-y-z coordinates of the point
			vertices[count + 20] = facet[facet_index].point[2].x;
			vertices[count + 21] = facet[facet_index].point[2].y;
			vertices[count + 22] = facet[facet_index].point[2].z;
			// x-y-z coordinates of the normal
			vertices[count + 23] = facet[facet_index].normal.x;
			vertices[count + 24] = facet[facet_index].normal.y;
			vertices[count + 25] = facet[facet_index].normal.z;
			// (R,G,B,A) color values 
			vertices[count + 26] = fabs(facet[facet_index].normal.x);
			vertices[count + 27] = fabs(facet[facet_index].normal.y);
			vertices[count + 28] = fabs(facet[facet_index].normal.z);
			vertices[count + 29] = 1.0;
		}

		count += 30;
	}
}

