#include "Humano.h"
#include <iostream>

using namespace std;

Humano::Humano(string nombre, int hpMax, int ataque, int defensa)
    : Personaje(nombre, hpMax, ataque, defensa), turnos(0), UsadoEnBatalla(false) {
}

int Humano::getTurnos() {
    return turnos;
}

bool Humano::getUsadoEnBatalla() {
    return UsadoEnBatalla;
}

void Humano::reiniciarBatalla() {
    turnos = 0;
    UsadoEnBatalla = false;
}

int Humano::getAtaqueEfectivo() {
    if (tiene()) {
        return ataque + 40;
    }
    return ataque;
}

void Humano::atacar(Personaje& objetivo) {
    cout << nombre << " dispara armamento tactico a " << objetivo.getNombre() << "!" << endl;
    objetivo.recibirAtaque(getAtaqueEfectivo());
}

void Humano::habilidadEspecial(Personaje& objetivo) {
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

void Humano::activar() {
    if (!UsadoEnBatalla) {
        turnos = 3;
        UsadoEnBatalla = true;
        cout << "-> " << nombre << " activa V24 por 3 turnos (+40 de ataque)!" << endl;
    }
    else {
        cout << "-> " << nombre << " ya utilizo V24 en esta batalla!" << endl;
    }
}

void Humano::pasarTurno() {
    if (turnos > 0) {
        turnos--;
    }
}

bool Humano::tiene() {
    return turnos > 0;
}