# Prosty chat z wykorzystaniem TCP/IP

Napisz prosty chat typu klient-serwer w którym komunikacja pomiędzy uczestmnikami czatu / klientami / klientami i serwerem realizoiwana jest za pośrednictwem socketów z życiem protokołu strumieniowego.

Adres / port serwera podawany jest jako argument jego uruchomienia

Klient przyjmuje jako swoje argumenty:

- swoją nazwę/identyfikator (string o z góry ograniczonej długości)
- adres serwera (adres IPv4 i numer portu) 

Protokół komunikacyjny powinien obsługiwać następujące operacje:

- LIST:
    Pobranie z serwera i wylistowanie wszystkich aktywnych klientów
- 2ALL string:
    Wysłania wiadomości do wszystkich pozostałych klientów. Klient wysyła ciąg znaków do serwera, a serwer rozsyła ten ciąg wraz z identyfikatorem nadawcy oraz aktualną datą do wszystkich pozostałych klientów
- 2ONE id_klienta string:
    Wysłanie wiadomości do konkretnego klienta. Klient wysyła do serwera ciąg znaków podając jako adresata konkretnego klienta o identyfikatorze z listy aktywnych klientów. Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wskazanego klienta.
- STOP: Zgłoszenie zakończenia pracy klienta.  Powinno skutkować usunięciem klienta z listy klientów przechowywanej na serwerze 
- ALIVE - serwer powinien cyklicznie "pingować" zarejestrowanych klientów, aby zweryfikować że wciąż odpowiadają na żądania, a jeśli nie - usuwać ich z listy klientów.
- Klient przy wyłączeniu Ctrl+C powinien wyrejestrować się z serwera

Dla uproszczenia można przyjąć, że serwer przechowuje informacje o klientach w statycznej tablicy (rozmiar tablicy ogranicza liczbę klientów, którzy mogą jednocześnie byc uczestnikami czatu).