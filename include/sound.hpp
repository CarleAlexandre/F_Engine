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
	
		void play(){}

		void playSpatial() {

		}

		void updateSpatial(){}

		void update(){}

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