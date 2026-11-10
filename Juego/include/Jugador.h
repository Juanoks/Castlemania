#pragma once
#include <SFML/Graphics.hpp>
#include "Sonidos.h"

class Jugador {
private:
    sf::RectangleShape _forma;
    float _velocidadX;
    float _velocidadY;
    float _desplazamientoX;
    float _gravedad;
    float _fuerzaSalto;
    int _saltosRestantes;
    float _alturaPiesAnterior;
    bool _enElSuelo;
    bool _estabaEnElSuelo; 
    bool _saltoApretadoAntes;
    bool _tieneDobleSalto;
    float _cortarSalto;
    float _velocidadMaximaCaida;
    Sonidos& _sonidos;

    void sonarAterrizaje(); 

public:
    Jugador(sf::Vector2f posicion, Sonidos& sonidos);

    void dibujar(sf::RenderWindow& ventana);
    void reiniciar(sf::Vector2f posicion);
    void mover(float dt);
    void actualizar(float dt);
    void aterrizarEn(sf::FloatRect plataformaHitbox);
    void chocarHorizontal(sf::FloatRect bloqueHitbox);
    void chocarVertical(sf::FloatRect bloqueHitbox);
    bool estaEnElSuelo() const;
    void setDobleSalto(bool tieneDobleSalto);
    sf::FloatRect getHitbox() const;
};