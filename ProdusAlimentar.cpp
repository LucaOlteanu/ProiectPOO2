#include "ProdusAlimentar.h"
#include <chrono>
#include <iomanip>
#include <ctime>

ProdusAlimentar::ProdusAlimentar()
    : Produs(), dataExpirare(std::chrono::system_clock::now() + std::chrono::hours(24 * 30)) {}

ProdusAlimentar::ProdusAlimentar(const std::string& nume, double pret, int stoc,
                                 const std::chrono::system_clock::time_point& dataExp)
    : Produs(nume, pret, stoc), dataExpirare(dataExp) {}

ProdusAlimentar::ProdusAlimentar(const ProdusAlimentar& other)
    = default;

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

ProdusAlimentar::~ProdusAlimentar() = default;

double ProdusAlimentar::calculeazaPretFinal() const {
    double pret = getPretBaza();
    if (esteExpirat())
        return 0.0;
    auto now = std::chrono::system_clock::now(); //timpul acum
    auto diff = std::chrono::duration_cast<std::chrono::hours>(dataExpirare - now).count(); // diferenta intre timpul curent si data expirarii in ore ca sa putem compara cu 24 mai usor
    if (diff < 24)//daca maia are o zi de valabilitate, produsul este la 50% reducere
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
       << ", Expira la: ";
    std::time_t t = std::chrono::system_clock::to_time_t(dataExpirare);
    os << std::put_time(std::localtime(&t), "%Y-%m-%d");
}