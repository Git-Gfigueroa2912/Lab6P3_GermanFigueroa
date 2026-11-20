#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

template <typename T>
class Inventario {
private:
    vector<T> elementos;

public:
    void agregar(T item) {
        elementos.push_back(item);
    }

    T extraer(int indice) {
        T item = elementos.at(indice);
        elementos.erase(elementos.begin() + indice);
        return item;
    }

    T obtener(int indice) const {
        return elementos.at(indice);
    }

    int cantidad() const {
        return static_cast<int>(elementos.size());
    }

    bool estaVacio() const {
        return elementos.empty();
    }
};

class Personaje {
protected:
    string nombre;
    int hp;
    int hpMax;
    int ataque;
    int defensa;

public:
    Personaje(string nombre, int hpMax, int ataque, int defensa)
        : nombre(nombre), hp(hpMax), hpMax(hpMax), ataque(ataque), defensa(defensa) {
    }

    virtual ~Personaje() {}

    string getNombre() const { return nombre; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }

    void setHp(int nuevoHp) {
        if (nuevoHp < 0) hp = 0;
        else if (nuevoHp > hpMax) hp = hpMax;
        else hp = nuevoHp;
    }

    virtual int getAtaqueEfectivo() const {
        return ataque;
    }

    virtual void atacar(Personaje& objetivo) {
        cout << nombre << " ataca a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirAtaque(getAtaqueEfectivo());
    }

    virtual void habilidadEspecial(Personaje& objetivo) = 0;

    virtual void recibirAtaque(int cantidad) {
        int ataqueAplicado = cantidad - (defensa / 2);
        if (ataqueAplicado < 0) ataqueAplicado = 0;
        setHp(hp - ataqueAplicado);
        cout << "-> " << nombre << " recibe " << ataqueAplicado << " de dano (HP: " << hp << "/" << hpMax << ")" << endl;
    }

    void curar(int cantidad) {
        int hpAntes = hp;
        setHp(hp + cantidad);
        int recuperado = hp - hpAntes;
        cout << "-> " << nombre << " recupera " << recuperado << " HP (HP: " << hp << "/" << hpMax << ")" << endl;
    }

    bool estaVivo() const {
        return hp > 0;
    }
};

class Humano : public Personaje {
private:
    int turnos;
    bool UsadoEnBatalla;

public:
    Humano(string nombre, int hpMax, int ataque, int defensa)
        : Personaje(nombre, hpMax, ataque, defensa), turnos(0), UsadoEnBatalla(false) {
    }

    int getTurnos() const { return turnos; }
    bool getUsadoEnBatalla() const { return UsadoEnBatalla; }

    void reiniciarBatalla() {
        turnos = 0;
        UsadoEnBatalla = false;
    }

    int getAtaqueEfectivo() {
        if (tiene()) return ataque + 40;
        return ataque;
    }

    void atacar(Personaje& objetivo) {
        cout << nombre << " dispara armamento tactico a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirAtaque(getAtaqueEfectivo());
    }

    void habilidadEspecial(Personaje& objetivo) {
        if (tiene()) {
            cout << "[V24 ACTIVO] " << nombre << " lanza un laser contra " << objetivo.getNombre() << "!" << endl;
            int ataqueLaser = (ataque + 40) * 2;
            objetivo.recibirAtaque(ataqueLaser);
        }
        else {
            cout << "[ANTI-SUPE] " << nombre << " usa balas perforantes contra " << objetivo.getNombre() << "!" << endl;
            int ataqueBalas = (objetivo.getDefensa() / 2) + 15;
            objetivo.recibirAtaque(ataqueBalas);
        }
    }

    void activar() {
        if (UsadoEnBatalla) {
            turnos = 3;
            UsadoEnBatalla = true;
            cout << "-> " << nombre << " activa V24 por 3 turnos (+40 de ataque)!" << endl;
        }
        else {
            cout << "-> " << nombre << " ya utilizo V24 en esta batalla!" << endl;
        }
    }

    void pasarTurno() {
        if (turnos > 0) {
            turnos--;
        }
    }

    bool tiene() const {
        return turnos > 0;
    }
};

class Super : public Personaje {
private:
    char rango;
    string poder;
    double multiplicador;

    int aleatorio(int valorMinimo, int valorMaximo) {
        return valorMinimo + (rand() % (valorMaximo - valorMinimo + 1));
    }

public:
    Super(string nombre, char rango, string poder)
        : Personaje(nombre, 100, 10, 10), rango(toupper(rango)), poder(poder), multiplicador(1.0) {
        switch (this->rango) {
        case 'A':
            hpMax = aleatorio(450, 520);
            ataque = aleatorio(65, 78);
            defensa = aleatorio(42, 52);
            multiplicador = 2.0;
            break;
        case 'B':
            hpMax = aleatorio(230, 280);
            ataque = aleatorio(40, 50);
            defensa = aleatorio(26, 36);
            multiplicador = 1.6;
            break;
        case 'C':
            hpMax = aleatorio(160, 210);
            ataque = aleatorio(28, 36);
            defensa = aleatorio(16, 24);
            multiplicador = 1.4;
            break;
        case 'D':
        default:
            this->rango = 'D';
            hpMax = aleatorio(110, 140);
            ataque = aleatorio(18, 25);
            defensa = aleatorio(8, 14);
            multiplicador = 1.2;
            break;
        }
        hp = hpMax;
    }

    char getRango() const { return rango; }
    string getPoder() const { return poder; }
    double getMultiplicador() const { return multiplicador; }

    string getRangoTexto() const {
        switch (rango) {
        case 'A': return "Rango ALPHA";
        case 'B': return "Rango BETA";
        case 'C': return "Rango OMEGA";
        case 'D': return "Rango GAMMA";
        default: return "Rango DESCONOCIDO";
        }
    }

    void atacar(Personaje& objetivo) override {
        cout << nombre << " ataca a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirDanio(ataque);
    }

    void habilidadEspecial(Personaje& objetivo) override {
        cout << "[" << getRangoTexto() << "] " << nombre << " usa " << poder << " contra " << objetivo.getNombre() << "!" << endl;
        int danioEspecial = static_cast<int>(ataque * multiplicador);
        objetivo.recibirDanio(danioEspecial);
    }
};

bool equipoEstaVivo(const vector<Humano*>& equipo) {
    for (Humano* h : equipo) {
        if (h != nullptr && h->estaVivo()) {
            return true;
        }
    }
    return false;
}

bool luchar(vector<Humano*>& equipo, Super* enemigo, Inventario<string>& mochila) {
    for (Humano* h : equipo) {
        if (h != nullptr && h->estaVivo()) {
            h->reiniciarBatalla();
        }
    }

    cout << "\n>>> APARECE: " << enemigo->getNombre() << " [" << enemigo->getRangoTexto() << "] (HP: " << enemigo->getHp() << ") <<<" << endl;

    while (equipoEstaVivo(equipo) && enemigo->estaVivo()) {
        for (size_t i = 0; i < equipo.size(); ++i) {
            Humano* jugador = equipo[i];
            if (jugador == nullptr || !jugador->estaVivo() || !enemigo->estaVivo()) {
                continue;
            }

            int opcion = 0;
            cout << "\nTurno de " << jugador->getNombre() << " (HP: " << jugador->getHp() << "/" << jugador->getHpMax() << ")" << endl;
            cout << "1. Atacar   2. Habilidad Especial   3. Usar Mochila: ";
            if (!(cin >> opcion)) {
                cin.clear();
                cin.ignore(10000, '\n');
                opcion = 1;
            }

            if (opcion == 1) {
                jugador->atacar(*enemigo);
            }
            else if (opcion == 2) {
                jugador->habilidadEspecial(*enemigo);
            }
            else if (opcion == 3) {
                if (mochila.estaVacio()) {
                    cout << "La mochila esta vacia! Se realizara un ataque normal." << endl;
                    jugador->atacar(*enemigo);
                }
                else {
                    for (int idx = 0; idx < mochila.cantidad(); ++idx) {
                        cout << idx << ". " << mochila.obtener(idx) << endl;
                    }
                    int idxObjeto = -1;
                    cout << "Elige objeto: ";
                    if (!(cin >> idxObjeto)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        idxObjeto = -1;
                    }

                    if (idxObjeto >= 0 && idxObjeto < mochila.cantidad()) {
                        string item = mochila.extraer(idxObjeto);
                        if (item == "Botiquin") {
                            jugador->curar(75);
                        }
                        else if (item == "Vial V24") {
                            jugador->activarV24();
                        }
                        else if (item == "Granada Termita") {
                            cout << jugador->getNombre() << " lanza una Granada Termita a " << enemigo->getNombre() << "!" << endl;
                            enemigo->recibirDanio(95);
                        }
                        else {
                            jugador->atacar(*enemigo);
                        }
                    }
                    else {
                        cout << "Indice fuera de rango! Se realizara un ataque normal." << endl;
                        jugador->atacar(*enemigo);
                    }
                }
            }
            else {
                jugador->atacar(*enemigo);
            }

            jugador->pasarTurno();

            if (!enemigo->estaVivo()) {
                break;
            }
        }

        if (enemigo->estaVivo() && equipoEstaVivo(equipo)) {
            vector<Humano*> vivos;
            for (Humano* h : equipo) {
                if (h != nullptr && h->estaVivo()) {
                    vivos.push_back(h);
                }
            }
            if (!vivos.empty()) {
                Humano* objetivo = vivos[rand() % vivos.size()];
                int accionEnemigo = rand() % 2;
                cout << endl;
                if (accionEnemigo == 0) {
                    enemigo->atacar(*objetivo);
                }
                else {
                    enemigo->habilidadEspecial(*objetivo);
                }
            }
        }
    }

    if (!enemigo->estaVivo()) {
        cout << "\n" << enemigo->getNombre() << " VENCIDO! ***" << endl;
        for (Humano* h : equipo) {
            if (h != nullptr && h->estaVivo()) {
                h->curar(30);
            }
        }
        return true;
    }

    return false;
}

void liberarMemoria(vector<Humano*>& equipo, vector<Super*>& superDisponibles, vector<Super*>& superVencidos, Super* homelander) {
    for (Humano* humano : equipo) {
        delete humano;
    }
    equipo.clear();

    for (Super* super : supersDisponibles) {
        delete super;
    }
    superDisponibles.clear();

    for (Super* super : superVencidos) {
        delete super;
    }
    superVencidos.clear();

    delete homelander;
}

void mostrarSuperVencidos(const vector<Super*>& supersVencidos) {
    cout << " SUPES VENCIDOS (" << supersVencidos.size() << ")" << endl;
    for (Super* s : supersVencidos) {
        if (s != nullptr) {
            cout << s->getNombre() << " [" << s->getRangoTexto() << "]" << endl;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Humano*> equipo;
    equipo.push_back(new Humano("Billy Butcher", 180, 22, 14));
    equipo.push_back(new Humano("Mother's Milk", 165, 18, 18));
    equipo.push_back(new Humano("Frenchie", 150, 19, 10));
    equipo.push_back(new Humano("Kimiko", 170, 24, 12));

    vector<Super*> supersDisponibles;
    supersDisponibles.push_back(new Super("Starlight", 'B', "Explosion de Plasma"));
    supersDisponibles.push_back(new Super("A-Train", 'B', "Super Velocidad"));
    supersDisponibles.push_back(new Super("The Deep", 'C', "Control Marino"));
    supersDisponibles.push_back(new Super("Black Noir", 'B', "Sigilo Letal"));
    supersDisponibles.push_back(new Super("Love Sausage", 'C', "Elasticidad Extrema"));
    supersDisponibles.push_back(new Super("Oh-Father", 'C', "Fuerza Radiante"));
    supersDisponibles.push_back(new Super("Termite", 'D', "Encogimiento"));
    supersDisponibles.push_back(new Super("Popclaw", 'D', "Garras Oseas"));
    supersDisponibles.push_back(new Super("Translucent", 'C', "Piel de Diamante Invisible"));
    supersDisponibles.push_back(new Super("Stormfront", 'A', "Descarga Electrica"));
    supersDisponibles.push_back(new Super("Soldier Boy", 'A', "Explosion Nuclear"));
    supersDisponibles.push_back(new Super("Firecracker", 'D', "Chispas Pirotecnicas"));

    vector<Super*> supersVencidos;

    Super* homelander = new Super("Homelander", 'A', "Vision Calorifica");

    Inventario<string> mochila;
    mochila.agregar("Botiquin");
    mochila.agregar("Botiquin");
    mochila.agregar("Botiquin");
    mochila.agregar("Vial V24");
    mochila.agregar("Vial V24");
    mochila.agregar("Granada Termita");
    mochila.agregar("Granada Termita");

    bool derrota = false;
    int rondas = 5;

    for (int r = 0; r < rondas; ++r) {
        if (supersDisponibles.empty() || !equipoEstaVivo(equipo)) {
            break;
        }

        int idxEnemigo = rand() % static_cast<int>(supersDisponibles.size());
        Super* enemigoActual = supersDisponibles[idxEnemigo];
        supersDisponibles.erase(supersDisponibles.begin() + idxEnemigo);

        bool ganoRonda = luchar(equipo, enemigoActual, mochila);
        if (ganoRonda) {
            supersVencidos.push_back(enemigoActual);
        }
        else {
            supersDisponibles.push_back(enemigoActual);
            derrota = true;
            break;
        }
    }

    if (!derrota && equipoEstaVivo(equipo)) {
        bool ganoHomelander = luchar(equipo, homelander, mochila);
        if (ganoHomelander) {
            supersVencidos.push_back(homelander);
            homelander = nullptr;
            cout << "\nVICTORIA TOTAL! Homelander ha sido derrotado." << endl;
            mostrarSupersVencidos(supersVencidos);
        }
        else {
            derrota = true;
        }
    }

    if (derrota) {
        cout << "\nDERROTA! El equipo ha caido en la simulacion." << endl;
        mostrarSupersVencidos(supersVencidos);
    }

    liberarMemoria(equipo, supersDisponibles, supersVencidos, homelander);

    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

template <typename T>
class Inventario {
private:
    vector<T> elementos;

public:
    void agregar(T item) {
        elementos.push_back(item);
    }

    T extraer(int indice) {
        T item = elementos.at(indice);
        elementos.erase(elementos.begin() + indice);
        return item;
    }

    T obtener(int indice) const {
        return elementos.at(indice);
    }

    int cantidad() const {
        return static_cast<int>(elementos.size());
    }

    bool estaVacio() const {
        return elementos.empty();
    }
};

class Personaje {
protected:
    string nombre;
    int hp;
    int hpMax;
    int ataque;
    int defensa;

public:
    Personaje(string nombre, int hpMax, int ataque, int defensa)
        : nombre(nombre), hp(hpMax), hpMax(hpMax), ataque(ataque), defensa(defensa) {
    }

    virtual ~Personaje() {}

    string getNombre() const { return nombre; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    int getAtaque() const { return ataque; }
    int getDefensa() const { return defensa; }

    void setHp(int nuevoHp) {
        if (nuevoHp < 0) hp = 0;
        else if (nuevoHp > hpMax) hp = hpMax;
        else hp = nuevoHp;
    }

    virtual int getAtaqueEfectivo() const {
        return ataque;
    }

    virtual void atacar(Personaje& objetivo) {
        cout << nombre << " ataca a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirDanio(getAtaqueEfectivo());
    }

    virtual void habilidadEspecial(Personaje& objetivo) = 0;

    virtual void recibirDanio(int cantidad) {
        int danioAplicado = cantidad - (defensa / 2);
        if (danioAplicado < 0) danioAplicado = 0;
        setHp(hp - danioAplicado);
        cout << "-> " << nombre << " recibe " << danioAplicado << " de dano (HP: " << hp << "/" << hpMax << ")" << endl;
    }

    void curar(int cantidad) {
        int hpAntes = hp;
        setHp(hp + cantidad);
        int recuperado = hp - hpAntes;
        cout << "-> " << nombre << " recupera " << recuperado << " HP (HP: " << hp << "/" << hpMax << ")" << endl;
    }

    bool estaVivo() const {
        return hp > 0;
    }
};

class Humano : public Personaje {
private:
    int turnosV24;
    bool v24UsadoEnBatalla;

public:
    Humano(string nombre, int hpMax, int ataque, int defensa)
        : Personaje(nombre, hpMax, ataque, defensa), turnosV24(0), v24UsadoEnBatalla(false) {
    }

    int getTurnosV24() const { return turnosV24; }
    bool getV24UsadoEnBatalla() const { return v24UsadoEnBatalla; }

    void reiniciarBatalla() {
        turnosV24 = 0;
        v24UsadoEnBatalla = false;
    }

    int getAtaqueEfectivo() const override {
        if (tieneV24()) return ataque + 40;
        return ataque;
    }

    void atacar(Personaje& objetivo) override {
        cout << nombre << " dispara armamento tactico a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirDanio(getAtaqueEfectivo());
    }

    void habilidadEspecial(Personaje& objetivo) override {
        if (tieneV24()) {
            cout << "[V24 ACTIVO] " << nombre << " lanza un laser contra " << objetivo.getNombre() << "!" << endl;
            int danioLaser = (ataque + 40) * 2;
            objetivo.recibirDanio(danioLaser);
        }
        else {
            cout << "[ANTI-SUPE] " << nombre << " usa balas perforantes contra " << objetivo.getNombre() << "!" << endl;
            int danioBalas = (objetivo.getDefensa() / 2) + 15;
            objetivo.recibirDanio(danioBalas);
        }
    }

    void activarV24() {
        if (!v24UsadoEnBatalla) {
            turnosV24 = 3;
            v24UsadoEnBatalla = true;
            cout << "-> " << nombre << " activa V24 por 3 turnos (+40 de ataque)!" << endl;
        }
        else {
            cout << "-> " << nombre << " ya utilizo V24 en esta batalla!" << endl;
        }
    }

    void pasarTurno() {
        if (turnosV24 > 0) {
            turnosV24--;
        }
    }

    bool tieneV24() const {
        return turnosV24 > 0;
    }
};

class Super : public Personaje {
private:
    char rango;
    string poder;
    double multiplicador;

    int aleatorio(int valorMin, int valorMax) {
        return valorMin + (rand() % (valorMax - valorMin + 1));
    }

public:
    Super(string nombre, char rango, string poder)
        : Personaje(nombre, 100, 10, 10), rango(toupper(rango)), poder(poder), multiplicador(1.0) {
        switch (this->rango) {
        case 'A':
            hpMax = aleatorio(450, 520);
            ataque = aleatorio(65, 78);
            defensa = aleatorio(42, 52);
            multiplicador = 2.0;
            break;
        case 'B':
            hpMax = aleatorio(230, 280);
            ataque = aleatorio(40, 50);
            defensa = aleatorio(26, 36);
            multiplicador = 1.6;
            break;
        case 'C':
            hpMax = aleatorio(160, 210);
            ataque = aleatorio(28, 36);
            defensa = aleatorio(16, 24);
            multiplicador = 1.4;
            break;
        case 'D':
        default:
            this->rango = 'D';
            hpMax = aleatorio(110, 140);
            ataque = aleatorio(18, 25);
            defensa = aleatorio(8, 14);
            multiplicador = 1.2;
            break;
        }
        hp = hpMax;
    }

    char getRango() const { return rango; }
    string getPoder() const { return poder; }
    double getMultiplicador() const { return multiplicador; }

    string getRangoTexto() const {
        switch (rango) {
        case 'A': return "Rango ALPHA";
        case 'B': return "Rango BETA";
        case 'C': return "Rango OMEGA";
        case 'D': return "Rango GAMMA";
        default: return "Rango DESCONOCIDO";
        }
    }

    void atacar(Personaje& objetivo) {
        cout << nombre << " ataca a " << objetivo.getNombre() << "!" << endl;
        objetivo.recibirDanio(ataque);
    }

    void habilidadEspecial(Personaje& objetivo) {
        cout << "[" << getRangoTexto() << "] " << nombre << " usa " << poder << " contra " << objetivo.getNombre() << "!" << endl;
        int ataqueEspecial = static_cast<int>(ataque * multiplicador);
        objetivo.recibirAtaque(ataqueEspecial);
    }
};

bool equipoEstaVivo(const vector<Humano*>& equipo) {
    for (Humano* humano : equipo) {
        if (humano != nullptr && humano->estaVivo()) {
            return true;
        }
    }
    return false;
}

bool luchar(vector<Humano*>& equipo, Super* enemigo, Inventario<string>& mochila) {
    for (Humano* humano : equipo) {
        if (humano != nullptr && humano->estaVivo()) {
            humano->reiniciarBatalla();
        }
    }

    cout << " APARECE: " << enemigo->getNombre() << " [" << enemigo->getRangoTexto() << "] (HP: " << enemigo->getHp() << ") <<<" << endl;

    while (equipoEstaVivo(equipo) && enemigo->estaVivo()) {
        for (size_t i = 0; i < equipo.size(); ++i) {
            Humano* jugador = equipo[i];
            if (jugador == nullptr || !jugador->estaVivo() || !enemigo->estaVivo()) {
                continue;
            }

            int opcion = 0;
            cout << " Turno de " << jugador->getNombre() << " (HP: " << jugador->getHp() << "/" << jugador->getHpMax() << ")" << endl;
            cout << "1. Atacar   2. Habilidad Especial   3. Usar Mochila: ";
            if (!(cin >> opcion)) {
                cin.clear();
                cin.ignore(10000, '\n');
                opcion = 1;
            }

            if (opcion == 1) {
                jugador->atacar(*enemigo);
            }
            else if (opcion == 2) {
                jugador->habilidadEspecial(*enemigo);
            }
            else if (opcion == 3) {
                if (mochila.estaVacio()) {
                    cout << "La mochila esta vacia! Se realizara un ataque normal." << endl;
                    jugador->atacar(*enemigo);
                }
                else {
                    for (int idx = 0; idx < mochila.cantidad(); ++idx) {
                        cout << idx << ". " << mochila.obtener(idx) << endl;
                    }
                    int idxObjeto = -1;
                    cout << "Elige objeto: ";
                    if (!(cin >> idxObjeto)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        idxObjeto = -1;
                    }

                    if (idxObjeto >= 0 && idxObjeto < mochila.cantidad()) {
                        string item = mochila.extraer(idxObjeto);
                        if (item == "Botiquin") {
                            jugador->curar(75);
                        }
                        else if (item == "Vial V24") {
                            jugador->activarV24();
                        }
                        else if (item == "Granada Termita") {
                            cout << jugador->getNombre() << " lanza una Granada Termita a " << enemigo->getNombre() << "!" << endl;
                            enemigo->recibirDanio(95);
                        }
                        else {
                            jugador->atacar(*enemigo);
                        }
                    }
                    else {
                        cout << "Indice fuera de rango! Se realizara un ataque normal." << endl;
                        jugador->atacar(*enemigo);
                    }
                }
            }
            else {
                jugador->atacar(*enemigo);
            }

            jugador->pasarTurno();

            if (!enemigo->estaVivo()) {
                break;
            }
        }

        if (enemigo->estaVivo() && equipoEstaVivo(equipo)) {
            vector<Humano*> vivos;
            for (Humano* humano : equipo) {
                if (humano != nullptr && humano->estaVivo()) {
                    vivos.push_back(humano);
                }
            }
            if (!vivos.empty()) {
                Humano* objetivo = vivos[rand() % vivos.size()];
                int accionEnemigo = rand() % 2;
                cout << endl;
                if (accionEnemigo == 0) {
                    enemigo->atacar(*objetivo);
                }
                else {
                    enemigo->habilidadEspecial(*objetivo);
                }
            }
        }
    }

    if (!enemigo->estaVivo()) {
        cout << "\n" << enemigo->getNombre() << " VENCIDO! " << endl;
        for (Humano* humano : equipo) {
            if (humano != nullptr && humano->estaVivo()) {
                humano->curar(30);
            }
        }
        return true;
    }

    return false;
}

void liberarMemoria(vector<Humano*>& equipo, vector<Super*>& superDisponibles, vector<Super*>& superVencidos, Super* homelander) {
    for (Humano* humano : equipo) {
        delete humano;
    }
    equipo.clear();

    for (Super* super : superDisponibles) {
        delete super;
    }
    superDisponibles.clear();

    for (Super* super : superVencidos) {
        delete super;
    }
    superVencidos.clear();

    delete homelander;
}

void mostrarSuperVencidos(const vector<Super*>& superVencidos) {
    cout << " SUPES VENCIDOS (" << supersVencidos.size() << ")" << endl;
    for (Super* super : supersVencidos) {
        if (super != nullptr) {
            cout << super->getNombre() << " [" << super->getRangoTexto() << "]" << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "spanish");
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Humano*> equipo;
    equipo.push_back(new Humano("Billy Butcher", 180, 22, 14));
    equipo.push_back(new Humano("Mother's Milk", 165, 18, 18));
    equipo.push_back(new Humano("Frenchie", 150, 19, 10));
    equipo.push_back(new Humano("Kimiko", 170, 24, 12));

    vector<Super*> superDisponibles;
    superDisponibles.push_back(new Super("Starlight", 'B', "Explosion de Plasma"));
    superDisponibles.push_back(new Super("A-Train", 'B', "Super Velocidad"));
    superDisponibles.push_back(new Super("The Deep", 'C', "Control Marino"));
    superDisponibles.push_back(new Super("Black Noir", 'B', "Sigilo Letal"));
    superDisponibles.push_back(new Super("Love Sausage", 'C', "Elasticidad Extrema"));
    superDisponibles.push_back(new Super("Oh-Father", 'C', "Fuerza Radiante"));
    superDisponibles.push_back(new Super("Termite", 'D', "Encogimiento"));
    superDisponibles.push_back(new Super("Popclaw", 'D', "Garras Oseas"));
    superDisponibles.push_back(new Super("Translucent", 'C', "Piel de Diamante Invisible"));
    superDisponibles.push_back(new Super("Stormfront", 'A', "Descarga Electrica"));
    superDisponibles.push_back(new Super("Soldier Boy", 'A', "Explosion Nuclear"));
    superDisponibles.push_back(new Super("Firecracker", 'D', "Chispas Pirotecnicas"));

    vector<Super*> supersVencidos;

    Super* homelander = new Super("Homelander", 'A', "Vision Calorifica");

    Inventario<string> mochila;
    mochila.agregar("Botiquin");
    mochila.agregar("Botiquin");
    mochila.agregar("Botiquin");
    mochila.agregar("Vial V24");
    mochila.agregar("Vial V24");
    mochila.agregar("Granada Termita");
    mochila.agregar("Granada Termita");

    bool derrota = false;
    int rondas = 5;

    for (int r = 0; r < rondas; ++r) {
        if (superDisponibles.empty() || !equipoEstaVivo(equipo)) {
            break;
        }

        int idxEnemigo = rand() % static_cast<int>(superDisponibles.size());
        Super* enemigoActual = superDisponibles[idxEnemigo];
        superDisponibles.erase(superDisponibles.begin() + idxEnemigo);

        bool rondaGanada = luchar(equipo, enemigoActual, mochila);
        if (rondaGanada) {
            superVencidos.push_back(enemigoActual);
        }
        else {
            superDisponibles.push_back(enemigoActual);
            derrota = true;
            break;
        }
    }

    if (!derrota && equipoEstaVivo(equipo)) {
        bool ganoHomelander = luchar(equipo, homelander, mochila);
        if (ganoHomelander) {
            superVencidos.push_back(homelander);
            homelander = nullptr;
            cout << " VICTORIA! Homelander ha sido derrotado." << endl;
            mostrarSuperVencidos(superVencidos);
        }
        else {
            derrota = true;
        }
    }

    if (derrota) {
        cout << " DERROTA! El equipo ha caido en la simulacion." << endl;
        mostrarSuperVencidos(superVencidos);
    }

    liberarMemoria(equipo, superDisponibles, superVencidos, homelander);

    return 0;

}


























