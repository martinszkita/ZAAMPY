#include "AbstractComChannel.hh"
#include <iostream>

class ComChannel : public AbstractComChannel {
    int fd;
public:
    ComChannel() = default;
    ~ComChannel() = default;

    int GetSocket() const  override;
    bool Init(int Socket) override;
    void LockAccess() override;
    void UnlockAccess() override;
    std::mutex &UseGuard() override;

    void SendMoveCommand(const std::string& objectName, double speed, double distance);

};
