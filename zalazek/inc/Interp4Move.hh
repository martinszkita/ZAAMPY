#ifndef INTERP4MOVE_HH
#define INTERP4MOVE_HH

#include "AbstractInterp4Command.hh"
#include "Vector3D.hh"
#include "AbstractScene.hh"
#include "AbstractComChannel.hh"

class Interp4Move : public AbstractInterp4Command {
  private:
    double _Speed_mmS;   // Prędkość ruchu
    double _Distance_mm; // Długość trasy
    const char * _robotName;

  public:
    Interp4Move() = default;

    const char* GetCmdName() const override;
    bool ExecCmd(AbstractScene &rScn, const char *sMobObjName, AbstractComChannel &rComChann) override;
    bool ReadParams(std::istream &rStrm_CmdsList) override;
    void PrintCmd() const override;
    void PrintSyntax() const override;
    void setRobotName(const char * _name);
    const char * getRobotName() const;
    void setSpeed(const double _speed);
    void setDistance(const double distance);
    const double & getSpeed() const;
    const double & getDistance() const;

    static AbstractInterp4Command* CreateCmd();
    
};

std::istream & operator >> (std::istream & strWej, Interp4Move & interp);


#endif
