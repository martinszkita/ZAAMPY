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
    const Vector3D & GetPosition_m() const override {
      return _position;
    }

    void SetPosition_m(const Vector3D &rPos) override {
      _position = rPos;
    }

    double GetAng_Roll_deg() const override {
      return _roll;
    }

    void SetAng_Roll_deg(double Ang_Roll_deg) override {
      _roll = Ang_Roll_deg;
    }

    double GetAng_Pitch_deg() const override {
      return _pitch;
    }

    void SetAng_Pitch_deg(double Ang_Pitch_deg) override {
      _pitch = Ang_Pitch_deg;
    }

    double GetAng_Yaw_deg() const override {
      return _yaw;
    }

    void SetAng_Yaw_deg(double Ang_Yaw_deg) override {
      _yaw = Ang_Yaw_deg;
    }

    const std::string & GetName() const override {
      return _name;
    }

    void SetName(const char* sName) override {
      _name = std::string(sName);
    }
};
