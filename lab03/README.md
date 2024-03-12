W zadaniu 1 można wybrać do zaimplementowania tylko jeden wariant:

    albo fopen(), fseek(), fread(), fwrite(), fclose()
    albo open(), lseek(), read(), write(), close()

Wybrany wariant należy opracować na dwa sposoby:

    Czytanie po 1 znaku.
    Czytanie bloków po 1024 znaki (plik wynikowy powinien być identyczny jak w wariancie 1.)

Dla obu sposobów implementacji należy przeprowadzić pomiar czasu wykonywania. Wyniki należy przedstawić w formie pliku pomiar_zad_2.txt 

Zadanie 1 Napisz program, który kopiuje zawartość jednego pliku do drugiego, odwróconą bajt po bajcie.

Wskazówki: Wywołania w rodzaju fseek(infile, +1024, SEEK_END) lub lseek(in, +1024, SEEK_END) są zupełnie legalne i nie powodują żadnych skutków ubocznych. Aby po przeczytaniu bloku znaków cofnąć się na początek poprzedniego bloku, należy jako drugi argument funkcji fseek(..., ..., SEEK_CUR) lub lseek(...., ..., SEEK_CUR) podać podwojoną długość bloku ze znakiem minus. Działanie programu należy zweryfikować następująco: 1) odwrócić krótki plik tekstowy, podejrzeć wynik, sprawdzić szczególnie początkowe i końcowe znaki. 2) ./reverse plik_binarny tmp1 ; ./reverse tmp1 tmp2 ; diff -s tmp2 plik_binarny 3) można też porównać (diff -s) wynik działania programu i wynik polecenia tac < plik_wejściowy | rev > plik_wyjściowy

Zadanie 2 Napisz program, który będzie przeglądał bieżący katalog, korzystając z funkcji opendir(), readdir() i stat(). Dla każdego znalezionego pliku, który nie jest katalogiem, czyli !S_ISDIR(bufor_stat.st_mode), należy wypisać rozmiar i nazwę pliku. Ponadto na koniec należy wypisać sumaryczny rozmiar wszystkich plików. Nie należy przeglądać podkatalogów! Sumaryczny rozmiar plików należy przechowywać w zmiennej typu long long i wypisywać ją przez format %lld.

Działanie programu porównaj z działaniem polecenia wc --bytes *