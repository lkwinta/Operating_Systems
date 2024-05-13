# Problem Świętego Mikołaja

Święty Mikołaj śpi w swoim warsztacie na biegunie północnym i może być obudzony tylko sytuacji gdy wszystkie 9 reniferów wróciło z wakacji.

Należy zaimplementować program, w którym Mikołaj oraz renifery to osobne wątki.
Zachowania reniferów:

- Są na wakacjach w ciepłych krajach losowy okres czasu (5-10s)
- Wracaja na biegun północny (Komunikat: Renifer: czeka _ reniferów na Mikołaja, ID), jeśli wracający renifer jest dziewiątym reniferem to wybudza Mikołaja (Komunikat: Renifer: wybudzam Mikołaja, ID).
- Dostarczają wraz z Mikołajem zabawki grzecznym dzieciom (i studentom którzy nie spóźniają się z dostarczaniem zestawów) przez (2-4s).   
- Lecą na wakacje.

Zachowania Mikołaja:

- Śpi.
- Kiedy zostaje wybudzony (Komunikat: Mikołaj: budzę się) to dostarcza wraz z reniferami zabawki (Komunikat: Mikołaj: dostarczam zabawki) (2-4s).
- Wraca do snu (Komunikat: Mikołaj: zasypiam).

Program należy zaimplementować korzystając z wątków i mechanizmów synchronizacji biblioteki POSIX Threads. Po uruchomieniu programu wątek główny tworzy wątki dla Mikołaja oraz reniferów. Możemy założyć że Mikołaj dostarczy 4 razy prezenty, po czym kończy działanie wszystkich wątków. Do spania Mikołaja powinny być wykorzystane Warunki Sprawdzające (Condition Variables). Użycie odpowiednich mechanizmów ma zagwarantować niedopouszczenie, np. do zdarzeń: Mikołaj śpi chociaż czeka na niego 9 reniferów.  