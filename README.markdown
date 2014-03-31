*Задача*

См. файл 'Прокси-сервер (задача).odt'.

Коротко. Для построения маршрутов используем открытый API от компании Визиком,
предоставляющий, помимо прочего, возможность получения маршрута между двумя точками.
При большом количестве запросов некоторые из них часто (изо дня в день) повторяются.
Необходимо создать сервер, предоставляющий доступ к визикомовскому сервису с
возможностью кеширования получаемых треков и обновлением некоторой области кеша по
определённой команде.

* PostgreSQL
* boost::asio
* Visicom API
* libjson
* libpqxx




*Команды для работы с прокси-сервером*

Сервер понимает REST-запросы вида
http://localhost:ПОРТ/КОМАНДА

Примеры запросов:
(1) http://localhost:9999/route.json?start=30.51258,50.470351&finish=30.51326,50.47388&a=10&key=8c1b40dc9c4d17d3e54360dda073b9eb

(2) http://localhost:9999/area?start=30.51258,50.470351&finish=31.51326,52.47388&key=8c1b40dc9c4d17d3e54360dda073b9eb

(3) http://localhost:9999/off


* /route.json  Возвращает детализированную информацию о маршруте
               между двумя точками. Делает к API Visicom, два запроса:
               для получения общей инф. по маршруту (route) и
               для детализации этой информации (feauture).

* /area     Возвращает все маршруты из заданной прямоугольной области,
               которые когда-либо обрабатывались прокси-сервером. При указании
               параметра '&update' обновляет маршруты в РБД.

* /off        Выключает сервер.



Реализация

* Используется boost::filesystem. Поэтому библиотеки boost_filesystem и
  boost_system должны быть доступны:
  ~$ sudo ldconfig ПУТЬ_К_ПАПКЕ_BOOST_БИБЛИОТЕК

* Файл configure.h содержит параметры по умолчанию (подключение к Postgres,
  порт сервера и пр.). Логин / пароль по умолчанию для Postgres:
  postgres / postgres
  Параметры также могут быть переданы в виде командной строки:
  geoproxy \
      PORT_SERVER\
      KEY_API_VISICOM\
      NAME_DB USER_DB\
      PASSWORD_DB\
      HOST_DB\
      PORT_DB

* Работаем с Postgre SQL 9.2: с этой версии встроена поддержка JSON.

* Перед запуском должна быть создана РБД 'geoproxy'. В консоли:
  ~$ sudo -u postgres psql -W
  =# CREATE DATABASE geoproxy

* Исключения сделаны с помощью boost::exception. Смысл исключения понятен из
  названия. Например, tagNotFoundFolder или tagNotFoundFile - следом идёт
  значение не найденной папки или файла.



Возможные ошибки и пути их решения

(1)
error while loading shared libraries: libboost_filesystem.so.1.53.0:
cannot open shared object file: No such file or directory

Приложение должно знать расположение динамических boost-библиотек. В консоли:
~$ sudo ldconfig ПУТЬ_К_BOOST/stage/lib

(2)
Dynamic exception type: boost::exception_detail::clone_impl
<boost::exception_detail::error_info_injector<boost::system::system_error> >
std::exception::what: bind: Address already in use

Сервер уже слушает порт. Или укажите другой порт при запуске приложения. Или
закройте приложение. Или снимете процесс в консоли:
~$ ps -ax | grep geoproxy
~$ kill UID_ПРОЦЕССА
