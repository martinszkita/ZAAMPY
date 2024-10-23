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

  public:
    Interp4Move();

    virtual const char* GetCmdName() const override;
    virtual bool ExecCmd(AbstractScene &rScn, 
                         const char *sMobObjName,
                         AbstractComChannel &rComChann) override;
    virtual bool ReadParams(std::istream &rStrm_CmdsList) override;
    virtual void PrintCmd() const override;
    virtual void PrintSyntax() const override;

    static AbstractInterp4Command* CreateCmd();
};

#endif
