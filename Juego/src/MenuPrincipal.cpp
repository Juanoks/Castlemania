#include "MenuPrincipal.h"

MenuPrincipal::MenuPrincipal(const sf::Font& fuente) : _titulo(fuente,"Punto B", 40){
	_seleccion = 0; // arranca en jugar
	_opciones.push_back(sf::Text(fuente, "Jugar", 25));
	_opciones.push_back(sf::Text(fuente, "Records", 25));
	_opciones.push_back(sf::Text(fuente, "Creditos", 25));
	_opciones.push_back(sf::Text(fuente, "Salir", 25));

	_titulo.setPosition(sf::Vector2f(330.f, 50.f));		  // Titulo
	_opciones[0].setPosition(sf::Vector2f(330.f, 180.f)); // Jugar
	_opciones[1].setPosition(sf::Vector2f(330.f, 260.f)); // Records
	_opciones[2].setPosition(sf::Vector2f(330.f, 340.f)); // Creditos
	_opciones[3].setPosition(sf::Vector2f(330.f, 420.f)); // Salir
}

// sube la seleccion, sin pasar de la primera opción
void MenuPrincipal::subir() {
	if (_seleccion > 0) {
		_seleccion--;
	}	
}

void MenuPrincipal::bajar() {
	if (_seleccion < _opciones.size() - 1) {
		_seleccion++;
	}
	
}

// convierte la posición elegida (0, 1, 2) en la opción del enum.
// funciona porque OpcionMenu está declarado en el mismo orden que los textos.
OpcionMenu MenuPrincipal::opcionElegida() const {
	return static_cast<OpcionMenu>(_seleccion);
	// el static_cast hace falta porque un enum class no se convierte solo desde un int.
}

// dibuja el título y las opciones, con la seleccionada en amarillo
void MenuPrincipal::dibujar(sf::RenderWindow& ventana) {
	ventana.draw(_titulo);
	for (int i = 0; i < _opciones.size(); i++) {
		if (i == _seleccion) {
			_opciones[i].setFillColor(sf::Color::Yellow);
		}
		else {
			_opciones[i].setFillColor(sf::Color::White);
		}
		ventana.draw(_opciones[i]);
	}
}