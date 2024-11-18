#include "Interp4Rotate.hh"
#include <thread>
#include <chrono>
#include <iostream>

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* Interp4Rotate::GetCmdName() const { return "Rotate"; }
}

Interp4Rotate::Interp4Rotate()
    : _robotName(""), _axis('Z'), _angularVelocity(0.0), _angle(0.0) {}

// Getters
const std::string& Interp4Rotate::GetRobotName() const {
    return _robotName;
}

char Interp4Rotate::GetAxis() const {
    return _axis;
}

double Interp4Rotate::GetAngularVelocity() const {
    return _angularVelocity;
}

double Interp4Rotate::GetAngle() const {
    return _angle;
}

// Setters
void Interp4Rotate::SetRobotName(const std::string& name) {
    _robotName = name;
}

void Interp4Rotate::SetAxis(char axis) {
    if (axis == 'X' || axis == 'Y' || axis == 'Z') {
        _axis = axis;
    } else {
        std::cerr << "Invalid axis: " << axis << ". Using default 'Z' axis." << std::endl;
        _axis = 'Z';
    }
}

void Interp4Rotate::SetAngularVelocity(double angularVelocity) {
    _angularVelocity = angularVelocity;
}

void Interp4Rotate::SetAngle(double angle) {
    _angle = angle;
}


void Interp4Rotate::PrintCmd() const {
    std::cout << GetCmdName() << " " << _robotName << " "
              << _axis << " " << _angularVelocity << " " << _angle << std::endl;
}

bool Interp4Rotate::ExecCmd( AbstractScene &rScn, const char * sMobObjName,AbstractComChannel &rComChann) {
    AbstractMobileObj * pObj = rScn.FindMobileObj(sMobObjName);

  if (pObj == nullptr){
    std::cerr << "Nie znaleziono obiektu o nazwie: "<< sMobObjName << std::endl;
    return false;
  }

    double _new_orientation_deg = 0;

    switch(GetAxis()){
        case 'x':
            _new_orientation_deg = GetAngle() + pObj->GetAng_Roll_deg();
            pObj->SetAng_Roll_deg(_new_orientation_deg);
            break;
        case 'y':
            _new_orientation_deg = GetAngle() + pObj->GetAng_Pitch_deg();
            pObj->SetAng_Pitch_deg(_new_orientation_deg);
            break;
        case 'z':
            _new_orientation_deg = GetAngle() + pObj->GetAng_Yaw_deg();
            pObj->SetAng_Yaw_deg(_new_orientation_deg);
            break;;
        default:
            std::cerr << "Rotation axis error!" << std::endl;
            return false;

    }

  return true;
}


bool Interp4Rotate::ReadParams(std::istream& strm) {
    strm >> _robotName >> _axis >> _angularVelocity >> _angle;
    if (strm.fail() || (_axis != 'X' && _axis != 'Y' && _axis != 'Z')) {
        std::cerr << "Error reading parameters for Rotate command." << std::endl;
        return false;
    }
    return true;
}

void Interp4Rotate::PrintSyntax() const {
     std::cout <<GetCmdName() << " syntax: Rotate <object_name> <axis> <angular_velocity_deg/s> <angle_deg>" << std::endl;
}
void Interp4Rotate::PrintParams() const {
    std::cout << GetCmdName() <<" command parameters:" << std::endl;
    std::cout << "  Object Name       : " << GetRobotName() << std::endl;
    std::cout << "  Axis of Rotation  : " << GetAxis() << std::endl;
    std::cout << "  Angular Velocity  : " << GetAngularVelocity() << " deg/s" << std::endl;
    std::cout << "  Angle of Rotation : " << GetAngle() << " deg" << std::endl;
}

AbstractInterp4Command* Interp4Rotate::CreateCmd() {
    return new Interp4Rotate();
}
