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
    int _saltosRestantes;
    float _alturaPiesAnterior;
    bool _enElSuelo;
    bool _saltoApretadoAntes;
    float _cortarSalto;
    float _velocidadMaximaCaida;

public:
    Jugador(sf::Vector2f posicion);

    void dibujar(sf::RenderWindow& ventana);
    void reiniciar(sf::Vector2f posicion);
    void mover(float dt);
    void actualizar(float dt);
    void aterrizarEn(sf::FloatRect plataformaHitbox);
    void chocarHorizontal(sf::FloatRect bloqueHitbox);
    void chocarVertical(sf::FloatRect bloqueHitbox);
    sf::FloatRect getHitbox() const;
};