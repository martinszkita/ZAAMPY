#include "MobileObj.hh"
#include "Vector3D.hh"


const Vector3D & MobileObj::GetPosition_m() const {
    return _position;
}

void MobileObj::SetPosition_m(const Vector3D &rPos)  {
    _position = rPos;
}

const Vector3D & MobileObj::GetRotXYZ_deg() const {
    return _position;
}

void MobileObj::SetRotXYZ_deg(const Vector3D &rPos)  {
    _position = rPos;
}

const std::string & MobileObj::GetName() const  {
    return _name;
}

void MobileObj::SetName(const char* sName)  {
    _name = std::string(sName);
}




