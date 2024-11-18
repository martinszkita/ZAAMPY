#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH
#include "AbstractComChannel.hh"
#include <iostream>
#include <mutex>

class ComChannel : public AbstractComChannel {
    int socket;
    std::mutex access_mutex;
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

#endif
