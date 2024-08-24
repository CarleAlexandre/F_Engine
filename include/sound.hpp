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

typedef struct s_spatial_sound{
	Vector2 pos;
	int		idx;
}t_spatial_sound;

class SOUND {
	private:
		std::vector<Sound> sounds;
		std::queue<int> music_queue;
		std::queue<t_spatial_sound> spatial_queue;
	public:
	
		void play(int idx) {
			music_queue.push(idx);
		}

		//get sound pos, raycast to play, if obstacle then muffle and diminish the sound by the distance traveled
		void playSpatial() {
		}

		void updateSpatial() {

		}

		void update() {

		}

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