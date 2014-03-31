#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <boost/exception/all.hpp>


namespace geoproxy {


typedef boost::error_info< struct tagNotFoundFolder, std::string >
	NotFoundFolder;

typedef boost::error_info< struct tagNotFoundFile, std::string >
	NotFoundFile;
typedef boost::error_info< struct tagNotOpenFile, std::string >
	NotOpenFile;
typedef boost::error_info< struct tagCorruptFile, std::string >
	CorruptFile;

/**
 * В файле *.track обнаружен индекс, которого нет в *.node.
 */
typedef boost::error_info<
	struct tagCorruptTrackIndex,
	// индекс в файле *.track
	int
> CorruptTrackIndex;




struct Exception: virtual boost::exception, virtual std::exception {
};


} // geoproxy




#endif /* EXCEPTION_H_ */
