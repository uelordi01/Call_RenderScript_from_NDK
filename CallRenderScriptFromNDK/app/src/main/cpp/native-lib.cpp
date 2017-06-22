#include <jni.h>
#include "logger.h"
#include <string>
#include <android/bitmap.h>
#include "cpu-features.h"
#include <RenderScript.h>
#include "ScriptC_mono.h"
bool isDeviceSupportByNEON();
using namespace android::RSC;


#define LOG_TAG "native-lib"
extern "C"
{
jstring
Java_uelordi_android_hellocomputendk_1rs_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT void JNICALL
Java_uelordi_android_hellocomputendk_1rs_MainActivity_nativeRsMono(JNIEnv *env, jobject instance,
                                                                 jstring pathObj, jint X,
                                                                 jint Y, jobject jbitmapIn,
                                                                 jobject jbitmapOut)
{
    void* inputPtr = NULL;
    void* outputPtr = NULL;

    AndroidBitmap_lockPixels(env, jbitmapIn, &inputPtr);
    AndroidBitmap_lockPixels(env, jbitmapOut, &outputPtr);

    const char * path = env->GetStringUTFChars(pathObj, NULL);

    sp<RS>  rs = new RS();
    rs->init(path);
    env->ReleaseStringUTFChars(pathObj, path);

    sp<const Element> e = Element::RGBA_8888(rs);

    sp<const Type> t = Type::create(rs, e, X, Y, 0);

    sp<Allocation> inputAlloc = Allocation::createTyped(rs, t, RS_ALLOCATION_MIPMAP_NONE,
                                                        RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT,
                                                        inputPtr);
    sp<Allocation> outputAlloc = Allocation::createTyped(rs, t, RS_ALLOCATION_MIPMAP_NONE,
                                                         RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT,
                                                         outputPtr);


    inputAlloc->copy2DRangeFrom(0, 0, X, Y, inputPtr);
    ScriptC_mono* sc = new ScriptC_mono(rs);//new ScriptC_mono(rs);
    sc->forEach_root(inputAlloc, outputAlloc);
    outputAlloc->copy2DRangeTo(0, 0, X, Y, outputPtr);


    AndroidBitmap_unlockPixels(env, jbitmapIn);
    AndroidBitmap_unlockPixels(env, jbitmapOut);
}
/*
 *
 */
JNIEXPORT void JNICALL
Java_uelordi_android_hellocomputendk_1rs_MainActivity_nativeNeonMono(JNIEnv *env, jobject instance,
                                                                     jstring cacheDir_, jint X,
                                                                     jint Y, jobject in,
                                                                     jobject out) {
    const char *cacheDir = env->GetStringUTFChars(cacheDir_, 0);

    // TODO
    if(isDeviceSupportByNEON) {
        //todo do the neon job in bitmap to grayscale:
    }
    env->ReleaseStringUTFChars(cacheDir_, cacheDir);
}

}
bool isDeviceSupportByNEON() {

    AndroidCpuFamily family = android_getCpuFamily();
    if ((family != ANDROID_CPU_FAMILY_ARM) &&
        (family != ANDROID_CPU_FAMILY_X86))
    {
        LOGV("Not an ARM and not an X86 CPU !\n");
        return false;
    }

    uint64_t  features = android_getCpuFeatures();
    if (((features & ANDROID_CPU_ARM_FEATURE_ARMv7) == 0) &&
        ((features & ANDROID_CPU_X86_FEATURE_SSSE3) == 0))
    {
        LOGV("Not an ARMv7 and not an X86 SSSE3 CPU !\n");
        return false;
    }
}
