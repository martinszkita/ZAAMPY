#include "AbstractMobileObj.hh"
#include "Vector3D.hh"

class MobileObj : public AbstractMobileObj {
  private:
    Vector3D _position;
    double _roll;
    double _pitch;
    double _yaw;
    std::string _name;

  public:
    const Vector3D & GetPosition_m() const override;
    void SetPosition_m(const Vector3D &rPos) override;
    double GetAng_Roll_deg() const override;
    void SetAng_Roll_deg(double Ang_Roll_deg) override ;
    double GetAng_Pitch_deg() const override;
    void SetAng_Pitch_deg(double Ang_Pitch_deg) override;
    double GetAng_Yaw_deg() const override;
    void SetAng_Yaw_deg(double Ang_Yaw_deg) override;
    const std::string & GetName() const override;
    void SetName(const char* sName) override;
};
