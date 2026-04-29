#ifndef COS_H
#define COS_H

#include <vector>
#include <memory>
#include "Produs.h"
#include "Utilizator.h"

class Cos {
private:
    Utilizator proprietar;
    std::vector<std::unique_ptr<Produs>> produse;
    double total;

public:
    Cos();
    Cos(const Utilizator& prop);
    Cos(const Cos& other);
    Cos(Cos&& other) noexcept;
    ~Cos();

    Cos& operator=(const Cos& other);
    Cos& operator=(Cos&& other) noexcept;

    void adaugaProdus(const Produs& produs, int cantitate);
    void eliminaProdus(int index);
    double getTotal() const;
    void afisareCos(std::ostream& os) const;
    void finalizeazaComanda();

    friend std::ostream& operator<<(std::ostream& os, const Cos& cos);
};

#endif