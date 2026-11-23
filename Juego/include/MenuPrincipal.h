#pragma once
#include <vector>
#include <SFML/graphics.hpp>
enum class OpcionMenu { Jugar, Instrucciones, Records, Creditos, Salir };

class MenuPrincipal {
private:
	std::vector<sf::Text> _opciones;
	int _seleccion;
	sf::Text _titulo;
	sf::Texture _texturaFondo;
	sf::Sprite _fondo;
public:
	MenuPrincipal(const sf::Font& fuente);

	void subir();
	void bajar();
	OpcionMenu opcionElegida() const;
	void dibujarFondo(sf::RenderWindow& ventana);
	void dibujar(sf::RenderWindow& ventana);
};

