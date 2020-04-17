/*
 *    Copyright (c) 2020, The OpenThread Authors.
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *    POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *  The file defines file logger.
 *
 */

#include "file_logger.hpp"

#include <ctime>

#include <utils.hpp>

namespace ot {

namespace commissioner {

static std::string ToString(LogLevel aLevel)
{
    switch (aLevel)
    {
    case LogLevel::kOff:
        return "off";
    case LogLevel::kCritical:
        return "critical";
    case LogLevel::kError:
        return "error";
    case LogLevel::kWarn:
        return "warn";
    case LogLevel::kInfo:
        return "info";
    case LogLevel::kDebug:
        return "debug";
    default:
        ASSERT(false);
        return "unknown";
    }
}

FileLogger::FileLogger(const std::string &aFilename, ot::commissioner::LogLevel aLogLevel)
    : mFileStream(aFilename)
    , mLogLevel(aLogLevel)
{
}

void FileLogger::Log(ot::commissioner::LogLevel aLevel, const std::string &aMsg)
{
    char        dateBuf[64];
    std::time_t now = std::time(nullptr);

    VerifyOrExit(aLevel <= mLogLevel);
    VerifyOrExit(mFileStream.is_open());

    std::strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    mFileStream << "[ " << dateBuf << " ] [ " << ToString(aLevel) << " ] " << aMsg << std::endl;

exit:
    return;
}

} // namespace commissioner

} // namespace ot
