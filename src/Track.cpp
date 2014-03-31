#include "../include/stdafx.h"
#include "../configure.h"
#include "../include/Track.h"


namespace geoproxy {


Track::Track(
	const track_t& track
) :
	mTrack( track )
{
	assert( !mTrack.empty()
		&& "Получен пустой трек." );
}




Track::~Track() {
}




Track::set_t Track::valueOf( const std::string& file ) {

	assert( !file.empty()
		&& "Имя файла должно быть указано." );

	const auto nodeFile  = load< NodeFile >( file + ".node" );
	const auto edgeFile  = load< EdgeFile >( file + ".edge" );
	const auto trackFile = load< TrackFile >( file + ".track" );

	// собираем треки из полученного содержимого
#ifdef _DEBUG
	std::cout << "harvest tracks ..." << std::endl;
#endif

	set_t r;
	for (auto itr = trackFile.content.raw.get();
		 itr < (trackFile.content.raw.get() + trackFile.content.size);
		 ++itr
	) {
		// # Запомним трек в человеческом (не sql-ризованном) виде.
		track_t track;
		// кол-во точек в треке
		const int n = *itr;
#ifdef _DEBUG
		//std::cout << "build track " << r.size() << " from " << n << " points ..." << std::endl;
#endif
		// выберем этот трек из потока
		for (int o = 0; o < n; ++o) {
			++itr;
			// индекс точки в NodeFile
			const int i = *itr;
			if (static_cast< size_t >( i ) >= nodeFile.content.size()) {
				throw Exception() << CorruptTrackIndex( i );
			}
			const coord_t& coord = nodeFile.content[ i ].coord;
			track.push_back( coord );
		}
		// трек собран, следующий
		r.push_back( { track } );

#ifdef _DEBUG
		//std::cout << "the track from " << track.size() << " points is completed" << std::endl;
#endif
	} // for (auto itr = ...

#ifdef _DEBUG
	std::cout << "the " << r.size() << " tracks are harvested from the file '" << file << "'\n" << std::endl;
#endif

	return r;
}




template< class T >
T Track::load( const std::string& pathName ) {

	namespace fs = boost::filesystem;

#ifdef _DEBUG
	std::cout << "load '" << pathName << "'" << std::endl;
#endif

	if ( !fs::exists( pathName ) ) {
		throw Exception() << NotFoundFile( pathName );
	}

	std::ifstream  stream( pathName, std::ios::binary );
	if ( !stream.is_open() ) {
		throw Exception() << NotOpenFile( pathName );
	}

#ifdef _DEBUG
	{
		std::streampos size = stream.tellg();
		stream.seekg( 0, std::ios::end );
		size = stream.tellg() - size;
		std::cout << "size file '" << pathName << "' " << size << " byte" << std::endl;
		stream.seekg( 0, std::ios::beg );
	}
#endif


	T r;
	stream >> r;

#ifdef _DEBUG
	std::cout << r << std::endl;
#endif

	return r;
}


} // geoproxy








namespace std {


std::ifstream& operator>>( std::ifstream& ifs,  typename geoproxy::Track::NodeFile& r ) {

	using namespace geoproxy;

	/* - Заменено. См. ниже.
	std::copy(
		std::istream_iterator< Track::nodeFile_t >( ifs ),
		std::istream_iterator< Track::nodeFile_t >(),
		std::back_inserter( r.content )
	);
	*/

	// прочитаем
	std::streampos pos = ifs.tellg();
	ifs.seekg( 0, std::ios::end );
	const auto size = static_cast< size_t >( ifs.tellg() - pos );
	ifs.seekg( 0, std::ios::beg );

	const auto sizeStruct = sizeof( Track::nodeFile_t );
	const auto countStruct = size / sizeStruct;
	if ((size % sizeStruct) != 0) {
		throw Exception() <<
			CorruptFile( "Size of the file is not assigned with size of the struct Track::nodeFile_t." );
	}

	std::unique_ptr< char >  buffer( new char[ size ] );
	ifs.read( buffer.get(),  size );

	// соберём
	for (auto itr = buffer.get(); itr < (buffer.get() + size); itr += sizeStruct) {
		const auto t = *reinterpret_cast< const Track::nodeFile_t* >( itr );
		r.content.push_back( t );
	}

	return ifs;
}




// @todo fine Свернуть operator>>() в один метод.
std::ifstream& operator>>( std::ifstream& ifs,  typename geoproxy::Track::EdgeFile& r ) {

	using namespace geoproxy;

	// прочитаем
	std::streampos pos = ifs.tellg();
	ifs.seekg( 0, std::ios::end );
	const auto size = static_cast< size_t >( ifs.tellg() - pos );
	ifs.seekg( 0, std::ios::beg );

	const auto sizeStruct = sizeof( Track::edgeFile_t );
	const auto countStruct = size / sizeStruct;
	if ((size % sizeStruct) != 0) {
		throw Exception() <<
			CorruptFile( "Size of the file is not assigned with size of the struct Track::edgeFile_t." );
	}

	std::unique_ptr< char >  buffer( new char[ size ] );
	ifs.read( buffer.get(),  size );

	// соберём
	for (auto itr = buffer.get(); itr < (buffer.get() + size); itr += sizeStruct) {
		const auto t = *reinterpret_cast< const Track::edgeFile_t* >( itr );
		r.content.push_back( t );
	}

	return ifs;
}




std::ifstream& operator>>( std::ifstream& ifs,  typename geoproxy::Track::TrackFile& r ) {

	using namespace geoproxy;

	// прочитаем
	std::streampos pos = ifs.tellg();
	ifs.seekg( 0, std::ios::end );
	const auto size = static_cast< size_t >( ifs.tellg() - pos );
	ifs.seekg( 0, std::ios::beg );

	const auto sizeInt = sizeof( int );
	const auto countInt = size / sizeInt;
	if ((size % sizeInt) != 0) {
		throw Exception() <<
			CorruptFile( "Size of the file is not assigned with size of the struct Track::trackFile_t." );
	}

	std::unique_ptr< char >  buffer( new char[ size ] );
	ifs.read( buffer.get(),  size );

	// соберём
	// # Запомним как int, чтобы проще было работать.
	r.content.size = size / sizeInt;
	std::unique_ptr< int >  raw( new int[ r.content.size ] );
	size_t k = 0;
	for (auto itr = buffer.get(); itr < (buffer.get() + size); itr += sizeInt) {
		raw.get()[ k ] = *reinterpret_cast< const int* >( itr );
		++k;
	}
	r.content.raw = std::move( raw );

	return ifs;
}




std::ostream& operator<<( std::ostream& os,  const typename geoproxy::Track::NodeFile& r ) {
	for (auto itr = r.content.cbegin(); itr != r.content.cend(); ++itr) {
		const size_t i = std::distance( r.content.cbegin(), itr );
		os << "(" << i << ") " <<
			itr->coord << "\t" <<
			(itr->vis ? "yes" : "no") <<
			std::endl;
		if (i > geoproxy::LIMIT_COUNT_PRINT_STREAM) {
			os << "... (" << r.content.size() << ")" << std::endl;
			break;
		}
	}
	return os;
}




std::ostream& operator<<( std::ostream& os,  const typename geoproxy::Track::EdgeFile& r ) {
	for (auto itr = r.content.cbegin(); itr != r.content.cend(); ++itr) {
		const size_t i = std::distance( r.content.cbegin(), itr );
		os << "(" << i << ") " <<
			itr->source << "\t" <<
			itr->target <<
			std::endl;
		if (i > geoproxy::LIMIT_COUNT_PRINT_STREAM) {
			os << "... (" << r.content.size() << ")" << std::endl;
			break;
		}
	}
	return os;
}




std::ostream& operator<<( std::ostream& os,  const typename geoproxy::Track::TrackFile& r ) {
	return os << "(" << r.content.size << ") int values" << std::endl;
}




std::ostream& operator<<( std::ostream& os,  const typename geoproxy::Track::coord_t& r ) {
	return os <<
		( static_cast< double >( r.lat ) * 1e-5 ) << "\t" <<
		( static_cast< double >( r.lon ) * 1e-5 );
}


} // std
