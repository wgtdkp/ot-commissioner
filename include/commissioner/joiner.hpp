/*
 *    Copyright (c) 2020, The OpenThread Commissioner Authors.
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
 *   The file defines the joiner information.
 *
 */

#ifndef OT_COMM_JOINER_HPP_
#define OT_COMM_JOINER_HPP_

#include <chrono>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <unordered_set>

#include <commissioner/commissioner.hpp>
#include <commissioner/network_data.hpp>

#include "common/address.hpp"

namespace ot {

namespace commissioner {

/**
 * @brief Enumeration of Joiner Type for steering.
 *
 */
enum class JoinerType : uint8_t
{
    kMeshCoPAny,       ///< Indicates accepting any meshcop joiner.
    kMeshCoPEui64,     ///< Conventional non-CCM joiner.
    kMeshCoPDiscerner, ///< Conventional non-CCM joiner identifier by discerner.
    kCcmAe,            ///< CCM AE joiner.
    kCcmNmkp,          ///< CCM NMKP joiner.
};

struct JoinerDiscerner
{
    uint64_t mData;
    size_t   mBitLength;
};

/**
 * @brief Definition of joiner information.
 *
 */
struct JoinerInfo
{
    JoinerType mType;

    /**
     * The joiner EUI64.
     *
     * Must be set if mType is kMeshCoP.
     *
     * @note: Zero indicates an invalid error.
     *
     */
    uint64_t mEui64 = 0;

    /**
     * The joiner discerner.
     *
     * Must present if mType is kMeshCoPDiscerner.
     *
     */
    JoinerDiscerner mDiscerner;

    /**
     * The joiner pre-shared device key.
     *
     * Must present if mType is kMeshCoP, kMeshCoPAny or kMeshCoPDiscerner.
     *
     */
    std::string mPSKd;

    /**
     * The URL used for vendor-specific provisioning.
     *
     * Optional.
     * Set/get only if mType is kMeshCoP, kMeshCoPAny or kMeshCoPDiscerner.
     *
     */
    std::string mProvisioningUrl;
};

} // namespace commissioner

} // namespace ot

#endif // OT_COMM_JOINER_HPP_
