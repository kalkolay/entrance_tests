# Тестовое задание Elverils 

Задание:\
Написать приложение на C++ для Windows 10 с иcпользованием графического апи на выбор (DirectX, OpenGL, Vulkan) которое выводит на экран графики загруженности CPU (общий и по ядрам).\
Как пример интерфейса можно использовать стандартный Task Manager Windows 10.

Дополнительные требования:
- На графике используются разные цвета в зависимости от степени нагрузки (например диапазон пиковой загрузки - красный);
- Для хранения данных графика используется самостоятельно написанный контейнер (т.е. не использовать std, boost и другие библиотеки), который использует шаблон для определения типа хранимых в нем данных;
- Память для данных графиков выделяется один раз при запуске приложения;
- Каждый отдельный график обновляется в отдельном потоке (в том числе и общий). Потоки так же должны быть созданы один раз при запуске приложения. Потоки создаются средствами операционной системы (через WinAPI);
- Класс который обновляет конкретный график, наследуется от базового интерфейса Worker (по факту их выходит 2, один для ядер и один для общего);
- Приветствуется использование новых стандартов C++. Но не стоит ими злоупотреблять;
- Минимизировать вызовы WinAPI.

________________________________________

Проект **"cpu_consumption"** выполнен в *Visual Studio 2015* с использованием библиотеки *GLUT*.\
Проект **"cpu_consumption_mac_os"** выполнен в *Xcode v11.5* с использованием библиотек *GLEW/GLFW* и шейдеров на *GLSL (version 330 core)*.
