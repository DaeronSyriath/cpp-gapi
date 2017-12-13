/*
 * gapi.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: @DaeronSyriath
 */

#include <iostream>
#include <math.h>

class TablePosition{
public:
	int x=0;
	int y=0;
	TablePosition(int const x, int const y): x(x), y(y){};
	double distanceTo(TablePosition const & target);
};

double TablePosition::distanceTo(TablePosition const & target){
	return sqrt(pow(target.x - x, 2) + pow(target.y -y, 2));
}

class TableModel{
public:
	TablePosition position;
	int radius = 20;

	TableModel(int const & x, int const & y, int const & radius ):
			position(TablePosition(x,y)), radius(radius){};

	double distanceTo(TableModel const & target);
};

double TableModel::distanceTo(TableModel const & target){
	return position.distanceTo(target.position) - radius - target.radius;
}

int main(int argc, char** argv){

	TableModel mod1(0,0, 25);
	TableModel mod2(110,110, 32);

	std::cout<<"Distance to target: " <<mod1.distanceTo(mod2)<<std::endl;

	std::cout<<"GAPI closing."<<std::endl;
	return 0;
}


