#include "turnPTU.h"

#include <iostream>
#include <chrono>

PTU::PTU(char* com, int baudrate, unsigned short get_id){
	set_baud_rate(baudrate);
	this->COMstream = open_host_port(com);
	this->id = get_id;
	int initErrorCode = this->initialize();
	if (initErrorCode){
		std::cerr << "Initialize failed [Error code:" << initErrorCode << "]" << std::endl;
	}
	this->panAngle = 0;
	this->tiltAngle = 0;
}

PTU::~PTU(){}

int PTU::initialize(){

	if (this->COMstream == PORT_NOT_OPENED) return -1;
	SerialStringOut(COMstream, (unsigned char*)"   ");
	select_unit(this->id);

	switch (reset_PTU_parser(5000)) {
	case PTU_OK:
		break;
	case PTU_FIRMWARE_VERSION_TOO_LOW:
		return -2;
	case PTU_NOT_RESPONDING:
		return -3;
	}

	Sleep(2000);

	this->pan_resolution = get_desired(PAN, RESOLUTION);
	this->tilt_resolution = get_desired(TILT, RESOLUTION);

	std::cout << "PAN  RESOLUTION [" << this->pan_resolution << "]" << std::endl;
	std::cout << "TILT RESOLUTION [" << this->tilt_resolution << "]" << std::endl;

	const auto start = std::chrono::system_clock::now();
	unsigned short getBase = 1;
	set_desired(PAN, BASE, (PTU_PARM_PTR*)&getBase, ABSOLUTE);
	set_desired(TILT, BASE, (PTU_PARM_PTR*)&getBase, ABSOLUTE);

	long a = 1;
	set_desired(PAN, ACCELERATION, (PTU_PARM_PTR*)&a, ABSOLUTE);
	set_desired(TILT, ACCELERATION, (PTU_PARM_PTR*)&a, ABSOLUTE);

	short low_power = PTU_REG_POWER;
	set_desired(PAN, HOLD_POWER_LEVEL, (PTU_PARM_PTR*)&low_power, ABSOLUTE);
	set_desired(TILT, HOLD_POWER_LEVEL, (PTU_PARM_PTR*)&low_power, ABSOLUTE);
	set_desired(PAN, MOVE_POWER_LEVEL, (PTU_PARM_PTR*)&low_power, ABSOLUTE);
	set_desired(TILT, MOVE_POWER_LEVEL, (PTU_PARM_PTR*)&low_power, ABSOLUTE);

	return 0;
}

int PTU::turn(double pan, double tilt, bool absolute){

	if (absolute){
		short panPos = this->pan_deg2step(RAD2DEG(pan));
		short tiltPos = this->tilt_deg2step(RAD2DEG(tilt));
		set_desired(PAN, POSITION, (PTU_PARM_PTR *)&panPos, ABSOLUTE);
		set_desired(TILT, POSITION, (PTU_PARM_PTR *)&tiltPos, ABSOLUTE);
	}
	else{
		short panPos = this->pan_deg2step(RAD2DEG(pan));
		short tiltPos = this->tilt_deg2step(RAD2DEG(tilt));
		set_desired(PAN, POSITION, (PTU_PARM_PTR *)&panPos, RELATIVE);
		set_desired(TILT, POSITION, (PTU_PARM_PTR *)&tiltPos, RELATIVE);
	}
	return 0;
}

int PTU::setSpeed(){
	set_mode(COMMAND_EXECUTION_MODE, EXECUTE_IMMEDIATELY);
	const auto start = std::chrono::system_clock::now();
	short vel = SPEED_PAN_MAX / SPEED_FACTOR;
	set_desired(PAN, SPEED, (PTU_PARM_PTR *)&vel, ABSOLUTE);
	vel = SPEED_TILT_MAX / SPEED_FACTOR;
	set_desired(TILT, SPEED, (PTU_PARM_PTR *)&vel, ABSOLUTE);
	const auto end = std::chrono::system_clock::now();

	return 0;
}

int PTU::turnHome(){
	//short vel = SPEED_PAN_MAX / 2.;
	//set_desired(PAN, SPEED, (PTU_PARM_PTR *)&vel, ABSOLUTE);
	//vel = SPEED_TILT_MAX / 2.;
	//set_desired(TILT, SPEED, (PTU_PARM_PTR *)&vel, ABSOLUTE);
	short pos = 0;
	set_desired(PAN, POSITION, (PTU_PARM_PTR *)&pos, ABSOLUTE);
	set_desired(TILT, POSITION, (PTU_PARM_PTR *)&pos, ABSOLUTE);

	return 0;
}

int PTU::getPosition(){
	this->panAngle = DEG2RAD(this->pan_step2deg(get_current(PAN, POSITION)));
	this->tiltAngle = DEG2RAD(this->tilt_step2deg(get_current(TILT, POSITION)));
	//std::cout << "pan:" << this->panAngle << " tilt:" << this->tiltAngle << std::endl;
	return 0;
}


double PTU::pan_deg2step(double x){
	return (-x / (this->pan_resolution / 3600. / 60.));
}
double PTU::tilt_deg2step(double x){
	return (-x / (this->tilt_resolution / 3600. / 60.));
}
double PTU::pan_step2deg(double x){
	return (-x*(this->pan_resolution / 3600. / 60.));
}
double PTU::tilt_step2deg(double x){
	return  (-x*(this->tilt_resolution / 3600. / 60.));
}