Imię: Maciej
Nazwisko: Kisielewski
Tytuł projektu: Arkanoid

Projekt Arkanoid był wykonywany systematycznie na cotygodniowych zajęciach. Jest to projekt z użyciem biblioteki SFML w programie C++.

Funkcjonalność gry:
  Animacja gry w oknie SFML następujących obiektów:
  - piłkę odbijającą się od ścian, klocków i paletki;
  - paletkę sterowaną klawiaturą;
  - klocki z punktami życia;
  - dodatkowy obiekt nieregularny;
  - dynamiczny tekst — licznik odbić piłki od paletki.

Sterowanie:
  - A / ←     ruch paletki w lewo
  - D / →     ruch paletki w prawo
  - ENTER     wybór opcji w menu
  - ESC       powrót do menu
  - F5        zapis stanu gry

Zasady działania gry:
  - Gracz uruchamia aplikację i widzi menu:
    Nowa gra / Wyczytaj grę / Wyjście
  - Po wybraniu Nowa gra rozpoczyna się rozgrywka;
  - Gracz odbija piłkę, niszczy klocki;
  - Gra umożliwia zapis gry i powrót do tego samoego ustawienia;
  - Gdy piłka spadnie — gra kończy partię wyświetlając komunikat;
  - Kolcki mają określoną ilość żyć:
    Czerwony - 1 życie
    Żółty - 2 życia
    Niebieski - 3 życia
