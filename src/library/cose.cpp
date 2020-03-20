/*
 *  Copyright (c) 2019, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements COSE.
 */

#include "cose.hpp"

#include <mbedtls/bignum.h>
#include <mbedtls/ecp.h>

#include <error_macros.hpp>
#include <utils.hpp>

#include "mbedtls_error.hpp"

namespace ot {

namespace commissioner {

namespace cose {

Sign1Message::Sign1Message(void)
    : mSign(nullptr)
{
}

void Sign1Message::Free(void)
{
    if (mSign != nullptr)
    {
        COSE_Sign0_Free(mSign);
        mSign = nullptr;
    }
}

Error Sign1Message::Init(int aCoseInitFlags)
{
    mSign = COSE_Sign0_Init(static_cast<COSE_INIT_FLAGS>(aCoseInitFlags), nullptr);
    return mSign ? ERROR_NONE : ERROR_OUT_OF_MEMORY("create COSE SIGN1 message");
}

Error Sign1Message::Serialize(ByteArray &aBuf)
{
    size_t length;
    length = COSE_Encode((HCOSE)mSign, nullptr, 0, 0) + 1;
    aBuf.resize(length);
    length = COSE_Encode((HCOSE)mSign, aBuf.data(), 0, aBuf.size());
    aBuf.resize(length);

    return ERROR_NONE;
}

Error Sign1Message::Deserialize(Sign1Message &aCose, const ByteArray &aBuf)
{
    if (aBuf.empty())
    {
        return ERROR_INVALID_ARGS("COSE SIGN1 message must not be empty");
    }

    int         type;
    HCOSE_SIGN0 sign =
        reinterpret_cast<HCOSE_SIGN0>(COSE_Decode(aBuf.data(), aBuf.size(), &type, COSE_sign0_object, nullptr));
    if (sign == nullptr || type != COSE_sign0_object)
    {
        return ERROR_BAD_FORMAT("deserialize COSE SIGN1 message");
    }

    aCose.mSign = sign;
    return ERROR_NONE;
}

Error Sign1Message::Validate(const CborMap &aCborPublicKey)
{
    if (!aCborPublicKey.IsValid())
    {
        return ERROR_INVALID_ARGS("validate COSE SIGN1 message with invalid public key");
    }

    if (!COSE_Sign0_validate(mSign, aCborPublicKey.GetImpl(), nullptr))
    {
        return ERROR_SECURITY("validate COSE SIGN1 message failed");
    }
    return ERROR_NONE;
}

Error Sign1Message::Validate(const mbedtls_pk_context &aPubKey)
{
    const struct mbedtls_ecp_keypair *eckey;

    // Accepts only EC keys
    if (!mbedtls_pk_can_do(&aPubKey, MBEDTLS_PK_ECDSA) || (eckey = mbedtls_pk_ec(aPubKey)) == nullptr)
    {
        return ERROR_INVALID_ARGS("validate COSE SIGN1 message without valid EC public key");
    }

    if (!COSE_Sign0_validate_eckey(mSign, eckey, nullptr))
    {
        return ERROR_SECURITY("validate COSE SIGN1 message failed");
    }

    return ERROR_NONE;
}

Error Sign1Message::Sign(const mbedtls_pk_context &aPrivateKey)
{
    const mbedtls_ecp_keypair *eckey;

    // Accepts only EC keys
    if (!mbedtls_pk_can_do(&aPrivateKey, MBEDTLS_PK_ECDSA) || (eckey = mbedtls_pk_ec(aPrivateKey)) == nullptr)
    {
        return ERROR_INVALID_ARGS("sign COSE SIGN1 message without valid EC private key");
    }

    if (!COSE_Sign0_Sign_eckey(mSign, eckey, nullptr))
    {
        return ERROR_SECURITY("sign COSE SIGN1 message failed");
    }

    return ERROR_NONE;
}

Error Sign1Message::SetContent(const ByteArray &aContent)
{
    if (!aContent.empty())
    {
        if (!COSE_Sign0_SetContent(mSign, &aContent[0], aContent.size(), nullptr))
        {
            return ERROR_UNKNOWN("set COSE SIGN1 message content");
        }
    }
    else
    {
        uint8_t emptyContent;
        if (!COSE_Sign0_SetContent(mSign, &emptyContent, 0, nullptr))
        {
            return ERROR_UNKNOWN("set COSE SIGN1 message content");
        }
    }

    return ERROR_NONE;
}

Error Sign1Message::SetExternalData(const ByteArray &aExternalData)
{
    if (aExternalData.empty())
    {
        return ERROR_INVALID_ARGS("cannot set COSE SIGN1 message to empty external data");
    }

    if (!COSE_Sign0_SetExternal(mSign, aExternalData.data(), aExternalData.size(), nullptr))
    {
        return ERROR_UNKNOWN("set COSE SIGN1 message external data failed");
    }

    return ERROR_NONE;
}

Error Sign1Message::AddAttribute(int key, int value, int flags)
{
    if (cn_cbor *cbor = cn_cbor_int_create(value, nullptr))
    {
        auto error = AddAttribute(key, cbor, flags);
        if (cbor != nullptr && cbor->parent == nullptr)
        {
            cn_cbor_free(cbor);
        }
        return error;
    }
    else
    {
        return ERROR_OUT_OF_MEMORY("add COSE SIGN1 message attribute");
    }
}

Error Sign1Message::AddAttribute(int aKey, const ByteArray &aValue, int aFlags)
{
    if (aValue.empty())
    {
        return ERROR_INVALID_ARGS("add empty COSE SIGN1 message attribute");
    }

    if (cn_cbor *cbor = cn_cbor_data_create(aValue.data(), aValue.size(), nullptr))
    {
        auto error = AddAttribute(aKey, cbor, aFlags);
        if (cbor != nullptr && cbor->parent == nullptr)
        {
            cn_cbor_free(cbor);
        }
        return error;
    }
    else
    {
        return ERROR_OUT_OF_MEMORY("add COSE SIGN1 message attribute");
    }
}

Error Sign1Message::AddAttribute(int key, cn_cbor *value, int flags)
{
    if (!COSE_Sign0_map_put_int(mSign, key, value, flags, nullptr))
    {
        return ERROR_UNKNOWN("add COSE SIGN1 message attribute");
    }

    return ERROR_NONE;
}

static cn_cbor *CborArrayAt(cn_cbor *arr, size_t index)
{
    if (index >= static_cast<size_t>(arr->length))
    {
        return nullptr;
    }

    cn_cbor *ele = arr->first_child;
    for (size_t i = 0; i < index; ++i)
    {
        ele = ele->next;
    }
    return ele;
}

const uint8_t *Sign1Message::GetPayload(size_t &aLength)
{
    const uint8_t *ret = nullptr;
    cn_cbor *      cbor;
    cn_cbor *      payload;

    ASSERT(mSign != nullptr);
    if ((cbor = COSE_get_cbor(reinterpret_cast<HCOSE>(mSign))) == nullptr)
    {
        return nullptr;
    }

    if (cbor->type != CN_CBOR_ARRAY || (payload = CborArrayAt(cbor, 2)) == nullptr)
    {
        return nullptr;
    }

    if (payload->type != CN_CBOR_BYTES)
    {
        return nullptr;
    }

    ret     = payload->v.bytes;
    aLength = payload->length;
    return ret;
}

Error MakeCoseKey(ByteArray &aEncodedCoseKey, const mbedtls_pk_context &aKey, const ByteArray &aKeyId)
{
    static constexpr size_t kMaxCoseKeyLength = 1024;

    Error                             error;
    const struct mbedtls_ecp_keypair *eckey;
    CborMap                           coseKey;
    int                               ec2Curve;
    uint8_t                           xPoint[MBEDTLS_ECP_MAX_PT_LEN];
    size_t                            xLength;
    uint8_t                           yPoint[MBEDTLS_ECP_MAX_PT_LEN];
    size_t                            yLength;
    uint8_t                           encodedCoseKey[kMaxCoseKeyLength];
    size_t                            encodedCoseKeyLength;

    if (!mbedtls_pk_can_do(&aKey, MBEDTLS_PK_ECDSA) || (eckey = mbedtls_pk_ec(aKey)) == nullptr)
    {
        ExitNow(error = ERROR_INVALID_ARGS("Make COSE key without valid EC key"));
    }

    SuccessOrExit(error = coseKey.Init());

    // Cose key id('kid')
    if (!aKeyId.empty())
    {
        SuccessOrExit(error = coseKey.Put(kKeyId, aKeyId.data(), aKeyId.size()));
    }

    // Cose key type
    SuccessOrExit(error = coseKey.Put(kKeyType, kKeyTypeEC2));

    // Cose key EC2 curve
    switch (eckey->grp.id)
    {
    case MBEDTLS_ECP_DP_SECP256R1:
        ec2Curve = kKeyEC2CurveP256;
        break;
    case MBEDTLS_ECP_DP_SECP384R1:
        ec2Curve = kKeyEC2CurveP384;
        break;
    case MBEDTLS_ECP_DP_SECP521R1:
        ec2Curve = kKeyEC2CurveP521;
        break;
    default:
        ExitNow(error = ERROR_INVALID_ARGS("make COSE key with invalid EC2 curve group ID {}", eckey->grp.id));
    }
    SuccessOrExit(error = coseKey.Put(kKeyEC2Curve, ec2Curve));

    // Cose key EC2 X
    if (int fail = mbedtls_mpi_write_binary(&eckey->Q.X, xPoint, sizeof(xPoint)))
    {
        ExitNow(error = ErrorFromMbedtlsError(fail));
    }

    // 'mbedtls_mpi_write_binary' writes to the end of buffer
    xLength = mbedtls_mpi_size(&eckey->Q.X);
    SuccessOrExit(error = coseKey.Put(kKeyEC2X, xPoint + sizeof(xPoint) - xLength, xLength));

    // TODO(wgtdkp): handle the situation that Y point is not presented.
    // Cose key EC2 Y
    if (int fail = mbedtls_mpi_write_binary(&eckey->Q.Y, yPoint, sizeof(yPoint)))
    {
        ExitNow(error = ErrorFromMbedtlsError(fail));
    }

    yLength = mbedtls_mpi_size(&eckey->Q.Y);
    SuccessOrExit(error = coseKey.Put(kKeyEC2Y, yPoint + sizeof(yPoint) - yLength, yLength));

    SuccessOrExit(error = coseKey.Serialize(encodedCoseKey, encodedCoseKeyLength, sizeof(encodedCoseKey)));

    aEncodedCoseKey.assign(encodedCoseKey, encodedCoseKey + encodedCoseKeyLength);

exit:
    return error;
}

} // namespace cose

} // namespace commissioner

} // namespace ot
