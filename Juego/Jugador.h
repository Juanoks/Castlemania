#pragma once
#include <SFML/graphics.hpp>

class Jugador {
private:
    sf::RectangleShape _forma;
    float _velocidadX;
    float _velocidadY;
    float _desplazamientoX;
    float _gravedad;
    float _fuerzaSalto;
    float _alturaPiesAnterior;
    bool _enElSuelo;

public:
    Jugador(sf::Vector2f posicion);

    void dibujar(sf::RenderWindow& ventana);
    void reiniciar(sf::Vector2f posicion);
    void mover(float dt);
    void actualizar(float dt);
    void aterrizarEn(sf::FloatRect plataformaHitbox);
    void chocarHorizontal(sf::FloatRect bloqueHitBox);
    void chocarVertical(sf::FloatRect bloqueHitBox);
    sf::FloatRect getHitbox() const;
};