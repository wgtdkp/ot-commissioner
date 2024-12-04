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
 *   This file defines the SWIG interface of the commissioner.
 *
 * See http://www.swig.org for more information about SWIG.
 *
 */

// Defines %ignoreall and %unignore for explicit API declaration.
// Usage:
//
// %ignoreall
// %unignore SomeName;   // explcitly allow an API
// %include "somelib.h"
#ifndef ignoreall
%define %ignoreall %ignore ""; %enddef
#endif

#ifndef unignore
%define %unignore %rename("%s") %enddef
#endif

#ifndef unignore_method
%define %unignore_method %rename("%(lowercamelcase)s") %enddef
#endif

#ifndef unignore_field
%define %unignore_field %rename("%(regex:/^(m)(.*)/\\2/)s") %enddef
#endif

#ifndef unignoreall
%define %unignoreall %rename("%s") ""; %enddef
#endif

%ignoreall

%module(directors="1") commissionerModule

%{
#include <commissioner/defines.hpp>
#include <commissioner/error.hpp>
#include <commissioner/network_data.hpp>
#include <commissioner/commissioner.hpp>
%}

%include <std_shared_ptr.i>
%include <std_string.i>
%include <std_vector.i>
%include <stl.i>
%include <typemaps.i>

// This is intentional.
//
// We know that Java has no unsigned integers and SWIG maps
// Java short to C++ uint8_t (it is done in `stdint.i`). But
// it is natural for Java to represent a byte sequence in byte[]
// or AbstractList<byte> rather than a AbstractList<short>.
//
// %include <stdint.i>

// This does the trick that we direct SWIG use the same typemap of
// `signed char` for `uint8_t` in only context of `std::vector<uint8_t>`.
%apply signed char { uint8_t };
%apply const signed char & { const uint8_t & };
%template(ByteArray) std::vector<uint8_t>;

// Override the typemap of `uint8_t`.
%apply unsigned char { uint8_t };
%apply const unsigned char & { const uint8_t & };
%apply unsigned char & OUTPUT { uint8_t &aStatus };

%apply unsigned short { uint16_t };
%apply const unsigned short & { const uint16_t & };
%apply unsigned int { uint32_t };
%apply const unsigned int & { const uint32_t & };
%apply unsigned long long { uint64_t };
%apply const unsigned long long & { const uint64_t & };

// Remove the 'm' prefix of all members.
// %rename("%(regex:/^(m)(.*)/\\2/)s") "";

// Convert first character of function names to lowercase.
// %rename("%(firstlowercase)s", %$isfunction) "";

// Insert the code of loading native shared library into generated Java class.
%pragma(java) jniclasscode=%{
    static {
        try {
            System.loadLibrary("commissioner-java");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("failed to load native commissioner library!\n" + e);
            System.exit(1);
        }
    }
%}

%feature("director") ot::commissioner::CommissionerHandler;
%feature("director") ot::commissioner::Logger;

%template(ChannelMask) std::vector<ot::commissioner::ChannelMaskEntry>;
%template(StringVector) std::vector<std::string>;

%typemap(jstype) std::string& OUTPUT "String[]"
%typemap(jtype)  std::string& OUTPUT "String[]"
%typemap(jni)    std::string& OUTPUT "jobjectArray"
%typemap(javain) std::string& OUTPUT "$javainput"
%typemap(in)     std::string& OUTPUT (std::string temp) {
    if (!$input) {
        SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
        return $null;
    }
    if (JCALL1(GetArrayLength, jenv, $input) == 0) {
        SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
    }
    $1 = &temp;
}
%typemap(argout) std::string& OUTPUT {
  jstring jvalue = JCALL1(NewStringUTF, jenv, temp$argnum.c_str()); 
  JCALL3(SetObjectArrayElement, jenv, $input, 0, jvalue);
}

%apply std::string& OUTPUT { std::string &aMeshLocalAddr }
%apply std::string& OUTPUT { std::string &aExistingCommissionerId }

%shared_ptr(ot::commissioner::Logger)
%shared_ptr(ot::commissioner::Commissioner)

%unignore ot;
%unignore ot::commissioner;
%unignore ot::commissioner::Commissioner;
%unignore_method ot::commissioner::Commissioner::Create;
%unignore_method ot::commissioner::Commissioner::Init;
%unignore_method ot::commissioner::Commissioner::GetConfig;
%unignore_method ot::commissioner::Commissioner::Connect(const std::string &aAddr, uint16_t aPort);
%unignore_method ot::commissioner::Commissioner::Disconnect;
%unignore_method ot::commissioner::Commissioner::GetSessionId;
%unignore_method ot::commissioner::Commissioner::GetState;
%unignore_method ot::commissioner::Commissioner::IsActive;
%unignore_method ot::commissioner::Commissioner::IsCcmMode;
%unignore_method ot::commissioner::Commissioner::GetDomainName;
%unignore_method ot::commissioner::Commissioner::CancelRequests;
%unignore_method ot::commissioner::Commissioner::Petition(std::string &aExistingCommissionerId, const std::string &aAddr, uint16_t aPort);
%unignore_method ot::commissioner::Commissioner::Resign();
%unignore_method ot::commissioner::Commissioner::GetCommissionerDataset(CommissionerDataset &aDataset, uint16_t aDatasetFlags);
%unignore_method ot::commissioner::Commissioner::SetCommissionerDataset(const CommissionerDataset &aDataset);
%unignore_method ot::commissioner::Commissioner::SetBbrDataset(const BbrDataset &aDataset);
%unignore_method ot::commissioner::Commissioner::GetBbrDataset(BbrDataset &aDataset, uint16_t aDatasetFlags);
%unignore_method ot::commissioner::Commissioner::GetActiveDataset(ActiveOperationalDataset &aDataset, uint16_t aDatasetFlags);
%unignore_method ot::commissioner::Commissioner::GetRawActiveDataset(ByteArray &aRawDataset, uint16_t aDatasetFlags);
%unignore_method ot::commissioner::Commissioner::SetActiveDataset(const ActiveOperationalDataset &aActiveDataset);
%unignore_method ot::commissioner::Commissioner::GetPendingDataset(PendingOperationalDataset &aDataset, uint16_t aDatasetFlags);
%unignore_method ot::commissioner::Commissioner::SetPendingDataset(const PendingOperationalDataset &aPendingDataset);
%unignore_method ot::commissioner::Commissioner::SetSecurePendingDataset(uint32_t aMaxRetrievalTimer, const PendingOperationalDataset &aDataset);
%unignore_method ot::commissioner::Commissioner::CommandReenroll(const std::string &aDstAddr);
%unignore_method ot::commissioner::Commissioner::CommandDomainReset(const std::string &aDstAddr);
%unignore_method ot::commissioner::Commissioner::CommandMigrate(const std::string &aDstAddr, const std::string &aDesignatedNetwork);
%unignore_method ot::commissioner::Commissioner::AnnounceBegin(uint32_t aChannelMask, uint8_t aCount, uint16_t aPeriod, const std::string &aDstAddr);
%unignore_method ot::commissioner::Commissioner::PanIdQuery(uint32_t aChannelMask, uint16_t aPanId, const std::string &aDstAddr);
%unignore_method ot::commissioner::Commissioner::EnergyScan(uint32_t aChannelMask, uint8_t aCount, uint16_t aPeriod, uint16_t aScanDuration, const std::string &aDstAddr);
%unignore_method ot::commissioner::Commissioner::RegisterMulticastListener(uint8_t &aStatus, const std::vector<std::string> &aMulticastAddrList, uint32_t aTimeout);
%unignore_method ot::commissioner::Commissioner::RequestToken(ByteArray &aSignedToken, const std::string &aAddr, uint16_t aPort);
%unignore_method ot::commissioner::Commissioner::SetToken(const ByteArray &aSignedToken);
%unignore_method ot::commissioner::Commissioner::GeneratePSKc(ByteArray &aPSKc, const std::string &aPassphrase, const std::string &aNetworkName, const ByteArray &aExtendedPanId);
%unignore_method ot::commissioner::Commissioner::ComputeJoinerId(uint64_t aEui64);
%unignore_method ot::commissioner::Commissioner::AddJoiner(ByteArray &aSteeringData, const ByteArray &aJoinerId);
%unignore_method ot::commissioner::Commissioner::GetVersion(void);

%unignore ot::commissioner::State;
%unignore ot::commissioner::State::kDisabled;
%unignore ot::commissioner::State::kConnected;
%unignore ot::commissioner::State::kPetitioning;
%unignore ot::commissioner::State::kActive;

%unignore ot::commissioner::ActiveOperationalDataset;
%unignore_field ot::commissioner::ActiveOperationalDataset::mActiveTimestamp;
%unignore_field ot::commissioner::ActiveOperationalDataset::mExtendedPanId;
%unignore_field ot::commissioner::ActiveOperationalDataset::mPanId;
%unignore_field ot::commissioner::ActiveOperationalDataset::mNetworkName;
%unignore_field ot::commissioner::ActiveOperationalDataset::mChannel;
%unignore_field ot::commissioner::ActiveOperationalDataset::mChannelMask;
%unignore_field ot::commissioner::ActiveOperationalDataset::mMeshLocalPrefix;
%unignore_field ot::commissioner::ActiveOperationalDataset::mNetworkMasterKey;
%unignore_field ot::commissioner::ActiveOperationalDataset::mPSKc;
%unignore_field ot::commissioner::ActiveOperationalDataset::mSecurityPolicy;
%unignore_field ot::commissioner::ActiveOperationalDataset::mPresentFlags;
%unignore ot::commissioner::ActiveOperationalDataset::kActiveTimestampBit;
%unignore ot::commissioner::ActiveOperationalDataset::kChannelBit;
%unignore ot::commissioner::ActiveOperationalDataset::kChannelMaskBit;
%unignore ot::commissioner::ActiveOperationalDataset::kExtendedPanIdBit;
%unignore ot::commissioner::ActiveOperationalDataset::kMeshLocalPrefixBit;
%unignore ot::commissioner::ActiveOperationalDataset::kNetworkMasterKeyBit;
%unignore ot::commissioner::ActiveOperationalDataset::kNetworkNameBit;
%unignore ot::commissioner::ActiveOperationalDataset::kPanIdBit;
%unignore ot::commissioner::ActiveOperationalDataset::kPSKcBit;
%unignore ot::commissioner::ActiveOperationalDataset::kSecurityPolicyBit;
%unignore_method ot::commissioner::ActiveOperationalDataset::ActiveOperationalDataset;



%include <commissioner/defines.hpp>
%include <commissioner/error.hpp>
%include <commissioner/network_data.hpp>
%include <commissioner/commissioner.hpp>
%include <commissioner/commissioner.hpp>

%unignoreall
