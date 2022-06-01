/*==============================================================================

Copyright 2018-2019 Dmitry Popov

This Work or file is part of the greater total Work, software or group of
files named HyperFun Polygonizer.

HyperFun Polygonizer can be redistributed and/or modified under the terms
of the CGPL, The Common Good Public License as published by and at CGPL.org
(http://CGPL.org).  It is released under version 1.0 Beta of the License
until the 1.0 version is released after which either version 1.0 of the
License, or (at your option) any later version can be applied.

THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
CGPL, The Common Good Public License for more information.)

You should have received a copy of the CGPL along with HyperFun Polygonizer;
if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/

#define UNITSMULTIPLIER 1

#include "mainapp.h"
#include <math.h>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int CMainApp::cliOut() {
	int layers = 0;
	double zmin = itsSliser_->GetZMin();
	double zmax = itsSliser_->GetZMax();

	//TODO: consider cmd input for deltas
	double zdelta = itsSliser_->GetZDelta();
	ofstream toFile(itsCL.itsCLIName.c_str());

	if (!toFile) {
		cout << "\n File: " << itsCL.itsCLIName << " could not be opened.\n";
		exit(0);
	}
	toFile << "$$HEADERSTART\n";
	toFile << "$$ASCII\n";

	//All numbers x below will mean x * UNITSMULTIPLIER mm

	toFile << "$$UNITS/" << UNITSMULTIPLIER << "\n";

	struct tm *timenow;
	time_t rawtime;
	time(&rawtime);
	timenow = gmtime(&rawtime);
	toFile << "$$DATE/" << setfill('0') << setw(2) << timenow->tm_mday << setfill('0') << setw(2) << timenow->tm_mon + 1 << setfill('0') << setw(2) << timenow->tm_year % 100 << "\n";
	toFile << "$$HEADEREND\n\n";
	toFile << "$$GEOMETRYSTART\n";

	cout << "\n"; 

	auto t1 = std::chrono::high_resolution_clock::now();
	cout << "Z range: " << "[" << zmin << "; " << zmax << "]\n";
	for (double z = zmin; z < zmax; z += zdelta) {
		cout << "\r                                                     ";
		cout << "\rCurrent layer z = "<< z;
		layers += itsSliser_->WriteCLILayer(toFile, z);
	}
	//vector<double> x(3, 0.0);
	toFile << "$$GEOMETRYEND\n\0";
	
	/*try {
		toFile.close();
	}
	catch (const std::exception& e) {
		std::cout << e.what();
	}*/

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
	//printf("\n\nTime: %.5f seconds\n", fp_ms.count() / 1000);
	return layers;
};