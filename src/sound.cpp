#include "include/sound.hpp"
#include <SFML/Audio.hpp>
#include <string>


namespace Sound {

    sf::Music current_music;
    int current_music_id = -1;



    /*

    0 -> background music (old_doll_slowed.mp3 for now)
    1 -> chase music (old_doll_slowed.mp3 for now)
    2 -> main menu music (faint-signal.mp3)
    3 -> win condition music (faint-signal.mp3)
    4 -> lose condition music (faint-signal.mp3)
    5 -> pause screen music (faint-signal.mp3)

    the play, pause and stop functions should control the active music i.e. the one started by startMusic

    if you need global variables (like storing the id, or music playing), declare them inside Sound namespace

    */

    void startMusic(int id) {

        if (current_music_id == id && current_music.getStatus() == sf::Music::Status::Playing) {
            return;
        }

        std::string filepath;

        bool loop = true;

        if (id == 0) {
            filepath = "assets/old_doll_slowed.mp3";
            loop = true;
        }
        else if (id == 1) {
            filepath = "assets/old_doll_slowed.mp3";
            loop = true;
        }
        else if (id == 2) {
            filepath = "assets/faint-signal.mp3";
            loop = true;
        }
        else if (id == 3) {
            filepath = "assets/faint-signal.mp3";
            loop = true;
        }
        else if (id == 4) {
            filepath = "assets/faint-signal.mp3";
            loop = true;
        }
        else if (id == 5) {
            filepath = "assets/faint-signal.mp3";
            loop = true;
        }
        else {
            return;
        }

        current_music.stop();

        if (current_music.openFromFile(filepath)) {

            current_music.setLooping(loop);
            current_music.play();
            current_music_id = id;
        }

    }

    void playMusic() {
        if (current_music_id != -1) {
            current_music.play();
        }
    }

    void pauseMusic() {
        if (current_music_id != -1) {
            current_music.pause();
        }

    }

    void stopMusic() {
        if (current_music_id != -1) {
            current_music.stop();
            current_music_id = -1;
        }

    }

}


