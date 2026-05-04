#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include <ctime>

#include "Produs.h"
#include "ProdusAlimentar.h"
#include "ProdusElectronic.h"
#include "ProdusMobilier.h"
#include "Utilizator.h"
#include "Cos.h"
#include "Exceptii.h"

//functie ce converteste date din formatul YYYY-MM-DD in time point
std::chrono::system_clock::time_point stringToDate(const std::string& dataStr) {
    int an, luna, zi;
    char dash1, dash2;
    std::istringstream ss(dataStr);
    ss >> an >> dash1 >> luna >> dash2 >> zi;
    if (ss.fail() || dash1 != '-' || dash2 != '-') {
        throw std::invalid_argument("Data invalida: " + dataStr);
    }
    std::tm tm = {};
    tm.tm_year = an - 1900;
    tm.tm_mon = luna - 1;
    tm.tm_mday = zi;
    tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0;
    std::time_t time = std::mktime(&tm);
    if (time == -1) {
        throw std::invalid_argument("Data invalida (mktime): " + dataStr);
    }
    return std::chrono::system_clock::from_time_t(time);
}

//functie ce elimina spatiile goale pentru parsare
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

int main() {
    std::cout << "=== BINE ATI VENIT LA MAGAZINUL ONLINE ===\n\n";

    std::ifstream fin("date_intrare.txt");

    std::vector<std::unique_ptr<Produs>> catalog;
    Utilizator user("", "", 0.0); // temporar
    bool userCitit = false;

    std::string linie;
    while (std::getline(fin, linie)) {
        linie = trim(linie);
        if (linie.empty()) continue;

        std::istringstream iss(linie);
        std::string tip;
        std::getline(iss, tip, ':');
        tip = trim(tip);
        if (tip.empty()) continue;

        std::string rest;
        std::getline(iss, rest);
        rest = trim(rest);
        if (rest.empty()) continue;

        if (tip == "USER") {
            // Format: nume, adresa, sold
            std::string nume, adresa;
            double sold;
            size_t pos1 = rest.find(',');
            if (pos1 == std::string::npos) continue;
            nume = trim(rest.substr(0, pos1));
            size_t pos2 = rest.find(',', pos1 + 1);
            if (pos2 == std::string::npos) continue;
            adresa = trim(rest.substr(pos1 + 1, pos2 - pos1 - 1));
            sold = std::stod(trim(rest.substr(pos2 + 1)));
            user = Utilizator(nume, adresa, sold);
            userCitit = true;
            std::cout << "Utilizator citit: " << user << "\n";
        }
        else if (tip == "PRODUS_ALIMENTAR") {
            // Format: nume, pret, stoc, data_expirare
            std::string nume, dataExp;
            double pret;
            int stoc;
            size_t pos1 = rest.find(',');
            if (pos1 == std::string::npos) continue;
            nume = trim(rest.substr(0, pos1));
            size_t pos2 = rest.find(',', pos1 + 1);
            if (pos2 == std::string::npos) continue;
            pret = std::stod(trim(rest.substr(pos1 + 1, pos2 - pos1 - 1)));
            size_t pos3 = rest.find(',', pos2 + 1);
            if (pos3 == std::string::npos) continue;
            stoc = std::stoi(trim(rest.substr(pos2 + 1, pos3 - pos2 - 1)));
            dataExp = trim(rest.substr(pos3 + 1));
            auto tp = stringToDate(dataExp);
            catalog.push_back(std::make_unique<ProdusAlimentar>(nume, pret, stoc, tp));
        }
        else if (tip == "PRODUS_ELECTRONIC") {
            // Format: nume, pret, stoc, garantieLuni, putereWati
            std::string nume;
            double pret, putere;
            int stoc, garantie;
            size_t pos1 = rest.find(',');
            if (pos1 == std::string::npos) continue;
            nume = trim(rest.substr(0, pos1));
            size_t pos2 = rest.find(',', pos1 + 1);
            if (pos2 == std::string::npos) continue;
            pret = std::stod(trim(rest.substr(pos1 + 1, pos2 - pos1 - 1)));
            size_t pos3 = rest.find(',', pos2 + 1);
            if (pos3 == std::string::npos) continue;
            stoc = std::stoi(trim(rest.substr(pos2 + 1, pos3 - pos2 - 1)));
            size_t pos4 = rest.find(',', pos3 + 1);
            if (pos4 == std::string::npos) continue;
            garantie = std::stoi(trim(rest.substr(pos3 + 1, pos4 - pos3 - 1)));
            putere = std::stod(trim(rest.substr(pos4 + 1)));
            catalog.push_back(std::make_unique<ProdusElectronic>(nume, pret, stoc, garantie, putere));
        }
        else if (tip == "PRODUS_MOBILIER") {
            // Format: nume, pret, stoc, material, greutateKg
            std::string nume, material;
            double pret, greutate;
            int stoc;
            size_t pos1 = rest.find(',');
            if (pos1 == std::string::npos) continue;
            nume = trim(rest.substr(0, pos1));
            size_t pos2 = rest.find(',', pos1 + 1);
            if (pos2 == std::string::npos) continue;
            pret = std::stod(trim(rest.substr(pos1 + 1, pos2 - pos1 - 1)));
            size_t pos3 = rest.find(',', pos2 + 1);
            if (pos3 == std::string::npos) continue;
            stoc = std::stoi(trim(rest.substr(pos2 + 1, pos3 - pos2 - 1)));
            size_t pos4 = rest.find(',', pos3 + 1);
            if (pos4 == std::string::npos) continue;
            material = trim(rest.substr(pos3 + 1, pos4 - pos3 - 1));
            greutate = std::stod(trim(rest.substr(pos4 + 1)));
            catalog.push_back(std::make_unique<ProdusMobilier>(nume, pret, stoc, material, greutate));
        }
    }
    fin.close();

    if (!userCitit || catalog.empty()) {
        std::cerr << "Eroare: Nu s-au citit datele corect (utilizator sau catalog gol).\n";
        return 1;
    }

    std::cout << "\n=== CATALOG PRODUSE ===\n";
    for (size_t i = 0; i < catalog.size(); ++i) {
        std::cout << i + 1 << ". " << *catalog[i] << '\n';
    }
    std::cout << "=======================\n\n";

    std::cout << "\n=== PRETURI FINALE (dupa reduceri/taxe) ===\n";
    for (const auto & i : catalog) {
        std::cout << i->getNume() << " : "
                  << i->calculeazaPretFinal() << " lei\n";
    }
    std::cout << "=======================\n\n";


    Cos cos(user);
    try {
        cos.adaugaProdus(*catalog[0], 2);   // Mere
        cos.adaugaProdus(*catalog[1], 1);   // Lapte (va avea reducere 50%)
        cos.adaugaProdus(*catalog[2], 1);   // Mouse (reducere 10%)
        cos.adaugaProdus(*catalog[3], 1);   // Tastatura (fara reducere)
        cos.adaugaProdus(*catalog[4], 1);   // Birou (taxa transport + 67.5)
        cos.adaugaProdus(*catalog[5], 1);   // Scaun (sub 30 de kg, fara reducere)
        cos.adaugaProdus(*catalog[6], 1);   // Paine (expirata, pret 0)
    } catch (const std::exception& e) {
        std::cerr << "Eroare la adaugare: " << e.what() << '\n';
    }
    std::cout << "Cos dupa adaugare:\n" << cos;

    std::cout << "\n=== VERIFICARE EXPIRARE PRODUSE ALIMENTARE ===\n";
    for (const auto& prod : catalog) {
        if (auto* alimentar = dynamic_cast<ProdusAlimentar*>(prod.get())) {
            if (alimentar->esteExpirat())
                std::cout << prod->getNume() << " este EXPIRAT!\n";
            else
                std::cout << prod->getNume() << " este proaspat.\n";
        }
    }
    std::cout << "=============================================\n";

    std::cout << "\nEliminam produsul expirat (Paine) din cos...\n";
    try {
        cos.eliminaProdus(6);   // index 6 (al 7-lea produs adăugat)
    } catch (const std::out_of_range& e) {
        std::cerr << "Eroare: " << e.what() << '\n';
    }
    std::cout << cos << '\n';

    std::cout << "=== FINALIZARE COMADA ===\n";
    try {
        cos.finalizeazaComanda(catalog);
        std::cout << "Comanda finalizata cu succes!\n";
        std::cout << "Sold ramas: " << user.getSold() << '\n';
    } catch (const SoldInsuficientException& e) {
        std::cerr << "Eroare sold: " << e.what() << '\n';
    } catch (const StocInsuficientException& e) {
        std::cerr << "Eroare stoc: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Eroare: " << e.what() << '\n';
    }

    std::cout << "\n=== CATALOG FINAL ===\n";
    for (const auto& prod : catalog) {
        std::cout << *prod << '\n';
    }

    std::cout << "\n=== PROGRAM TERMINAT ===\n";
    return 0;
}