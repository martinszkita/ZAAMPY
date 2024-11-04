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
    Interp4Move() = default;

    const char* GetCmdName() const override;
    bool ExecCmd(AbstractScene &rScn, 
                        const char *sMobObjName,
                        AbstractComChannel &rComChann) override;
    bool ReadParams(std::istream &rStrm_CmdsList) override;
    void PrintCmd() const override;
    void PrintSyntax() const override;

    static AbstractInterp4Command* CreateCmd();
};

#endif
