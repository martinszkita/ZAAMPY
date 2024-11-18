#ifndef INTERP4ROTATE__HH
#define INTERP4ROTATE__HH

#include "AbstractInterp4Command.hh"
#include "Vector3D.hh"
#include <string>
#include <iostream>

class Interp4Rotate : public AbstractInterp4Command {
private:
    std::string _robotName;
    char _axis;
    double _angularVelocity; 
    double _angle;          

public:
    Interp4Rotate();

    const std::string& GetRobotName() const;
    char GetAxis() const;
    double GetAngularVelocity() const;
    double GetAngle() const;

    void SetRobotName(const std::string& name);
    void SetAxis(char axis);
    void SetAngularVelocity(double angularVelocity);
    void SetAngle(double angle);

    virtual void PrintCmd() const override;
    virtual const char* GetCmdName() const override;
    virtual bool ExecCmd( AbstractScene&rScn, const char *sMobObjName, AbstractComChannel &rComChann ) override;
    virtual bool ReadParams(std::istream& strm) override;
    virtual void PrintSyntax() const override;
    virtual void PrintParams() const override;

    static AbstractInterp4Command* CreateCmd();
};

#endif
