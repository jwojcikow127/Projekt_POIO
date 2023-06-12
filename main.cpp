#include <iostream>
#include <unordered_map>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

// Klasa reprezentująca pojazd na parkingu
class Pojazd {
private:
    string numerRejestracyjny;
    time_point<system_clock> czasWjazdu;
    double oplata;

public:
    Pojazd() = default;
    Pojazd(const string& nrRejestracyjny) : numerRejestracyjny(nrRejestracyjny) {
        czasWjazdu = system_clock::now();
    }

    const string& getNumerRejestracyjny() const {
        return numerRejestracyjny;
    }

    time_t getCzasPobytu() const {
        auto teraz = system_clock::now();
        return duration_cast<minutes>(teraz - czasWjazdu).count();
    }

    void ustawOplate(double oplata) {
        this->oplata = oplata;
    }


};

// Klasa reprezentująca parking
class Parking {
private:
    int liczbaMiejsc;
    unordered_map<string, Pojazd> mapaPojazdow;

public:
    Parking(int liczbaMiejsc) : liczbaMiejsc(liczbaMiejsc) {}

    bool czyWolneMiejsce() const {
        return mapaPojazdow.size() < liczbaMiejsc;
    }

    void dodajPojazd(const Pojazd& pojazd) {
        mapaPojazdow[pojazd.getNumerRejestracyjny()] = pojazd;
        cout << "Pojazd o numerze rejestracyjnym " << pojazd.getNumerRejestracyjny() << " wjechal na parking." << endl;
    }

    void usunPojazd(const string& numerRejestracyjny) {
        auto it = mapaPojazdow.find(numerRejestracyjny);
        if (it != mapaPojazdow.end()) {
            Pojazd& pojazd = it->second;
            time_t czasPobytu = pojazd.getCzasPobytu();
            double oplata = obliczOplate(czasPobytu);

            cout << "Pojazd o numerze rejestracyjnym " << numerRejestracyjny << " opuszcza parking." << endl;
            cout << "Czas pobytu: " << czasPobytu << " minut" << endl;
            cout << "Oplata do zaplacenia: " << oplata << " zl" << endl;

            // Sprawdzenie płatności
            while (true) {
                cout << "Czy oplata zostala dokonana? (T/N): ";
                char odpowiedz;
                cin >> odpowiedz;

                if (odpowiedz == 'T' || odpowiedz == 't') {
                    pojazd.ustawOplate(oplata);
                    cout << "Do widzenia !!" << endl;
                    break;
                } else if (odpowiedz == 'N' || odpowiedz == 'n') {
                    cout << "Nalezy dokonac platnosci przed wyjazdem z parkingu." << endl;

                    return;
                } else {
                    cout << "Nieprawidlowa odpowiedz. Prosze podac T (Tak) lub N (Nie)." << endl;
                }
            }

            mapaPojazdow.erase(it);
        }
    }

    double obliczOplate(time_t czasPobytu) const {
        // Przykładowe obliczanie opłaty - 1 zł za minutę
        return czasPobytu * 1.0;
    }

    void informacjeOParkingu() const {
        cout << "Liczba wolnych miejsc: " << liczbaMiejsc - mapaPojazdow.size() << endl;
        cout << "Liczba zajetych miejsc: " << mapaPojazdow.size() << endl;
        cout << "Lista pojazdow na parkingu:" << endl;
        for (const auto& para : mapaPojazdow) {
            const Pojazd& pojazd = para.second;
            cout << "Numer rejestracyjny: " << pojazd.getNumerRejestracyjny() << ", czas pobytu: " << pojazd.getCzasPobytu() << " minut";
            cout << " Aktualna oplata za parkowanie: " << obliczOplate(pojazd.getCzasPobytu()) << endl;
        }
    }
};

int main() {
    system("cls");
    Parking parking(10);  // Tworzenie obiektu klasy Parking z 10 miejscami

    while (true) {
         
        cout << "MENU Parkingu" << endl;
        cout << "1. Wjazd na parking" << endl;
        cout << "2. Opuszczenie parkingu" << endl;
        cout << "3. Informacje o parkingu" << endl;
        cout << "0. Zakoncz program" << endl;

        int wybor;
        cin >> wybor;

        if (wybor == 1) {
            if (parking.czyWolneMiejsce()) {
                string numerRejestracyjny;
                cout << "Podaj numer rejestracyjny pojazdu: ";
                cin >> numerRejestracyjny;

                Pojazd pojazd(numerRejestracyjny);
                parking.dodajPojazd(pojazd);
               
            } else {
                cout << "Brak wolnych miejsc na parkingu." << endl;
            }
        } else if (wybor == 2) {
            string numerRejestracyjny;
            cout << "Podaj numer rejestracyjny pojazdu do opuszczenia parkingu: ";
            cin >> numerRejestracyjny;

            parking.usunPojazd(numerRejestracyjny);
        } else if (wybor == 3) {
            parking.informacjeOParkingu();
        } else if (wybor == 0) {
            break;
        } else {
            cout << "Nieprawidlowy wybor." << endl;
        }

        cout << endl;
        
    }

    return 0;
}