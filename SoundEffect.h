#ifndef SoundEffect_h
#define SoundEffect_h

#include"CommonFunc.h"

class SoundEffect {
public:
    SoundEffect();
    ~SoundEffect();

    bool loadFromFile(const std::string& path);
    void playOnce();
    void reset(); 
    void setVolume(int volume);

private:
    Mix_Chunk* mChunk;
    bool mPlayed;
};
#endif