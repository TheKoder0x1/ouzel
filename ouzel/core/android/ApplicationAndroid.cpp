// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <cstdlib>
#include "ApplicationAndroid.h"
#include "core/Engine.h"
#include "utils/Log.h"

namespace ouzel
{
    ApplicationAndroid::ApplicationAndroid(JavaVM* aJavaVM):
        javaVM(aJavaVM)
    {
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        uriClass = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(jniEnv->FindClass("android/net/Uri")));
        parseMethod = jniEnv->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
        intentClass = reinterpret_cast<jclass>(jniEnv->NewGlobalRef(jniEnv->FindClass("android/content/Intent")));
        intentConstructor = jniEnv->GetMethodID(intentClass, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
    }

    ApplicationAndroid::~ApplicationAndroid()
    {
        if (updateThread.joinable()) updateThread.join();

        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        if (mainActivity) jniEnv->DeleteGlobalRef(mainActivity);
        if (surface) jniEnv->DeleteGlobalRef(surface);
        if (intentClass) jniEnv->DeleteGlobalRef(intentClass);
        if (uriClass) jniEnv->DeleteGlobalRef(uriClass);
    }

    void ApplicationAndroid::setMainActivity(jobject aMainActivity)
    {
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        mainActivity = jniEnv->NewGlobalRef(aMainActivity);

        jclass mainActivityClass = jniEnv->GetObjectClass(mainActivity);
        startActivityMethod = jniEnv->GetMethodID(mainActivityClass, "startActivity", "(Landroid/content/Intent;)V");
    }

    void ApplicationAndroid::setAssetManager(jobject aAssetManager)
    {
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        assetManager = AAssetManager_fromJava(jniEnv, aAssetManager);
    }

    void ApplicationAndroid::setSurface(jobject aSurface)
    {
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return;
        }

        surface = jniEnv->NewGlobalRef(aSurface);
    }

    int ApplicationAndroid::run()
    {
        updateThread = std::thread(&ApplicationAndroid::update, this);

        return EXIT_SUCCESS;
    }

    bool ApplicationAndroid::openURL(const std::string& url)
    {
        JNIEnv* jniEnv;

        if (javaVM->GetEnv(reinterpret_cast<void**>(&jniEnv), JNI_VERSION_1_6) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to get JNI environment";
            return false;
        }

        jstring actionString = jniEnv->NewStringUTF("android.intent.action.VIEW");
        jstring urlString = jniEnv->NewStringUTF(url.c_str());
        jobject uri = jniEnv->CallStaticObjectMethod(uriClass, parseMethod, urlString);
        jobject intentObject = jniEnv->NewObject(intentClass, intentConstructor, actionString, uri);

        jniEnv->CallVoidMethod(mainActivity, startActivityMethod, intentObject);

        jniEnv->DeleteLocalRef(intentObject);
        jniEnv->DeleteLocalRef(uri);
        jniEnv->DeleteLocalRef(urlString);
        jniEnv->DeleteLocalRef(actionString);

        if (jniEnv->ExceptionCheck())
        {
            Log(Log::Level::ERR) << "Failed to open URL";
            jniEnv->ExceptionClear();
            return false;
        }

        return true;
    }

    void ApplicationAndroid::update()
    {
        JNIEnv* jniEnv;
        JavaVMAttachArgs attachArgs;
        attachArgs.version = JNI_VERSION_1_6;
        attachArgs.name = NULL; // thread name
        attachArgs.group = NULL; // thread group
        if (javaVM->AttachCurrentThread(&jniEnv, &attachArgs) != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to attach current thread to Java VM";
        }

        ouzelMain(ouzel::sharedApplication->getArgs());

        if (!sharedEngine)
        {
            return;
        }

        while (active)
        {
            executeAll();

            if (!sharedEngine->draw())
            {
                break;
            }
        }

        if (ouzel::sharedEngine)
        {
            ouzel::sharedEngine->stop();
        }

        if (javaVM->DetachCurrentThread() != JNI_OK)
        {
            Log(Log::Level::ERR) << "Failed to detach current thread from Java VM";
        }

        ::exit(EXIT_SUCCESS);
    }
}
