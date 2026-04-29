#ifndef UTILIZATOR_H
#define UTILIZATOR_H

#include <string>

class Utilizator {
private:
    static int nextId;
    const int id;
    std::string nume;
    std::string adresa;
    double sold;

public:
    Utilizator();
    Utilizator(const std::string& nume, const std::string& adresa, double sold);
    Utilizator(const Utilizator& other);
    Utilizator(Utilizator&& other) noexcept;
    ~Utilizator();

    Utilizator& operator=(const Utilizator& other);
    Utilizator& operator=(Utilizator&& other) noexcept;

    int getId() const;
    const std::string& getNume() const;
    const std::string& getAdresa() const;
    double getSold() const;

    void setSold(double soldNou);
    void adaugaFonduri(double suma);
    void extrageFonduri(double suma);

    friend std::ostream& operator<<(std::ostream& os, const Utilizator& u);
};

#endif