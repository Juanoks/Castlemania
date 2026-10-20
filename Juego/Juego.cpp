#include <iostream>
#include <SFML/graphics.hpp> /// SFML versión 3.1
#include <SFML/window.hpp>
#include <optional>
#include "Constantes.h"
#include "Nivel.h"
#include "Jugador.h"

int main() {
	sf::RenderWindow ventana(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Punto B"); // resolucion y titulo en la ventana
	//Si ponemos directamente {ANCHO_MUNDO, ALTO_MUNDO}, el compilador se queja porque 
	//pasar de float a entero puede perder decimales.
	//Hay que convertirlos explicitamente con static_cast<unsigned int>(...)

	ventana.setFramerateLimit(60); // setea los frames

	int numeroNivel = 1;
	int totalNiveles = 4;
	Nivel nivel(numeroNivel);

	Jugador personaje(nivel.getRespawn());


	sf::Clock tiempo;
	float dt=0.f; // cantidad por segundo real transcurrido


	while(ventana.isOpen()){  // isOpen metodo de la clase RenderWindow que devuelve true o false si la ventana (el juego) está abierta
		while (const std::optional evento = ventana.pollEvent()) {
			if (evento->is<sf::Event::Closed>()) {
				ventana.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
				ventana.close();
			}
		}
		
		dt = tiempo.restart().asSeconds(); // "reiniciá el reloj y devolveme cuánto pasó" (restart(), te da un sf::Time)
		// "ahora convertí ese resultado a segundos" (.asSeconds(), te da un float)
		// 
		//dt en SFML se refiere comúnmente al Delta Time (Tiempo Delta). Es el tiempo que transcurre entre el fotograma (frame) anterior 
		//y el fotograma actual en un videojuego.

		personaje.mover(dt);

		personaje.actualizar(dt);

		// ¿cayó sobre alguna plataforma?
		for (const Plataforma& p : nivel.getPlataformas()) { 
			personaje.aterrizarEn(p.getHitbox());
		}

		if (nivel.jugadorTocoPinche(personaje.getHitbox()) || nivel.jugadorCayoAlVacio(personaje.getHitbox())) {
			personaje.reiniciar(nivel.getRespawn());
		}

		if (nivel.jugadorTocoMeta(personaje.getHitbox())) {
			if (numeroNivel < totalNiveles) {
				numeroNivel++;
				nivel = Nivel(numeroNivel);
				personaje.reiniciar(nivel.getRespawn());
			}
			else {
				std::cout << "Ganaste!" << std::endl;
			}
			
		}

		ventana.clear();			// "Limpiar" la ventana borrando el fotograma anterior

		personaje.dibujar(ventana);	// "Dibujar" algo (recibe como argumento el objeto que querés dibujar, en este caso el rectángulo)
		nivel.dibujar(ventana);

		ventana.display();			// "Mostrar" en pantalla todo lo que se dibujó
			
	}

	return 0;
}