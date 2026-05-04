# Proiect POO – Sistem de Gestiune Comenzi Online (Tema 2)

**Autor:** Luca Olteanu  
**Tema:** Comert – Comenzi online pentru un magazin universal  

---

##  Descriere

Aplicatia simuleaza un magazin online unde utilizatorul poate:
- Vizualiza un catalog de produse (alimentare, electronice, mobilier)
- Adauga produse in cos (cu verificarea stocului)
- Elimina produse din cos
- Finaliza comanda – se valideaza soldul utilizatorului si se actualizeaza stocurile
- Gestiona situatii exceptionale (stoc insuficient, fonduri insuficiente, produs inexistent)
- Observa comportamentul polimorf – pretul final al produselor se calculeaza diferit in functie de tip:
    - **Alimentare:** reducere 50% daca expira in < 24h, pret 0 daca expirat
    - **Electronice:** reducere 10% pentru garantie > 24 luni
    - **Mobilier:** taxa suplimentara de transport in functie de greutate

Datele de intrare (utilizator si catalog) sunt citite dintr-un fisier text.

---

##  Arhitectura si concepte OOP implementate

### Ierarhia de clase

Produs (clasa abstracta)
├── ProdusAlimentar
├── ProdusElectronic
└── ProdusMobilier

Utilizator

Cos
└── contine std::vector<std::unique_ptr<Produs>>


### Caracteristici specifice

- **Mostenire publica** + **functii virtuale pure** (`calculeazaPretFinal`, `clone`, `afisare`)
- **Constructor virtual** – metoda `clone()` pentru copierea polimorfa
- **Interfata non-virtuala (NVI)** – `operator<<` public non-virtual apeleaza `afisare` virtuala `protected`
- **Regula celor cinci** implementata in `Produs`, `Utilizator`, `Cos` (copy-and-swap in `Cos`)
- **Smart pointers** – `std::unique_ptr` pentru ownership exclusiv (catalog si cos)
- **Dynamic cast** – verificare tip `ProdusAlimentar*` pentru a afisa avertizare de expirare
- **Exceptii proprii** – ierarhie derivata din `std::runtime_error`:
    - `StocInsuficientException`
    - `SoldInsuficientException`
    - `ProdusInexistentException`
- **Atribute si functii statice** – generare automata ID (`nextId`)
- **Separare cod** – fisiere `.h` si `.cpp` pentru fiecare clasa

---

## ⚙️ Compilare si rulare

### Folosind CMake (CLion)

1. Deschide proiectul in CLion.
2. Asigura-te ca fisierul `CMakeLists.txt` include toate sursele:
   ```cmake
   add_executable(Proiect2POO 
       main.cpp 
       Produs.cpp 
       ProdusAlimentar.cpp 
       ProdusElectronic.cpp 
       ProdusMobilier.cpp 
       Utilizator.cpp 
       Cos.cpp
   )
3.Apasa Build → Run.

Fisierul de intrare (date_intrare.txt)
Format exemplu:

USER: Popescu Ion, Strada Principala 10, 500.0

PRODUS_ALIMENTAR: Mere, 4.5, 100, 2025-05-10
PRODUS_ALIMENTAR: Lapte, 6.0, 30, 2025-05-04
PRODUS_ELECTRONIC: Mouse Wireless, 120.0, 15, 24, 2.5
PRODUS_ELECTRONIC: Tastatura mecanica, 350.0, 8, 36, 5.0
PRODUS_MOBILIER: Birou, 450.0, 5, lemn masiv, 35.0
PRODUS_MOBILIER: Scaun ergonomic, 320.0, 12, material textil, 18.0
PRODUS_ALIMENTAR: Paine, 2.5, 50, 2025-05-02

Data expirarii in format YYYY-MM-DD. Programul compara cu data curenta a sistemului.

Scenariu de utilizare (fragment din main.cpp)

// Citire catalog si utilizator din fisier
// ... parsare ...

// Creare cos
Cos cos(user);

// Adaugare produse
cos.adaugaProdus(*catalog[0], 2);
cos.adaugaProdus(*catalog[1], 1);

// Afisare cos
std::cout << cos;

// Finalizare comanda (cu tratarea exceptiilor)
try {
    cos.finalizeazaComanda(catalog);
} catch (const SoldInsuficientException& e) {
    std::cerr << "Eroare: " << e.what() << '\n';
}
