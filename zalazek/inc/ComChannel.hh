#include "AbstractComChannel.hh"
#include <iostream>

class ComChannel : public AbstractComChannel {
public:
    ComChannel() = default;
    ~ComChannel() = default;

    void SendMoveCommand(const std::string& objectName, double speed, double distance) override {
        std::cout << "Wysyłanie komendy ruchu dla " << objectName
                  << " z prędkością " << speed << " i odległością " << distance << std::endl;
    }
};
