#include <android/native_window_jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstdint>
#include "bridge_init.h"

namespace {
std::atomic<bool> running{false};
std::thread render_thread;

void logi(const char* msg) { __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "%s", msg); }

void render_loop(ANativeWindow* window) {
    const auto pid = static_cast<long long>(getpid());
    const auto tid = static_cast<long long>(syscall(__NR_gettid));
    __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "RENDER_THREAD_START pid=%lld tid=%lld", pid, tid);

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY || !eglInitialize(display, nullptr, nullptr)) {
        logi("RENDER_INIT_FAILED eglInitialize");
        ANativeWindow_release(window);
        return;
    }

    const EGLint config_attrs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };
    EGLConfig config = nullptr;
    EGLint count = 0;
    if (!eglChooseConfig(display, config_attrs, &config, 1, &count) || count != 1) {
        logi("RENDER_INIT_FAILED eglChooseConfig");
        eglTerminate(display);
        ANativeWindow_release(window);
        return;
    }

    const EGLint ctx_attrs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctx_attrs);
    EGLSurface surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (context == EGL_NO_CONTEXT || surface == EGL_NO_SURFACE ||
        !eglMakeCurrent(display, surface, surface, context)) {
        logi("RENDER_INIT_FAILED egl context/surface");
        if (surface != EGL_NO_SURFACE) eglDestroySurface(display, surface);
        if (context != EGL_NO_CONTEXT) eglDestroyContext(display, context);
        eglTerminate(display);
        ANativeWindow_release(window);
        return;
    }

    __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "RENDER_INIT_OK api=OpenGL-ES version=%s vendor=%s renderer=%s pid=%lld tid=%lld",
        glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER), pid, tid);
    bridge_initialize();
    logi("BRIDGE_RENDER_PATH_READY");

    uint64_t frame = 0;
    while (running.load()) {
        ++frame;
        __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "FRAME_BEGIN frame=%llu pid=%lld tid=%lld", (unsigned long long)frame, pid, tid);
        float r = 0.08f + 0.04f * static_cast<float>(frame % 10);
        glViewport(0, 0, 1280, 720);
        glClearColor(r, 0.08f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "FRAME_RENDERED frame=%llu", (unsigned long long)frame);
        eglSwapBuffers(display, surface);
        __android_log_print(ANDROID_LOG_INFO, "PVR-RenderTarget", "FRAME_PRESENT frame=%llu", (unsigned long long)frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);
    ANativeWindow_release(window);
    logi("RENDER_THREAD_STOP");
}
}

extern "C" JNIEXPORT void JNICALL
Java_com_pvr_controlledrendertarget_MainActivity_nativeStart(JNIEnv* env, jclass, jobject surface) {
    if (running.exchange(true)) return;
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (!window) { running.store(false); logi("RENDER_START_FAILED no native window"); return; }
    render_thread = std::thread(render_loop, window);
}

extern "C" JNIEXPORT void JNICALL
Java_com_pvr_controlledrendertarget_MainActivity_nativeStop(JNIEnv*, jclass) {
    if (!running.exchange(false)) return;
    if (render_thread.joinable()) render_thread.join();
}
