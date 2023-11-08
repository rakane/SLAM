#include "PWM.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

BBIO::PWM::PWM(PWMLabel pwmLabel) :
  pwmLabel_(pwmLabel),
  pwmPin_(PWMPin[pwmLabel]),
  pwmChip_(PWMChip[pwmLabel]),
  pwmNumber_(PWMNumber[pwmLabel]),
  dutyCycleInNs_(0),
  periodInNs_(0),
  enabled_(false)
{
    std::cout << "PWM::PWM()" << std::endl;

    pwmChipFilePath_ = "/sys/class/pwm/" + pwmChip_ + "/";
    pwmPinFilePath_ = "/sys/class/pwm/" + pwmChip_ + "/pwm" + pwmNumber_ + "/";
}

BBIO::PWM::~PWM()
{
    std::cout << "PWM::~PWM()" << std::endl;
}

bool BBIO::PWM::configure()
{
    // Configure PWM pin
    system(("config-pin " + pwmPin_ + " pwm").c_str());

    // Wait for OS to set up PWM pin
    usleep(100000);

    // Check that pwmChipFilePath_/export file exists
    std::ofstream exportFile(pwmChipFilePath_ + "export");
    if (!exportFile.good())
    {
        std::cout << "PWM::configure() - " << pwmChipFilePath_ + "export" << " does not exist" << std::endl;
        return false;
    }

    // Export PWM pin
    exportFile << pwmNumber_;
    exportFile.flush();
    exportFile.close();

    // OS needs to set up PWM pin and expose directory
    usleep(100000);

    // Check that pwmPinFilePath_ exists
    std::ofstream pwmPinFile(pwmPinFilePath_ + "enable");
    if (!pwmPinFile.good())
    {
        std::cout << "PWM::configure() - " << pwmPinFilePath_ << " does not exist" << std::endl;
        return false;
    }
    pwmPinFile.close();

    // Default to disabled
    setEnabled(false);
    setDutyCycle(0);
    setPeriod(0);
}

bool BBIO::PWM::setDutyCycle(unsigned int dutyCycleInNs)
{
    dutyCycleInNs_ = dutyCycleInNs;

    std::string dutyCycleFile = pwmPinFilePath_ + "duty_cycle";
    try
    {
        system(("echo " + std::to_string(dutyCycleInNs_) + " > " + dutyCycleFile).c_str());
    }
    catch (const std::exception& e)
    {
        std::cout << "PWM::setDutyCycle() - " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool BBIO::PWM::setPeriod(unsigned int periodInNs)
{
    periodInNs_ = periodInNs;

    std::string periodFile = pwmPinFilePath_ + "period";
    try
    {
        system(("echo " + std::to_string(periodInNs_) + " > " + periodFile).c_str());
    }
    catch (const std::exception& e)
    {
        std::cout << "PWM::setPeriod() - " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool BBIO::PWM::setEnabled(bool enabled)
{
    enabled_ = enabled;

    int enableValue = enabled_ ? 1 : 0;

    std::string enabledFile = pwmPinFilePath_ + "enable";
    try
    {
        system(("echo " + std::to_string(enableValue) + " > " + enabledFile).c_str());
    }
    catch (const std::exception& e)
    {
        std::cout << "PWM::setEnabled() - " << e.what() << std::endl;
        return false;
    }

    return true;
}

