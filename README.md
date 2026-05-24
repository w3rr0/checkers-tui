# Checkers TUI

## Uruchomienie programu i instrukcja obsługi

1. Kompilacja programu odbywa się za pomocą narzędzia Make. W głównym katalogu wykonaj komendę:
   ```bash
   make
   ```
2. Pierwszy gracz (białe) musi wystartować i utworzyć nową grę (zostaną utworzone semafory oraz pamięć współdzielona).
   ```bash
   ./program white
   ```
3. Drugi gracz (czarne) dołącza z poziomu osobnego terminala do już działającej gry.
   ```bash
   ./program black
   ```
4. Wykonywanie ruchów odbywa się poprzez wpisanie współrzędnych pola z którego pionek startuje, a następnie po spacji, pola docelowego, np.:
   ```
   B3 C4
   ```

## Mechanizmy komunikacji międzyprocesowej i synchronizacji

Program składa się z dwóch niezależnych procesów wymieniających informacje przez pamięć współdzieloną POSIX (tworzoną za pomocą funkcji `mmap` i `shm_open`). Najistotniejsza jest tutaj implementacja synchronizacji i obsługi nagłych zdarzeń.

### Wykorzystanie semaforów POSIX

Do kontroli tur obu graczy program wykorzystuje dwa nazwane semafory:
- `/checkers_sem_white` – przypisany do białych, domyślnie zainicjowany wartością 1.
- `/checkers_sem_black` – przypisany do czarnych, domyślnie zainicjowany wartością 0.

Mechanizm działania:
* Gracz oczekuje na swoją turę, wywołując instrukcję `sem_wait(my_sem)`, która wstrzymuje wykonanie procesu do momentu, w którym odpowiedni semafor podniesie swoją wartość powyżej zera. 
* Ponieważ semafor białych zaczyna z wartością 1, proces białych wykonuje się od razu, podczas gdy czarne oczekują na `sem_wait`.
* Po zweryfikowaniu poprawnego ruchu na planszy i wyświetleniu zmian na ekranie, tura się kończy, a program wywołuje funkcję `sem_post(opp_sem)`. Podbija to wartość semafora przeciwnika, odblokowując jego proces.

### Wykorzystanie sygnałów (Signals)

Program w sposób celowy modyfikuje standardową obsługę sygnałów systemowych w celu uniknięcia zawieszeń, zabezpieczenia przed zakleszczeniami i egzekwowania reguł gry (np. czasu na ruch).

1. **Przechwytywanie przerwań (SIGINT / SIGTERM)**: 
   Standardowe zachowanie podczas przerwania programu z klawiatury (np. `Ctrl+C`) natychmiastowo zabiłoby proces jednego gracza, zostawiając drugiego trwale oczekującego na swoim `sem_wait()` oraz nieczyszcząc zasobów systemowych IPC. Zamiast tego rejestrowana jest funkcja `handle_signal` za pomocą wywołania `sigaction`.
   Zadaniem funkcji jest ustawienie odpowiednich flag przerwania działania, wpisanie faktu rozłączenia gracza na planszę oraz najważniejsze – wykonanie `sem_post(global_opp_sem)`. Oznacza to, że proces wychodzącego gracza celowo odblokowuje drugiego gracza, pozwalając na eleganckie i bezpieczne wyjście z pętli zdarzeń.

2. **Czas na ruch i systemowy timer (SIGALRM)**:
   Program daje graczom limit 15 sekund na ruch. W tym celu korzysta z funkcji `alarm(15)`, która po upływie zdefiniowanego czasu wysyła do aplikacji sygnał `SIGALRM`. Program obsługuje go wykorzystując własną funkcję `handle_sigalrm`.
   Jest to bardzo skuteczne przy blokujących wywołaniach (takich jak `scanf()` przy odczytywaniu ruchu). Dostarczenie `SIGALRM` wyrywa wątek z blokującego odczytu, funkcja obsługi podnosi odpowiednie flagi na planszy (`timeout = true`), a następnie proces kończy grę i powiadamia drugiego gracza wywołaniem `sem_post()`.