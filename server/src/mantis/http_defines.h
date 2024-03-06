#pragma once
#include "defines.h"

#include <typeinfo>
#include <string>
#include <cstring>

namespace mantis {

namespace method {
    /// @brief HTTP Request method verbs
    enum request_method_e {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };
}

namespace status {
    /// @brief Status codes for HTTP Response messages
    enum response_status_code_e {
        // 1XX Informational Response
        Continue = 100,
        SwitchingProtocols = 101,
        Processing = 102,
        EarlyHints = 103,

        // 2XX Success
        OK = 200,
        Created = 201,
        Accepted = 202,
        NonAuthorativeInformation = 203,
        NoContent = 204,
        ResetContent = 205,
        PartialContent = 206,
        MultiStatus = 207,
        AlreadyReported = 208,
        IMUsed = 226,

        // 3XX Redirection
        MultipleChoices = 300,
        MovedPermanently = 301,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        UseProxy = 305,
        SwitchProxy = 306,
        TemporaryRedirect = 307,
        PermanentRedirect = 308,

        // 4XX Client Errors
        BadRequest = 400,
        Unauthorized = 401,
        PaymentRequired = 402,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        NotAcceptable = 406,
        ProxyAuthenticationRequired = 407,
        RequestTimeout = 408,
        Conflict = 409,
        Gone = 410,
        LengthRequired = 411,
        PreconditionFailed = 412,
        PayloadTooLarge = 413,
        URITooLong = 414,
        UnsupportedMediaType = 415,
        RangeNotSatisfiable = 416,
        ExpectationFailed = 417,
        ImATeapot = 418,
        MisdirectedRequest = 421,
        UnprocessableContent = 422,
        Locked = 423,
        FailedDependency = 424,
        TooEarly = 425,
        UpgradeRequired = 426,
        PreconditionRequired = 428,
        TooManyRequests = 429,
        RequestHandlerFieldsTooLarge = 431,
        UnavailableForLegalReasons = 451,

        // 5XX Server Errors
        InternalServerError = 500,
        NotImplimented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503,
        GatewayTimeout = 504,
        HTTPVersionNotSupported = 505,
        VariantAlsoNegotiates = 506,
        InsufficientStorage = 507,
        LoopDetected = 508,
        NotExtended = 510,
        NetworkAuthenticationRequired = 511,
    };

inline std::string cstr_to_str(const char* cstr) {
    std::string s(cstr);
    return s;
}

inline i32 status_value(response_status_code_e status) {
    return status;
}

#define MAKE_STATUS_STR(code)\
    std::string code_str(#code);\
    std::string str =  std::to_string(status_value(code)) + " " + code_str;\
    return str;


inline std::string make_status_str(status::response_status_code_e code) {
    std::string rv;
    switch(code) {
        case Continue: {
            MAKE_STATUS_STR(Continue);
        } break;
        case SwitchingProtocols: {
            MAKE_STATUS_STR(SwitchingProtocols);
        } break;
        case Processing: {
            MAKE_STATUS_STR(Processing);
        } break;
        case EarlyHints: {
            MAKE_STATUS_STR(EarlyHints);
        } break;
        case OK: {
            MAKE_STATUS_STR(OK);
        } break;
        case Created: {
            MAKE_STATUS_STR(Created);
        } break;
        case Accepted: {
            MAKE_STATUS_STR(Accepted);
        } break;
        case NonAuthorativeInformation: {
            MAKE_STATUS_STR(NonAuthorativeInformation);
        } break;
        case NoContent: {
            MAKE_STATUS_STR(NoContent);
        } break;
        case ResetContent: {
            MAKE_STATUS_STR(ResetContent);
        } break;
        case PartialContent: {
            MAKE_STATUS_STR(PartialContent);
        } break;
        case MultiStatus: {
            MAKE_STATUS_STR(MultiStatus);
        } break;
        case AlreadyReported: {
            MAKE_STATUS_STR(AlreadyReported);
        } break;
        case IMUsed: {
            MAKE_STATUS_STR(IMUsed);
        } break;
        case MultipleChoices: {
            MAKE_STATUS_STR(MultipleChoices);
        } break;
        case MovedPermanently: {
            MAKE_STATUS_STR(MovedPermanently);
        } break;
        case Found: {
            MAKE_STATUS_STR(Found);
        } break;
        case SeeOther: {
            MAKE_STATUS_STR(SeeOther);
        } break;
        case NotModified: {
            MAKE_STATUS_STR(NotModified);
        } break;
        case UseProxy: {
            MAKE_STATUS_STR(UseProxy);
        } break;
        case SwitchProxy: {
            MAKE_STATUS_STR(SwitchProxy);
        } break;
        case TemporaryRedirect: {
            MAKE_STATUS_STR(TemporaryRedirect);
        } break;
        case PermanentRedirect: {
            MAKE_STATUS_STR(PermanentRedirect);
        } break;
        case BadRequest: {
            MAKE_STATUS_STR(BadRequest);
        } break;
        case Unauthorized: {
            MAKE_STATUS_STR(Unauthorized);
        } break;
        case PaymentRequired: {
            MAKE_STATUS_STR(PaymentRequired);
        } break;
        case Forbidden: {
            MAKE_STATUS_STR(Forbidden);
        } break;
        case NotFound: {
            MAKE_STATUS_STR(NotFound);
        } break;
        case MethodNotAllowed: {
            MAKE_STATUS_STR(MethodNotAllowed);
        } break;
        case NotAcceptable: {
            MAKE_STATUS_STR(NotAcceptable);
        } break;
        case ProxyAuthenticationRequired: {
            MAKE_STATUS_STR(ProxyAuthenticationRequired);
        } break;
        case RequestTimeout: {
            MAKE_STATUS_STR(RequestTimeout);
        } break;
        case Conflict: {
            MAKE_STATUS_STR(Conflict);
        } break;
        case Gone: {
            MAKE_STATUS_STR(Gone);
        } break;
        case LengthRequired: {
            MAKE_STATUS_STR(LengthRequired);
        } break;
        case PreconditionFailed: {
            MAKE_STATUS_STR(PreconditionFailed);
        } break;
        case PayloadTooLarge: {
            MAKE_STATUS_STR(PayloadTooLarge);
        } break;
        case URITooLong: {
            MAKE_STATUS_STR(URITooLong);
        } break;
        case UnsupportedMediaType: {
            MAKE_STATUS_STR(UnsupportedMediaType);
        } break;
        case RangeNotSatisfiable: {
            MAKE_STATUS_STR(RangeNotSatisfiable);
        } break;
        case ExpectationFailed: {
            MAKE_STATUS_STR(ExpectationFailed);
        } break;
        case ImATeapot: {
            MAKE_STATUS_STR(ImATeapot);
        } break;
        case MisdirectedRequest: {
            MAKE_STATUS_STR(MisdirectedRequest);
        } break;
        case UnprocessableContent: {
            MAKE_STATUS_STR(UnprocessableContent);
        } break;
        case Locked: {
            MAKE_STATUS_STR(Locked);
        } break;
        case FailedDependency: {
            MAKE_STATUS_STR(FailedDependency);
        } break;
        case TooEarly: {
            MAKE_STATUS_STR(TooEarly);
        } break;
        case UpgradeRequired: {
            MAKE_STATUS_STR(UpgradeRequired);
        } break;
        case PreconditionRequired: {
            MAKE_STATUS_STR(PreconditionRequired);
        } break;
        case TooManyRequests: {
            MAKE_STATUS_STR(TooManyRequests);
        } break;
        case RequestHandlerFieldsTooLarge: {
            MAKE_STATUS_STR(RequestHandlerFieldsTooLarge);
        } break;
        case UnavailableForLegalReasons: {
            MAKE_STATUS_STR(UnavailableForLegalReasons);
        } break;

        case InternalServerError: {
            MAKE_STATUS_STR(InternalServerError);
        } break;
        case NotImplimented: {
            MAKE_STATUS_STR(NotImplimented);
        } break;
        case BadGateway: {
            MAKE_STATUS_STR(BadGateway);
        } break;
        case ServiceUnavailable: {
            MAKE_STATUS_STR(ServiceUnavailable);
        } break;
        case GatewayTimeout: {
            MAKE_STATUS_STR(GatewayTimeout);
        } break;
        case HTTPVersionNotSupported: {
            MAKE_STATUS_STR(HTTPVersionNotSupported);
        } break;
        case VariantAlsoNegotiates: {
            MAKE_STATUS_STR(VariantAlsoNegotiates);
        } break;
        case InsufficientStorage: {
            MAKE_STATUS_STR(InsufficientStorage);
        } break;
        case LoopDetected: {
            MAKE_STATUS_STR(LoopDetected);
        } break;
        case NotExtended: {
            MAKE_STATUS_STR(NotExtended);
        } break;
        case NetworkAuthenticationRequired: {
            MAKE_STATUS_STR(NetworkAuthenticationRequired);
        } break;
    }

    // NOTE: If we get here, something has gone wrong
    return rv;
}

#define STATUS_CODE_TO_STR(code)\
    cstr_to_str(#code) + " " + std::to_string(code);

}

namespace protocol {
    enum stream_protocol_e {
        STREAM_TCP,
        STREAM_UDP,
    };
}

} // http
