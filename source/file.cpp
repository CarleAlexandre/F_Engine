#include <engine.hpp>
#include <fstream>
#include <sstream>

char *readFile(const char *filepath) {
	std::ifstream file;
	std::stringstream str;
	char *ret;

	if (FileExists(filepath)) {
		file.open(filepath);
		str << file.rdbuf();
		ret = (char *)MemAlloc(str.str().size() + 1);
		ret[str.str().size()] = 0;
		str.str().copy(ret, str.str().size());
		str.clear();
		//need to free returned value after use;
		file.close();
		return (ret);
	}
	return (NULL);
}
