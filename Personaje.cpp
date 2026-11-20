
#include "Personaje.h"
#include <iostream>

using namespace std;

Personaje::Personaje(string nombre, int hpMax, int ataque, int defensa)
    : nombre(nombre), hp(hpMax), hpMax(hpMax), ataque(ataque), defensa(defensa) {
}

Personaje::~Personaje() {}

string Personaje::getNombre(){
    return nombre;
}

int Personaje::getHp(){
    return hp;
}

int Personaje::getHpMax(){
    return hpMax;
}

int Personaje::getAtaque(){
    return ataque;
}

int Personaje::getDefensa(){
    return defensa;
}

void Personaje::setHp(int nuevoHp) {
    if (nuevoHp < 0) {
        hp = 0;
    }
    else if (nuevoHp > hpMax) {
        hp = hpMax;
    }
    else {
        hp = nuevoHp;
    }
}

int Personaje::getAtaqueEfectivo() {
    return ataque;
}

void Personaje::atacar(Personaje& objetivo) {
    cout << nombre << " ataca a " << objetivo.getNombre() << "!" << endl;
    objetivo.recibirAtaque(getAtaqueEfectivo());
}

void Personaje::recibirAtaque(int cantidad) {
    int ataqueAplicado = cantidad - (defensa / 2);
    if (ataqueAplicado < 0) {
        ataqueAplicado = 0;
    }
    setHp(hp - ataqueAplicado);
    cout << "-> " << nombre << " recibe " << ataqueAplicado << " de dano (HP: " << hp << "/" << hpMax << ")" << endl;
}

void Personaje::curar(int cantidad) {
    int hpAntes = hp;
    setHp(hp + cantidad);
    int recuperado = hp - hpAntes;
    cout << "-> " << nombre << " recupera " << recuperado << " HP (HP: " << hp << "/" << hpMax << ")" << endl;
}

bool Personaje::estaVivo(){
    return hp;
}