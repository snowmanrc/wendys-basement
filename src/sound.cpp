#include "include/sound.hpp"
#include <SFML/Audio.hpp>


namespace Sound {

    sf::Music myMusic;
    int currentMusicId = -1;



    /*

    0 -> background music
    1 -> chase music

    for now, treat old doll as background music and do not add other music types.

    the play, pause and stop functions should control the active music i.e. the one started by startMusic

    if you need global variables (like storing the id, or music playing), declare them inside Sound namespace

    */

    void startMusic(int id) {

        if (id == 0) {

            if (myMusic.openFromFile("assets/old_doll_slowed.mp3")) {
                myMusic.setLooping(true);
                myMusic.play();
                currentMusicId = id;

            }

        }

    }

    void playMusic() {
        if (currentMusicId != -1) {
            myMusic.play();
        }
    }

    void pauseMusic() {
        if (currentMusicId != -1) {
            myMusic.pause();
        }

    }

    void stopMusic() {
        if (currentMusicId != -1) {
            myMusic.stop();
            currentMusicId = -1;
        }

    }

}


