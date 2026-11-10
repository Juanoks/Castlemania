#pragma once
#include <vector>
#include <SFML/graphics.hpp>
enum class OpcionMenu {Jugar,Instrucciones,Records,Creditos,Salir};

class MenuPrincipal {
private:
	std::vector<sf::Text> _opciones;
	int _seleccion;
	sf::Text _titulo;
public:
	MenuPrincipal(const sf::Font& fuente);

	void subir();
	void bajar();
	OpcionMenu opcionElegida() const;
	void dibujar(sf::RenderWindow& ventana);
};

