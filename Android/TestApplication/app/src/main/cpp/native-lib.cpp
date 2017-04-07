#include <jni.h>
#include <string>
#include <android/log.h>

using namespace std;

static JavaVM *gJVM;

static jclass gTestApplicationClass;

static jmethodID gTestApplicationGetCurrentActivityMethod;
static jmethodID gTestApplicationGetCurrentActivitySimpleNameMethod;
static jmethodID gCurrentActivityFillFieldsMethod;

class TestClass {
public:
    static void handleName(const char *firstname, const char *lastname) {
        __android_log_print(ANDROID_LOG_DEBUG, "LLDB", "[%s] : Firstname: %s, Lastname: %s", __FUNCTION__, firstname, lastname);

        JNIEnv *env;
        int res = gJVM->AttachCurrentThread(&env, NULL);
        if (res != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "LLDB", "[%s] : Unable to environment out of JVM.", __FUNCTION__);
            return;
        }

        jclass testApplicationClass = env->FindClass("makhun/com/testapplication/TestApplication");
        if (env->ExceptionCheck()) {
            env->ExceptionClear();

            __android_log_print(ANDROID_LOG_ERROR, "LLDB", "[%s] : env->FindClass() failed to find class makhun/com/testapplication/TestApplication.", __FUNCTION__);
            return;
        }

        gTestApplicationClass = (jclass)env->NewGlobalRef(testApplicationClass);
        env->DeleteLocalRef(testApplicationClass);

        gTestApplicationGetCurrentActivityMethod = env->GetStaticMethodID(gTestApplicationClass, "getCurrentActivity", "()Landroid/app/Activity;");

        jobject currentActivity = env->CallStaticObjectMethod(gTestApplicationClass, gTestApplicationGetCurrentActivityMethod);
        if (currentActivity) {
            __android_log_print(ANDROID_LOG_DEBUG, "LLDB", "[%s] : CurrentActivity object was found.", __FUNCTION__);
        }

        gTestApplicationGetCurrentActivitySimpleNameMethod = env->GetStaticMethodID(gTestApplicationClass, "getCurrentActivitySimpleName", "()Ljava/lang/String;");
        jstring currentActivitySimpleName = (jstring)env->CallStaticObjectMethod(gTestApplicationClass, gTestApplicationGetCurrentActivitySimpleNameMethod);
        const char *currentActivitySimpleNameStr = env->GetStringUTFChars(currentActivitySimpleName, NULL);
        string expectedName = "MainActivity";

        if (currentActivitySimpleName && expectedName.compare(string(currentActivitySimpleNameStr)) == 0) {
            __android_log_print(ANDROID_LOG_DEBUG, "LLDB", "[%s] : CurrentActivity is currently present on screen.", __FUNCTION__);

            jclass currentActivityClass = env->GetObjectClass(currentActivity);
            gCurrentActivityFillFieldsMethod = env->GetMethodID(currentActivityClass, "fillFields", "(Ljava/lang/String;Ljava/lang/String;)Z");
            env->DeleteLocalRef(currentActivityClass);

            jstring firstnameStr = env->NewStringUTF(firstname);
            jstring lastnameStr = env->NewStringUTF(lastname);

            jboolean res = (jboolean)env->CallBooleanMethod(currentActivity, gCurrentActivityFillFieldsMethod, firstnameStr, lastnameStr);
            if (res) {
                __android_log_print(ANDROID_LOG_DEBUG, "LLDB", "[%s] : Successfully edited fields in CurrentActivity.", __FUNCTION__);
            } else {
                __android_log_print(ANDROID_LOG_ERROR, "LLDB", "[%s] : Unable to edit fields in CurrentActivity.", __FUNCTION__);
            }

            env->DeleteLocalRef(firstnameStr);
            env->DeleteLocalRef(lastnameStr);
        }

        env->ReleaseStringUTFChars(currentActivitySimpleName, currentActivitySimpleNameStr);
    }
};

extern "C" {

JNIEXPORT void JNICALL
Java_makhun_com_testapplication_MainActivity_init(
        JNIEnv* env,
        jobject /* this */) {
    int res = env->GetJavaVM(&gJVM);
    if (res != 0) {
        __android_log_print(ANDROID_LOG_ERROR, "LLDB", "[%s] : Unable to cache JVM.", __FUNCTION__);
        return;
    }

    TestClass::handleName("John1", "Doe2");
}

}