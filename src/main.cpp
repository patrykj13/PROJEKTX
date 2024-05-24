#include <pybind11/pybind11.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <algorithm>
#include <matplot/matplot.h>
#include <audiofile.h>

using namespace std;
using namespace matplot;

const double fs = 1024.0;
const double pi_number = 3.14159265358979323846;


void wizualizacja_sygnalu();

void generowanie_sygnalu(int wybor, double Czas, double Czestotliwosc, double Amplituda);

void DFT_iDFT(int wybor, double czas_trwania, double Czestotliwosc, double Amplituda);

void DFT_iDFT_audio();

void usuwanie_czestotliwosci(double czestotliwosc_odciecia);


//--   1   --//

void wizualizacja_sygnalu() {
    AudioFile<double> plikAudio;

    plikAudio.load("test-audio.wav");

    const auto& probki = plikAudio.samples[0];
    int czestotliwoscProbkowania = plikAudio.getSampleRate();
    int liczbaProbek = probki.size();

    // Redukcja liczby probek
    int wspolczynnikRedukcji = 10; // co dziesiata probke aby wykres generowal sie szybciej w matplocie
    vector<double> zredukowaneProbki;
    zredukowaneProbki.reserve(liczbaProbek / wspolczynnikRedukcji); //zmiana pojemnosci wektora

    for (int i = 0; i < liczbaProbek; i += wspolczynnikRedukcji) {
        zredukowaneProbki.push_back(probki[i]);
    }

    vector<int> zredukowaneIndeksy(zredukowaneProbki.size());
    iota(zredukowaneIndeksy.begin(), zredukowaneIndeksy.end(), 0); // ustawienie wartosci od 0 dla lepszej wizualizacji

    figure();
    plot(zredukowaneIndeksy, zredukowaneProbki);
    title("Sygnal dzwiekowy");
    xlabel("Czas [probki]");
    ylabel("Amplituda");
    show();
}

//----------//


//--   2   --//

void generowanie_sygnalu(int wybor, double Czas, double Czestotliwosc, double Amplituda) {

    //Wybierz sygnal (1 - sinus, 2 - cosinus, 3 - prostokatny, 4 - piloksztaltny)


    vector<double> probki_czasowe = linspace(0, Czas, static_cast<size_t>(Czas * fs));
    vector<double> y(probki_czasowe.size());

    if (wybor == 1) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            y[i] = Amplituda * sin(2 * pi * Czestotliwosc * probki_czasowe[i]);
        }

        plot(probki_czasowe, y);
        title("Sygnal sinusoidalny");
        xlabel("Czas [s]");
        ylabel("Amplituda");
        show();
    }
    else if (wybor == 2) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            y[i] = Amplituda * cos(2 * pi * Czestotliwosc * probki_czasowe[i]);
        }

        plot(probki_czasowe, y);
        title("Sygnal cosinus");
        xlabel("Czas [s]");
        ylabel("Amplituda");
        show();
    }
    else if (wybor == 3) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            if (sin(2 * pi * Czestotliwosc * probki_czasowe[i]) >= 0) {
                y[i] = Amplituda * 1;
            }
            else {
                y[i] = Amplituda * -1;
            }
        }

        plot(probki_czasowe, y);
        title("Sygnal prostokatny");
        xlabel("Czas [s]");
        ylabel("Amplituda");
        show();
    }
    else if (wybor == 4) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            y[i] = Amplituda * (2.0 * (probki_czasowe[i] * Czestotliwosc - floor(probki_czasowe[i] * Czestotliwosc + 0.5))); //zaokraglamy w dol
        }

        plot(probki_czasowe, y);
        title("Sygnal piloksztaltny");
        xlabel("Czas [s]");
        ylabel("Amplituda");
        show();
    }
    else if (wybor == 5) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            double znorm_wart_czas = fmod(i / fs, 1.0 / Czestotliwosc); // Normalizacja czasu - obliczanie reszty
            double okres_sygnalu = 1.0 / Czestotliwosc;
            if (znorm_wart_czas < okres_sygnalu / 2)
                y[i] = (4 * Amplituda / okres_sygnalu) * znorm_wart_czas; //nadanie ksztaltu fali
            else
                y[i] = (-4 * Amplituda / okres_sygnalu) * znorm_wart_czas + (4 * Amplituda); //nadanie ksztaltu fali
        }

        plot(probki_czasowe, y);
        title("Sygnal trojkatny");
        xlabel("Czas [s]");
        ylabel("Amplituda");
        show();
    }
    else {
        cout << "BlaD: Nieprawidlowy wybor sygnalu!" << endl;
    }
}

//----------//


//--  3 --//

void DFT_iDFT(int wybor, double czas_trwania, double Czestotliwosc, double Amplituda) {

    // wektor liniowo rozlożonych wartosci
    auto rozkladanie_wartosci_czasu = [](double start, double end, size_t num) {
        vector<double> wartosci_czasu;
        double roznica = (end - start) / static_cast<double>(num - 1);
        for (size_t i = 0; i < num; ++i) {
            wartosci_czasu.push_back(start + roznica * i);
        }
        return wartosci_czasu;
        };

    // dyskretna transformata Fouriera (DFT)
    auto DFT = [](const vector<double>& sygnal_do_dft) {
        size_t liczba_probek = sygnal_do_dft.size();
        vector<complex<double>> wynik_DFT(liczba_probek);

        for (size_t k = 0; k < liczba_probek; ++k) { // Iteracja po numerach harmonicznej
            complex<double> suma_probek_sygnalu = 0;
            for (size_t n = 0; n < liczba_probek; ++n) { // Iteracja po probkach sygnalu
                suma_probek_sygnalu += sygnal_do_dft[n] * exp(complex<double>(0, -2 * pi_number * k * n / liczba_probek));
            }
            wynik_DFT[k] = suma_probek_sygnalu;
        }

        return wynik_DFT;
        };

    // odwrotna dyskretna transformata Fouriera (IDFT)
    auto IDFT = [](const vector<complex<double>>& sygnal_do_idft) {
        size_t liczba_probek = sygnal_do_idft.size();
        vector<double> wynik_IDFT(liczba_probek, 0.0);

        for (size_t n = 0; n < liczba_probek; ++n) { // Iteracja po probkach sygnalu
            complex<double> suma_probek_sygnalu = 0;
            for (size_t k = 0; k < liczba_probek; ++k) { // Iteracja po numerach harmonicznej
                double kat = 2 * pi_number * k * n / liczba_probek;
                suma_probek_sygnalu += sygnal_do_idft[k] * exp(complex<double>(0, kat));
            }
            wynik_IDFT[n] = suma_probek_sygnalu.real() / liczba_probek; // dzielenie ze wzoru
        }

        return wynik_IDFT;
        };

    vector<double> probki_czasowe = rozkladanie_wartosci_czasu(0, czas_trwania, static_cast<size_t>(czas_trwania * fs));
    vector<double> sygnal_transformata(probki_czasowe.size());

    if (wybor == 1) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            sygnal_transformata[i] = Amplituda * sin(2 * pi_number * Czestotliwosc * probki_czasowe[i]);
        }
    }
    else if (wybor == 2) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            sygnal_transformata[i] = Amplituda * cos(2 * pi_number * Czestotliwosc * probki_czasowe[i]);
        }
    }
    else if (wybor == 3) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            if (sin(2 * pi_number * Czestotliwosc * probki_czasowe[i]) >= 0) {
                sygnal_transformata[i] = Amplituda * 1;
            }
            else {
                sygnal_transformata[i] = Amplituda * -1;
            }
        }
    }
    else if (wybor == 4) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            sygnal_transformata[i] = Amplituda * (2.0 * (probki_czasowe[i] * Czestotliwosc - floor(probki_czasowe[i] * Czestotliwosc + 0.5)));
        }
    }
    else if (wybor == 5) {
        for (size_t i = 0; i < probki_czasowe.size(); ++i) {
            double znorm_wart_czas = fmod(i / fs, 1.0 / Czestotliwosc);
            double okres_sygnalu = 1.0 / Czestotliwosc;
            if (znorm_wart_czas < okres_sygnalu / 2)
                sygnal_transformata[i] = (4 * Amplituda / okres_sygnalu) * znorm_wart_czas; //nadanie ksztaltu fali
            else
                sygnal_transformata[i] = (-4 * Amplituda / okres_sygnalu) * znorm_wart_czas + (4 * Amplituda); //nadanie ksztaltu fali
        }
    }
    else {
        cout << "BlaD: Nieprawidlowy wybor sygnalu!" << endl;
        return;
    }

    // Obliczenie DFT
    vector<complex<double>> dft_licz = DFT(sygnal_transformata);

    // Obliczenie IDFT
    vector<double> IDFT_WYKRES = IDFT(dft_licz);

    // Wykresy BUDOWANIE
    vector<double> DFT_WYKRES(dft_licz.size());
    for (size_t i = 0; i < dft_licz.size(); ++i) {
        DFT_WYKRES[i] = abs(dft_licz[i]);
    }

    figure();
    subplot(3, 1, 1);
    title("DFT");
    plot(DFT_WYKRES);

    figure();
    subplot(3, 1, 1);
    title("IDFT");
    plot(probki_czasowe, IDFT_WYKRES);

    show();
}

void DFT_iDFT_audio() {
    // dyskretna transformata Fouriera (DFT)
    auto DFT = [](const vector<double>& sygnal_do_dft) -> vector<complex<double>> {
        size_t liczba_probek = sygnal_do_dft.size();
        vector<complex<double>> wynik_DFT(liczba_probek);

        for (size_t k = 0; k < liczba_probek; ++k) { // Iteracja po numerach harmonicznej
            complex<double> suma_probek_sygnalu = 0;
            for (size_t n = 0; n < liczba_probek; ++n) { // Iteracja po probkach sygnalu
                suma_probek_sygnalu += sygnal_do_dft[n] * exp(complex<double>(0, -2 * pi_number * k * n / liczba_probek));
            }
            wynik_DFT[k] = suma_probek_sygnalu;
        }

        return wynik_DFT;
        };

    // Funkcja realizujaca odwrotna dyskretna transformate Fouriera (IDFT)
    auto IDFT = [](const vector<complex<double>>& sygnal_do_idft) -> vector<double> {
        size_t liczba_probek = sygnal_do_idft.size();
        vector<double> wynik_IDFT(liczba_probek, 0.0);

        for (size_t n = 0; n < liczba_probek; ++n) {
            complex<double> suma_probek_sygnalu = 0;
            for (size_t k = 0; k < liczba_probek; ++k) {
                double kat = 2 * pi_number * k * n / liczba_probek;
                suma_probek_sygnalu += sygnal_do_idft[k] * exp(complex<double>(0, kat));
            }
            wynik_IDFT[n] = suma_probek_sygnalu.real() / liczba_probek; // dzielenie ze wzoru wiki
        }

        return wynik_IDFT;
        };

    AudioFile<double> plik_audio;
    if (!plik_audio.load("test-audio.wav")) {
        cerr << "Blad wczytywania pliku audio.\n";
        return;
    }

    // Pobranie probki audio test-audio.wav
    vector<double> sygnal_transformata;
    for (size_t i = 0; i < plik_audio.samples[0].size(); i += 10) { // Zwieksz odstepy
        sygnal_transformata.push_back(plik_audio.samples[0][i]);
    }

    // Obliczenie DFT
    vector<complex<double>> dft_licz = DFT(sygnal_transformata);

    // Obliczenie IDFT
    vector<double> IDFT_WYKRES = IDFT(dft_licz);

    // Wykresy - budowanie w innych oknach
    vector<double> DFT_WYKRES(dft_licz.size());
    for (size_t i = 0; i < dft_licz.size(); ++i) {
        DFT_WYKRES[i] = abs(dft_licz[i]);
    }

    figure();
    plot(DFT_WYKRES);
    title("Modul DFT");

    figure();
    plot(IDFT_WYKRES);
    title("Odtworzony sygnal za pomoca IDFT");

    show();
}

//-----------//


//--USUWANIE WYS. CZEST.//

void usuwanie_czestotliwosci(double czestotliwosc_odciecia) {
    AudioFile<double> plik_audio;
    if (!plik_audio.load("test-audio.wav")) {
        cerr << "Blad wczytywania pliku audio.\n";
        return;
    }

    // Pobranie dzwieku audio
    vector<double> probki_dzwieku;
    for (size_t i = 0; i < plik_audio.samples[0].size(); i += 1) { // !!! można zwiekszyc odstepy w celu przyspieszenia budowania ale zaleca sie nie robic tego, gdyż w przypadku wiekszej ilosci pobierania probek na s od probkowania sygnalu, moga wystapic ogromne bledy
        probki_dzwieku.push_back(plik_audio.samples[0][i]);
    }

    // dyskretna transformata Fouriera (DFT)
    auto DFT = [](const vector<double>& sygnal_do_dft) {
        size_t liczba_probek = sygnal_do_dft.size();
        vector<complex<double>> wynik_DFT(liczba_probek);

        for (size_t k = 0; k < liczba_probek; ++k) { // Iteracja po numerach harmonicznej
            complex<double> suma_probek_sygnalu = 0;
            for (size_t n = 0; n < liczba_probek; ++n) { // Iteracja po probkach sygnalu
                suma_probek_sygnalu += sygnal_do_dft[n] * exp(complex<double>(0, -2 * pi_number * k * n / liczba_probek));
            }
            wynik_DFT[k] = suma_probek_sygnalu;
        }

        return wynik_DFT;
        };

    // odwrotna dyskretna transformata Fouriera (IDFT)
    auto IDFT = [](const vector<complex<double>>& sygnal_do_idft) {
        size_t liczba_probek = sygnal_do_idft.size();
        vector<double> wynik_IDFT(liczba_probek, 0.0);

        for (size_t n = 0; n < liczba_probek; ++n) {
            complex<double> suma_probek_sygnalu = 0;
            for (size_t k = 0; k < liczba_probek; ++k) {
                double kat = 2 * pi_number * k * n / liczba_probek;
                suma_probek_sygnalu += sygnal_do_idft[k] * exp(complex<double>(0, kat));
            }
            wynik_IDFT[n] = suma_probek_sygnalu.real() / liczba_probek; // Normalizacja
        }

        return wynik_IDFT;
        };

    // Funkcja do filtrowania czestotliwosciowego
    auto filtrowanie_czestotliwosci = [](vector<complex<double>>& dft_licz, double czestotliwosc_odciecia) {
        size_t liczba_probek = dft_licz.size();
        size_t indeks_odciecie = static_cast<size_t>(czestotliwosc_odciecia * liczba_probek / fs);

        // Usuwanie czestotliwosci powyżej wartosci odciecia
        for (size_t i = indeks_odciecie; i < liczba_probek; ++i) {
            dft_licz[i] = 0.0;
        }
        };

    // Obliczenie DFT
    vector<complex<double>> dft_licz = DFT(probki_dzwieku);

    // Filtrowanie czestotliwosci
    filtrowanie_czestotliwosci(dft_licz, czestotliwosc_odciecia);

    // Obliczenie IDFT
    vector<double> IDFT_WYKRES = IDFT(dft_licz);

    // Wykres sygnalu audio
    figure();
    plot(plik_audio.samples[0]);
    title("Oryginalny sygnal audio");

    // Wykres sygnalu po filtracji
    figure();
    plot(IDFT_WYKRES);
    title("Sygnal po filtracji ");
    show();
}

//----------------------//
PYBIND11_MODULE(Signal, m) {
    m.doc() = "Signal processing library";

    m.def("wizualizacja_sygnalu", &wizualizacja_sygnalu, "Function to visualize the signal");

    m.def("generowanie_sygnalu", &generowanie_sygnalu, "Function to generate a signal");

    m.def("DFT_iDFT", &DFT_iDFT, "Function to compute and visualize DFT and IDFT");

    m.def("DFT_iDFT_audio", &DFT_iDFT_audio, "Function to compute DFT and IDFT for audio signal");

    m.def("usuwanie_czestotliwosci", &usuwanie_czestotliwosci, "Function to remove high frequencies from audio signal");
}