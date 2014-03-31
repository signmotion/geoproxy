#include "../include/stdafx.h"
#include "../configure.h"
#include "../include/Loader.h"


namespace geoproxy {


Loader::Loader(
	const std::string& db,
	const std::string& user,
	const std::string& password,
	const int port
) :
	mDB( db ),
	mUser( user ),
	mPassword( password ),
	mPort( port )
{
	assert( !mDB.empty()
		&& "Название базы должно быть указано." );
	assert( !mUser.empty()
		&& "Имя пользователя должно быть указано." );

	/*
	std::stringstream ss;
	ss << "dbname="   << mDB <<
		 " user="     << mUser <<
		 " password=" << mPassword;
	pqxx::connection  connection( ss.str() );
	pqxx::work  work( connection );
	//pqxx::result r = work.exec( "SELECT 1" );
	//work.commit();
	//std::cout << r[0][0].as< int >() << std::endl;
	*/
}




Loader::~Loader() {
}




Loader& Loader::operator<<( const std::string& pathFolder ) {
	assert( !pathFolder.empty()
		&& "Название папки должно быть указано." );

	const auto lf = listFile( pathFolder );
	assert( !lf.empty()
		&& "Отсутствует тестовый набор файлов." );

	size_t countAllTrack = 0;
	for (auto itr = lf.cbegin(); itr != lf.cend(); ++itr) {
		const std::string& file = *itr;
		// получим набор треков из файла и запомним набор в хранилище
		const auto trackSet = Track::valueOf( pathFolder + "/" + file );
		memorize( trackSet );
		countAllTrack += trackSet.size();
	}

#ifdef _DEBUG
	std::cout << "from the " << lf.size() <<
		"(x3) files of the folder '" << pathFolder << "' are harvested " <<
		countAllTrack << " tracks" <<
		std::endl;
#endif

	return *this;
}




void Loader::memorize( const Track::set_t& ts ) {

}




std::unordered_set< std::string > Loader::listFile( const std::string&  pathFolder ) {

    namespace fs = boost::filesystem;

    std::unordered_set< std::string >  r;

    const fs::path  path( pathFolder );
    if ( !fs::exists( path ) ) {
    	throw Exception() << NotFoundFolder( pathFolder );
    }

    fs::directory_iterator endItr;
    for (fs::directory_iterator itr( path ); itr != endItr; ++itr) {
        if ( fs::is_regular_file( itr->status() ) ) {
            const std::string ext = itr->path().extension().string();
            if ( boost::iequals( ".node",  ext)
              || boost::iequals( ".edge",  ext)
              || boost::iequals( ".track", ext)
            ) {
                const std::string f = itr->path().stem().string();
                r.insert( f );
            }
        }
    }

    return r;
}


} // geoproxy
