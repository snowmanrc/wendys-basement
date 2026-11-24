#include "include/sound.hpp"
#include "include/assets.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>
#include <string>
#include <vector>


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
                filepath = Assets::ambience_music; break;
            case MusicType::Chase:
                filepath = Assets::old_doll_music; break;
            case MusicType::Win:
                filepath = Assets::golden_wind_music; break;
            case MusicType::Lose:
                filepath = Assets::old_doll_music; break;
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

    void playSound(std::string filepath) {

        static std::map<std::string, sf::SoundBuffer> buffers;
        static std::vector<sf::Sound> sounds;

        while (sounds.size() > 0) {
            if (sounds[0].getStatus() == sf::Sound::Status::Stopped) {
                sounds.erase(sounds.begin());
                continue;
            }
            break;
        }

        auto [iter, inserted] = buffers.try_emplace(filepath);
        
        if (inserted && !iter->second.loadFromFile(filepath)) {
            buffers.erase(iter);
            return;
        }
        
        auto& sound = sounds.emplace_back(iter->second);
        sound.setLooping(false);
        sound.play();
    }
}


