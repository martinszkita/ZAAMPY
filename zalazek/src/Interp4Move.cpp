#include "Interp4Move.hh"

void Interp4Move::setRobotName(const char * _name){
    _robotName = _name;
}
const char * Interp4Move::getRobotName() const{
    return _robotName;
}
const char* Interp4Move::GetCmdName() const {}
bool Interp4Move::ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) {}
bool Interp4Move::ReadParams(std::istream &rStrm_CmdsList) {}
void Interp4Move::PrintCmd() const {
    std::cout << "Obecne polecenie: " <<std::endl;
    std::cout << "Move " << getRobotName() <<" " 
}
void Interp4Move::PrintSyntax() const {
    std::cout << "Skladnia polecenia Move: " << std::endl;
    std::cout << "<nazwa_obiektu> <szybkosc> dlugosc_drogi" << std::endl;
}

static AbstractInterp4Command* CreateCmd(){}