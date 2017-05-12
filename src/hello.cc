#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define UNICODE
#include <windows.h>
#endif

#include <nan.h>

using v8::Function;
using v8::Local;
using v8::Value;
using v8::String;
using v8::FunctionTemplate;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;

class CopyFileWorker : public AsyncWorker {
    public:
        CopyFileWorker(Callback *callback, const wchar_t* lpExistingFileName, const wchar_t* lpNewFileName)
            : AsyncWorker(callback),
            lpExistingFileName(lpExistingFileName),
            lpNewFileName(lpNewFileName),
            result(FALSE) {
        }

        ~CopyFileWorker() {
        }

        void Execute () {
            result = CopyFile(lpExistingFileName, lpNewFileName, false);
        }

        void Destroy() {
            delete[] lpExistingFileName;
            lpExistingFileName = NULL;
            delete[] lpNewFileName;
            lpNewFileName = NULL;

            AsyncWorker::Destroy();
        }

    private:
        const wchar_t* lpExistingFileName;
        const wchar_t* lpNewFileName;
        BOOL result;
};

#ifdef _WIN32
Local<String> wcharToString(wchar_t* str) {
    const uint16_t* data = reinterpret_cast<const uint16_t*>(str);
    return String::NewFromTwoByte(v8::Isolate::GetCurrent(), data, v8::NewStringType::kNormal).ToLocalChecked();
}

wchar_t* stringToWchar(Local<String> value) {
    wchar_t* buffer = new wchar_t[value->Length() + 1];
    value->Write(reinterpret_cast<uint16_t*>(buffer));
    return buffer;
}

NAN_METHOD(Win32CopyFile) {
    if (info.Length() < 3) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    const Local<Value> vExistingFileName = info[0];
    const Local<Value> vNewFileName = info[1];
    const Local<Value> vCallback = info[2];

    if (!vExistingFileName->IsString()) {
        Nan::ThrowTypeError("existingFileName should be a string");
        return;
    }

    if (!vNewFileName->IsString()) {
        Nan::ThrowTypeError("existingFileName should be a string");
        return;
    }

    const Local<String> sExistingFileName = Local<String>::Cast(vExistingFileName);
    const Local<String> sNewFileName = Local<String>::Cast(vNewFileName);
    const Local<String> maxpathOptin = wcharToString(L"\\\\?\\");

    const wchar_t* lpExistingFileName = stringToWchar(String::Concat(maxpathOptin, sExistingFileName));
    const wchar_t* lpNewFileName = stringToWchar(String::Concat(maxpathOptin, sNewFileName));


    Callback* callback = new Callback(vCallback.As<Function>());
    AsyncQueueWorker(new CopyFileWorker(callback, lpExistingFileName, lpNewFileName));
}
#endif


NAN_MODULE_INIT(Init) {
#ifdef _WIN32
    Nan::Set(
        target,
        Nan::New("win32CopyFile").ToLocalChecked(),
        Nan::New<FunctionTemplate>(Win32CopyFile)->GetFunction());
#endif
}

NODE_MODULE(hello, Init)
