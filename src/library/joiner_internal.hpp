/*
 *    Copyright (c) 2019, The OpenThread Commissioner Authors.
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
 *   This file includes definition of internal joiner functions.
 */

#ifndef OT_COMM_LIBRARY_JOINER_INTERNAL_HPP_
#define OT_COMM_LIBRARY_JOINER_INTERNAL_HPP_

#include <commissioner/error.hpp>
#include <commissioner/joiner.hpp>

namespace ot {

namespace commissioner {

/**
 * Validate if the joiner contains valid information.
 *
 * @param aJoinerInfo  The given joiner jnfo.
 *
 * @return Error::kNone         The joiner info is valid;
 * @return Error::kInvalidArgs  The joiner info is invalid;
 *
 */
Error ValidateJoiner(const JoinerInfo &aJoinerInfo);

/**
 * Derive Joiner ID from EUI64, defined by the Thread 1.1 specification.
 *
 * @param[in] aEui64  The joiner EUI64.
 *
 * @return The Joiner ID;
 *
 */
ByteArray ComputeJoinerId(uint64_t aEui64);

/**
 * Derive Joiner ID from Discerner.
 *
 * Copies the discerner value up to its bit length to the Joiner ID
 * array, assuming big-endian encoding (i.e., the discerner lowest bits
 * are copied at end of the array). Any initial/remaining
 * bits of the Joiner ID array remain unchanged.
 *
 * @param[in] aDiscerner  The joiner discerner.
 *
 * @return The Joiner ID.
 *
 */
ByteArray ComputeJoinerId(const JoinerDiscerner &aDiscerner);

} // namespace commissioner

} // namespace ot

#endif // OT_COMM_LIBRARY_JOINER_INTERNAL_HPP_
