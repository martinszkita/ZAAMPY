#include "MobileObj.hh"
#include "Vector3D.hh"


const Vector3D & MobileObj::GetPosition_m() const {
    return _position;
}

void MobileObj::SetPosition_m(const Vector3D &rPos)  {
    _position = rPos;
}

double MobileObj::GetAng_Roll_deg() const  {
    return _roll;
}

void MobileObj::SetAng_Roll_deg(double Ang_Roll_deg)  {
    _roll = Ang_Roll_deg;
}

double MobileObj::GetAng_Pitch_deg() const  {
    return _pitch;
}

void MobileObj::SetAng_Pitch_deg(double Ang_Pitch_deg)  {
    _pitch = Ang_Pitch_deg;
}

double MobileObj::GetAng_Yaw_deg() const  {
    return _yaw;
}

void MobileObj::SetAng_Yaw_deg(double Ang_Yaw_deg)  {
    _yaw = Ang_Yaw_deg;
}

const std::string & MobileObj::GetName() const  {
    return _name;
}

void MobileObj::SetName(const char* sName)  {
    _name = std::string(sName);
}

