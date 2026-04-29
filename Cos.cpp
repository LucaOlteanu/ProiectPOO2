#include "Cos.h"
#include "ProdusAlimentar.h"
#include "Exceptii.h"
#include <algorithm>
#include <iostream>

Cos::Cos() : proprietar(), total(0.0) {}

Cos::Cos(const Utilizator& prop) : proprietar(prop), total(0.0) {}

Cos::Cos(const Cos& other)
    : proprietar(other.proprietar), total(other.total) {
    for (const auto& prod : other.produse) {
        produse.push_back(std::unique_ptr<Produs>(prod->clone()));
    }
}

Cos::Cos(Cos&& other) noexcept
    : proprietar(std::move(other.proprietar)),
      produse(std::move(other.produse)),
      total(other.total) {
    other.total = 0.0;
}

Cos& Cos::operator=(const Cos& other) {
    if (this != &other) {
        Cos temp(other);
        swap(temp);
    }
    return *this;
}

Cos& Cos::operator=(Cos&& other) noexcept {
    if (this != &other) {
        proprietar = std::move(other.proprietar);
        produse = std::move(other.produse);
        total = other.total;
        other.total = 0.0;
    }
    return *this;
}

Cos::~Cos() {}

void swap(Cos& first, Cos& second) noexcept {
    using std::swap;
    swap(first.proprietar, second.proprietar);
    swap(first.produse, second.produse);
    swap(first.total, second.total);
}

void Cos::adaugaProdus(const Produs& produs, int cantitate) {
    if (cantitate <= 0) {
        throw std::invalid_argument("Cantitatea trebuie să fie pozitivă");
    }
    for (int i = 0; i < cantitate; ++i) {
        produse.push_back(std::unique_ptr<Produs>(produs.clone()));
    }
    total += produs.calculeazaPretFinal() * cantitate;
}

void Cos::eliminaProdus(int index) {
    if (index < 0 || index >= static_cast<int>(produse.size())) {
        throw std::out_of_range("Index invalid");
    }
    total -= produse[index]->calculeazaPretFinal();
    produse.erase(produse.begin() + index);
}

double Cos::getTotal() const {
    return total;
}

void Cos::afisareCos(std::ostream& os) const {
    os << "=== COȘ DE CUMPĂRĂTURI ===\n";
    os << proprietar << "\n";
    if (produse.empty()) {
        os << "Coșul este gol.\n";
    } else {
        os << "Produse în coș:\n";
        for (size_t i = 0; i < produse.size(); ++i) {
            os << i + 1 << ". ";
            produse[i]->afisare(os);
            if (auto* alimentar = dynamic_cast<ProdusAlimentar*>(produse[i].get())) {
                if (alimentar->esteExpirat()) {
                    os << " (ATENȚIE: EXPIRAT!)";
                }
            }
            os << "\n";
        }
        os << "Total: " << total << " lei\n";
    }
    os << "========================\n";
}

void Cos::finalizeazaComanda(std::vector<std::unique_ptr<Produs>>& catalog) {
    if (produse.empty()) {
        throw std::runtime_error("Coșul este gol");
    }

    if (proprietar.getSold() < total) {
        throw SoldInsuficientException("Sold insuficient pentru a finaliza comanda");
    }

    for (const auto& prodCos : produse) {
        bool gasit = false;
        for (auto& prodCat : catalog) {
            if (prodCat->getId() == prodCos->getId()) {
                if (prodCat->getStoc() < 1) {
                    throw StocInsuficientException("Stoc insuficient pentru produsul " +
                                                    std::string(prodCat->getNume()));
                }
                prodCat->actualizeazaStoc(-1);  // scade cu 1
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            throw ProdusInexistentException("Produsul cu ID " +
                                             std::to_string(prodCos->getId()) +
                                             " nu mai există în catalog");
        }
    }

    proprietar.extrageFonduri(total);

    produse.clear();
    total = 0.0;
}

std::ostream& operator<<(std::ostream& os, const Cos& cos) {
    cos.afisareCos(os);
    return os;
}