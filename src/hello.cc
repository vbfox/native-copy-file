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
using v8::Number;
using v8::FunctionTemplate;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;

#ifdef _WIN32
Local<String> wcharToString(const wchar_t* str) {
    if (str == NULL) {
        return New<String>().ToLocalChecked();
    }

    const uint16_t* data = reinterpret_cast<const uint16_t*>(str);
    return String::NewFromTwoByte(
        v8::Isolate::GetCurrent(),
        data);
}

const wchar_t* stringToWchar(Local<String> value) {
    wchar_t* buffer = new wchar_t[value->Length() + 1];
    value->Write(reinterpret_cast<uint16_t*>(buffer));
    return buffer;
}

const wchar_t* FormatSystemMessageAllocateNoCrLf(DWORD errorCode) {
    wchar_t* result = NULL;
    const DWORD charCount = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        0,
        (LPWSTR)&result,
        0,
        NULL);
    if (charCount >= 2 && result[charCount-2] == '\r' && result[charCount-1] == '\n') {
        result[charCount-2] = '\0';
        result[charCount-1] = '\0';
    }

    return result;
}

class CopyFileWorker : public AsyncWorker {
    public:
        CopyFileWorker(Callback *callback, const wchar_t* existingFileName, const wchar_t* newFileName)
            : AsyncWorker(callback),
            existingFileName(existingFileName),
            newFileName(newFileName),
            result(FALSE),
            errorCode(0),
            errorMessage(NULL) {
        }

        ~CopyFileWorker() {
        }

        void Execute () {
            result = CopyFile(existingFileName, newFileName, false);
            if (!result) {
                errorCode = GetLastError();
                errorMessage = FormatSystemMessageAllocateNoCrLf(errorCode);
            }
        }

        virtual void HandleOKCallback() {
            HandleScope scope;

            if (result) {
                callback->Call(0, NULL);
            } else {
                Local<String> errorString = wcharToString(errorMessage);
                Local<Value> argv[] = {
                    v8::Exception::Error(errorString),
                    New<Number>(errorCode)
                };
                callback->Call(2, argv);
            }
        }

        void Destroy() {
            delete[] existingFileName;
            existingFileName = NULL;

            delete[] newFileName;
            newFileName = NULL;

            if (errorMessage != NULL) {
                HeapFree (GetProcessHeap(), 0, (void*)errorMessage);
                errorMessage = NULL;
            }

            AsyncWorker::Destroy();
        }

    private:
        const wchar_t* existingFileName;
        const wchar_t* newFileName;
        const wchar_t* errorMessage;
        BOOL result;
        DWORD errorCode;
};

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

    const wchar_t* existingFileName = stringToWchar(String::Concat(maxpathOptin, sExistingFileName));
    const wchar_t* newFileName = stringToWchar(String::Concat(maxpathOptin, sNewFileName));


    Callback* callback = new Callback(vCallback.As<Function>());
    AsyncQueueWorker(new CopyFileWorker(callback, existingFileName, newFileName));
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
