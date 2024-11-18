#ifndef INTERP4SET__HH
#define INTERP4SET__HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include "AbstractInterp4Command.hh"
#include <iostream>
#include <string>
#include <sstream>

class Interp4Set: public AbstractInterp4Command{
    std::string _robotName;
    Vector3D pos;
    double ang_x_deg;
    double ang_y_deg;
    double ang_z_deg;

    public:
        Interp4Set();
        virtual void PrintCmd() const override;
        virtual const char* GetCmdName() const override;
        virtual bool ExecCmd( AbstractScene&rScn, const char *sMobObjName, AbstractComChannel &rComChann ) override;
        virtual bool ReadParams(std::istream& strm) override;
        virtual void PrintSyntax() const override;
        virtual void PrintParams() const override;
        static AbstractInterp4Command* CreateCmd();

        const std::string& GetRobotName() const;
        const Vector3D& GetPosition() const;
        double GetAngleX() const;
        double GetAngleY() const;
        double GetAngleZ() const;

        // Setters
        void SetRobotName(const std::string& name);
        void SetPosition(const Vector3D& position);
        void SetAngleX(double value);
        void SetAngleY(double value);
        void SetAngleZ(double value);


};

#endif