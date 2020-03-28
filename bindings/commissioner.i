// SWIG interface file of commissioner/commissioner.hpp

%module(directors="1") commissionerModule

%{
#include <commissioner/defines.hpp>
#include <commissioner/error.hpp>
#include <commissioner/network_data.hpp>
#include <commissioner/border_agent.hpp>
#include <commissioner/commissioner.hpp>
%}

%include <std_string.i>
%include <stdint.i>
%include <std_list.i>
%include <std_shared_ptr.i>
%include <std_vector.i>
%include <stl.i>
%include <typemaps.i>

%rename("%(regex:/^(m)(.*)/\\2/)s") "";

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

%template(ByteArray) std::vector<uint8_t>;
%template(BorderAgentList) std::list<ot::commissioner::BorderAgent>;
%template(ChannelMask) std::vector<ot::commissioner::ChannelMaskEntry>;
%template(StringVector) std::vector<std::string>;

%apply uint8_t& OUTPUT { uint8_t& aStatus }

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

%shared_ptr(ot::commissioner::Commissioner)

// %typemap(javainterfaces) ot::commissioner::Commissioner "Parcelable"

namespace ot {
namespace commissioner {
    // Remove async commissioner APIs.
    %ignore Commissioner::Discover(Handler<std::list<BorderAgent>> aHandler);
    %ignore Commissioner::Connect(ErrorHandler aHandler, const std::string &aAddr, uint16_t aPort);
    %ignore Commissioner::Petition(ErrorHandler aHandler, const std::string &aAddr, uint16_t aPort);
    %ignore Commissioner::Resign(ErrorHandler aHandler);
    %ignore Commissioner::GetCommissionerDataset(Handler<CommissionerDataset> aHandler, uint16_t aDatasetFlags);
    %ignore Commissioner::SetCommissionerDataset(ErrorHandler aHandler, const CommissionerDataset &aDataset);
    %ignore Commissioner::SetBbrDataset(ErrorHandler aHandler, const BbrDataset &aDataset);
    %ignore Commissioner::GetBbrDataset(Handler<BbrDataset> aHandler, uint16_t aDatasetFlags);
    %ignore Commissioner::GetActiveDataset(Handler<ActiveOperationalDataset> aHandler, uint16_t aDatasetFlags);
    %ignore Commissioner::SetActiveDataset(ErrorHandler aHandler, const ActiveOperationalDataset &aActiveDataset);
    %ignore Commissioner::GetPendingDataset(Handler<PendingOperationalDataset> aHandler, uint16_t aDatasetFlags);
    %ignore Commissioner::SetPendingDataset(ErrorHandler aHandler, const PendingOperationalDataset &aPendingDataset);
    %ignore Commissioner::SetSecurePendingDataset(ErrorHandler                     aHandler,
                                                  const std::string &              aPbbrAddr,
                                                  uint32_t                         aMaxRetrievalTimer,
                                                  const PendingOperationalDataset &aDataset);
    %ignore Commissioner::CommandReenroll(ErrorHandler aHandler, const std::string &aDstAddr);
    %ignore Commissioner::CommandDomainReset(ErrorHandler aHandler, const std::string &aDstAddr);
    %ignore Commissioner::CommandMigrate(ErrorHandler       aHandler,
                                         const std::string &aDstAddr,
                                         const std::string &aDesignatedNetwork);
    %ignore Commissioner::AnnounceBegin(ErrorHandler       aHandler,
                                        uint32_t           aChannelMask,
                                        uint8_t            aCount,
                                        uint16_t           aPeriod,
                                        const std::string &aDstAddr);
    %ignore Commissioner::PanIdQuery(ErrorHandler       aHandler,
                                     uint32_t           aChannelMask,
                                     uint16_t           aPanId,
                                     const std::string &aDstAddr);
    %ignore Commissioner::EnergyScan(ErrorHandler       aHandler,
                                     uint32_t           aChannelMask,
                                     uint8_t            aCount,
                                     uint16_t           aPeriod,
                                     uint16_t           aScanDuration,
                                     const std::string &aDstAddr);
    %ignore Commissioner::RegisterMulticastListener(Handler<uint8_t>                aHandler,
                                                    const std::string &             aPbbrAddr,
                                                    const std::vector<std::string> &aMulticastAddrList,
                                                    uint32_t                        aTimeout);
    %ignore Commissioner::RequestToken(Handler<ByteArray> aHandler, const std::string &aAddr, uint16_t aPort);
}
}

%include <commissioner/defines.hpp>
%include <commissioner/error.hpp>
%include <commissioner/network_data.hpp>
%include <commissioner/border_agent.hpp>
%include <commissioner/commissioner.hpp>
%include <commissioner/commissioner.hpp>
