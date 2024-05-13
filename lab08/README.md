# Zadanie 1. Symulator systemu wydruku

Wykorzystując semafory i pamięć wspólną z IPC Systemu V lub standardu POSIX napisz program symulujący działanie systemu wydruku: 

System składa się z N użytkowników oraz M drukarek. Każdy z użytkowników może wysłać do systemu zadanie wydruku tekstu składającego się z 10 znaków. Drukarka, która nie jest aktualnie zajęta, podejmuje się zadania "wydruku" tekstu danego zadania. Wydruk w zadaniu polega na wypisaniu na standardowym wyjściu znaków wysłanych wcześniej do wydruku w ten sposób, że każdy następny znak wpisywany jest co jedną sekundę. Jeżeli wszystkie drukarki są zajęte to zlecenia wydruku są kolejkowane w opisywanym systemie wydruku. Jeżeli kolejka jest pełna to użytkownik chcący zlecić zadanie wydruku czeka do momentu gdy będzie można zlecenie wpisać do kolejki.

Każdy z N użytkowników powinien przesyłać do systemu wydruku zadanie wydruku 10 losowych znaków (od 'a' do 'z') a następnie odczekać losową liczbe sekund. Zadania zlecenia wydruku i odczekania powinny być wykonane w nieskończonej pętli. 

Należy zsynchronizować prace użytkowników oraz drukarek. Należy użyć mechanizmów System V lub POSIX. 