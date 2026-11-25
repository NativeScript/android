//
// Created by Osei Fortune on 15/011/2023.
//

#include "URLImpl.h"

#include <optional>

using namespace tns;
using namespace ada;

URLImpl::URLImpl(url_aggregator url) : url_(std::move(url)) {}

URLImpl *URLImpl::GetPointer(v8::Local<v8::Object> object) {
    if (object->InternalFieldCount() < 1) {
        return nullptr;
    }
    auto ptr = object->GetAlignedPointerFromInternalField(0, v8::kEmbedderDataTypeTagDefault);
    if (ptr == nullptr) {
        return nullptr;
    }
    return static_cast<URLImpl *>(ptr);
}

url_aggregator *URLImpl::GetURL() {
    return &this->url_;
}

v8::Local<v8::FunctionTemplate> URLImpl::GetCtor(v8::Isolate *isolate) {
    v8::Local<v8::FunctionTemplate> ctorTmpl = v8::FunctionTemplate::New(isolate, Ctor);
    ctorTmpl->SetClassName(ArgConverter::ConvertToV8String(isolate, "URL"));

    auto tmpl = ctorTmpl->InstanceTemplate();
    tmpl->SetInternalFieldCount(1);
    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "hash"),
            GetHash, SetHash);
    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "host"),
            GetHost, SetHost);
    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "hash"),
            GetHash, SetHash);
    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "hostname"),
            GetHostName, SetHostName);
    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "href"),
            GetHref, SetHref);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "origin"),
            GetOrigin);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "password"),
            GetPassword, SetPassword);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "pathname"),
            GetPathName, SetPathName);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "port"),
            GetPort, SetPort);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "protocol"),
            GetProtocol, SetProtocol);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "search"),
            GetSearch, SetSearch);

    tmpl->SetNativeDataProperty(
            ArgConverter::ConvertToV8String(isolate, "username"),
            GetUserName, SetUserName);

    tmpl->Set(ArgConverter::ConvertToV8String(isolate, "toString"),
              v8::FunctionTemplate::New(isolate, &ToString));


    ctorTmpl->Set(ArgConverter::ConvertToV8String(isolate, "canParse"),
                  v8::FunctionTemplate::New(isolate, &CanParse));


    return ctorTmpl;
}

void URLImpl::Ctor(const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto count = args.Length();
    auto value = args[0];
    auto isolate = args.GetIsolate();
    auto context = isolate->GetCurrentContext();
    if (count < 1 || !value->IsString()) {
        isolate->ThrowException(
                v8::Exception::TypeError(v8::String::Empty(isolate)));
        return;
    }

    auto url_string = ArgConverter::ConvertToString(args[0].As<v8::String>());

    std::optional<std::string> base_string;
    if (count > 1) {
        if (args[1]->IsString()) {
            base_string = ArgConverter::ConvertToString(args[1].As<v8::String>());
        } else if (args[1]->IsObject()) {
            v8::Local<v8::String> base;
            if (!args[1]->ToString(context).ToLocal(&base)) {
                return;
            }
            base_string = ArgConverter::ConvertToString(base);
        }
        // Any other 2nd argument is treated as undefined.
    }

    auto parse = [&]() {
        if (!base_string) {
            return ada::parse<url_aggregator>(url_string, nullptr);
        }
        auto base = ada::parse<url_aggregator>(*base_string, nullptr);
        if (!base) {
            return base;
        }
        return ada::parse<url_aggregator>(url_string, &base.value());
    };
    auto result = parse();
    if (!result) {
        isolate->ThrowException(
                v8::Exception::TypeError(v8::String::Empty(isolate)));
        return;
    }

    auto ret = args.This();

    auto urlImpl = new URLImpl(std::move(result.value()));


    ret->SetAlignedPointerInInternalField(0, urlImpl, v8::kEmbedderDataTypeTagDefault);

    urlImpl->BindFinalizer(isolate, ret);

    args.GetReturnValue().Set(ret);
}


void URLImpl::GetHash(v8::Local<v8::Name> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_hash();
    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHash(v8::Local<v8::Name> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_hash(val.c_str());
}


void URLImpl::GetHost(v8::Local<v8::Name> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_host();
    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHost(v8::Local<v8::Name> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_host(val.c_str());
}


void URLImpl::GetHostName(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();
    auto value = ptr->GetURL()->get_hostname();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHostName(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_hostname(val.c_str());
}


void URLImpl::GetHref(v8::Local<v8::Name> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_href();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHref(v8::Local<v8::Name> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_href(val.c_str());
}

void URLImpl::GetOrigin(v8::Local<v8::Name> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_origin();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::GetPassword(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_password();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPassword(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_password(val.c_str());
}

void URLImpl::GetPathName(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_pathname();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPathName(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_pathname(val.c_str());
}

void URLImpl::GetPort(v8::Local<v8::Name> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_port();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPort(v8::Local<v8::Name> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_port(val.c_str());
}

void URLImpl::GetProtocol(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_protocol();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetProtocol(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_protocol(val.c_str());
}


void URLImpl::GetSearch(v8::Local<v8::Name> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_search();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetSearch(v8::Local<v8::Name> property,
                        v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_search(val.c_str());
}


void URLImpl::GetUserName(v8::Local<v8::Name> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_username();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetUserName(v8::Local<v8::Name> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.Holder());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_username(val.c_str());
}


void URLImpl::ToString(const v8::FunctionCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();


    auto value = ptr->GetURL()->get_href();

    auto ret = ArgConverter::ConvertToV8String(isolate, value.data(), value.length());

    info.GetReturnValue().Set(ret);
}


void URLImpl::CanParse(const v8::FunctionCallbackInfo<v8::Value> &info) {
    // ada::parse rather than ada::can_parse: the constructor already links it,
    // and can_parse pulls in a second instantiation of ada's parser.
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    v8::Local<v8::String> input;
    if (!info[0]->ToString(context).ToLocal(&input)) {
        return;
    }
    auto url_string = ArgConverter::ConvertToString(input);
    bool value;
    if (info.Length() > 1 && !info[1]->IsUndefined()) {
        v8::Local<v8::String> base_input;
        if (!info[1]->ToString(context).ToLocal(&base_input)) {
            return;
        }
        auto base = ada::parse<url_aggregator>(
                ArgConverter::ConvertToString(base_input), nullptr);
        value = base && ada::parse<url_aggregator>(url_string, &base.value());
    } else {
        value = ada::parse<url_aggregator>(url_string, nullptr).has_value();
    }

    info.GetReturnValue().Set(value);
}
