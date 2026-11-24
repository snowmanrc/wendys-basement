#pragma once

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
}