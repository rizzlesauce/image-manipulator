#ifndef Utils_h
#define Utils_h

#include <string>

// Utils
//
class Utils {
	public:

	static Utils& getInstance();
	std::string itos(int);
	std::string date(time_t);	

	private:
	Utils();
};	

#endif
