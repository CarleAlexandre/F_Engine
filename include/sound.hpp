#ifndef SOUND_HPP
# define SOUND_HPP

#include "include.hpp"

typedef enum {
	intro,
	tech,
	select,
	skizo,
	stb,
}sound_e;

class SOUND {
	private:
		std::vector<Sound> sounds;
	public:
	
		void play(int idx) {
			if (!IsSoundPlaying(sounds[idx])) {
				PlaySound(sounds[idx]);
			}
		}
		//get sound pos, raycast to play, if obstacle then muffle and diminish the sound by the distance traveled
		//ican cheat, if something is on screen then we get sound else not
		SOUND() {
			FilePathList File = LoadDirectoryFiles("assets/sound");
            for (int i = 0; i < File.count; i++) {
                sounds.push_back(LoadSound(File.paths[i]));
            }
            UnloadDirectoryFiles(File);
		}
		~SOUND() {

		}
};

#endif