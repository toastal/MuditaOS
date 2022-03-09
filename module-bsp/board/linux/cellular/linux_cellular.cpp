// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "linux_cellular.hpp"
#include <module-bsp/bsp/cellular/CellularResult.hpp>

#include <iostream>
#include <algorithm>

#include <log/log.hpp>
#include "mutex.hpp"
#include <EventStore.hpp>
#include <errno.h>
#include <fcntl.h>
#include <map>
#include <sys/types.h>
#include <ticks.hpp>
#include <unistd.h>

#define _LINUX_UART_DEBUG 0

namespace bsp
{

    std::map<uint32_t, speed_t> PortSpeeds_text = {
        {9600U, B9600}, {19200U, B19200}, {38400U, B38400}, {57600U, B57600}, {115200U, B115200}, {230400U, B230400}};

    LinuxCellular::LinuxCellular(const char *term, uint32_t portSpeed)
    {

        if (strcmp(term, "0") == 0) {
            fd.fd = 0;
        }
        else {
            // open serial port
            fd.fd = open(term, O_RDWR | O_NOCTTY | O_NONBLOCK);
            if (fd.fd == -1) {
                LOG_FATAL("Failed to open serial port: %s (%d)", term, portSpeed);
                return;
            }

            struct termios t;
            memset(&t, 0, sizeof(t));
            tcgetattr(fd.fd, &t);
            cfmakeraw(&t);
            t.c_cflag |= CLOCAL;
            // if(ctsrts == 1)
            //  t.c_cflag |= CRTSCTS; //enable the flow control on dev board
            // else
            t.c_cflag &= ~(CRTSCTS);                    // disable the flow control on dev board
            speed_t speed = PortSpeeds_text[portSpeed]; // B115200;
            cfsetispeed(&t, speed);
            cfsetospeed(&t, speed);
            tcsetattr(fd.fd, TCSANOW, &t);
            int status = TIOCM_DTR | TIOCM_RTS;
            ioctl(fd.fd, TIOCMBIS, &status);

            fd.events = POLLIN;
        }

        isInitialized = true;
    }

    LinuxCellular::~LinuxCellular()
    {
        if (fd.fd >= 0) {
            close(fd.fd);
        }
    }

    void LinuxCellular::powerUp()
    {}

    void LinuxCellular::powerDown()
    {}

    void LinuxCellular::restart()
    {}

    ssize_t LinuxCellular::read(void *buf, size_t nbytes, std::chrono::milliseconds timeoutMs)
    {

        cpp_freertos::LockGuard lock(serOutMutex);

        auto buffer = static_cast<bsp::cellular::CellularDMAResultStruct *>(buf);

        int ret;
        for (;;) {
            ret = ::read(fd.fd, buffer->data, nbytes);
            if (ret != -1 || errno != EINTR)
                break;
        }
        if (ret > 0) {
            buffer->resultCode = bsp::cellular::CellularResultCode::ReceivedAndIdle;
            buffer->dataSize   = ret;
        }
#if _LINUX_UART_DEBUG
        if (ret > 0) {
            LOG_PRINTF("[RX] ");
            uint8_t *ptr = (uint8_t *)buffer->data;
            for (size_t i = 0; i < ret; i++)
                LOG_PRINTF("%02X ", (uint8_t)*ptr++);
            LOG_PRINTF("\n");
        }
#endif
        return ret;
    }

    ssize_t LinuxCellular::write(void *buf, size_t nbytes)
    {
        cpp_freertos::LockGuard lock(serOutMutex);
#if _LINUX_UART_DEBUG
        LOG_PRINTF("[TX] ");
        uint8_t *ptr = (uint8_t *)buf;
        for (size_t i = 0; i < nbytes; i++)
            LOG_PRINTF("%02X ", (uint8_t)*ptr++);
        LOG_PRINTF("\n");
#endif
        int ret;
        for (;;) {
            ret = ::write(fd.fd, buf, nbytes);
            if (ret != -1 || errno != EINTR)
                break;
        }
        return ret;
    }

    void LinuxCellular::informModemHostAsleep(void)
    {}

    void LinuxCellular::informModemHostWakeup(void)
    {}

    void LinuxCellular::enterSleep()
    {}

    void LinuxCellular::exitSleep()
    {}

    uint32_t LinuxCellular::wait(std::chrono::milliseconds timeoutMs)
    {
        auto timeoutTicks = pdMS_TO_TICKS(timeoutMs.count());

        uint32_t currentTime   = cpp_freertos::Ticks::GetTicks();
        uint64_t timeoutNeeded = currentTime + timeoutTicks;
        uint64_t timeElapsed   = currentTime;

        for (;;) {
            if (timeElapsed >= timeoutNeeded) {
                return 0; // timeout
            }

            const auto ret = poll(&fd, 1, timeoutNeeded - timeElapsed);
            timeElapsed    = cpp_freertos::Ticks::GetTicks();

            if (ret == 0) {
                return 0; // timeout
            }
            else if ((ret == -1) && (errno == EINTR)) {
                continue;
            }
            else {
                return 1;
            }
        }
    }

    void LinuxCellular::setSpeed(uint32_t portSpeed)
    {
        struct termios t;
        memset(&t, 0, sizeof(t));
        tcgetattr(fd.fd, &t);
        cfmakeraw(&t);
        t.c_cflag |= CLOCAL;
        // if(ctsrts == 1)
        //  t.c_cflag |= CRTSCTS; //enable the flow control on dev board
        // else
        t.c_cflag &= ~(CRTSCTS);                    // disable the flow control on dev board
        speed_t speed = PortSpeeds_text[portSpeed]; // B115200;
        cfsetispeed(&t, speed);
        cfsetospeed(&t, speed);
        tcsetattr(fd.fd, TCSANOW, &t);
        int status = TIOCM_DTR | TIOCM_RTS;
        ioctl(fd.fd, TIOCMBIS, &status);
    }

    void LinuxCellular::selectAntenna(bsp::cellular::antenna antenna)
    {}

    bsp::cellular::antenna LinuxCellular::getAntenna()
    {
        return bsp::cellular::antenna::lowBand;
    }
    bsp::Board LinuxCellular::getBoard()
    {
        return bsp::Board::Linux;
    }

    namespace cellular
    {
        auto init(QueueHandle_t qHandle) -> int
        {
            return 0;
        }

        namespace USB
        {
            BootPinState getBootPin()
            {
                return BootPinState::NORMAL_BOOT;
            }

            PassthroughState getPassthrough()
            {
                return PassthroughState::DISABLED;
            }

            void setBootPin(BootPinState dfu){};
            void setPassthrough(PassthroughState pass){};
        } // namespace USB

        namespace status
        {

            bsp::cellular::status::value getStatus()
            {
                return bsp::cellular::status::value::ACTIVE;
            }

            BaseType_t statusIRQhandler()
            {
                return pdFALSE;
            }
        } // namespace status
        namespace sim
        {

            auto trayIRQHandler() -> BaseType_t
            {
                return BaseType_t();
            }

            auto getTray() -> Store::GSM::Tray
            {
                return Store::GSM::Tray::IN;
            }

            void hotSwapTrigger()
            {}

            void simSelect()
            {}
        } // namespace sim
        namespace ringIndicator
        {
            auto riIRQHandler() -> BaseType_t
            {
                return BaseType_t();
            }
        } // namespace ringIndicator
    }     // namespace cellular
} // namespace bsp
