#pragma once
#include <SFML/Graphics.hpp>

enum class EstadoLanza {Volando, Clavada, Volviendo};

class Lanza {
private:
	sf::RectangleShape _forma;
	sf::Vector2f _velocidad; // va en un Vector2f y no en un float porque se mueve en X y en Y a la vez
	float _gravedad;
	EstadoLanza _estado;
	sf::Sprite _sprite;
public:
	Lanza(sf::Vector2f origen, sf::Vector2f destino, const sf::Texture& textura);

	void actualizar(float dt, sf::Vector2f posicionJugador);
	void clavar(sf::FloatRect zonaDeChoque, sf::FloatRect objetivo);
	bool estaClavada() const;
	void volver();
	bool estaVolviendo() const;
	void dibujar(sf::RenderWindow& ventana);
	sf::FloatRect getHitbox() const;

};

