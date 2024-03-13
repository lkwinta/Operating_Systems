# Lab 1 - Podstawy Makefile
1. Zapoznaj się z koncepcją plików Makefile: https://www.gnu.org/software/make/manual/html_node/Introduction.html
2. Zainstaluj/skonfiguruj IDE, w którym będziesz pracować przez resztę laboratoriów. (np. VS Code, Vim, etc.)
3. Stwórz nowy projekt w IDE.
4. Napisz prosty program countdown.c, który będzie w pętli odliczał od 10 do 0 i wypisywał aktualną liczbę na konsolę (każda liczba w nowej linii).
5.  Stwórz plik Makefile, za pomocą którego skompilujesz swój program.
Makefile powinien zawierać co najmniej trzy targety: `all`, `countdown`, `clean`. <br>
`all` — powinien budować wszystkie targety (na razie tylko countdown, w kolejnych zadaniach będziemy dodawać nowe targety). <br>
`countdown` — buduje program countdown.c <br>
`clean` — usuwa wszystkie pliki binarne, czyści stan projektu <br>
Użyj zmiennych CXX oraz CFLAGS do zdefiniowania kompilatora (gcc) i flag compilacji (-Wall, ...). 
Dodatkowo w Makefile powinien być specjalny target .PHONY. 
6. Skompiluj i uruchom program. 
7. Korzystając z gdb (lub lldb) zademonstruj zatrzymywanie programu (breakpoint) wewnątrz pętli. Wypisz aktualny indeks pętli, kontynuuj wykonywanie programu i wypisz następny. <br>
https://sourceware.org/gdb/current/onlinedocs/gdb.html/ <br>
https://sourceware.org/gdb/current/onlinedocs/gdb.html/Sample-Session.html#Sample-Session <br>
https://lldb.llvm.org/use/tutorial.html <br>
8. Skonfiguruj swoje IDE do współpracy z Makefile. Postaw breakpoint (graficznie, klikając) w środku pętli. Uruchom program w trybie Debug i zademonstruj, jak można podejrzeć wartość indeksu pętli i kontynuować wykonywanie programu.
9. Usuń breakpoint i kontnynuuj wykonywanie programu do końca.