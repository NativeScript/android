//
// Created by Osei Fortune on 15/011/2023.
//

#include "URLImpl.h"

using namespace tns;
using namespace ada;

URLImpl::URLImpl(url_aggregator url) : url_(url) {}

URLImpl *URLImpl::GetPointer(v8::Local<v8::Object> object) {
    auto ptr = object->GetAlignedPointerFromInternalField(0);
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
    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "hash"),
            GetHash, SetHash);
    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "host"),
            GetHost, SetHost);
    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "hash"),
            GetHash, SetHash);
    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "hostname"),
            GetHostName, SetHostName);
    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "href"),
            GetHref, SetHref);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "origin"),
            GetOrigin);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "password"),
            GetPassword, SetPassword);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "pathname"),
            GetPathName, SetPathName);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "port"),
            GetPort, SetPort);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "protocol"),
            GetProtocol, SetProtocol);

    tmpl->SetAccessor(
            ArgConverter::ConvertToV8String(isolate, "search"),
            GetSearch, SetSearch);

    tmpl->SetAccessor(
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
    if (count >= 1 && !value->IsString()) {
        isolate->ThrowException(
                v8::Exception::TypeError(v8::String::Empty(isolate)));
        return;
    }

    url_aggregator url;

    auto url_string = ArgConverter::ConvertToString(args[0].As<v8::String>());

    if (count > 1) {
        if (args[1]->IsString()) {
            auto base_string = ArgConverter::ConvertToString(args[1].As<v8::String>());
            std::string_view base_string_view(base_string.data(), base_string.length());

            if (!can_parse(url_string, &base_string_view)) {
                isolate->ThrowException(
                        v8::Exception::TypeError(v8::String::Empty(isolate)));
                return;
            }
            auto base_url = ada::parse<ada::url_aggregator>(base_string_view, nullptr);

            auto result = ada::parse<ada::url_aggregator>(url_string, &base_url.value());

            if (result) {
                url = result.value();
            } else {
                isolate->ThrowException(
                        v8::Exception::TypeError(v8::String::Empty(isolate)));
                return;
            }
        } else if (args[1]->IsObject()) {
            auto base_string = ArgConverter::ConvertToString(
                    args[1]->ToString(context).ToLocalChecked());
            std::string_view base_string_view(base_string.data(), base_string.length());
            if (!can_parse(std::string_view(url_string.data(), url_string.length()),
                           &base_string_view)) {
                isolate->ThrowException(
                        v8::Exception::TypeError(v8::String::Empty(isolate)));
                return;
            }


            auto base_url = ada::parse<ada::url_aggregator>(base_string_view, nullptr);

            auto result = ada::parse<ada::url_aggregator>(url_string, &base_url.value());

            if (result) {
                url = result.value();
            } else {
                isolate->ThrowException(
                        v8::Exception::TypeError(v8::String::Empty(isolate)));
                return;
            }
        } else {
            // treat 2nd arg as undefined otherwise.
            auto result = ada::parse<ada::url_aggregator>(url_string, nullptr);
            if (result) {
                url = result.value();
            } else {
                isolate->ThrowException(
                        v8::Exception::TypeError(v8::String::Empty(isolate)));
                return;
            }
        }
    } else {
        auto result = ada::parse<ada::url_aggregator>(url_string, nullptr);
        if (result) {
            url = result.value();
        } else {
            isolate->ThrowException(
                    v8::Exception::TypeError(v8::String::Empty(isolate)));
            return;
        }
    }

    auto ret = args.This();

    auto urlImpl = new URLImpl(url);


    ret->SetAlignedPointerInInternalField(0, urlImpl);

    urlImpl->BindFinalizer(isolate, ret);

    args.GetReturnValue().Set(ret);
}


void URLImpl::GetHash(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_hash();
    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHash(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_hash(val.c_str());
}


void URLImpl::GetHost(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_host();
    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHost(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_host(val.c_str());
}


void URLImpl::GetHostName(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();
    auto value = ptr->GetURL()->get_hostname();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHostName(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_hostname(val.c_str());
}


void URLImpl::GetHref(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_href();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetHref(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_href(val.c_str());
}

void URLImpl::GetOrigin(v8::Local<v8::String> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_origin();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::GetPassword(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_password();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPassword(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_password(val.c_str());
}

void URLImpl::GetPathName(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_pathname();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPathName(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_pathname(val.c_str());
}

void URLImpl::GetPort(v8::Local<v8::String> property,
                      const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_port();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetPort(v8::Local<v8::String> property,
                      v8::Local<v8::Value> value,
                      const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_port(val.c_str());
}

void URLImpl::GetProtocol(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_protocol();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetProtocol(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_protocol(val.c_str());
}


void URLImpl::GetSearch(v8::Local<v8::String> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_search();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetSearch(v8::Local<v8::String> property,
                        v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        return;
    }
    auto isolate = info.GetIsolate();
    auto context = isolate->GetCurrentContext();
    auto val = ArgConverter::ConvertToString(value->ToString(context).ToLocalChecked());
    ptr->GetURL()->set_search(val.c_str());
}


void URLImpl::GetUserName(v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
    URLImpl *ptr = GetPointer(info.This());
    if (ptr == nullptr) {
        info.GetReturnValue().SetEmptyString();
        return;
    }
    auto isolate = info.GetIsolate();

    auto value = ptr->GetURL()->get_username();

    info.GetReturnValue().Set(
            ArgConverter::ConvertToV8String(isolate, value.data(), value.length()));
}

void URLImpl::SetUserName(v8::Local<v8::String> property,
                          v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
    URLImpl *ptr = GetPointer(info.This());
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
    bool value;
    auto count = info.Length();


    if (count > 1) {
        auto url_string = ArgConverter::ConvertToString(info[0].As<v8::String>());
        auto base_string = ArgConverter::ConvertToString(info[1].As<v8::String>());
        std::string_view base_string_view(base_string.data(), base_string.length());
        value = can_parse(url_string, &base_string_view);
    } else {
        value = can_parse(ArgConverter::ConvertToString(info[0].As<v8::String>()).c_str(), nullptr);
    }

    info.GetReturnValue().Set(value);
}
