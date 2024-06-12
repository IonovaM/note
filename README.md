### note
###### Установка приложения на MacOS:
1.	Зайдите в командную строку
2.	Необходимо установить следующие пакеты qt@6, cmake, ninja:
```brew install qt@6 cmake ninja```
3.	Зайдите в папку проекта
4.	Создайте директорию build, настройте проект с помощью CMake, используя Ninja в качестве генератора и указывая тип сборки Debug:
```cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug > cmake_log.txt```
5.	Затем соберите проект:
```cmake --build build > build_log.txt```
6.	Дождитесь появления в командной строке промпта командной строки (строки приглашения). Перейдите в папку build и откройте собранное приложение. 
* сборка проекта происходит в режиме отладки и лог с возможными ворнингами направляется в файл.

###### Установка приложения на Ubuntu:
1.	Зайдите в командную строку
2.	Необходимо установить следующие пакеты cmake g++ git qt6-base-private-dev qt6-declarative-dev libgl-dev:
```sudo apt update && sudo apt install cmake g++ git qt6-base-private-dev qt6-declarative-dev libgl-dev```
3.	Зайдите в папку проекта
4.	Создайте директорию build:
```mkdir build && cd build```
6.	Выполните конфигурацию проекта с помощью CMake, указывая каталог установки /usr и тип сборки Debug, а затем запустит сборку проекта с помощью make с максимальным количеством потоков, доступных на вашей системе, указанным через nproc.:
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug && make -j$(nproc)
7.	Затем соберите проект (как суперпользователь):
```make install```
8.	После выполнения этой команды Notes будет установлен как обычное Linux приложение и будет доступен для запуска из командной строки или через меню приложений. 
