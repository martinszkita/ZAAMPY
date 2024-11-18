#include <iostream>
#include "Interp4Move.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* Interp4Move::GetCmdName() const { return "Move"; }
}

AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 * Konstruktor klasy
 */
Interp4Move::Interp4Move(): _Speed_mmS(0), _Distance_mm(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  cout << GetCmdName() << " " << _Speed_mmS  << " " <<_Distance_mm<<" "<< endl;
}

void Interp4Move::setRobotName(const char * _name){
    _robotName = _name;
}
const char * Interp4Move::getRobotName() const{
    return _robotName;
}
void Interp4Move::setSpeed(const double _speed){
    _Speed_mmS = _speed;
}
void Interp4Move::setDistance(const double _distance){
    _Distance_mm = _distance;
}
const double & Interp4Move::getSpeed() const{
    return _Speed_mmS;
}
const double & Interp4Move::getDistance() const{
    return _Distance_mm;
}

bool Interp4Move::ExecCmd( AbstractScene &rScn, const char * sMobObjName,AbstractComChannel &rComChann)
{
  AbstractMobileObj * pObj = rScn.FindMobileObj(sMobObjName);

  if (pObj == nullptr){
    std::cerr << "Nie znaleziono obiektu o nazwie: "<< sMobObjName << endl;
    return false;
  }

  Vector3D currentPosition = pObj->GetPosition_m();
  double currentYawDeg = pObj->GetAng_Yaw_deg();
  // Zamiana kąta yaw z stopni na radiany
  double yawRadians = currentYawDeg * M_PI / 180.0;

  // Obliczenie nowego przesunięcia na podstawie odległości (ruch w przód)
  double distanceInMeters = _Distance_mm / 1000.0;  // Konwersja odległości z mm na metry

  // Nowa pozycja obliczona na podstawie obecnej pozycji i przesunięcia
  Vector3D Vtrans;

  // wektor przesuniecia w obecnej orientacji
  Vtrans[0] = distanceInMeters * cos(yawRadians);
  Vtrans[1] = distanceInMeters * sin(yawRadians);
  Vtrans[2] = 0;

  // oblcizanie nowej pozycji
  Vector3D newPosition = currentPosition + Vtrans;

  // ustawienie nowej pozycji
  pObj->SetPosition_m(newPosition);

  return true;
}


/*!
 * Funkcja wczytuje wartości prędkości i przesunięcia dla obiektu
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  Strm_CmdsList >> _Speed_mmS >> _Distance_mm;
  
  if (!Strm_CmdsList) {
    std::cerr << "Błąd wczytywania danych!" << std::endl;
    return false;
  } 

  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
