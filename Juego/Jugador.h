#pragma once
#include <SFML/graphics.hpp>

class Jugador {
private:
    sf::RectangleShape _forma;
    float _velocidadX;
    float _velocidadY;
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
    sf::FloatRect getHitbox() const;
};

