# Gra w życie 
Napisz współbieżną wersję gry w życie, gdzie obliczanie stanu planszy jest rozdzielone na wątki. Utwórz n wątków, gdzie n to parametr programu, i każdemu przypisz komórki, za które dany wątek jest odpowiedzialny. Rozdziel prace pomiędzy wątkami maksymalnie równomiernie.

Każdy z utworzonych wątków powinien obliczać stan komórek, za które jest odpowiedzialny w następnym kroku czasowym. Wątek główny powinien zajmować się tylko zlecaniem prac i wyświetlaniem wyników. Wątki powinny być tworzone tylko raz na początku programu i powinny wykonywać swoją pracę raz na iterację. (sugestia rozwiązania: `pause()` i `pthread_kill()`)

Do skompilowania gry potrzebna jest biblioteka ncurses. W niektórych dystrybucjach Linux-a trzeba zainstalować nagłówki tej biblioteki. Np.: `apt install libncurses-dev`.

Gra wykorzystuje dwie tablice dla obecnego i następnego kroku czasowego. Każdy wątek zapisuje dane tylko do swoich komórek tablicy następnego kroku, a zatem dostęp do tych tablic nie musi być synchronizowany. 

Pomiędzy wyświetleniami planszy program czeka 0,5 sekundy na wykonanie obliczeń. W tym zadaniu ignorujemy możliwość wystąpienia sytuacji, gdzie jakiś wątek nie zdążył wykonać obliczeń w wyznaczonym czasie, tj. żadna dodatkowa synchronizacja wątków nie jest wymagana.  