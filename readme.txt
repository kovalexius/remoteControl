Используется libvncserver отсюда https://github.com/LibVNC/
И SDL2 отсюда https://www.libsdl.org/download-2.0.php

Чтобы remoteArm не работал в одном потоке, нужно его запустить в background mode.
Но под Windows для это требуется pthreads library.

В данном проекте все зависимости храняться в репозитории. 
Пока не хватило времени организовать автоматическую сборку pthreads, а самое главное - заставить найти libvncserver эту библиотеку pthreads.

pthread взяты отсюда:
https://sourceforge.net/projects/pthreads4w/

и отсюда:
https://www.sourceware.org/pthreads-win32/