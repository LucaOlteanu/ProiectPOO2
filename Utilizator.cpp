#include "Utilizator.h"
#include <stdexcept>
#include <string>
#include <iostream>

int Utilizator::nextId = 1;

Utilizator::Utilizator()
    : id(nextId++), nume("Anonim"), adresa("Necunoscuta"), sold(0.0) {}

Utilizator::Utilizator(const std::string& nume, const std::string& adresa, double sold)
    : id(nextId++), nume(nume), adresa(adresa), sold(sold) {
    if (sold < 0)
        throw std::invalid_argument("Soldul nu poate fi negativ");
}

Utilizator::Utilizator(const Utilizator& other)
    : id(other.id), nume(other.nume), adresa(other.adresa), sold(other.sold) {}

Utilizator::Utilizator(Utilizator&& other) noexcept
    : id(other.id), nume(std::move(other.nume)), adresa(std::move(other.adresa)), sold(other.sold) {
    other.sold = 0.0;
}

Utilizator& Utilizator::operator=(const Utilizator& other) {
    if (this != &other) {
        nume = other.nume;
        adresa = other.adresa;
        sold = other.sold;
    }
    return *this;
}

Utilizator& Utilizator::operator=(Utilizator&& other) noexcept {
    if (this != &other) {
        nume = std::move(other.nume);
        adresa = std::move(other.adresa);
        sold = other.sold;
        other.sold = 0.0;
    }
    return *this;
}

Utilizator::~Utilizator() = default;

int Utilizator::getId() const { return id; }
const std::string& Utilizator::getNume() const { return nume; }
const std::string& Utilizator::getAdresa() const { return adresa; }
double Utilizator::getSold() const { return sold; }

void Utilizator::setSold(double soldNou) {
    if (soldNou < 0)
        throw std::invalid_argument("Soldul nu poate fi negativ");
    sold = soldNou;
}

void Utilizator::adaugaFonduri(double suma) {
    if (suma < 0)
        throw std::invalid_argument("Suma adăugata nu poate fi negativa");
    sold += suma;
}

void Utilizator::extrageFonduri(double suma) {
    if (suma < 0)
        throw std::invalid_argument("Suma extrasa nu poate fi negativa");
    if (sold < suma)
        throw std::runtime_error("Fonduri insuficiente");
    sold -= suma;
}

int Utilizator::getNextId() { return nextId; }

std::ostream& operator<<(std::ostream& os, const Utilizator& u) {
    os << "Utilizator ID: " << u.getId()
       << ", Nume: " << u.getNume()
       << ", Adresa: " << u.getAdresa()
       << ", Sold: " << u.getSold() << " lei";
    return os;
}