#include "Interp4Move.hh"

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

std::istream & operator >> (std::istream & strWej, Interp4Move & interp){
    double speed;
    double distance;
    char * robotName;
    strWej >> robotName >> speed >> distance;

    interp.setRobotName(robotName);
    interp.setSpeed(speed);
    interp.setDistance(distance);

    return strWej;
}

const char* Interp4Move::GetCmdName() const {}

bool Interp4Move::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) {}

bool Interp4Move::ReadParams(std::istream &rStrm_CmdsList) {
    // Próba wczytania wartości ze strumienia
    if (rStrm_CmdsList >> *this) {
        // Wczytano poprawnie
        return true;
    } else {
        // Wystąpił błąd podczas wczytywania
        std::cerr << "Błąd: nie można wczytać parametrów dla Interp4Move." << std::endl;
        return false;
    }
}


void Interp4Move::PrintCmd() const {
    std::cout << "Obecne polecenie: " <<std::endl;
    std::cout << "Move " << getRobotName() <<" " <<getSpeed() << " "<< getDistance() << std::endl;
}
void Interp4Move::PrintSyntax() const {
    std::cout << "Skladnia polecenia Move: " << std::endl;
    std::cout << "<nazwa_obiektu> <szybkosc> dlugosc_drogi" << std::endl;
}

static AbstractInterp4Command* CreateCmd(){}