#include "Interp4Set.hh"
#include <cmath>
#include <iostream>

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* Interp4Set::GetCmdName() const { return "Set"; }
}

AbstractInterp4Command* CreateCmd(void){
    return Interp4Set::CreateCmd();
}

AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


Interp4Set::Interp4Set(): pos(Vector3D()), ang_x_deg(0), ang_y_deg(0), ang_z_deg(0) {}

// Getters
const std::string& Interp4Set::GetRobotName() const {
    return _robotName;
}

const Vector3D& Interp4Set::GetPosition() const {
    return pos;
}

double Interp4Set::GetAngleX() const {
    return ang_x_deg;
}

double Interp4Set::GetAngleY() const {
    return ang_y_deg;
}

double Interp4Set::GetAngleZ() const {
    return ang_z_deg;
}

// Setters
void Interp4Set::SetRobotName(const std::string& name) {
    _robotName = name;
}

void Interp4Set::SetPosition(const Vector3D& position) {
    pos = position;
}

void Interp4Set::SetAngleX(double value) {
    ang_x_deg = value;
}

void Interp4Set::SetAngleY(double value) {
    ang_y_deg = value;
}

void Interp4Set::SetAngleZ(double value) {
    ang_z_deg = value;
}

void Interp4Set::PrintCmd() const {
    std::cout << "Obecne polecenie: Set " <<std::endl;
    std::cout << GetCmdName() << " " << _robotName << " "
              << GetPosition()[0] << " " << GetPosition()[1] << " " << GetPosition()[2] << " "
              << ang_x_deg << " " << ang_y_deg << " " << ang_z_deg << std::endl;
}

bool Interp4Set::ExecCmd( AbstractScene &rScn, const char * sMobObjName,AbstractComChannel &rComChann)  {
    AbstractMobileObj * pObj = rScn.FindMobileObj(sMobObjName);

  if (pObj == nullptr){
    std::cerr << GetCmdName() <<"  -- nie znaleziono obiektu o nazwie: "<< sMobObjName << std::endl;
    return false;
  }

 
  pObj->SetPosition_m(pos);
  pObj->SetAng_Roll_deg(ang_x_deg);
  pObj->SetAng_Pitch_deg(ang_y_deg);
  pObj->SetAng_Yaw_deg(ang_z_deg);

  return true;
}

bool Interp4Set::ReadParams(std::istream& strm) {
    strm >> _robotName >> pos[0] >> pos[1] >> pos[2] >> ang_x_deg >> ang_y_deg >> ang_z_deg;
    return !strm.fail();
}

void Interp4Set::PrintSyntax() const{
    std::cout << "Set Command Syntax:" << std::endl;
    std::cout << "Set nazwa_obiektu wsp_x wsp_y wsp_z kat_OX kat_OY kat_OZ" << std::endl;
}
void Interp4Set::PrintParams() const{
    std::cout << "Set Command Params:" << std::endl;
    std::cout << "nazwa_obiektu: " << GetRobotName() << std::endl;
    std::cout << "wsp_x: " << GetPosition()[0] << std::endl;
    std::cout << "wso_y: " << GetPosition()[1] << std::endl;
    std::cout << "wsp_z: " << GetPosition()[2] << std::endl;
    std::cout << "kat_OX: " << GetAngleX() << std::endl;
    std::cout << "kat_OY: " << GetAngleY() << std::endl;
    std::cout << "kat_OZ: " << GetAngleZ() << std::endl;
}


