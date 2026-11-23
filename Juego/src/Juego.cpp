#include <iostream>  // Ctrl+K, Ctrl+D para ordenar el codigo
#include <string>
#include <vector>
#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <optional>
#include "ArchivoPartidas.h"
#include "Constantes.h"
#include "MenuPrincipal.h"
#include "EstadoJuego.h"
#include "Partida.h"
#include "Texturas.h"
#include "Sonidos.h"
#include "Musica.h"


// convierte segundos (con decimales) en texto con formato minutos:segundos
std::string ajustarTiempo(float segundos) {
	int totalSegundos = static_cast<int>(segundos); 
	int minutos = totalSegundos / 60;               
	int segundosRestantes = totalSegundos % 60;    

	std::string tiempo = std::to_string(minutos) + ":";
	if (segundosRestantes < 10) {
		tiempo += "0"; // para que quede "2:05" y no "2:5"
	}
	tiempo += std::to_string(segundosRestantes);
	return tiempo;
}

// lee todos los records del archivo, los ordena por tiempo 
// y devuelve el texto con los mejores MAX_RECORDS
std::string armarTextoRecords(ArchivoPartidas& registro) {
	// pasar los registros del archivo a un vector
	std::vector<RegistroPartida> lista;
	int cantidad = registro.contarRegistros();
	for (int i = 0; i < cantidad; i++) {
		lista.push_back(registro.leer(i));
	}

	// ordenarlos de menor a mayor tiempo
	for (int p = 0; p < static_cast<int>(lista.size()) - 1; p++) {
		for (int j = 0; j < static_cast<int>(lista.size()) - 1 - p; j++) {
			if (lista[j].getTiempo() > lista[j + 1].getTiempo()) {
				RegistroPartida aux = lista[j];
				lista[j] = lista[j + 1];
				lista[j + 1] = aux;
			}
		}
	}

	// armar el texto con los primeros
	std::string texto = "RECORDS\n\n";
	for (int i = 0; i < static_cast<int>(lista.size()) && i < MAX_RECORDS; i++) {
		texto += "Tiempo: " + ajustarTiempo(lista[i].getTiempo()) +
			" | Muertes: " + std::to_string(lista[i].getMuertes()) + "\n";
	}
	return texto;
}

// ajusta la camara para que el mundo entre en una ventana de ese tamaño
// sin deformarse: lo que sobra queda en negro (letterbox).
void ajustarVista(sf::View& vista, sf::Vector2u tamanioVentana) {
	// proporción = ancho / alto. Si son distintas, sobra espacio en algún lado.
	float proporcionVentana = static_cast<float>(tamanioVentana.x) / static_cast<float>(tamanioVentana.y);
	float proporcionMundo = ANCHO_MUNDO / ALTO_MUNDO;

	// el viewport es la parte de la ventana donde se dibuja el mundo, en fracciones (0 a 1).
	sf::FloatRect viewport({ 0.f, 0.f }, { 1.f, 1.f });

	if (proporcionVentana > proporcionMundo) {
		// sobra a los costados
		viewport.size.x = proporcionMundo / proporcionVentana;
		viewport.position.x = (1 - viewport.size.x) / 2;   // lo que sobra, repartido en dos franjas
	}
	else {
		// sobra arriba y abajo
		viewport.size.y = proporcionVentana / proporcionMundo;
		viewport.position.y = (1 - viewport.size.y) / 2;
	}
	vista.setViewport(viewport);	
}

int main() {
	sf::RenderWindow ventana(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Castlemania"); // resolucion y titulo en la ventana
	// si ponemos directamente {ANCHO_MUNDO, ALTO_MUNDO}, el compilador se queja porque 
	// pasar de float a entero puede perder decimales.
	// hay que convertirlos explicitamente con static_cast<unsigned int>(...)

	ventana.setFramerateLimit(FPS); //setea los frames

	// icono de la ventana y de la barra de tareas
	sf::Image icono;
	bool hayIcono = icono.loadFromFile("sprites/icono.png");
	if (hayIcono) {
		ventana.setIcon(icono);
	}

	// camara: muestra siempre el mundo entero (ANCHO_MUNDO x ALTO_MUNDO),
	// sin importar el tamaño de la ventana.
	sf::View vista(sf::FloatRect({ 0.f, 0.f }, { ANCHO_MUNDO, ALTO_MUNDO }));
	ventana.setView(vista); // todo lo que se dibuje pasa por esta cámara
	bool pantallaCompleta = false;

	// titulo y fuentes
	sf::Font fuente;
	if (!fuente.openFromFile("fuentes/PressStart2P-Regular.ttf")) {
		std::cout << "No se pudo cargar la fuente";
		return -1;
	}
	Sonidos sonidos; Texturas texturas; Musica musica;
	MenuPrincipal menuPrincipal(fuente);

	// instrucciones
	sf::Text instrucciones(fuente, "", 15);
	instrucciones.setString(
		"INSTRUCCIONES\n\n\n\n"
		"MOVIMIENTO\n\n\n"
		"- A / D o Flechas: Movimientos horizontales\n\n"
		"- W o Flecha arriba: Saltar\n\n\n"
		"ACCIONES\n\n\n"
		"- Click izquierdo: Lanzar / llamar\n\n\n"
		"OPCIONES\n\n\n"
		"- F11: Pantalla completa\n\n"
		"- ESC: Volver al menu\n\n\n\n"
	);
	instrucciones.setPosition(sf::Vector2f(70.f, 30.f));
	instrucciones.setFillColor(sf::Color::White);
	instrucciones.setOutlineColor(sf::Color::Black);
	instrucciones.setOutlineThickness(2.f);

	// creditos
	sf::Text creditos(fuente, "", 15);
	creditos.setString(
		"CREDITOS\n\n\n\n"
		"Integrantes:\n\n"
		"Juan Ignacio Crettaz\n\n"
		"Mateo Viviani\n\n\n\n\n\n"
		"Proyecto desarrollado para la materia\n"
		"Programacion II\n\n"
		"UTN - Facultad Regional General Pacheco\n\n"
		"2026\n\n\n\n"
		"Hecho con C++ y SFML 3\n\n"
		"Fuente: \"Press Start 2P\" (licencia OFL)\n\n\n\n"
		"Presiona ESC para volver al menu principal"
	);
	creditos.setPosition(sf::Vector2f(70.f, 30.f));

	sf::Text records(fuente, "", 15);
	records.setPosition(sf::Vector2f(100.f, 80.f));
	records.setOutlineColor(sf::Color::Black); // color del borde
	records.setOutlineThickness(2.f);	// grosor del borde, en pixeles

	sf::Text textoSala(fuente, "", 20);
	textoSala.setPosition(sf::Vector2f(28.f, 28.f));
	textoSala.setOutlineColor(sf::Color::Black);   
	textoSala.setOutlineThickness(2.f);            

	// ---- herramientas para armar salas (solo con MODO_DEBUG) ----
	// 
	// texto con las coordenadas del mouse en el mundo, al lado del cursor
	sf::Text textoMouse(fuente, "", 10);
	textoMouse.setOutlineColor(sf::Color::Black);
	textoMouse.setOutlineThickness(2.f);

	// grilla: una linea cada 20 px (tenue) y cada 100 px (mas marcada). Se arma una sola vez y se dibuja con G
	bool mostrarGrilla = false;
	sf::VertexArray grilla(sf::PrimitiveType::Lines);
	for (int x = 0; x <= static_cast<int>(ANCHO_MUNDO); x += 20) {
		sf::Color color = (x % 100 == 0) ? sf::Color(255, 255, 255, 110) : sf::Color(255, 255, 255, 35);
		grilla.append(sf::Vertex{ sf::Vector2f(static_cast<float>(x), 0.f), color });
		grilla.append(sf::Vertex{ sf::Vector2f(static_cast<float>(x), ALTO_MUNDO), color });
	}
	for (int y = 0; y <= static_cast<int>(ALTO_MUNDO); y += 20) {
		sf::Color color = (y % 100 == 0) ? sf::Color(255, 255, 255, 110) : sf::Color(255, 255, 255, 35);
		grilla.append(sf::Vertex{ sf::Vector2f(0.f, static_cast<float>(y)), color });
		grilla.append(sf::Vertex{ sf::Vector2f(ANCHO_MUNDO, static_cast<float>(y)), color });
	}
	// numeros de la grilla (cada 100), para no tener que contar lineas
	sf::Text numeroGrilla(fuente, "", 8);
	numeroGrilla.setOutlineColor(sf::Color::Black);
	numeroGrilla.setOutlineThickness(1.f);

	sf::Text victoria(fuente, "", 30);
	victoria.setPosition(sf::Vector2f(250.f, 200.f));
	victoria.setFillColor(sf::Color::Magenta);

	EstadoJuego estadoActual = EstadoJuego::Menu;

	Partida partida(sonidos, texturas); // el nivel, el jugador, el progreso de la partida en curso y sonidos
	ArchivoPartidas registro;

	// cronometro para medir cuánto dura cada frame.
	sf::Clock tiempo;

	float dt = 0.f;
	// dt (delta time): duración del frame anterior, en segundos (a 60 FPS, ~0.016).
	// todas las velocidades del juego estan en "píxeles por segundo" y se multiplican
	// por dt, así el juego va igual de rápido en una PC lenta o en una rápida.

	while (ventana.isOpen()) {  // isOpen metodo de la clase RenderWindow que devuelve true o false si la ventana (el juego) está abierta
		while (const std::optional evento = ventana.pollEvent()) {
			if (evento->is<sf::Event::Closed>()) {
				ventana.close();
			}
			// la ventana cambió de tamaño: recalcular dónde se dibuja el mundo
			// para que no se deforme (si sobra espacio, queda en negro).
			if (const auto* cambio = evento->getIf<sf::Event::Resized>()) {
				ajustarVista(vista, cambio->size);
				ventana.setView(vista);
			}

			if (const auto* clic = evento->getIf<sf::Event::MouseButtonPressed>()) {
				if (clic->button == sf::Mouse::Button::Left && estadoActual == EstadoJuego::Jugando) {
					// el mouse da pixeles de la ventana; se pasan a coordenadas del mundo (por la camara)
					sf::Vector2f destino = ventana.mapPixelToCoords(clic->position);
					partida.lanzarLanza(destino);
				}
			}

			// F11: alterna entre ventana y pantalla completa
			if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
				if (tecla->code == sf::Keyboard::Key::F11) {
					pantallaCompleta = !pantallaCompleta; // // alterna: si estaba en pantalla completa sale, y si no, entra

					if (pantallaCompleta) {
						// pantalla completa con la resolución del monitor
						ventana.create(sf::VideoMode::getDesktopMode(), "Castlemania", sf::State::Fullscreen);
					}
					else {
						// vuelve a la ventana normal, del tamaño del mundo
						ventana.create(sf::VideoMode({ static_cast<unsigned int>(ANCHO_MUNDO), static_cast<unsigned int>(ALTO_MUNDO) }), "Castlemania");
					}

					// la ventana nueva arranca de cero: hay que volver a configurarla
					ventana.setFramerateLimit(FPS);
					if (hayIcono) {
						ventana.setIcon(icono);
					}
					ajustarVista(vista, ventana.getSize());
					ventana.setView(vista);
				}
				
				// selecciona las opciones del menu
				if (estadoActual == EstadoJuego::Menu) {
					if (tecla->code == sf::Keyboard::Key::Up || tecla->code == sf::Keyboard::Key::W) {
						menuPrincipal.subir();
						sonidos.reproducir(TipoSonido::MenuMover);
					}
					if (tecla->code == sf::Keyboard::Key::Down || tecla->code == sf::Keyboard::Key::S) {
						menuPrincipal.bajar();
						sonidos.reproducir(TipoSonido::MenuMover);
					}

					if (tecla->code == sf::Keyboard::Key::Enter) { 
						if (menuPrincipal.opcionElegida() == OpcionMenu::Jugar) {
							estadoActual = EstadoJuego::Jugando;

							if (partida.termino()) {
								partida.reiniciarPartida();
							}
						}
						if (menuPrincipal.opcionElegida() == OpcionMenu::Instrucciones) {
							estadoActual = EstadoJuego::Instrucciones;
						}

						if (menuPrincipal.opcionElegida() == OpcionMenu::Records) {
							estadoActual = EstadoJuego::Records;
							records.setString(armarTextoRecords(registro));
						}

						if (menuPrincipal.opcionElegida() == OpcionMenu::Creditos) {
							estadoActual = EstadoJuego::Creditos;
						}
						if (menuPrincipal.opcionElegida() == OpcionMenu::Salir) {
							ventana.close();
						}
						sonidos.reproducir(TipoSonido::MenuSeleccionar);
					}
				}

				if (tecla->code == sf::Keyboard::Key::Escape) {
					estadoActual = EstadoJuego::Menu;
					sonidos.reproducir(TipoSonido::MenuVolver);
				}
				// herramientas para armar salas
				if (MODO_DEBUG && estadoActual == EstadoJuego::Jugando) {
					if (tecla->code == sf::Keyboard::Key::R) {
						partida.recargarSala(); // vuelve a cargar la sala 
					}
					if (tecla->code == sf::Keyboard::Key::G) {
						mostrarGrilla = !mostrarGrilla; // prende y apaga la grilla
					}
				}
			}
		}

		dt = tiempo.restart().asSeconds(); // cuanto paso desde el frame anterior (y reinicia el cronómetro)

		// se llama en cada frame: reproducir no hace nada si ya suena esa musica,
		// asi que alcanza con decir que musica corresponde a cada estado
		if (estadoActual == EstadoJuego::Jugando) { musica.reproducir(TipoMusica::Juego); }
		else if (estadoActual == EstadoJuego::Victoria) { musica.reproducir(TipoMusica::Victoria); }
		else { musica.reproducir(TipoMusica::Menu); }

		if (estadoActual == EstadoJuego::Jugando) {
			partida.actualizar(dt);

			if (partida.termino()) {
				RegistroPartida registroPartida(partida.getTiempoDeCompletado(),partida.getCantidadMuertes());
				registro.guardar(registroPartida);

				estadoActual = EstadoJuego::Victoria;
				victoria.setString("Gracias por jugar\n\nMuertes: " + std::to_string(partida.getCantidadMuertes()) + 
					"\n\nTiempo: " + ajustarTiempo(partida.getTiempoDeCompletado()));
			}
		}

		ventana.clear();			// "Limpiar" la ventana borrando el fotograma anterior
		switch (estadoActual) {
		case EstadoJuego::Menu:
			menuPrincipal.dibujar(ventana);
			break;
		case EstadoJuego::Instrucciones:
			menuPrincipal.dibujarFondo(ventana);
			ventana.draw(instrucciones);
			break;
		case EstadoJuego::Records:
			menuPrincipal.dibujarFondo(ventana);
			ventana.draw(records);
			break;
		case EstadoJuego::Creditos:
			ventana.draw(creditos);
			break;
		case EstadoJuego::Jugando:
			partida.dibujar(ventana);
			textoSala.setString("Sala " + std::to_string(partida.getNumeroNivel()));
			ventana.draw(textoSala);

			if (MODO_DEBUG) {
				if (mostrarGrilla) {
					ventana.draw(grilla);
					// los numeros de las lineas marcadas (cada 100): arriba los de x, a la izquierda los de y
					for (int x = 100; x < static_cast<int>(ANCHO_MUNDO); x += 100) {
						numeroGrilla.setString(std::to_string(x));
						numeroGrilla.setPosition(sf::Vector2f(x + 2.f, 4.f));
						ventana.draw(numeroGrilla);
					}
					for (int y = 100; y < static_cast<int>(ALTO_MUNDO); y += 100) {
						numeroGrilla.setString(std::to_string(y));
						numeroGrilla.setPosition(sf::Vector2f(4.f, y + 2.f));
						ventana.draw(numeroGrilla);
					}
				}
				// coordenadas del mouse en el mundo                     |>da la posición del mouse en píxeles de la ventana.
				sf::Vector2f mouse = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
				// mapPixelToCoords la convierte a coordenadas del mundo, pasando por la cámara.

				// arma el texto
				textoMouse.setString(std::to_string(static_cast<int>(mouse.x)) + ", " + std::to_string(static_cast<int>(mouse.y)));

				// lo ubica
				textoMouse.setPosition(sf::Vector2f(mouse.x + 14.f, mouse.y + 14.f)); // al lado de donde esta el mouse
				ventana.draw(textoMouse);
			}
			break;
		case EstadoJuego::Victoria:
			partida.dibujar(ventana);
			ventana.draw(victoria);

			break;
		}

		ventana.display();			// "Mostrar" en pantalla todo lo que se dibujó

	}

	return 0;
}