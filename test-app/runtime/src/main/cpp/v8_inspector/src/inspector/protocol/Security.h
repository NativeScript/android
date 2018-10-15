// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Security_h
#define v8_inspector_protocol_Security_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.

namespace v8_inspector {
namespace protocol {
namespace Security {

// ------------- Forward and enum declarations.
using CertificateId = int;
using MixedContentType = String;
using SecurityState = String;
class SecurityStateExplanation;
class InsecureContentStatus;
using CertificateErrorAction = String;
class CertificateErrorNotification;
class SecurityStateChangedNotification;

namespace MixedContentTypeEnum {
extern const char* Blockable;
extern const char* OptionallyBlockable;
extern const char* None;
} // namespace MixedContentTypeEnum

namespace SecurityStateEnum {
extern const char* Unknown;
extern const char* Neutral;
extern const char* Insecure;
extern const char* Secure;
extern const char* Info;
} // namespace SecurityStateEnum

namespace CertificateErrorActionEnum {
extern const char* Continue;
extern const char* Cancel;
} // namespace CertificateErrorActionEnum

// ------------- Type and builder declarations.

class  SecurityStateExplanation : public Serializable {
        PROTOCOL_DISALLOW_COPY(SecurityStateExplanation);
    public:
        static std::unique_ptr<SecurityStateExplanation> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SecurityStateExplanation() override { }

        String getSecurityState() {
            return m_securityState;
        }
        void setSecurityState(const String& value) {
            m_securityState = value;
        }

        String getTitle() {
            return m_title;
        }
        void setTitle(const String& value) {
            m_title = value;
        }

        String getSummary() {
            return m_summary;
        }
        void setSummary(const String& value) {
            m_summary = value;
        }

        String getDescription() {
            return m_description;
        }
        void setDescription(const String& value) {
            m_description = value;
        }

        String getMixedContentType() {
            return m_mixedContentType;
        }
        void setMixedContentType(const String& value) {
            m_mixedContentType = value;
        }

        protocol::Array<String>* getCertificate() {
            return m_certificate.get();
        }
        void setCertificate(std::unique_ptr<protocol::Array<String>> value) {
            m_certificate = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<SecurityStateExplanation> clone() const;

        template<int STATE>
        class SecurityStateExplanationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    SecurityStateSet = 1 << 1,
                    TitleSet = 1 << 2,
                    SummarySet = 1 << 3,
                    DescriptionSet = 1 << 4,
                    MixedContentTypeSet = 1 << 5,
                    CertificateSet = 1 << 6,
                    AllFieldsSet = (SecurityStateSet | TitleSet | SummarySet | DescriptionSet | MixedContentTypeSet | CertificateSet | 0)
                };


                SecurityStateExplanationBuilder<STATE | SecurityStateSet>& setSecurityState(const String& value) {
                    static_assert(!(STATE & SecurityStateSet), "property securityState should not be set yet");
                    m_result->setSecurityState(value);
                    return castState<SecurityStateSet>();
                }

                SecurityStateExplanationBuilder<STATE | TitleSet>& setTitle(const String& value) {
                    static_assert(!(STATE & TitleSet), "property title should not be set yet");
                    m_result->setTitle(value);
                    return castState<TitleSet>();
                }

                SecurityStateExplanationBuilder<STATE | SummarySet>& setSummary(const String& value) {
                    static_assert(!(STATE & SummarySet), "property summary should not be set yet");
                    m_result->setSummary(value);
                    return castState<SummarySet>();
                }

                SecurityStateExplanationBuilder<STATE | DescriptionSet>& setDescription(const String& value) {
                    static_assert(!(STATE & DescriptionSet), "property description should not be set yet");
                    m_result->setDescription(value);
                    return castState<DescriptionSet>();
                }

                SecurityStateExplanationBuilder<STATE | MixedContentTypeSet>& setMixedContentType(const String& value) {
                    static_assert(!(STATE & MixedContentTypeSet), "property mixedContentType should not be set yet");
                    m_result->setMixedContentType(value);
                    return castState<MixedContentTypeSet>();
                }

                SecurityStateExplanationBuilder<STATE | CertificateSet>& setCertificate(std::unique_ptr<protocol::Array<String>> value) {
                    static_assert(!(STATE & CertificateSet), "property certificate should not be set yet");
                    m_result->setCertificate(std::move(value));
                    return castState<CertificateSet>();
                }

                std::unique_ptr<SecurityStateExplanation> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SecurityStateExplanation;
                SecurityStateExplanationBuilder() : m_result(new SecurityStateExplanation()) { }

                template<int STEP> SecurityStateExplanationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SecurityStateExplanationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Security::SecurityStateExplanation> m_result;
        };

        static SecurityStateExplanationBuilder<0> create() {
            return SecurityStateExplanationBuilder<0>();
        }

    private:
        SecurityStateExplanation() {
        }

        String m_securityState;
        String m_title;
        String m_summary;
        String m_description;
        String m_mixedContentType;
        std::unique_ptr<protocol::Array<String>> m_certificate;
};


class  InsecureContentStatus : public Serializable {
        PROTOCOL_DISALLOW_COPY(InsecureContentStatus);
    public:
        static std::unique_ptr<InsecureContentStatus> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~InsecureContentStatus() override { }

        bool getRanMixedContent() {
            return m_ranMixedContent;
        }
        void setRanMixedContent(bool value) {
            m_ranMixedContent = value;
        }

        bool getDisplayedMixedContent() {
            return m_displayedMixedContent;
        }
        void setDisplayedMixedContent(bool value) {
            m_displayedMixedContent = value;
        }

        bool getContainedMixedForm() {
            return m_containedMixedForm;
        }
        void setContainedMixedForm(bool value) {
            m_containedMixedForm = value;
        }

        bool getRanContentWithCertErrors() {
            return m_ranContentWithCertErrors;
        }
        void setRanContentWithCertErrors(bool value) {
            m_ranContentWithCertErrors = value;
        }

        bool getDisplayedContentWithCertErrors() {
            return m_displayedContentWithCertErrors;
        }
        void setDisplayedContentWithCertErrors(bool value) {
            m_displayedContentWithCertErrors = value;
        }

        String getRanInsecureContentStyle() {
            return m_ranInsecureContentStyle;
        }
        void setRanInsecureContentStyle(const String& value) {
            m_ranInsecureContentStyle = value;
        }

        String getDisplayedInsecureContentStyle() {
            return m_displayedInsecureContentStyle;
        }
        void setDisplayedInsecureContentStyle(const String& value) {
            m_displayedInsecureContentStyle = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<InsecureContentStatus> clone() const;

        template<int STATE>
        class InsecureContentStatusBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    RanMixedContentSet = 1 << 1,
                    DisplayedMixedContentSet = 1 << 2,
                    ContainedMixedFormSet = 1 << 3,
                    RanContentWithCertErrorsSet = 1 << 4,
                    DisplayedContentWithCertErrorsSet = 1 << 5,
                    RanInsecureContentStyleSet = 1 << 6,
                    DisplayedInsecureContentStyleSet = 1 << 7,
                    AllFieldsSet = (RanMixedContentSet | DisplayedMixedContentSet | ContainedMixedFormSet | RanContentWithCertErrorsSet | DisplayedContentWithCertErrorsSet | RanInsecureContentStyleSet | DisplayedInsecureContentStyleSet | 0)
                };


                InsecureContentStatusBuilder<STATE | RanMixedContentSet>& setRanMixedContent(bool value) {
                    static_assert(!(STATE & RanMixedContentSet), "property ranMixedContent should not be set yet");
                    m_result->setRanMixedContent(value);
                    return castState<RanMixedContentSet>();
                }

                InsecureContentStatusBuilder<STATE | DisplayedMixedContentSet>& setDisplayedMixedContent(bool value) {
                    static_assert(!(STATE & DisplayedMixedContentSet), "property displayedMixedContent should not be set yet");
                    m_result->setDisplayedMixedContent(value);
                    return castState<DisplayedMixedContentSet>();
                }

                InsecureContentStatusBuilder<STATE | ContainedMixedFormSet>& setContainedMixedForm(bool value) {
                    static_assert(!(STATE & ContainedMixedFormSet), "property containedMixedForm should not be set yet");
                    m_result->setContainedMixedForm(value);
                    return castState<ContainedMixedFormSet>();
                }

                InsecureContentStatusBuilder<STATE | RanContentWithCertErrorsSet>& setRanContentWithCertErrors(bool value) {
                    static_assert(!(STATE & RanContentWithCertErrorsSet), "property ranContentWithCertErrors should not be set yet");
                    m_result->setRanContentWithCertErrors(value);
                    return castState<RanContentWithCertErrorsSet>();
                }

                InsecureContentStatusBuilder<STATE | DisplayedContentWithCertErrorsSet>& setDisplayedContentWithCertErrors(bool value) {
                    static_assert(!(STATE & DisplayedContentWithCertErrorsSet), "property displayedContentWithCertErrors should not be set yet");
                    m_result->setDisplayedContentWithCertErrors(value);
                    return castState<DisplayedContentWithCertErrorsSet>();
                }

                InsecureContentStatusBuilder<STATE | RanInsecureContentStyleSet>& setRanInsecureContentStyle(const String& value) {
                    static_assert(!(STATE & RanInsecureContentStyleSet), "property ranInsecureContentStyle should not be set yet");
                    m_result->setRanInsecureContentStyle(value);
                    return castState<RanInsecureContentStyleSet>();
                }

                InsecureContentStatusBuilder<STATE | DisplayedInsecureContentStyleSet>& setDisplayedInsecureContentStyle(const String& value) {
                    static_assert(!(STATE & DisplayedInsecureContentStyleSet), "property displayedInsecureContentStyle should not be set yet");
                    m_result->setDisplayedInsecureContentStyle(value);
                    return castState<DisplayedInsecureContentStyleSet>();
                }

                std::unique_ptr<InsecureContentStatus> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class InsecureContentStatus;
                InsecureContentStatusBuilder() : m_result(new InsecureContentStatus()) { }

                template<int STEP> InsecureContentStatusBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<InsecureContentStatusBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Security::InsecureContentStatus> m_result;
        };

        static InsecureContentStatusBuilder<0> create() {
            return InsecureContentStatusBuilder<0>();
        }

    private:
        InsecureContentStatus() {
            m_ranMixedContent = false;
            m_displayedMixedContent = false;
            m_containedMixedForm = false;
            m_ranContentWithCertErrors = false;
            m_displayedContentWithCertErrors = false;
        }

        bool m_ranMixedContent;
        bool m_displayedMixedContent;
        bool m_containedMixedForm;
        bool m_ranContentWithCertErrors;
        bool m_displayedContentWithCertErrors;
        String m_ranInsecureContentStyle;
        String m_displayedInsecureContentStyle;
};


class  CertificateErrorNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(CertificateErrorNotification);
    public:
        static std::unique_ptr<CertificateErrorNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CertificateErrorNotification() override { }

        int getEventId() {
            return m_eventId;
        }
        void setEventId(int value) {
            m_eventId = value;
        }

        String getErrorType() {
            return m_errorType;
        }
        void setErrorType(const String& value) {
            m_errorType = value;
        }

        String getRequestURL() {
            return m_requestURL;
        }
        void setRequestURL(const String& value) {
            m_requestURL = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<CertificateErrorNotification> clone() const;

        template<int STATE>
        class CertificateErrorNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    EventIdSet = 1 << 1,
                    ErrorTypeSet = 1 << 2,
                    RequestURLSet = 1 << 3,
                    AllFieldsSet = (EventIdSet | ErrorTypeSet | RequestURLSet | 0)
                };


                CertificateErrorNotificationBuilder<STATE | EventIdSet>& setEventId(int value) {
                    static_assert(!(STATE & EventIdSet), "property eventId should not be set yet");
                    m_result->setEventId(value);
                    return castState<EventIdSet>();
                }

                CertificateErrorNotificationBuilder<STATE | ErrorTypeSet>& setErrorType(const String& value) {
                    static_assert(!(STATE & ErrorTypeSet), "property errorType should not be set yet");
                    m_result->setErrorType(value);
                    return castState<ErrorTypeSet>();
                }

                CertificateErrorNotificationBuilder<STATE | RequestURLSet>& setRequestURL(const String& value) {
                    static_assert(!(STATE & RequestURLSet), "property requestURL should not be set yet");
                    m_result->setRequestURL(value);
                    return castState<RequestURLSet>();
                }

                std::unique_ptr<CertificateErrorNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CertificateErrorNotification;
                CertificateErrorNotificationBuilder() : m_result(new CertificateErrorNotification()) { }

                template<int STEP> CertificateErrorNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CertificateErrorNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Security::CertificateErrorNotification> m_result;
        };

        static CertificateErrorNotificationBuilder<0> create() {
            return CertificateErrorNotificationBuilder<0>();
        }

    private:
        CertificateErrorNotification() {
            m_eventId = 0;
        }

        int m_eventId;
        String m_errorType;
        String m_requestURL;
};


class  SecurityStateChangedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(SecurityStateChangedNotification);
    public:
        static std::unique_ptr<SecurityStateChangedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~SecurityStateChangedNotification() override { }

        String getSecurityState() {
            return m_securityState;
        }
        void setSecurityState(const String& value) {
            m_securityState = value;
        }

        bool getSchemeIsCryptographic() {
            return m_schemeIsCryptographic;
        }
        void setSchemeIsCryptographic(bool value) {
            m_schemeIsCryptographic = value;
        }

        protocol::Array<protocol::Security::SecurityStateExplanation>* getExplanations() {
            return m_explanations.get();
        }
        void setExplanations(std::unique_ptr<protocol::Array<protocol::Security::SecurityStateExplanation>> value) {
            m_explanations = std::move(value);
        }

        protocol::Security::InsecureContentStatus* getInsecureContentStatus() {
            return m_insecureContentStatus.get();
        }
        void setInsecureContentStatus(std::unique_ptr<protocol::Security::InsecureContentStatus> value) {
            m_insecureContentStatus = std::move(value);
        }

        bool hasSummary() {
            return m_summary.isJust();
        }
        String getSummary(const String& defaultValue) {
            return m_summary.isJust() ? m_summary.fromJust() : defaultValue;
        }
        void setSummary(const String& value) {
            m_summary = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<SecurityStateChangedNotification> clone() const;

        template<int STATE>
        class SecurityStateChangedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    SecurityStateSet = 1 << 1,
                    SchemeIsCryptographicSet = 1 << 2,
                    ExplanationsSet = 1 << 3,
                    InsecureContentStatusSet = 1 << 4,
                    AllFieldsSet = (SecurityStateSet | SchemeIsCryptographicSet | ExplanationsSet | InsecureContentStatusSet | 0)
                };


                SecurityStateChangedNotificationBuilder<STATE | SecurityStateSet>& setSecurityState(const String& value) {
                    static_assert(!(STATE & SecurityStateSet), "property securityState should not be set yet");
                    m_result->setSecurityState(value);
                    return castState<SecurityStateSet>();
                }

                SecurityStateChangedNotificationBuilder<STATE | SchemeIsCryptographicSet>& setSchemeIsCryptographic(bool value) {
                    static_assert(!(STATE & SchemeIsCryptographicSet), "property schemeIsCryptographic should not be set yet");
                    m_result->setSchemeIsCryptographic(value);
                    return castState<SchemeIsCryptographicSet>();
                }

                SecurityStateChangedNotificationBuilder<STATE | ExplanationsSet>& setExplanations(std::unique_ptr<protocol::Array<protocol::Security::SecurityStateExplanation>> value) {
                    static_assert(!(STATE & ExplanationsSet), "property explanations should not be set yet");
                    m_result->setExplanations(std::move(value));
                    return castState<ExplanationsSet>();
                }

                SecurityStateChangedNotificationBuilder<STATE | InsecureContentStatusSet>& setInsecureContentStatus(std::unique_ptr<protocol::Security::InsecureContentStatus> value) {
                    static_assert(!(STATE & InsecureContentStatusSet), "property insecureContentStatus should not be set yet");
                    m_result->setInsecureContentStatus(std::move(value));
                    return castState<InsecureContentStatusSet>();
                }

                SecurityStateChangedNotificationBuilder<STATE>& setSummary(const String& value) {
                    m_result->setSummary(value);
                    return *this;
                }

                std::unique_ptr<SecurityStateChangedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class SecurityStateChangedNotification;
                SecurityStateChangedNotificationBuilder() : m_result(new SecurityStateChangedNotification()) { }

                template<int STEP> SecurityStateChangedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<SecurityStateChangedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Security::SecurityStateChangedNotification> m_result;
        };

        static SecurityStateChangedNotificationBuilder<0> create() {
            return SecurityStateChangedNotificationBuilder<0>();
        }

    private:
        SecurityStateChangedNotification() {
            m_schemeIsCryptographic = false;
        }

        String m_securityState;
        bool m_schemeIsCryptographic;
        std::unique_ptr<protocol::Array<protocol::Security::SecurityStateExplanation>> m_explanations;
        std::unique_ptr<protocol::Security::InsecureContentStatus> m_insecureContentStatus;
        Maybe<String> m_summary;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse enable() = 0;
        virtual DispatchResponse setIgnoreCertificateErrors(bool in_ignore) = 0;
        virtual DispatchResponse handleCertificateError(int in_eventId, const String& in_action) = 0;
        virtual DispatchResponse setOverrideCertificateErrors(bool in_override) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void certificateError(int eventId, const String& errorType, const String& requestURL);
        void securityStateChanged(const String& securityState, bool schemeIsCryptographic, std::unique_ptr<protocol::Array<protocol::Security::SecurityStateExplanation>> explanations, std::unique_ptr<protocol::Security::InsecureContentStatus> insecureContentStatus, Maybe<String> summary = Maybe<String>());

        void flush();
        void sendRawNotification(const String&);
    private:
        FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class  Dispatcher {
    public:
        static void wire(UberDispatcher*, Backend*);

    private:
        Dispatcher() { }
};

// ------------- Metainfo.

class  Metainfo {
    public:
        using BackendClass = Backend;
        using FrontendClass = Frontend;
        using DispatcherClass = Dispatcher;
        static const char domainName[];
        static const char commandPrefix[];
        static const char version[];
};

} // namespace Security
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Security_h)
