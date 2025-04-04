#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

class Music {
private:
    std::vector<std::string> playlist;
    int currentTrack;
    Mix_Music* music;
    static Music* instance;
    static void onMusicFinished();

public:
    Music(const std::vector<std::string>& tracks);
    ~Music();
    void initMixer();
    void closeMixer();
    void playNext();
    void playPlaylist();
};

#endif // MUSIC_H