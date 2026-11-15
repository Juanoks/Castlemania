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

    sf::Sprite _sprite;
    float _tiempoAnimacion; // para saber cuándo pasar al siguiente cuadro de correr
    int _cuadroCorrer;  // de 0 a 3, cuál de los 4 de correr toca
    float _tiempoLanzando; // cuanto queda mostrando el cuadro de lanzar

    Sonidos& _sonidos;

    void sonarAterrizaje(); 
    int elegirCuadro() const;

public:
    Jugador(sf::Vector2f posicion, Sonidos& sonidos, const sf::Texture& textura);

    void dibujar(sf::RenderWindow& ventana);
    void reiniciar(sf::Vector2f posicion);
    void mover(float dt);
    void actualizar(float dt);
    void aterrizarEn(sf::FloatRect plataformaHitbox);
    void chocarHorizontal(sf::FloatRect bloqueHitbox);
    void chocarVertical(sf::FloatRect bloqueHitbox);
    bool estaEnElSuelo() const;
    void animarLanzamiento(float destinoX);
    void setDobleSalto(bool tieneDobleSalto);
    sf::FloatRect getHitbox() const;
};