#include "bridge_init.h"

#include <jni.h>

extern "C" JNIEXPORT jint JNICALL
Java_com_pvr_exp001_harness_MainActivity_nativeExplicitInitialize(
    JNIEnv* /*env*/, jobject /*thiz*/) {
    return static_cast<jint>(bridge_initialize());
}
