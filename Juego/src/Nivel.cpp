#include "Nivel.h"
#include "Constantes.h"
#include <string>
#include <iostream>

// Tamanio de la salida por defecto (un hueco angosto y alto, como la meta de antes)
sf::Vector2f salidaTamanio(30.f, 120.f);

// Mapa: cada fila es una sala y cada columna un lado de esa sala.
// izquierda, derecha, arriba, abajo | 0 = pared cerrada | -1 = fin del juego
// TERRENO_A_MANO (-2) = ese lado lo arma la sala a mano en cargarNivel (por ejemplo, con el hueco en otro lugar)
const int CONEXIONES[31][4] = {
	{0, 1, 0, TERRENO_A_MANO},  // 0: sala de prueba
	{0, TERRENO_A_MANO, 0, 0},  // 1: inicio.
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},  // 3: entra arriba a la izquierda (desde la 2), sale arriba a la derecha (a la 4)
	{TERRENO_A_MANO, 0, 0, TERRENO_A_MANO},  // 4: entra arriba a la izquierda (desde la 3)
	{0, 0, TERRENO_A_MANO, TERRENO_A_MANO},  // 5
	{0, TERRENO_A_MANO, TERRENO_A_MANO, 0},
	{TERRENO_A_MANO, 8, 0, 0},  
	{7, -1, 0, 9},  // 8   sala de la puerta
	{0, 10, 8, 0},    
	{9, 0, 0, TERRENO_A_MANO}   // 10
};

Nivel::Nivel(int numeroNivel, Sonidos& sonidos, const Texturas& texturas)
	: _numeroNivel(numeroNivel),
	_sonidos(sonidos),
	_texturas(texturas),
	_fondo(_texturas.get(TipoTextura::FondoGeneral)) {

	cargarNivel(numeroNivel);
}

void Nivel::actualizar(float dt) {
	for (Obstaculo* o : _obstaculos) {
		o->actualizar(dt);
	}
	for (Mecanismo* m : _mecanismos) {
		m->actualizar(dt);
	}
}

void Nivel::cargarNivel(int numeroNivel) {
	// Si solo se hace clear(), los objetos creados con new quedan en memoria
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; } 

	_obstaculos.clear(); _plataformas.clear(); _bloques.clear(); _salidas.clear(); _coleccionables.clear(); _botones.clear();
	_mecanismos.clear(); _puertas.clear(); _cristales.clear();

	_numeroNivel = numeroNivel;
	_respawn = sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR); // parado sobre el piso

	TipoTextura tipoFondo = TipoTextura::FondoGeneral;
	// casi todas las salas usan el fondo general; algunas zonas especiales tienen el suyo
	if (numeroNivel == 8) {
		tipoFondo = TipoTextura::FondoPuerta;
	}
	else if (numeroNivel == 30) {
		tipoFondo = TipoTextura::FondoFinal;
	}
	// el true ajusta el recorte del sprite al tamanio de la imagen nueva
	_fondo.setTexture(_texturas.get(tipoFondo), true);
	sf::Vector2u tamanio = _texturas.get(tipoFondo).getSize();
	// setScale no recibe un tamanio sino un multiplicador: lo que quiero que mida / lo que mide la imagen.
	_fondo.setScale(sf::Vector2f(ANCHO_MUNDO / tamanio.x, ALTO_MUNDO / tamanio.y));


	paredIzquierda(CONEXIONES[numeroNivel][0]);
	paredDerecha(CONEXIONES[numeroNivel][1]);
	techo(CONEXIONES[numeroNivel][2]);
	piso(CONEXIONES[numeroNivel][3]);
	
	//                            posicion (x,y), tamanio (ancho,alto)
	if (numeroNivel == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, ALTO_MUNDO-20.f), sf::Vector2f(400.f, 30.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(500.f, ALTO_MUNDO - 20.f), sf::Vector2f(400.f, 30.f), _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(300.f, 480.f), TipoColeccionable::LlaveAzul, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(200.f, 480.f), TipoColeccionable::LlaveAmarilla, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(500.f, 280.f), TipoColeccionable::Lanza, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(100.f, 480.f), TipoColeccionable::DobleSalto, _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 400.f), sf::Vector2f(200.f, 40.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(800.f, 300.f), sf::Vector2f(20.f, 100.f), _texturas, true));
		_obstaculos.push_back(new Pinche(sf::Vector2f(150.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(600.f, Y_PISO - 80.f), sf::Vector2f(40.f, 80.f), 600.f, 800.f, 150.f, _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 300.f), sf::Vector2f(100.f, 20.f), _texturas));
		_botones.push_back(Boton(sf::Vector2f(500.f, 200.f),1,_texturas));
		_mecanismos.push_back(new Compuerta(sf::Vector2f(500.f, 320.f), sf::Vector2f(20.f, 80.f), 1, _texturas));

		
		_cristales.push_back(Cristal(sf::Vector2f(40.f, Y_PISO - 44.f), 2, _texturas));   
		_cristales.push_back(Cristal(sf::Vector2f(230.f, 300.f - 44.f), 2, _texturas));   
		_mecanismos.push_back(new Barrera(sf::Vector2f(870.f, Y_PISO - 80.f), sf::Vector2f(20.f, 80.f), 2, _texturas));
	}


	if (numeroNivel == 1) {
		// sala 1 (inicio): una escalera de tres escalones de 80 (el salto llega a 90) 
		paredDerecha(2, 120.f); 
		
		_bloques.push_back(Bloque(sf::Vector2f(320.f, 440.f), sf::Vector2f(620.f, 80.f), _texturas)); 
		_bloques.push_back(Bloque(sf::Vector2f(520.f, 360.f), sf::Vector2f(420.f, 80.f), _texturas)); 
		_bloques.push_back(Bloque(sf::Vector2f(660.f, 280.f), sf::Vector2f(280.f, 80.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(760.f, 200.f), sf::Vector2f(280.f, 80.f), _texturas));
	}
	else if (numeroNivel == 2) {  //           posicion (x,y), tamanio (ancho,alto)
	
		paredIzquierda(1, 120.f); 

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 200.f), sf::Vector2f(340.f, 340.f), _texturas)); // entrada 
		_obstaculos.push_back(new Pinche(sf::Vector2f(160.f, 200.f-37.5f), _texturas));

		_obstaculos.push_back(new Pinche(sf::Vector2f(360.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(440.f, Y_PISO - 37.5f), _texturas));  // pinches caida
		_obstaculos.push_back(new Pinche(sf::Vector2f(480.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(520.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(560.f, Y_PISO - 37.5f), _texturas));

		_obstaculos.push_back(new Pinche(sf::Vector2f(760.f, 200.f - 37.5f), _texturas)); // piso salida
		_bloques.push_back(Bloque(sf::Vector2f(600.f, 200.f), sf::Vector2f(340.f, 340.f), _texturas)); 	

		paredDerecha(3, 120.f);
	}
	else if (numeroNivel == 3) {
		paredIzquierda(2, 120.f); 
		paredDerecha(4, 130.f, Y_PISO - TAMANIO_HUECO);

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 200.f), sf::Vector2f(110.f, 320.f), _texturas));  // entrada 
		_bloques.push_back(Bloque(sf::Vector2f(250.f, 100.f), sf::Vector2f(60.f, 240.f), _texturas));  // pilar 
		_bloques.push_back(Bloque(sf::Vector2f(690.f, 460.f), sf::Vector2f(250.f, 60.f), _texturas));  // escalon de abajo a la derecha
		_bloques.push_back(Bloque(sf::Vector2f(860.f, 390.f), sf::Vector2f(80.f, 70.f), _texturas));   // escalon de arriba a la derecha
		_bloques.push_back(Bloque(sf::Vector2f(290.f, 330.f), sf::Vector2f(440.f, 40.f), _texturas));  // estante del medio
		_bloques.push_back(Bloque(sf::Vector2f(310.f, 270.f), sf::Vector2f(100.f, 60.f), _texturas));  // escaloncito 
		_bloques.push_back(Bloque(sf::Vector2f(520.f, 210.f), sf::Vector2f(420.f, 30.f), _texturas));  // piso de la salida
		
		// pinches del piso
		_obstaculos.push_back(new Pinche(sf::Vector2f(330.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(520.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(564.f, Y_PISO - 37.5f), _texturas));
		
		// pinches antes de la salida
		_obstaculos.push_back(new Pinche(sf::Vector2f(620.f, 210.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(664.f, 210.f - 37.5f), _texturas));
	}
	else if (numeroNivel == 4) {
		
		paredIzquierda(3, Y_PISO - TAMANIO_HUECO, 130.f);
		techo(0);                 
		piso(5, 850.f);  

		_bloques.push_back(Bloque(sf::Vector2f(360.f, 150.f), sf::Vector2f(580.f, 30.f), _texturas));


		_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 280.f, 760.f, 140.f, _texturas));
	}
	else if (numeroNivel == 5) {
		techo(4, 850.f);

		_bloques.push_back(Bloque(sf::Vector2f(170.f, 170.f), sf::Vector2f(800.f, 20.f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, 170.f-56.f), sf::Vector2f(40.f, 56.f), 250.f, 760.f, 140.f, _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(20.f, 335.f), sf::Vector2f(800.f, 20.f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, 335.f - 56.f), sf::Vector2f(40.f, 56.f), 250.f, 760.f, 140.f, _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(220.f, 335.f -37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(420.f, 335.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(620.f, 335.f - 37.5f), _texturas));
		      
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(200.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 200.f, 800.f, 140.f, _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(800.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 200.f, 800.f, 140.f, _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(220.f, Y_PISO - 37.5f), _texturas));

		piso(6, 20.f);
	}
	else if (numeroNivel == 6) {
		techo(5, 20.f);
		
		_bloques.push_back(Bloque(sf::Vector2f(100.f, 20.f), sf::Vector2f(20.f, 200.f), _texturas));

		_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO - 240.f) / 2, Y_PISO - 40.f), sf::Vector2f(240.f, 40.f), _texturas));
		_bloques.back().setColor(sf::Color(255, 210, 120));
		_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO - 80.f) / 2, Y_PISO - 40.f - 40.f), sf::Vector2f(80.f, 40.f), _texturas));
		_bloques.back().setColor(sf::Color(255, 210, 120));
		_coleccionables.push_back(Coleccionable(sf::Vector2f((ANCHO_MUNDO - 60.f) / 2, 420.f - 40.f), TipoColeccionable::Lanza, _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(880.f,380.f), sf::Vector2f(240.f, 140.f), _texturas,true));
		paredDerecha(7, 300);
	}
	else if (numeroNivel == 7) { 
		paredIzquierda(6, 300.f);
		
		_bloques.push_back(Bloque(sf::Vector2f(20.f, 380.f), sf::Vector2f(80.f, 200.f), _texturas)); // entrada
		_bloques.push_back(Bloque(sf::Vector2f(200.f, 100.f), sf::Vector2f(30.f, 417.f), _texturas, true));
		_bloques.push_back(Bloque(sf::Vector2f(20.f, 230.f), sf::Vector2f(80.f, 30.f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(100.f, Y_PISO - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(150.f, Y_PISO - 37.5f), _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(230.f, 160.f), sf::Vector2f(600.f, 30.f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(300.f, 160.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(400.f, GROSOR_PARED), _texturas, 180.f));
		_obstaculos.push_back(new Pinche(sf::Vector2f(500.f, 160.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(600.f, GROSOR_PARED), _texturas, 180.f));
		_obstaculos.push_back(new Pinche(sf::Vector2f(700.f, 160.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(830.f, 155.f), _texturas, 90.f));

		_bloques.push_back(Bloque(sf::Vector2f(380.f, 390.f), sf::Vector2f(700.f, 30.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(730.f, 280.f), sf::Vector2f(30.f, 110.f), _texturas,true));
		_obstaculos.push_back(new Pinche(sf::Vector2f(805.f, 390.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(760.f, 390.f-37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(685.f, 390.f - 37.5f), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(640.f, 390.f - 37.5f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(350.f, 390.f), sf::Vector2f(30.f, 30.f), _texturas, true));
		_bloques.push_back(Bloque(sf::Vector2f(320.f, 280.f), sf::Vector2f(30.f, 140.f), _texturas));

	}
	else if (numeroNivel == 8) {
		_puertas.push_back(PuertaFinal(sf::Vector2f(ANCHO_MUNDO - 63.f * ESCALA_PUERTA, Y_PISO - 96.f * ESCALA_PUERTA), _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(300.f, 480.f), TipoColeccionable::LlaveAzul, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(200.f, 480.f), TipoColeccionable::LlaveAmarilla, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(400.f, 480.f), TipoColeccionable::LlaveVerde, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(500.f, 480.f), TipoColeccionable::LlaveRoja, _texturas));
	}
	else if (numeroNivel == 9) {	
		

		_bloques.push_back(Bloque(sf::Vector2f(380.f, 130.f), sf::Vector2f(180.f, 100.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(20.f, 250.f), sf::Vector2f(220.f, 270.f), _texturas));       
		_bloques.push_back(Bloque(sf::Vector2f(240.f, 250.f), sf::Vector2f(40.f, 150.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(400.f, 390.f), sf::Vector2f(200.f, 130.f), _texturas, true));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(700.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 660.f, 820.f, 130.f, _texturas)); 
	}
	else if (numeroNivel == 10) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(560.f, GROSOR_PARED), _texturas)); 

		_plataformas.push_back(Plataforma(sf::Vector2f(160.f, 450.f), sf::Vector2f(90.f, 10.f), _texturas));
		_plataformas.push_back(Plataforma(sf::Vector2f(300.f, 380.f), sf::Vector2f(90.f, 10.f), _texturas));
		_obstaculos.push_back(new PilarMovil(sf::Vector2f(140.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 140.f, 330.f, 120.f, _texturas));

		_bloques.push_back(Bloque(sf::Vector2f(400.f, 240.f), sf::Vector2f(40.f, 160.f), _texturas, true)); 
		_bloques.push_back(Bloque(sf::Vector2f(440.f, 240.f), sf::Vector2f(120.f, 300.f), _texturas));     
		_botones.push_back(Boton(sf::Vector2f(GROSOR_PARED, 120.f), 1, _texturas));                         
		_plataformas.push_back(Plataforma(sf::Vector2f(620.f, 200.f), sf::Vector2f(80.f, 10.f), _texturas));  
		_bloques.push_back(Bloque(sf::Vector2f(760.f, 170.f), sf::Vector2f(180.f, 30.f), _texturas));         
		_bloques.push_back(Bloque(sf::Vector2f(800.f, 20.f), sf::Vector2f(20.f, 70.f), _texturas));           
		_mecanismos.push_back(new Compuerta(sf::Vector2f(800.f, 90.f), sf::Vector2f(20.f, 80.f), 1, _texturas));
		_coleccionables.push_back(Coleccionable(sf::Vector2f(870.f, 134.f), TipoColeccionable::LlaveAzul, _texturas));
	}
	//proximos niveles

}

void Nivel::dibujar(sf::RenderWindow& ventana, const bool llaves[]) {
	ventana.draw(_fondo);

	for (Obstaculo* o : _obstaculos) {
		o->dibujar(ventana);
	}
	for (Mecanismo* m : _mecanismos) {
		m->dibujar(ventana);
	}
	for (Plataforma& p : _plataformas) {
		p.dibujar(ventana);
	}
	for (Bloque& b : _bloques) {
		b.dibujar(ventana);
	}
	for (Salida& s : _salidas) {
		s.dibujar(ventana);
	}
	for (Coleccionable& c : _coleccionables) {
		c.dibujar(ventana);
	}
	for (Boton& b : _botones) {
		b.dibujar(ventana);
	}
	for (PuertaFinal& p : _puertas) {
		p.dibujar(ventana, llaves);
	}
	for (Cristal& c : _cristales) {
		c.dibujar(ventana);
	}

}

bool Nivel::jugadorTocoObstaculo(sf::FloatRect hitboxJugador) const {
	for (Obstaculo* o : _obstaculos) { // pregunta a cada obstaculo si toca al jugador; cada uno usa su propia hitbox
		if (o->tocaAlJugador(hitboxJugador)) {
			return true;
		}
	}
	return false;
}

bool Nivel::jugadorCayoAlVacio(sf::FloatRect hitboxJugador) const {
	return hitboxJugador.position.y > ALTO_MUNDO; // si la posicion (y) del jugador es mayor a la del borde de abajo de la 
}											      // pantalla; devuelve true

// Devuelve un puntero a la salida que toca el jugador, o nullptr si no toca ninguna (igual que fopen).
const Salida* Nivel::salidaTocada(sf::FloatRect hitboxJugador) const {
	for (const Salida& s : _salidas) {
		if (hitboxJugador.findIntersection(s.getHitbox())) {
			return &s; // & = la direccion de esta salida (un puntero a ella)
		}
	}
	return nullptr;
}

const Coleccionable* Nivel::coleccionableTocado(sf::FloatRect hitboxJugador) const {
	for (const Coleccionable& c : _coleccionables) {
		if (hitboxJugador.findIntersection(c.getHitbox())) {
			return &c; // retorna &c = la direccion de este coleccionable (un puntero a el)
		}
	}
	return nullptr;
}

void Nivel::quitarColeccionable(TipoColeccionable tipo) { 
	// Saca de la sala el coleccionable de ese tipo (cada uno es unico en todo el mapa).
	for (int i = 0; i < static_cast<int>(_coleccionables.size()); i++) {
		if (_coleccionables[i].getTipo() == tipo) {
			_coleccionables.erase(_coleccionables.begin() + i);
			return;
		}
	}
}

const std::vector<Plataforma>& Nivel::getPlataformas() const {
	return _plataformas;
}

const std::vector<Bloque>& Nivel::getBloques() const {
	return _bloques;
}

const std::vector<Boton>& Nivel::getBotones() const {
	return _botones;
}

const std::vector<PuertaFinal>& Nivel::getPuertas() const
{
	return _puertas;
}

void Nivel::actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas) {
	// un boton esta activo si hay una lanza clavada tocandolo.
	// se recalcula en cada frame: si la lanza vuelve al jugador, el boton se apaga solo.
	for (Boton& b : _botones) {
		bool tocado = false; // se empieza suponiendo que ninguna lanza lo toca
		for (const Lanza& l : lanzas) {
			if (l.estaClavada() && l.getHitbox().findIntersection(b.getHitbox())) { // solo cuenta una lanza clavada 
				tocado = true;
			}
		}
		b.setActivo(tocado); // el sonido lo hace el mecanismo (compuerta) al cambiar: si sonaran los dos, se pisan
	}

	for (Cristal& c : _cristales) {
		if (c.estaRoto()) {
			continue;
		}
		for (const Lanza& l : lanzas) {
			// no clavada = volando o volviendo: la vuelta tambien rompe cristales
			if (!l.estaClavada() && l.getHitbox().findIntersection(c.getHitbox())) {
				c.romper();
				// ¿queda algun cristal entero de este canal? si no queda ninguno, este era el ultimo
				bool quedaAlguno = false;
				for (const Cristal& otro : _cristales) {
					if (otro.getCanal() == c.getCanal() && !otro.estaRoto()) {
						quedaAlguno = true;
					}
				}
				_sonidos.reproducir(quedaAlguno ? TipoSonido::CristalRomper : TipoSonido::BarreraApagar);
			}
		}
	}

	// un mecanismo se activa si hay un boton activo de su canal, o si su canal tiene cristales y estan todos rotos.
	for (Mecanismo* m : _mecanismos) {
		
		bool hayBotonActivo = false; 
		bool hayCristales = false;  
		bool todosRotos = true;      

		for (Boton& b : _botones) {
			if (b.estaActivo() && b.getCanal() == m->getCanal()) {
				hayBotonActivo = true;
			}
		}

		for (Cristal& c : _cristales) {
			if (c.getCanal() == m->getCanal()) {
				hayCristales = true;
				if (!c.estaRoto()) {
					todosRotos = false;
				}
			}
			
		}
		// hayCristales: si el canal no tiene cristales, todosRotos queda en true; sin esto se abririan las compuertas de botones

		bool activo = hayBotonActivo || (hayCristales && todosRotos);
		// sonido solo cuando CAMBIA de estado, y solo para los de botones:
		if (activo != m->estaActivo() && !hayCristales) {
			_sonidos.reproducir(activo ? TipoSonido::CompuertaAbrir : TipoSonido::CompuertaCerrar);
		}
		m->setActivo(activo);
	}
}

void Nivel::actualizarPuertas(sf::FloatRect hitboxJugador, bool tieneLasLlaves) {
	for (PuertaFinal& p : _puertas) {
		if (p.estaAbierta()) { continue; }

		// la puerta es solida: el jugador nunca llega a pisarse con ella. Por eso "cerca" se mide con una zona
		// que empieza 40 px antes de la puerta
		sf::FloatRect zona = p.getHitbox();
		zona.position.x -= 40.f;
		zona.size.x += 40.f;
		bool cerca = hitboxJugador.findIntersection(zona).has_value();

		if (cerca && tieneLasLlaves) {
			p.abrir();
			_sonidos.reproducir(TipoSonido::PuertaAbrir);
		}
		else if (cerca && !tieneLasLlaves && !p.jugadorEstabaCerca()) {
			_sonidos.reproducir(TipoSonido::PuertaCerrada);
		}
		p.setJugadorCerca(cerca);
	}

}

// Partida lo llama al entrar a una sala: se reaparece donde se entro
void Nivel::setRespawn(sf::Vector2f posicion) {
	_respawn = posicion;
}

sf::Vector2f Nivel::getRespawn() const {
	return _respawn;
}

const std::vector<Mecanismo*>& Nivel::getMecanismos() const {return _mecanismos;}

Nivel::~Nivel() {
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; }
}

// pared derecha de la sala. destino 0 = pared cerrada.
void Nivel::paredDerecha(int destino, float inicioHueco, float huecoDestino) {

	if (huecoDestino < 0) { huecoDestino = inicioHueco; }
	float finDestino = huecoDestino + TAMANIO_HUECO;

	float posicionX = ANCHO_MUNDO - GROSOR_PARED; // la pared va pegada al borde derecho del mundo
	float finHueco = inicioHueco + TAMANIO_HUECO; // donde termina el hueco (y empieza el bloque de abajo)
	if (destino == TERRENO_A_MANO) {return;}
	if (destino == 0) {
		// sin salida: un solo bloque de arriba a abajo
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO), _texturas));
	}
	else {
		// bloque de arriba del hueco (de y=0 hasta inicioHueco)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco), _texturas));
		// bloque de abajo del hueco (de finHueco hasta el fondo)
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco), _texturas));
		// la salida ocupa el hueco
		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), // donde está el hueco
			sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), // tamaño del hueco
			destino, // a que sala lleva
			sf::Vector2f(GROSOR_PARED + 10.f, finDestino - ALTO_JUGADOR))); // donde aparece el jugador en esa sala
	}
}

void Nivel::paredIzquierda(int destino, float inicioHueco, float huecoDestino) {

	if (huecoDestino < 0) { huecoDestino = inicioHueco; }
	float finDestino = huecoDestino + TAMANIO_HUECO;

	float posicionX = 0.f;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, 0.f), sf::Vector2f(GROSOR_PARED, inicioHueco), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(posicionX, finHueco), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - finHueco), _texturas));

		_salidas.push_back(Salida
		(sf::Vector2f(posicionX, inicioHueco), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO),
			destino, sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f, finDestino - ALTO_JUGADOR)));
	}
}

void Nivel::techo(int destino, float inicioHueco, float huecoDestino) {

	if (huecoDestino < 0) { huecoDestino = inicioHueco; }
	float finDestino = huecoDestino + TAMANIO_HUECO;

	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(ANCHO_MUNDO, GROSOR_PARED), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(inicioHueco, GROSOR_PARED), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, 0.f), sf::Vector2f(ANCHO_MUNDO - finHueco, GROSOR_PARED), _texturas));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, 0.f),
			sf::Vector2f(TAMANIO_HUECO, GROSOR_PARED), destino, sf::Vector2f(finDestino + 10.f, Y_PISO - ALTO_JUGADOR)));
	}
}

void Nivel::piso(int destino, float inicioHueco, float huecoDestino) {

	if (huecoDestino < 0) { huecoDestino = inicioHueco; }
	float finDestino = huecoDestino + TAMANIO_HUECO;

	float posicionY = Y_PISO;
	float alto = ALTO_MUNDO - posicionY;
	float finHueco = inicioHueco + TAMANIO_HUECO;
	if (destino == TERRENO_A_MANO) { return; }
	if (destino == 0) {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(ANCHO_MUNDO, alto), _texturas));
	}
	else {
		_bloques.push_back(Bloque(sf::Vector2f(0.f, posicionY), sf::Vector2f(inicioHueco, alto), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(finHueco, posicionY), sf::Vector2f(ANCHO_MUNDO - finHueco, alto), _texturas));

		_salidas.push_back(Salida(sf::Vector2f(inicioHueco, posicionY),
			sf::Vector2f(TAMANIO_HUECO, alto), destino,
			sf::Vector2f(huecoDestino + (TAMANIO_HUECO - ANCHO_JUGADOR) / 2, GROSOR_PARED + 5.f)));
	}
}