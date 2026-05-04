# Proiect POO – Sistem de Gestiune Comenzi Online (Tema 2)

**Autor:** Luca Olteanu  
**Tema:** Comert – Comenzi online pentru un magazin universal  
**Tehnologii:** C++17, CMake, STL, smart pointers, exceptii  

---

## 📌 Descriere

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

## 🏗️ Arhitectura si concepte OOP implementate

### Ierarhia de clase

<a href="https://www.instagram.com/thepiyushmalhotra/">
  <img height="50" src="https://user-images.githubusercontent.com/46517096/166974368-9798f39f-1f46-499c-b14e-81f0a3f83a06.png"/>
</a>
