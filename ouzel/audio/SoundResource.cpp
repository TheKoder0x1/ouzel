// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "SoundResource.h"

namespace ouzel
{
    namespace audio
    {
        SoundResource::SoundResource()
        {
        }

        SoundResource::~SoundResource()
        {
        }

        bool SoundResource::init(const std::shared_ptr<SoundData>& newSoundData)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            soundData = newSoundData;

            dirty |= DIRTY_SOUND_DATA | DIRTY_POSITION | DIRTY_PITCH | DIRTY_GAIN | DIRTY_PLAY_STATE;

            return true;
        }

        void SoundResource::setPosition(const Vector3& newPosition)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            position = newPosition;

            dirty |= DIRTY_POSITION;
        }

        void SoundResource::setPitch(float newPitch)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            pitch = newPitch;

            dirty |= DIRTY_PITCH;
        }

        void SoundResource::setGain(float newGain)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            gain = newGain;

            dirty |= DIRTY_GAIN;
        }

        bool SoundResource::play(bool repeatSound)
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            shouldPlay = true;
            repeat = repeatSound;

            dirty |= DIRTY_PLAY_STATE;

            return true;
        }

        bool SoundResource::pause()
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            shouldPlay = false;

            dirty |= DIRTY_PLAY_STATE;

            return true;
        }

        bool SoundResource::stop()
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            shouldPlay = false;
            reset = true;

            dirty |= DIRTY_PLAY_STATE;

            return true;
        }
    } // namespace audio
} // namespace ouzel
