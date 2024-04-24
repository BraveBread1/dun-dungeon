#include"SoundEffect.h"


SoundEffect::SoundEffect() : mChunk(nullptr), mPlayed(false) {}

SoundEffect::~SoundEffect() {
    Mix_FreeChunk(mChunk);
}

bool SoundEffect::loadFromFile(const std::string& path) {
    if (mChunk != nullptr) {
        Mix_FreeChunk(mChunk);
        mChunk = nullptr;
    }

    mChunk = Mix_LoadWAV(path.c_str());
    if (mChunk == nullptr) {
        printf("unable to load sound effect %s! SDL_mixer Error: %s\n", path.c_str(), Mix_GetError());
        return false;
    }
    return true;
}

void SoundEffect::playOnce() {
    if (!mPlayed && mChunk != nullptr) {
        Mix_PlayChannel(-1, mChunk, 0);
        mPlayed = true;
    }
}

void SoundEffect::reset() {
    mPlayed = false;
}

void SoundEffect::setVolume(int volume) {
    if (mChunk != nullptr) {
        Mix_VolumeChunk(mChunk, volume);
    }
}