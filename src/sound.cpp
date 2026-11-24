#include "include/sound.hpp"
#include "include/assets.hpp"
#include <SFML/Audio.hpp>
#include <string>


namespace Sound {

    sf::Music current_music;
    MusicType current_music_type = MusicType::None;


    void startMusic(MusicType type) {

        if (Sound::current_music_type == type && current_music.getStatus() == sf::Music::Status::Playing) {
            return;
        }


        std::string filepath;

        switch (type) {
            case MusicType::None:
                return;
            case MusicType::Menu:
                filepath = Assets::faint_signal_music; break;
            case MusicType::Background:
                filepath = Assets::old_doll_slowed_music; break;
            case MusicType::Chase:
                filepath = Assets::old_doll_slowed_music; break;
            case MusicType::Win:
                filepath = Assets::golden_wind_music; break;
            case MusicType::Lose:
                filepath = Assets::old_doll_slowed_music; break;
        }

        current_music.stop();

        if (current_music.openFromFile(filepath)) {
            current_music.setLooping(true);
            current_music.play();
            Sound::current_music_type = type;
        }

    }

    void playMusic() {
        if (Sound::current_music_type != MusicType::None) {
            current_music.play();
        }
    }

    void pauseMusic() {
        if (Sound::current_music_type != MusicType::None) {
            current_music.pause();
        }

    }

    void stopMusic() {
        if (Sound::current_music_type != MusicType::None) {
            current_music.stop();
            Sound::current_music_type = MusicType::None;
        }

    }

}


