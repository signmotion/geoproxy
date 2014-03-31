#ifndef LOADER_H_
#define LOADER_H_

#include "Track.h"


namespace geoproxy {


/**
 * Загружает треки в БД Postgre.
 *
 * Каждый объект складывается из файлов
 *  *.node  Точки
 *  *.edge  Рёбра
 *  *.track Треки
 */
class Loader {
public:
	Loader(
		const std::string& db,
		const std::string& user,
		const std::string& password,
		const int port
    );




	~Loader();




	/**
	 * Загружает файлы треков.
	 *
	 * @param pathFolder Путь к папке треков для загрузки.
	 */
	Loader& operator<<( const std::string& pathFolder );




	/**
	 * Запоминает набор треков в хранилище.
	 */
	void memorize( const Track::set_t& );




private:
	static std::unordered_set< std::string >
	listFile( const std::string& pathFolder );




private:
	const std::string  mDB;
	const std::string  mUser;
	const std::string  mPassword;
	const int          mPort;
};


} // geoproxy




#endif /* LOADER_H_ */
