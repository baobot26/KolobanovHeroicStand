#include "../header/music.h"
#include <iostream>

Music* Music::instance = nullptr;

Music::Music(const std::vector<std::string>& tracks) 
    : playlist(tracks), currentTrack(0), music(nullptr) {
    instance = this;
}

Music::~Music() {
    closeMixer();
}

void Music::initMixer() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Lỗi khởi tạo mixer: " << Mix_GetError() << std::endl;
    }
}

void Music::closeMixer() {
    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }
    Mix_CloseAudio();
    Mix_Quit();
}

void Music::playNext() {
    if (playlist.empty()) return;
    if (music) {
        Mix_FreeMusic(music);
        music = nullptr;
    }
    music = Mix_LoadMUS(playlist[currentTrack].c_str());
    if (!music) {
        std::cerr << "Lỗi load nhạc: " << Mix_GetError() << std::endl;
        return;
    }
    if (Mix_PlayMusic(music, 1) == -1) {
        std::cerr << "Lỗi phát nhạc: " << Mix_GetError() << std::endl;
    }
    currentTrack = (currentTrack + 1) % playlist.size();
}

void Music::playPlaylist() {
    if (playlist.empty()) return;
    playNext();
    Mix_HookMusicFinished(onMusicFinished);
}

void Music::onMusicFinished() {
    if (instance) {
        instance->playNext();
    }
}