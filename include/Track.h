#ifndef TRACK_H_
#define TRACK_H_


namespace geoproxy {


/**
 * Трек.
 */
class Track {
public:
	// @todo fine Заменить на unordered_set.
	typedef std::vector< Track >  set_t;


	struct __attribute__ ((packed)) coord_t {
		int  lat;
		int  lon;
	};


	struct __attribute__ ((packed)) nodeFile_t {
		coord_t  coord;
		bool     vis;
	};


	struct __attribute__ ((packed)) edgeFile_t {
		int  source;
		int  target;
	};


	/**
	 * NA, A1, A2, ..., NB, N1, N2, ..., ...
	 *
	 * NA, NB, ...  Кол-во точек трека.
	 * A1, A2, ...  Индексы очередной точки в одноимённом файле '*.node'.
	 */
	struct trackFile_t {
		// Кол-во int-элементов в наборе 'raw'.
		size_t  size;
		std::unique_ptr< int >  raw;
	};


	struct NodeFile {
		std::vector< nodeFile_t >  content;
	};


	struct EdgeFile {
		std::vector< edgeFile_t >  content;
	};


	struct TrackFile {
		trackFile_t  content;
	};


	typedef std::vector< coord_t >  track_t;




public:
	Track( const track_t& );




	~Track();




	/**
	 * Собирает треки из файлов
	 *  *.node  Точки
	 *  *.edge  Рёбра
	 *  *.track Треки
	 */
	static set_t valueOf( const std::string& file );




	inline track_t const& native() const {
		return mTrack;
	}




private:
	/**
	 * @param pathName Полный путь к загружаемому файлу трека.
	 */
	template< class T >
	static T load( const std::string& pathName );




private:
	track_t  mTrack;



	//friend class TrackTest;
};


} // geoproxy








namespace std {


std::ifstream& operator>>( std::ifstream&,  typename geoproxy::Track::NodeFile& );

std::ifstream& operator>>( std::ifstream&,  typename geoproxy::Track::EdgeFile& );

std::ifstream& operator>>( std::ifstream&,  typename geoproxy::Track::TrackFile& );



std::ostream& operator<<( std::ostream&,  const typename geoproxy::Track::NodeFile& );

std::ostream& operator<<( std::ostream&,  const typename geoproxy::Track::coord_t& );


} // std




#endif /* TRACK_H_ */
