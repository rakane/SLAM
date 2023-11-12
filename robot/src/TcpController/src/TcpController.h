#ifndef TCP_CONTROLLER_H
#define TCP_CONTROLLER_H

#include <mutex>

namespace SLAM
{
    class MotorControllerInterface;
}

class TcpController
{
public:
    TcpController(SLAM::MotorControllerInterface* motorController);
    ~TcpController();

    void run();

    void terminate();

private:

    SLAM::Direction commandToMotorDirection(const char command);

    enum class TcpControllerState
    {
        IDLE,
        RUNNING,
        TERMINATED
    };

    std::mutex mutex_;
    TcpControllerState state_;

    SLAM::MotorControllerInterface* motorController_;
};

#endif // TCP_CONTROLLER_H
