#include <jni.h>
#include <string>
#include <iostream>

#include <android/log.h>

using namespace std;

class TestClass {
public:
    static void handleName(const char *firstname, const char *lastname) {
        __android_log_print(ANDROID_LOG_DEBUG, "LLDB", "[%s]: Firstname: %s, Lastname: %s", __FUNCTION__, firstname, lastname);
    }
};

extern "C" {

JNIEXPORT jstring JNICALL
Java_makhun_com_testapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    // TestClass::handleName("John", "Doe");

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

}