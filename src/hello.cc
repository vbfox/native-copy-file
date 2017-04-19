#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define UNICODE

#include <windows.h>
#include <nan.h>

v8::Local<v8::String> wcharToString(wchar_t* str) {
    const uint16_t* data = reinterpret_cast<const uint16_t*>(str);
    return v8::String::NewFromTwoByte(v8::Isolate::GetCurrent(), data, v8::NewStringType::kNormal).ToLocalChecked();
}
wchar_t* s2w(v8::Local<v8::String> value) {
    wchar_t* buffer = new wchar_t[value->Length() + 1];
    value->Write(reinterpret_cast<uint16_t*>(buffer));
    return buffer;
}

void Win32CopyFile(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    const v8::Local<v8::Value> vExistingFileName = info[0];
    const v8::Local<v8::Value> vNewFileName = info[1];

    if (!vExistingFileName->IsString()) {
        Nan::ThrowTypeError("existingFileName should be a string");
        return;
    }

    if (!vNewFileName->IsString()) {
        Nan::ThrowTypeError("existingFileName should be a string");
        return;
    }

    const v8::Local<v8::String> sExistingFileName = v8::Local<v8::String>::Cast(vExistingFileName);
    const v8::Local<v8::String> sNewFileName = v8::Local<v8::String>::Cast(vNewFileName);
    const v8::Local<v8::String> maxpathOptin = wcharToString(L"\\\\?\\");

    const wchar_t* lpExistingFileName = s2w(v8::String::Concat(maxpathOptin, sExistingFileName));
    const wchar_t* lpNewFileName = s2w(v8::String::Concat(maxpathOptin, sNewFileName));

    const BOOL result = CopyFile(lpExistingFileName, lpNewFileName, false);

    delete[] lpExistingFileName;
    delete[] lpNewFileName;

    info.GetReturnValue().Set(Nan::New(result));
}
#endif

void Init(v8::Local<v8::Object> exports) {
#ifdef _WIN32
  exports->Set(Nan::New("win32CopyFile").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Win32CopyFile)->GetFunction());
#endif
}

NODE_MODULE(hello, Init)
