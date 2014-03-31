#include "include/stdafx.h"
#include "configure.h"
#include "include/Loader.h"


int main() {

	using namespace geoproxy;

	try {
		// загружаем тестовый набор
		Loader  loader( NAME_DB, USER_DB, PASSWORD_DB, PORT_DB );
		loader << geoproxy::PATH_TEST_DATA;

	} catch ( const boost::exception& ex ) {
		std::cout << "(!) " << boost::diagnostic_information( ex ) << std::endl;
		throw;
	}


	puts( "^" );

	return EXIT_SUCCESS;
}
