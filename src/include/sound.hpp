#pragma once

#include <string>
namespace Sound {

    enum class MusicType {
        None,
        Menu,
        Background,
        Chase,
        Win,
        Lose
    };

    void startMusic(MusicType type);
    void playMusic();
    void pauseMusic();
    void stopMusic();

    void playSound(std::string filepath);
}