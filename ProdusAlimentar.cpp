#include "ProdusAlimentar.h"
#include <iomanip>
#include <stdexcept>

ProdusAlimentar::ProdusAlimentar()
    : Produs(), dataExpirare(std::chrono::system_clock::now() + std::chrono::hours(24 * 30)) {}

ProdusAlimentar::ProdusAlimentar(const std::string& nume, double pret, int stoc,
                                 const std::chrono::system_clock::time_point& dataExp)
    : Produs(nume, pret, stoc), dataExpirare(dataExp) {}

ProdusAlimentar::ProdusAlimentar(const ProdusAlimentar& other)
    : Produs(other), dataExpirare(other.dataExpirare) {}

ProdusAlimentar::ProdusAlimentar(ProdusAlimentar&& other) noexcept
    : Produs(std::move(other)), dataExpirare(other.dataExpirare) {}

ProdusAlimentar& ProdusAlimentar::operator=(const ProdusAlimentar& other) {
    if (this != &other) {
        Produs::operator=(other);
        dataExpirare = other.dataExpirare;
    }
    return *this;
}

ProdusAlimentar& ProdusAlimentar::operator=(ProdusAlimentar&& other) noexcept {
    if (this != &other) {
        Produs::operator=(std::move(other));
        dataExpirare = other.dataExpirare;
    }
    return *this;
}

ProdusAlimentar::~ProdusAlimentar() {}

double ProdusAlimentar::calculeazaPretFinal() const {
    double pret = getPretBaza();
    if (esteExpirat())
        return 0.0;
    auto now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::hours>(dataExpirare - now).count();
    if (diff < 24)
        pret *= 0.5;
    return pret;
}


ProdusAlimentar* ProdusAlimentar::clone() const {
    return new ProdusAlimentar(*this);
}

bool ProdusAlimentar::esteExpirat() const {
    return std::chrono::system_clock::now() > dataExpirare;
}

void ProdusAlimentar::afisare(std::ostream& os) const {
    os << "Produs alimentar: " << getNume()
       << ", ID: " << getId()
       << ", Pret baza: " << getPretBaza()
       << ", Stoc: " << getStoc()
       << ", Expira la: " << std::chrono::system_clock::to_time_t(dataExpirare);
}