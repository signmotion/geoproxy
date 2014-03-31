#ifndef CONFIGURE_H_
#define CONFIGURE_H_


namespace geoproxy {


/**
 * Путь к тестовым файлам.
 */
const std::string PATH_TEST_DATA = "data";




/**
 * Название БД для работы с приложением и параметры подключения к ней.
 *
 * # Работаем на PostgreSQL.
 */
const static std::string NAME_DB     = "geoproxy";
const static std::string USER_DB     = "postgres";
const static std::string PASSWORD_DB = "12345";
const static int         PORT_DB     = 5432;




/**
 * Включено - вывод информации о состоянии процесса.
 */
#define _DEBUG



/**
 * Ограничение на количество печатаемых в поток записей.
 * Введено для визуальной отладки больших списков.
 */
static const size_t LIMIT_COUNT_PRINT_STREAM = 10;


} // geoproxy



#endif /* CONFIGURE_H_ */
