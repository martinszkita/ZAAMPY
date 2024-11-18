#ifndef INTERP4PAUSE_HH
#define INTERP4PAUSE_HH

#include "AbstractInterp4Command.hh"
#include <iostream>
#include <string>
#include <sstream>


class Interp4Pause: public AbstractInterp4Command{
    std::string _robotName;
    unsigned int _pause_time_ms;

    public:
        Interp4Pause();
        virtual void PrintCmd() const override;
        virtual const char* GetCmdName() const override;
        virtual bool ExecCmd( AbstractScene&rScn, const char *sMobObjName, AbstractComChannel &rComChann ) override;
        virtual bool ReadParams(std::istream& strm) override;
        virtual void PrintSyntax() const override;
        virtual void PrintParams() const override;
        static AbstractInterp4Command* CreateCmd();

        //const std::string& GetRobotName() const;
        const unsigned int & GetPauseTime() const;
        //void SetRobotName(const std::string& name);
        void SetPauseTime(const unsigned int & _ms);
};


#endif