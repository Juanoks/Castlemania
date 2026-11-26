#include "Nivel.h"
#include "Constantes.h"

// Mapa: cada fila es una sala y cada columna un lado de esa sala.
// izquierda, derecha, arriba, abajo | 0 = pared cerrada | -1 = fin del juego
// TERRENO_A_MANO (-2) = ese lado lo arma la sala a mano en cargarNivel (por ejemplo, con el hueco en otro lugar)
const int CONEXIONES[31][4] = {
	//fila=salas | columnas=lados
	{0, 1, 0, TERRENO_A_MANO},  // 0: sala de prueba
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},  // 1: inicio.
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},  // 2
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},  // 3: 
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, TERRENO_A_MANO},  // 4: 
	{0, 0, TERRENO_A_MANO, TERRENO_A_MANO},  // 5
	{0, TERRENO_A_MANO, TERRENO_A_MANO, 0}, // 6
	{TERRENO_A_MANO, 8, 0, 0},  // 7
	{7, 29, TERRENO_A_MANO, 9},  // 8   sala de la puerta 
	{TERRENO_A_MANO, 10, TERRENO_A_MANO, TERRENO_A_MANO},   // 9: centro del mapa 
	{9, 11, 0, 0},   // 10
	{10, TERRENO_A_MANO, 0, TERRENO_A_MANO},   // 11
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, TERRENO_A_MANO},   // 12
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},   // 13
	{TERRENO_A_MANO, 0, 0, TERRENO_A_MANO},   // 14: llave azul
	{TERRENO_A_MANO, TERRENO_A_MANO, TERRENO_A_MANO, TERRENO_A_MANO},   // 15
	{TERRENO_A_MANO, 0, 0, 0},   // 16
	{0, TERRENO_A_MANO, 0, 0},   // 17: laberinto
	{0, 0, TERRENO_A_MANO, TERRENO_A_MANO},   // 18: llave amarilla
	{20, TERRENO_A_MANO, 0, 0},   // 19: camino verde
	{TERRENO_A_MANO, 19, 0, 0},   // 20:
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},   // 21: 
	{0, TERRENO_A_MANO, 0, TERRENO_A_MANO},   // 22: llave verde
	{0, TERRENO_A_MANO, 0, TERRENO_A_MANO},   // 23: camino rojo
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},   // 24
	{TERRENO_A_MANO, 0, TERRENO_A_MANO, 0},   // 25
	{TERRENO_A_MANO, 0, 0, TERRENO_A_MANO},   // 26
	{TERRENO_A_MANO, TERRENO_A_MANO, 0, 0},   // 27
	{0, TERRENO_A_MANO, 0, TERRENO_A_MANO},   // 28: sala de la llave roja
	{8, 30, 0, 0},   // 29
	{TERRENO_A_MANO, TERRENO_A_MANO, TERRENO_A_MANO, TERRENO_A_MANO}   // 30: exterior, la salida de la derecha termina el juego
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
}

void Nivel::cargarNivel(int numeroNivel) {
	// Si solo se hace clear(), los objetos creados con new quedan en memoria
	for (Obstaculo* o : _obstaculos) { delete o; }
	for (Mecanismo* m : _mecanismos) { delete m; } 

	_obstaculos.clear(); _plataformas.clear(); _bloques.clear(); _salidas.clear(); _coleccionables.clear(); _botones.clear();
	_mecanismos.clear(); _puertas.clear(); _cristales.clear(); _decoraciones.clear();

	_numeroNivel = numeroNivel;
	_respawn = sf::Vector2f(50.f, Y_PISO - ALTO_JUGADOR); // parado sobre el piso

	TipoTextura tipoFondo = TipoTextura::FondoGeneral;
	_fondo.setColor(sf::Color::White); // sin tinte: cada sala arranca con el fondo normal
	// casi todas las salas usan el fondo general; algunas zonas especiales tienen el suyo

	if		(numeroNivel == 6) {
		_fondo.setColor(sf::Color(48, 58, 90)); // azul oscuro
	}
	else if (numeroNivel == 8) {
		tipoFondo = TipoTextura::FondoPuerta;
	}
	else if (numeroNivel == 14) {
		_fondo.setColor(sf::Color(150, 190, 255)); // azul
	}
	else if (numeroNivel == 18) {
		_fondo.setColor(sf::Color(255, 240, 190));  // amarillo
	}
	else if (numeroNivel == 22) {
		_fondo.setColor(sf::Color(170, 235, 180)); // verde
	}
	else if (numeroNivel == 28) {
		_fondo.setColor(sf::Color(255, 185, 175)); // rojo
	}
	else if (numeroNivel == 29) {
		_fondo.setColor(sf::Color(0, 0, 0));
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
	
	switch (numeroNivel) {
	case 0: cargarSala0(); break;
	case 1: cargarSala1(); break;
	case 2: cargarSala2(); break;
	case 3: cargarSala3(); break;
	case 4: cargarSala4(); break;
	case 5: cargarSala5(); break;
	case 6: cargarSala6(); break;
	case 7: cargarSala7(); break;
	case 8: cargarSala8(); break;
	case 9: cargarSala9(); break;
	case 10: cargarSala10(); break;
	case 11: cargarSala11(); break;
	case 12: cargarSala12(); break;
	case 13: cargarSala13(); break;
	case 14: cargarSala14(); break;
	case 15: cargarSala15(); break;
	case 16: cargarSala16(); break;
	case 17: cargarSala17(); break;
	case 18: cargarSala18(); break;
	case 19: cargarSala19(); break;
	case 20: cargarSala20(); break;
	case 21: cargarSala21(); break;
	case 22: cargarSala22(); break;
	case 23: cargarSala23(); break;
	case 24: cargarSala24(); break;
	case 25: cargarSala25(); break;
	case 26: cargarSala26(); break;
	case 27: cargarSala27(); break;
	case 28: cargarSala28(); break;
	case 29: cargarSala29(); break;
	case 30: cargarSala30(); break;
	}

	// la sala de la de la lanza y puerta final tiene los bloques teñidos
	for (Bloque& b : _bloques) {
		if (numeroNivel == 6) { b.setColor(b.esMadera() ? sf::Color(255, 205, 100) : sf::Color(170, 225, 245)); } // piedra celeste | madera dorada
		if (numeroNivel == 8) { b.setColor(sf::Color(150, 120, 180)); }
		if (numeroNivel == 14) { b.setColor(sf::Color(160, 185, 230)); } // azul
		if (numeroNivel == 18) { b.setColor(sf::Color(240, 215, 140)); } // amarilla
		if (numeroNivel == 22) { b.setColor(sf::Color(165, 225, 175)); } // verde
		if (numeroNivel == 28) { b.setColor(sf::Color(235, 155, 145)); } // roja
	}

}

void Nivel::cargarSala0() {
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

void Nivel::cargarSala1() {
	// sala 1 (inicio)
	paredDerecha(2, 120.f); 

	_decoraciones.push_back(Decoracion(sf::Vector2f(80.f, 380.f), TipoDecoracion::CartelMover, _texturas));
	_decoraciones.push_back(Decoracion(sf::Vector2f(320.f, 310.f), TipoDecoracion::CartelSaltar, _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(GROSOR_PARED, 80.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 80.f + TAMANIO_HUECO), sf::Vector2f(GROSOR_PARED, 330.f - 80.f - TAMANIO_HUECO), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 330.f), sf::Vector2f(GROSOR_PARED + 40.f, 40.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 370.f), sf::Vector2f(GROSOR_PARED, ALTO_MUNDO - 370.f), _texturas));

	_salidas.push_back(Salida(sf::Vector2f(0.f, 80.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 28,
		sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f, 240.f - ALTO_JUGADOR)));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 50.f), sf::Vector2f(80.f, 30.f), _texturas));
	
	_bloques.push_back(Bloque(sf::Vector2f(320.f, 440.f), sf::Vector2f(620.f, 80.f), _texturas)); 
	_bloques.push_back(Bloque(sf::Vector2f(520.f, 360.f), sf::Vector2f(420.f, 80.f), _texturas)); 
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 280.f), sf::Vector2f(280.f, 80.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(760.f, 200.f), sf::Vector2f(280.f, 80.f), _texturas));
}

void Nivel::cargarSala2() {
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

void Nivel::cargarSala3() {
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

void Nivel::cargarSala4() {
	paredIzquierda(3, Y_PISO - TAMANIO_HUECO, 130.f);
	techo(0);                 
	piso(5, 850.f);  
	paredDerecha(27, 70.f, 220.f); // vuelta del camino rojo: desde la 27 se cae arriba del estante

	_bloques.push_back(Bloque(sf::Vector2f(360.f, 150.f), sf::Vector2f(580.f, 30.f), _texturas));


	_obstaculos.push_back(new PilarMovil(sf::Vector2f(380.f, Y_PISO - 56.f), sf::Vector2f(40.f, 56.f), 280.f, 760.f, 140.f, _texturas));
}

void Nivel::cargarSala5() {
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

void Nivel::cargarSala6() {
	techo(5, 20.f);
	
	_decoraciones.push_back(Decoracion(sf::Vector2f(700.f, 400.f), TipoDecoracion::CartelLanza, _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(100.f, 20.f), sf::Vector2f(20.f, 200.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(ANCHO_MUNDO - 120.f, 20.f), sf::Vector2f(20.f, 200.f), _texturas)); 

	// pedestal 
	_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO - 240.f) / 2, Y_PISO - 40.f), sf::Vector2f(240.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO - 80.f) / 2, Y_PISO - 80.f), sf::Vector2f(80.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(300.f, Y_PISO - 20.f), sf::Vector2f(60.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(600.f, Y_PISO - 20.f), sf::Vector2f(60.f, 20.f), _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(400.f, 230.f), sf::Vector2f(160.f, 16.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(412.f, GROSOR_PARED), sf::Vector2f(8.f, 210.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(540.f, GROSOR_PARED), sf::Vector2f(8.f, 210.f), _texturas));
	
	_bloques.push_back(Bloque(sf::Vector2f(250.f, GROSOR_PARED), sf::Vector2f(30.f, 120.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(680.f, GROSOR_PARED), sf::Vector2f(30.f, 120.f), _texturas));
	_coleccionables.push_back(Coleccionable(sf::Vector2f(450.f, 370.f), TipoColeccionable::Lanza, _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(880.f,380.f), sf::Vector2f(240.f, 140.f), _texturas,true));
	paredDerecha(7, 300);
}

void Nivel::cargarSala7() {
	paredIzquierda(6, 300.f);

	_decoraciones.push_back(Decoracion(sf::Vector2f(370.f, 260.f), TipoDecoracion::CartelLanzaPiso, _texturas));
	
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

void Nivel::cargarSala8() {
	_puertas.push_back(PuertaFinal(sf::Vector2f(ANCHO_MUNDO - 60.f * ESCALA_PUERTA, Y_PISO - 96.f * ESCALA_PUERTA), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(843.f, 310.f), sf::Vector2f(100.f, 40.f), _texturas));
	_bloques.back().setColor(sf::Color(150, 120, 180));

	_plataformas.push_back(Plataforma(sf::Vector2f(20.f, 380.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(20.f, 230.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(20.f, 100.f), sf::Vector2f(200.f, 10.f), _texturas));

	techo(23, 140.f, GROSOR_PARED); // en la 23 se aparece en la esquina de abajo a la izquierda
}

void Nivel::cargarSala9() {
	paredIzquierda(19, 290.f);

	_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, GROSOR_PARED), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(50.f + TAMANIO_HUECO, 0.f), sf::Vector2f(440.f - 50.f - TAMANIO_HUECO, GROSOR_PARED), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(440.f + TAMANIO_HUECO, 0.f), sf::Vector2f(ANCHO_MUNDO - 440.f - TAMANIO_HUECO, GROSOR_PARED), _texturas));
	_salidas.push_back(Salida(sf::Vector2f(440.f, 0.f), sf::Vector2f(TAMANIO_HUECO, GROSOR_PARED), 8,
		sf::Vector2f(440.f + TAMANIO_HUECO + 10.f, Y_PISO - ALTO_JUGADOR)));

	// plataforma donde se cae desde el atajo
	_plataformas.push_back(Plataforma(sf::Vector2f(GROSOR_PARED, 200.f), sf::Vector2f(210.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 370.f), sf::Vector2f(40.f, Y_PISO - 370.f), _texturas, true));

	// cada camino se marca con el color de su llave 
	const sf::Color azul(100, 180, 255), amarilla(245, 205, 60), verde(80, 200, 90);

	// derecha: llave azul. 
	_bloques.push_back(Bloque(sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - 40.f, Y_PISO - TAMANIO_HUECO - 20.f), sf::Vector2f(60.f, 20.f), _texturas));
	_bloques.back().setColor(azul);

	// izquierda: llave verde.
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 270.f), sf::Vector2f(GROSOR_PARED + 40.f, 20.f), _texturas));
	_bloques.back().setColor(verde);

	// abajo: llave amarilla. 
	_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO - TAMANIO_HUECO) / 2 - 20.f, Y_PISO - 30.f), sf::Vector2f(20.f, 30.f), _texturas));
	_bloques.back().setColor(amarilla);
	_bloques.push_back(Bloque(sf::Vector2f((ANCHO_MUNDO + TAMANIO_HUECO) / 2, Y_PISO - 30.f), sf::Vector2f(20.f, 30.f), _texturas));
	_bloques.back().setColor(amarilla);

	piso(15, (ANCHO_MUNDO - TAMANIO_HUECO) / 2, 60.f); 

	_bloques.push_back(Bloque(sf::Vector2f(200.f, GROSOR_PARED), sf::Vector2f(50.f, 110.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(250.f, 100.f), sf::Vector2f(480.f, 30.f), _texturas));

	_plataformas.push_back(Plataforma(sf::Vector2f(740.f, 445.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 395.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(740.f, 345.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 295.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(740.f, 245.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 195.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(740.f, 145.f), sf::Vector2f(60.f, 10.f), _texturas));

	// jaula del doble salto
	_bloques.push_back(Bloque(sf::Vector2f(240.f, 235.f), sf::Vector2f(200.f, 30.f), _texturas)); 
	_bloques.push_back(Bloque(sf::Vector2f(240.f, 265.f), sf::Vector2f(40.f, 80.f), _texturas));  
	_bloques.push_back(Bloque(sf::Vector2f(240.f, 345.f), sf::Vector2f(200.f, 30.f), _texturas));
	_decoraciones.push_back(Decoracion(sf::Vector2f(460.f, 257.f), TipoDecoracion::CartelDobleSalto, _texturas));
	_coleccionables.push_back(Coleccionable(sf::Vector2f(290.f, 345.f - 60.f), TipoColeccionable::DobleSalto, _texturas));

	_mecanismos.push_back(new PuertaLlave(sf::Vector2f(360.f, 265.f), CANAL_PUERTA_VERDE, sf::Color(80, 200, 90), _texturas));
	_mecanismos.push_back(new PuertaLlave(sf::Vector2f(390.f, 265.f), CANAL_PUERTA_AMARILLA, sf::Color(245, 205, 60), _texturas));
	_mecanismos.push_back(new PuertaLlave(sf::Vector2f(420.f, 265.f), CANAL_PUERTA_AZUL, sf::Color(100, 180, 255), _texturas));

	
	_plataformas.push_back(Plataforma(sf::Vector2f(440.f, 345.f), sf::Vector2f(300.f, 10.f), _texturas));
}

void Nivel::cargarSala10() {
	// piso de pinches entre las dos columnas de madera
	for (int i = 0; i < 15; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(150.f + i * 44.f, Y_PISO - 37.5f), _texturas));
	}

	_bloques.push_back(Bloque(sf::Vector2f(130.f, 410.f), sf::Vector2f(20.f, 110.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(250.f, 300.f), sf::Vector2f(20.f, 80.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(130.f, 190.f), sf::Vector2f(20.f, 80.f), _texturas, true));

	_plataformas.push_back(Plataforma(sf::Vector2f(200.f, 110.f), sf::Vector2f(120.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(360.f, 110.f), sf::Vector2f(115.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(515.f, 110.f), sf::Vector2f(115.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(670.f, 110.f), sf::Vector2f(100.f, 10.f), _texturas));

	_obstaculos.push_back(new PilarMovil(sf::Vector2f(325.f, 20.f), sf::Vector2f(30.f, 70.f), 20.f, 200.f, 110.f, _texturas, true));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(480.f, 200.f), sf::Vector2f(30.f, 70.f), 20.f, 200.f, 140.f, _texturas, true));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(635.f, 100.f), sf::Vector2f(30.f, 70.f), 20.f, 200.f, 170.f, _texturas, true));

	_bloques.push_back(Bloque(sf::Vector2f(810.f, 190.f), sf::Vector2f(20.f, 80.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(690.f, 300.f), sf::Vector2f(20.f, 80.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(810.f, 410.f), sf::Vector2f(20.f, 110.f), _texturas, true));
}

void Nivel::cargarSala11() {
	_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(140.f, GROSOR_PARED), _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(240.f, 400.f), sf::Vector2f(30.f, ALTO_MUNDO - 400.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(420.f, 290.f), sf::Vector2f(30.f, ALTO_MUNDO - 290.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(600.f, 180.f), sf::Vector2f(30.f, ALTO_MUNDO - 180.f), _texturas, true));

	_obstaculos.push_back(new PilarMovil(sf::Vector2f(330.f, 150.f), sf::Vector2f(30.f, 70.f), 150.f, 420.f, 90.f, _texturas, true));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(510.f, 350.f), sf::Vector2f(30.f, 70.f), 100.f, 380.f, 120.f, _texturas, true));

	_bloques.push_back(Bloque(sf::Vector2f(780.f, 300.f), sf::Vector2f(160.f, ALTO_MUNDO - 300.f), _texturas));
	paredDerecha(12, 300.f - TAMANIO_HUECO);
}

void Nivel::cargarSala12() {
	const float hundido = 12.f;
	paredIzquierda(11, 220.f);
	paredDerecha(13, 220.f, Y_PISO - TAMANIO_HUECO); // en la 13 se entra abajo

	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 300.f), sf::Vector2f(120.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(140.f, 300.f), sf::Vector2f(300.f, 40.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(440.f, 300.f), sf::Vector2f(80.f, 40.f), _texturas));         
	_bloques.push_back(Bloque(sf::Vector2f(520.f, 300.f), sf::Vector2f(300.f, 40.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(820.f, 300.f), sf::Vector2f(120.f, 40.f), _texturas));       

	// pinches hundidos 12 px en la madera
	for (int i = 0; i < 7; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(140.f + i * 42.5f, 300.f - 37.5f + hundido), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(520.f + i * 42.5f, 300.f - 37.5f + hundido), _texturas));
	}

	_obstaculos.push_back(new PilarMovil(sf::Vector2f(140.f, 235.f), sf::Vector2f(70.f, 30.f), 140.f, 370.f, 120.f, _texturas, false, true));

	_obstaculos.push_back(new PilarMovil(sf::Vector2f(550.f, 140.f), sf::Vector2f(30.f, 70.f), 140.f, 400.f, 150.f, _texturas, true)); // abajo se esconde en el puente
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(760.f, 300.f), sf::Vector2f(30.f, 70.f), 140.f, 400.f, 150.f, _texturas, true));
}

void Nivel::cargarSala13() {
	const float hundido = 12.f;
	paredIzquierda(12, Y_PISO - TAMANIO_HUECO, 220.f);
	paredDerecha(14, 30.f);

	// izquierda
	_plataformas.push_back(Plataforma(sf::Vector2f(260.f, 370.f), sf::Vector2f(90.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(90.f, 280.f), sf::Vector2f(60.f, 40.f), _texturas, true));
	_plataformas.push_back(Plataforma(sf::Vector2f(230.f, 160.f), sf::Vector2f(30.f, 15.f), _texturas));

	_obstaculos.push_back(new PilarMovil(sf::Vector2f(205.f, 180.f), sf::Vector2f(35.f, 70.f), 140.f, 340.f, 130.f, _texturas, true));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(370.f, 30.f), sf::Vector2f(35.f, 70.f), 30.f, 450.f, 200.f, _texturas, true));
	
	_obstaculos.push_back(new Pinche(sf::Vector2f(390.f, Y_PISO - 37.5f), _texturas));
	_obstaculos.push_back(new Pinche(sf::Vector2f(340.f, Y_PISO - 37.5f), _texturas));

	// columna de madera del medio
	_bloques.push_back(Bloque(sf::Vector2f(440.f, 100.f), sf::Vector2f(35.f, Y_PISO), _texturas, true));
	_obstaculos.push_back(new Pinche(sf::Vector2f(475.f, 240.f), _texturas, 90.f));
	_obstaculos.push_back(new Pinche(sf::Vector2f(475.f, 290.f), _texturas, 90.f));

	// derecha arriba
	_bloques.push_back(Bloque(sf::Vector2f(475.f, 210.f), sf::Vector2f(245.f, 20.f), _texturas));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(495.f, 180.f), sf::Vector2f(70.f, 30.f), 495.f, 650.f, 90.f, _texturas, false, true));

	// derecha al medio
	_bloques.push_back(Bloque(sf::Vector2f(600.f, 360.f), sf::Vector2f(60.f, 15.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 360.f), sf::Vector2f(130.f, 20.f), _texturas, true));
	_obstaculos.push_back(new Pinche(sf::Vector2f(675.f, 340.f - 17.5f + hundido), _texturas));
	_obstaculos.push_back(new Pinche(sf::Vector2f(730.f, 340.f - 17.5f + hundido), _texturas));

	// derecha abajo
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(500.f, 400.f), sf::Vector2f(70.f, 30.f), 480.f, 700.f, 120.f, _texturas, false, true));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(650.f, 470.f), sf::Vector2f(70.f, 30.f), 480.f, 710.f, 90.f, _texturas, false, true));
	_bloques.push_back(Bloque(sf::Vector2f(790.f, GROSOR_PARED), sf::Vector2f(40.f, 440.f - GROSOR_PARED), _texturas));

	// escalera de plataformas hasta la salida
	const float alturas[6] = { 455.f, 390.f, 325.f, 260.f, 185.f, 110.f };
	for (int i = 0; i < 6; i++) {
		_plataformas.push_back(Plataforma(sf::Vector2f(860.f, alturas[i]), sf::Vector2f(80.f, 10.f), _texturas));
	}
}

void Nivel::cargarSala14() {
	// sala de la llave azul
	paredIzquierda(13, 30.f);
	piso(9, 820.f, 50.f); // atajo de vuelta: cae a la 9 

	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 110.f), sf::Vector2f(160.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 130.f), sf::Vector2f(140.f, Y_PISO - 130.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(160.f, 130.f), sf::Vector2f(20.f, Y_PISO - 130.f), _texturas));

	_plataformas.push_back(Plataforma(sf::Vector2f(220.f, 170.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(320.f, 230.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(420.f, 285.f), sf::Vector2f(60.f, 10.f), _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(360.f, 450.f), sf::Vector2f(80.f, Y_PISO - 450.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(440.f, 385.f), sf::Vector2f(80.f, Y_PISO - 385.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(520.f, 330.f), sf::Vector2f(80.f, Y_PISO - 330.f), _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(505.f, 315.f), sf::Vector2f(110.f, 15.f), _texturas));              
	_coleccionables.push_back(Coleccionable(sf::Vector2f(544.f, 315.f - 32.f - 6.f), TipoColeccionable::LlaveAzul, _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(480.f, GROSOR_PARED), sf::Vector2f(160.f, 25.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(490.f, 45.f), sf::Vector2f(14.f, 150.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(616.f, 45.f), sf::Vector2f(14.f, 150.f), _texturas));

	_bloques.push_back(Bloque(sf::Vector2f(700.f, 490.f), sf::Vector2f(60.f, 30.f), _texturas));
}

void Nivel::cargarSala15() {
	techo(9, 60.f, (ANCHO_MUNDO - TAMANIO_HUECO) / 2);
	piso(18, 104.f); 

	// paredes con dos huecos cada una: arriba (160 a 240) y abajo (440 a 520)
	const float xDerecha = ANCHO_MUNDO - GROSOR_PARED;
	const float xIzquierda = 0.f;
	const float xEnLa17 = ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f;
	const float xParedes[2] = { xIzquierda, xDerecha };
	for (int i = 0; i < 2; i++) {
		const float x = xParedes[i];
		_bloques.push_back(Bloque(sf::Vector2f(x, 0.f), sf::Vector2f(GROSOR_PARED, 160.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(x, 240.f), sf::Vector2f(GROSOR_PARED, 200.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(x, Y_PISO), sf::Vector2f(GROSOR_PARED, GROSOR_PARED), _texturas));
	}
	_salidas.push_back(Salida(sf::Vector2f(xDerecha, 160.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 16, sf::Vector2f(GROSOR_PARED + 10.f, 240.f - ALTO_JUGADOR)));
	_salidas.push_back(Salida(sf::Vector2f(xDerecha, 440.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 16, sf::Vector2f(GROSOR_PARED + 10.f, Y_PISO - ALTO_JUGADOR)));
	_salidas.push_back(Salida(sf::Vector2f(xIzquierda, 160.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 17, sf::Vector2f(900.f, 192.f - ALTO_JUGADOR))); 
	_salidas.push_back(Salida(sf::Vector2f(xIzquierda, 440.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 17, sf::Vector2f(900.f, Y_PISO - ALTO_JUGADOR)));

	// piso del medio
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 240.f), sf::Vector2f(60.f, 20.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(80.f, 240.f), sf::Vector2f(95.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(175.f, 240.f), sf::Vector2f(380.f, 20.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(555.f, 240.f), sf::Vector2f(295.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(850.f, 240.f), sf::Vector2f(xDerecha - 850.f, 20.f), _texturas));

	// compuertas de las salidas de arriba (boton 2)
	_mecanismos.push_back(new Compuerta(sf::Vector2f(60.f, 160.f), sf::Vector2f(20.f, 80.f), 2, _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 140.f), sf::Vector2f(60.f, 20.f), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(850.f, 160.f), sf::Vector2f(20.f, 80.f), 2, _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(850.f, 140.f), sf::Vector2f(xDerecha - 850.f, 20.f), _texturas));

	// pasillo de arriba
	_bloques.push_back(Bloque(sf::Vector2f(320.f, GROSOR_PARED), sf::Vector2f(20.f, 140.f), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(320.f, 160.f), sf::Vector2f(20.f, 80.f), 1, _texturas));
	_botones.push_back(Boton(sf::Vector2f(225.f, 90.f), 1, _texturas));


	_bloques.push_back(Bloque(sf::Vector2f(555.f, 350.f), sf::Vector2f(175.f, 15.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(710.f, 250.f), sf::Vector2f(20.f, 100.f), _texturas));
	_botones.push_back(Boton(sf::Vector2f(625.f, 330.f - 24.f), 2, _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(625.f, 365.f), sf::Vector2f(20.f, 75.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(555.f, 365.f), sf::Vector2f(20.f, 75.f), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(625.f, 440.f), sf::Vector2f(20.f, 80.f), 2, _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(555.f, 440.f), sf::Vector2f(20.f, 80.f), 2, _texturas,false,true));

	// abajo a la izquierda rincon 
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 408.f), sf::Vector2f(215.f, 16.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(235.f, 408.f), sf::Vector2f(30.f, Y_PISO - 408.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(272.f, 470.f), sf::Vector2f(85.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(65.f, 372.f), sf::Vector2f(100.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(65.f, 300.f), sf::Vector2f(100.f, 10.f), _texturas));
}

void Nivel::cargarSala16() {
	// pared izquierda con dos huecos 
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 0.f), sf::Vector2f(GROSOR_PARED, 160.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, 240.f), sf::Vector2f(GROSOR_PARED, 200.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(GROSOR_PARED, GROSOR_PARED), _texturas));
	const float xEntrada = ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f;
	_salidas.push_back(Salida(sf::Vector2f(0.f, 160.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 15, sf::Vector2f(xEntrada, 240.f - ALTO_JUGADOR)));
	_salidas.push_back(Salida(sf::Vector2f(0.f, 440.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 15, sf::Vector2f(xEntrada, Y_PISO - ALTO_JUGADOR)));

	// pasillo de arriba
	_plataformas.push_back(Plataforma(sf::Vector2f(GROSOR_PARED, 240.f), sf::Vector2f(300.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(320.f, 240.f), sf::Vector2f(200.f, 20.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(120.f, 110.f), sf::Vector2f(280.f, 10.f), _texturas)); // la de arriba
	_bloques.push_back(Bloque(sf::Vector2f(400.f, 110.f), sf::Vector2f(10.f, 70.f), _texturas, true));  

	// 5 compuertas del boton 1: cerrada, abierta, cerrada, abierta, cerrada
	const float xCompuertas[5] = { 140.f, 215.f, 296.f, 377.f, 500.f };
	const bool invertidas[5] = { false, true, false, true, false };
	for (int i = 0; i < 5; i++) {
		_mecanismos.push_back(new Compuerta(sf::Vector2f(xCompuertas[i], 160.f), sf::Vector2f(20.f, 80.f), 1, _texturas, false, invertidas[i]));
		// piedra arriba de cada una, para que no se salte: las 4 primeras hasta la plataforma, la ultima hasta el techo
		if (i < 4) {
			_bloques.push_back(Bloque(sf::Vector2f(xCompuertas[i], 120.f), sf::Vector2f(20.f, 40.f), _texturas));
		}
		else {
			_bloques.push_back(Bloque(sf::Vector2f(xCompuertas[i], GROSOR_PARED), sf::Vector2f(20.f, 140.f), _texturas));
		}
	}

	// boton 1
	_botones.push_back(Boton(sf::Vector2f(245.f, 340.f), 1, _texturas));

	// abajo de todo
	_plataformas.push_back(Plataforma(sf::Vector2f(160.f, 415.f), sf::Vector2f(325.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(485.f, 260.f), sf::Vector2f(20.f, 180.f), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(485.f, 440.f), sf::Vector2f(20.f, 80.f), 2, _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 415.f), sf::Vector2f(140.f, 25.f), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(120.f, 440.f), sf::Vector2f(20.f, 80.f), 1, _texturas)); // la de la salida

	// derecha pared del techo y pared delante del boton 2
	_bloques.push_back(Bloque(sf::Vector2f(570.f, GROSOR_PARED), sf::Vector2f(40.f, 380.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(730.f, 200.f), sf::Vector2f(40.f, Y_PISO - 200.f), _texturas));
	_botones.push_back(Boton(sf::Vector2f(850.f, Y_PISO - 24.f), 2, _texturas));
}

void Nivel::cargarSala17() {
	const float g = 12.f;                                     // grosor de los pisos
	const float y1 = 192.f, y2 = 274.f, y3 = 356.f, y4 = 438.f;
	const sf::Vector2f tamTrampilla(80.f, 16.f);                // casi el tamanio de la imagen acostada 
	const sf::Vector2f tamVertical(20.f, 82.f - g);             // compuertas y paredes entre dos pisos

	// pared derecha con dos huecos
	const float xPared = ANCHO_MUNDO - GROSOR_PARED;
	_bloques.push_back(Bloque(sf::Vector2f(xPared, 0.f), sf::Vector2f(GROSOR_PARED, y1 - TAMANIO_HUECO), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(xPared, y1), sf::Vector2f(GROSOR_PARED, 440.f - y1), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(xPared, Y_PISO), sf::Vector2f(GROSOR_PARED, GROSOR_PARED), _texturas));
	_salidas.push_back(Salida(sf::Vector2f(xPared, y1 - TAMANIO_HUECO), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 15, sf::Vector2f(GROSOR_PARED + 10.f, 240.f - ALTO_JUGADOR)));
	_salidas.push_back(Salida(sf::Vector2f(xPared, 440.f), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 15, sf::Vector2f(GROSOR_PARED + 10.f, Y_PISO - ALTO_JUGADOR)));

	_botones.push_back(Boton(sf::Vector2f(468.f, 70.f), 1, _texturas));

	// piso 1
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, y1), sf::Vector2f(109.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(129.f, y1), tamTrampilla, 1, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(209.f, y1), sf::Vector2f(48.f, g), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(257.f, y1), sf::Vector2f(454.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(711.f, y1), tamTrampilla, 1, _texturas, true, true));
	_bloques.push_back(Bloque(sf::Vector2f(791.f, y1), sf::Vector2f(xPared - 791.f, g), _texturas));

	// entre el piso 1 y el 2
	_mecanismos.push_back(new Compuerta(sf::Vector2f(100.f, y1 + g), tamVertical, 1, _texturas, false, true));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(258.f, y1 + g), tamVertical, 1, _texturas, false, true));
	_bloques.push_back(Bloque(sf::Vector2f(423.f, y1 + g), sf::Vector2f(27.f, tamVertical.y), _texturas));

	// piso 2
	_mecanismos.push_back(new Compuerta(sf::Vector2f(GROSOR_PARED, y2), tamTrampilla, 1, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(100.f, y2), sf::Vector2f(123.f, g), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(223.f, y2), sf::Vector2f(192.f, g), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(415.f, y2), sf::Vector2f(118.f, g), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(533.f, y2), sf::Vector2f(178.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(711.f, y2), tamTrampilla, 1, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(791.f, y2), sf::Vector2f(xPared - 791.f, g), _texturas));

	// entre el piso 2 y el 3
	_mecanismos.push_back(new Compuerta(sf::Vector2f(428.f, y2 + g), tamVertical, 1, _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(590.f, y2 + g), tamVertical, 1, _texturas, false, true));

	// piso 3
	_plataformas.push_back(Plataforma(sf::Vector2f(GROSOR_PARED, y3), sf::Vector2f(112.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(132.f, y3), tamTrampilla, 1, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(212.f, y3), sf::Vector2f(238.f, g), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(450.f, y3), sf::Vector2f(83.f, g), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(533.f, y3), sf::Vector2f(182.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(715.f, y3), tamTrampilla, 1, _texturas, true, true));
	_plataformas.push_back(Plataforma(sf::Vector2f(795.f, y3), sf::Vector2f(xPared - 795.f, g), _texturas));

	// entre el piso 3 y el 4
	_mecanismos.push_back(new Compuerta(sf::Vector2f(386.f, y3 + g), tamVertical, 1, _texturas, false, true));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(450.f, y3 + g), tamVertical, 1, _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(624.f, y3 + g), tamVertical, 1, _texturas, false, true));

	// piso 4
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, y4), sf::Vector2f(98.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(118.f, y4), tamTrampilla, 1, _texturas, true, true));
	_bloques.push_back(Bloque(sf::Vector2f(198.f, y4), sf::Vector2f(517.f, g), _texturas));
	_mecanismos.push_back(new Compuerta(sf::Vector2f(715.f, y4), tamTrampilla, 1, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(795.f, y4), sf::Vector2f(xPared - 795.f, g), _texturas));
}

void Nivel::cargarSala18() {
	techo(15, 104.f);
	piso(9, 860.f, 50.f); // atajo de vuelta: cae a la 9 

	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 160.f), sf::Vector2f(220.f, 20.f), _texturas)); 
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 180.f), sf::Vector2f(200.f, Y_PISO - 180.f), _texturas, true)); 
	_bloques.push_back(Bloque(sf::Vector2f(220.f, 180.f), sf::Vector2f(20.f, Y_PISO - 180.f), _texturas)); 
	// escalones
	for (int i = 0; i < 4; i++) {
		float alto = 232.f + i * 72.f;
		_bloques.push_back(Bloque(sf::Vector2f(240.f + i * 60.f, alto), sf::Vector2f(60.f, Y_PISO - alto), _texturas));
	}

	_bloques.push_back(Bloque(sf::Vector2f(600.f, GROSOR_PARED), sf::Vector2f(24.f, 260.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(796.f, GROSOR_PARED), sf::Vector2f(24.f, 260.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(600.f, 280.f), sf::Vector2f(220.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(624.f, GROSOR_PARED), sf::Vector2f(172.f, 260.f), _texturas, true)); 

	// pedestal con la llave
	_bloques.push_back(Bloque(sf::Vector2f(640.f, 490.f), sf::Vector2f(140.f, 30.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(670.f, 460.f), sf::Vector2f(80.f, 30.f), _texturas));
	_coleccionables.push_back(Coleccionable(sf::Vector2f(694.f, 460.f - 32.f - 6.f), TipoColeccionable::LlaveAmarilla, _texturas));
}

void Nivel::cargarSala19() {
	// camino verde
	paredDerecha(9, 290.f);

	// derecha pasillo de la entrada
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 370.f), sf::Vector2f(280.f, Y_PISO - 370.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 140.f), sf::Vector2f(280.f, 35.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 175.f), sf::Vector2f(45.f, 60.f), _texturas));
	_mecanismos.push_back(new Barrera(sf::Vector2f(662.f, 235.f), sf::Vector2f(40.f, 135.f), 1, _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(875.f, 260.f), sf::Vector2f(65.f, 10.f), _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(895.f, 260.f - 44.f), 1, _texturas));

	// medio
	_plataformas.push_back(Plataforma(sf::Vector2f(575.f, 147.f), sf::Vector2f(85.f, 10.f), _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(600.f, 147.f - 44.f), 2, _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(470, 410.f), sf::Vector2f(40.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(470, 480.f), sf::Vector2f(40.f, 10.f), _texturas));

	// izquierda
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 140.f), sf::Vector2f(200.f, 25.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(220.f, 140.f), sf::Vector2f(50.f, 220.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 326.f), sf::Vector2f(200.f, 34.f), _texturas));
	
	_bloques.push_back(Bloque(sf::Vector2f(270.f, 207.f), sf::Vector2f(85.f, 18.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(355.f, 225.f), sf::Vector2f(12.f, 81.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(220.f, 306.f), sf::Vector2f(147.f, 54.f), _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(300.f, 306.f - 44.f), 2, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(30.f, 140.f - 44.f), 2, _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(130.f, 230.f), sf::Vector2f(85.f, 10.f), _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(170.f, 230.f - 44.f), 2, _texturas));

	_mecanismos.push_back(new Barrera(sf::Vector2f(115.f, 360.f), sf::Vector2f(45.f, Y_PISO - 360.f), 2, _texturas));
}

void Nivel::cargarSala20() {
	paredIzquierda(21, 135.f);

	// parte 1
	_bloques.push_back(Bloque(sf::Vector2f(660.f, 400.f), sf::Vector2f(40.f, Y_PISO - 400.f), _texturas, true));
	cajaConCristal(sf::Vector2f(760.f, 310.f), 1);
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 445.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 370.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(860.f, 295.f), sf::Vector2f(70.f, 10.f), _texturas));
	_mecanismos.push_back(new Barrera(sf::Vector2f(620.f, GROSOR_PARED), sf::Vector2f(20.f, Y_PISO - GROSOR_PARED), 1, _texturas));

	// parte 2
	_bloques.push_back(Bloque(sf::Vector2f(340.f, GROSOR_PARED), sf::Vector2f(40.f, 200.f), _texturas, true));
	cajaConCristal(sf::Vector2f(440.f, 230.f), 2);
	_plataformas.push_back(Plataforma(sf::Vector2f(540.f, 435.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(540.f, 350.f), sf::Vector2f(60.f, 10.f), _texturas));
	_mecanismos.push_back(new Barrera(sf::Vector2f(320.f, GROSOR_PARED), sf::Vector2f(20.f, Y_PISO - GROSOR_PARED), 2, _texturas));

	// parte 3
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 400.f), sf::Vector2f(30.f, Y_PISO - 400.f), _texturas, true));
	cajaConCristal(sf::Vector2f(110.f, 360.f), 3);
	cajaConCristal(sf::Vector2f(190.f, 295.f), 3);
	_plataformas.push_back(Plataforma(sf::Vector2f(260.f, 445.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(260.f, 370.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(260.f, 295.f), sf::Vector2f(60.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 215.f), sf::Vector2f(160.f, 20.f), _texturas)); 
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 115.f), sf::Vector2f(160.f, 20.f), _texturas));
	_mecanismos.push_back(new Barrera(sf::Vector2f(160.f, 135.f), sf::Vector2f(20.f, 80.f), 3, _texturas));
}

void Nivel::cargarSala21() {
	paredDerecha(20, 135.f);
	paredIzquierda(22, 440.f, 260.f); 

	// pared del medio con las cajitas
	_bloques.push_back(Bloque(sf::Vector2f(450.f, GROSOR_PARED), sf::Vector2f(60.f, 60.f), _texturas));
	cajaConCristal(sf::Vector2f(450.f, 80.f), 1);
	_bloques.push_back(Bloque(sf::Vector2f(450.f, 144.f), sf::Vector2f(60.f, 56.f), _texturas));
	cajaConCristal(sf::Vector2f(450.f, 200.f), 1);
	_bloques.push_back(Bloque(sf::Vector2f(450.f, 264.f), sf::Vector2f(60.f, 56.f), _texturas));
	cajaConCristal(sf::Vector2f(450.f, 320.f), 1);
	_bloques.push_back(Bloque(sf::Vector2f(450.f, 384.f), sf::Vector2f(60.f, 16.f), _texturas));

	// lado izquierdo
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 230.f), sf::Vector2f(25.f, 110.f), _texturas, true));
	for (int i = 0; i < 4; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(230.f + i * 44.f, Y_PISO - 37.5f), _texturas));
	}
	_plataformas.push_back(Plataforma(sf::Vector2f(290.f, 450.f), sf::Vector2f(60.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 420.f), sf::Vector2f(80.f, 20.f), _texturas));
	_mecanismos.push_back(new Barrera(sf::Vector2f(80.f, 440.f), sf::Vector2f(20.f, 80.f), 1, _texturas));

	// lado derecho
	_bloques.push_back(Bloque(sf::Vector2f(860.f, 215.f), sf::Vector2f(80.f, 20.f), _texturas));
	for (int i = 0; i < 9; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(560.f + i * 42.f, Y_PISO - 37.5f), _texturas));
	}
	_plataformas.push_back(Plataforma(sf::Vector2f(760.f, 290.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(650.f, 460.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(570.f, 400.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(650.f, 325.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(570.f, 247.f), sf::Vector2f(70.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(600.f, 180.f), sf::Vector2f(60.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(515.f, 118.f), sf::Vector2f(60.f, 10.f), _texturas));
}

void Nivel::cargarSala22() {
	// sala de la llave verde
	paredDerecha(21, 260.f, 440.f);

	// piso con el pozo debajo del puente 
	_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(60.f, GROSOR_PARED), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(140.f, Y_PISO), sf::Vector2f(395.f, GROSOR_PARED), _texturas));
	_salidas.push_back(Salida(sf::Vector2f(60.f, Y_PISO), sf::Vector2f(TAMANIO_HUECO, GROSOR_PARED), 9,
		sf::Vector2f(50.f + (TAMANIO_HUECO - ANCHO_JUGADOR) / 2, GROSOR_PARED + 5.f)));

	// el puente
	_bloques.push_back(Bloque(sf::Vector2f(740.f, 340.f), sf::Vector2f(200.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(745.f, 360.f), sf::Vector2f(40.f, ALTO_MUNDO - 360.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(585.f, 340.f), sf::Vector2f(155.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(440.f, 340.f), sf::Vector2f(145.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(535.f, 360.f), sf::Vector2f(40.f, ALTO_MUNDO - 360.f), _texturas));

	// piramide escalonada
	_bloques.push_back(Bloque(sf::Vector2f(180.f, 480.f), sf::Vector2f(200.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(220.f, 440.f), sf::Vector2f(120.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(255.f, 400.f), sf::Vector2f(50.f, 40.f), _texturas));
	_coleccionables.push_back(Coleccionable(sf::Vector2f(264.f, 400.f - 32.f - 6.f), TipoColeccionable::LlaveVerde, _texturas));

	// cristales de adorno
	float enterrado = 16.f;
	_cristales.push_back(Cristal(sf::Vector2f(150.f, Y_PISO - 44.f + enterrado), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(190.f, 480.f - 44.f + enterrado), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(338.f, 480.f - 44.f + enterrado), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(400.f, Y_PISO - 44.f + enterrado), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(440.f, Y_PISO - 44.f + 24.f), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(470.f, 340.f - 44.f + enterrado), 99, _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(880.f, 340.f - 44.f + enterrado), 99, _texturas));

	// el fondo del pozo debajo del puente
	float yPozo[8] = { 512.f, 506.f, 516.f, 508.f, 514.f, 506.f, 512.f, 516.f };
	for (int i = 0; i < 8; i++) {
		_cristales.push_back(Cristal(sf::Vector2f(575.f + i * 20.f, yPozo[i]), 99, _texturas));
	}

	// lampara colgando
	_bloques.push_back(Bloque(sf::Vector2f(660.f, GROSOR_PARED), sf::Vector2f(4.f, 150.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(650.f, 170.f), sf::Vector2f(24.f, 16.f), _texturas));

	// estalactitas en el techo:
	float xEstalactita[5] = { 100.f, 260.f, 420.f, 540.f, 800.f };
	float largo[5] = { 40.f, 70.f, 30.f, 55.f, 45.f };
	for (int i = 0; i < 5; i++) {
		_bloques.push_back(Bloque(sf::Vector2f(xEstalactita[i], GROSOR_PARED), sf::Vector2f(40.f, 16.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(xEstalactita[i] + 12.f, GROSOR_PARED + 16.f), sf::Vector2f(16.f, largo[i]), _texturas));
	}
}

void Nivel::cargarSala23() {
	// camino rojo
	piso(8, GROSOR_PARED, 140.f);
	paredDerecha(24, 110.f);


	_bloques.push_back(Bloque(sf::Vector2f(320.f, 360.f), sf::Vector2f(60.f, Y_PISO - 360.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(430.f, GROSOR_PARED), sf::Vector2f(40.f, 380.f), _texturas)); 

	_bloques.push_back(Bloque(sf::Vector2f(560.f, 290.f), sf::Vector2f(60.f, 40.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(560.f, 330.f), sf::Vector2f(60.f, Y_PISO - 330.f), _texturas, true));


	for (int i = 0; i < 5; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(620.f + i * 40.f, Y_PISO - 37.5f), _texturas));
	}
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(705.f, 150.f), sf::Vector2f(30.f, 70.f), 150.f, 330.f, 100.f, _texturas, true));
	_bloques.push_back(Bloque(sf::Vector2f(820.f, 190.f), sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - 820.f, Y_PISO - 190.f), _texturas));
}

void Nivel::cargarSala24() {
	paredIzquierda(23, 110.f);
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 190.f), sf::Vector2f(80.f, 20.f), _texturas)); // entrada
	_bloques.push_back(Bloque(sf::Vector2f(180.f, GROSOR_PARED), sf::Vector2f(50.f, 280.f), _texturas));
	for (int i = 0; i < 22; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(30.f + i * 42.f, Y_PISO - 37.5f), _texturas));
	}
	_bloques.push_back(Bloque(sf::Vector2f(280.f, 460.f), sf::Vector2f(90.f, 20.f), _texturas));

	// plataformas chicas sobre los pinches.
	_plataformas.push_back(Plataforma(sf::Vector2f(430.f, 400.f), sf::Vector2f(45.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(500.f, GROSOR_PARED), sf::Vector2f(25.f, 320.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(585.f, 395.f), sf::Vector2f(32.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(565.f, 280.f), sf::Vector2f(40.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(700.f, 170.f), sf::Vector2f(40.f, 10.f), _texturas));
	_obstaculos.push_back(new PilarMovil(sf::Vector2f(800.f, 260.f), sf::Vector2f(30.f, 70.f), 120.f, 300.f, 140.f, _texturas, true));

	// salida 
	paredDerecha(25, 200.f, 74.f); 
	_bloques.push_back(Bloque(sf::Vector2f(880.f, 280.f), sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - 880.f, 20.f), _texturas));
}

void Nivel::cargarSala25() {
	paredIzquierda(24, 74.f, 200.f);
	techo(26, 440.f);

	// escalera de columnas, cada una con un pinche arriba (menos la de la entrada)
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 154.f), sf::Vector2f(67.f, Y_PISO - 154.f), _texturas));
	float yColumna[5] = { 265.f, 328.f, 381.f, 439.f, 495.f };
	for (int i = 0; i < 5; i++) {
		float x = 87.f + i * 58.f; // pegadas una al lado de la otra
		_bloques.push_back(Bloque(sf::Vector2f(x, yColumna[i]), sf::Vector2f(58.f, Y_PISO - yColumna[i]), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(x + 7.f, yColumna[i] - 37.5f), _texturas));
	}
	for (int i = 0; i < 13; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(377.f + i * 42.f, Y_PISO - 37.5f), _texturas));
	}

	// bloques colgando del techo, cada uno con un pinche hacia abajo
	float xColgante[4] = { 226.f, 272.f, 330.f, 393.f }; // pegados uno al lado del otro (el ultimo es el borde)
	float largoColgante[3] = { 43.f, 77.f, 125.f };
	for (int i = 0; i < 3; i++) {
		float ancho = xColgante[i + 1] - xColgante[i];
		_bloques.push_back(Bloque(sf::Vector2f(xColgante[i], GROSOR_PARED), sf::Vector2f(ancho, largoColgante[i]), _texturas));
		_obstaculos.push_back(new Pinche(sf::Vector2f(xColgante[i] + (ancho - 44.f) / 2, GROSOR_PARED + largoColgante[i]), _texturas, 180.f));
	}
	_bloques.push_back(Bloque(sf::Vector2f(393.f, 125.f), sf::Vector2f(41.f, 78.f), _texturas));
	_obstaculos.push_back(new Pinche(sf::Vector2f(392.f, 203.f), _texturas, 180.f));

	// bloque grande con pinches abajo (las plataformas de abajo estan justo a un salto de distancia)
	_bloques.push_back(Bloque(sf::Vector2f(434.f, 125.f), sf::Vector2f(318.f, 135.f), _texturas));
	for (int i = 0; i < 7; i++) {
		_obstaculos.push_back(new Pinche(sf::Vector2f(434.f + i * 45.f, 260.f), _texturas, 180.f));
	}
	_plataformas.push_back(Plataforma(sf::Vector2f(474.f, 465.f), sf::Vector2f(64.f, 10.f), _texturas));
	_plataformas.push_back(Plataforma(sf::Vector2f(706.f, 465.f), sf::Vector2f(69.f, 10.f), _texturas));

	// derecha
	_bloques.push_back(Bloque(sf::Vector2f(905.f, 255.f), sf::Vector2f(35.f, 135.f), _texturas, true));
	_plataformas.push_back(Plataforma(sf::Vector2f(752.f, 135.f), sf::Vector2f(188.f, 10.f), _texturas));
}

void Nivel::cargarSala26() {
	piso(25, 440.f);
	paredIzquierda(27, 100.f, 220.f);
	_bloques.push_back(Bloque(sf::Vector2f(560.f, GROSOR_PARED), sf::Vector2f(380.f, Y_PISO - GROSOR_PARED), _texturas));

	// escalones de abajo
	float xEscalon[6] = { 20.f, 90.f, 160.f, 230.f, 300.f, 370.f };
	float yEscalon[6] = { 180.f, 225.f, 285.f, 345.f, 405.f, 465.f };
	for (int i = 0; i < 6; i++) {
		_bloques.push_back(Bloque(sf::Vector2f(xEscalon[i], yEscalon[i]), sf::Vector2f(70.f, Y_PISO - yEscalon[i]), _texturas));
		if (i > 0) {
			_bloques.push_back(Bloque(sf::Vector2f(xEscalon[i], GROSOR_PARED), sf::Vector2f(70.f, yEscalon[i] - 140.f - GROSOR_PARED), _texturas));
		}
	}
	_bloques.push_back(Bloque(sf::Vector2f(440.f, GROSOR_PARED), sf::Vector2f(120.f, 360.f), _texturas)); // arriba de la entrada
}

void Nivel::cargarSala27() {
	paredDerecha(26, 220.f, 100.f);
	paredIzquierda(4, 220.f, 70.f);
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, GROSOR_PARED), sf::Vector2f(ANCHO_MUNDO - 2 * GROSOR_PARED, 200.f - GROSOR_PARED), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 300.f), sf::Vector2f(ANCHO_MUNDO - 2 * GROSOR_PARED, Y_PISO - 300.f), _texturas));
}

void Nivel::cargarSala28() {
	// sala de la llave roja
	paredDerecha(1, 160.f, 80.f);
	piso(9, 80.f, 50.f); // atajo de vuelta: cae a la 9

	// entrada
	_bloques.push_back(Bloque(sf::Vector2f(760.f, 240.f), sf::Vector2f(180.f, 20.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(760.f, 260.f), sf::Vector2f(20.f, Y_PISO - 260.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(780.f, 260.f), sf::Vector2f(160.f, Y_PISO - 260.f), _texturas, true));

	// escalera 
	for (int i = 0; i < 3; i++) {
		float alto = 310.f + i * 70.f;
		_bloques.push_back(Bloque(sf::Vector2f(690.f - i * 70.f, alto), sf::Vector2f(70.f, Y_PISO - alto), _texturas));
	}

	// altar con la llave
	_bloques.push_back(Bloque(sf::Vector2f(300.f, 490.f), sf::Vector2f(200.f, 30.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(330.f, 460.f), sf::Vector2f(140.f, 30.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(370.f, 430.f), sf::Vector2f(60.f, 30.f), _texturas));
	_coleccionables.push_back(Coleccionable(sf::Vector2f(384.f, 430.f - 32.f - 6.f), TipoColeccionable::LlaveRoja, _texturas));

	float xColumna[2] = { 240.f, 536.f };
	for (int i = 0; i < 2; i++) {
		_bloques.push_back(Bloque(sf::Vector2f(xColumna[i], GROSOR_PARED), sf::Vector2f(24.f, 300.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(xColumna[i] - 10.f, 320.f), sf::Vector2f(44.f, 12.f), _texturas));
	}

	// lampara arriba 
	_bloques.push_back(Bloque(sf::Vector2f(398.f, GROSOR_PARED), sf::Vector2f(4.f, 120.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(380.f, 140.f), sf::Vector2f(40.f, 14.f), _texturas));

	// marco del hueco del atajo
	_bloques.push_back(Bloque(sf::Vector2f(60.f, 490.f), sf::Vector2f(20.f, 30.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(160.f, 490.f), sf::Vector2f(20.f, 30.f), _texturas));

	float xEstalactita[3] = { 100.f, 640.f, 840.f };
	float largo[3] = { 50.f, 35.f, 60.f };
	for (int i = 0; i < 3; i++) {
		_bloques.push_back(Bloque(sf::Vector2f(xEstalactita[i], GROSOR_PARED), sf::Vector2f(40.f, 16.f), _texturas));
		_bloques.push_back(Bloque(sf::Vector2f(xEstalactita[i] + 12.f, GROSOR_PARED + 16.f), sf::Vector2f(16.f, largo[i]), _texturas));
	}
}

void Nivel::cargarSala29() {
	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, GROSOR_PARED), sf::Vector2f(ANCHO_MUNDO - 2 * GROSOR_PARED, 380.f ), _texturas));
}

void Nivel::cargarSala30() {
	_bloques.push_back(Bloque(sf::Vector2f(0.f, Y_PISO), sf::Vector2f(ANCHO_MUNDO, GROSOR_PARED), _texturas));
	_bloques.back().setColor(sf::Color::Transparent);

	_bloques.push_back(Bloque(sf::Vector2f(0.f, 300.f), sf::Vector2f(GROSOR_PARED, 140.f), _texturas));
	_bloques.back().setColor(sf::Color(110, 95, 85));

	_salidas.push_back(Salida(sf::Vector2f(0.f, Y_PISO - TAMANIO_HUECO), sf::Vector2f(GROSOR_PARED, TAMANIO_HUECO), 29,
		sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED - ANCHO_JUGADOR - 10.f, Y_PISO - ALTO_JUGADOR)));

	_bloques.push_back(Bloque(sf::Vector2f(GROSOR_PARED, 300.f), sf::Vector2f(110.f, 140.f), _texturas));
	_bloques.back().setColor(sf::Color(110, 95, 85));

	_salidas.push_back(Salida(sf::Vector2f(ANCHO_MUNDO - GROSOR_PARED, 0.f), sf::Vector2f(GROSOR_PARED, Y_PISO), -1, sf::Vector2f(0.f, 0.f)));
}

void Nivel::dibujar(sf::RenderWindow& ventana, const bool llaves[]) {
	ventana.draw(_fondo);

	for (Decoracion& d : _decoraciones) {
		d.dibujar(ventana);
	}

	for (Obstaculo* o : _obstaculos) {
		o->dibujar(ventana);
	}
	// los cristales antes que los bloques: asi se pueden enterrar un poco en la piedra
	for (Cristal& c : _cristales) {
		c.dibujar(ventana);
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
	// despues de las salidas: si no, una trampilla sobre un hueco queda tapada por el rectangulo de la salida
	for (Mecanismo* m : _mecanismos) {
		m->dibujar(ventana);
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

void Nivel::actualizarBotonesYMecanismos(const std::vector<Lanza>& lanzas, bool tieneLlaveAzul, bool tieneLlaveAmarilla, bool tieneLlaveVerde) {
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

		bool activo = hayBotonActivo || (hayCristales && todosRotos) || 
			(m->getCanal() == CANAL_PUERTA_AZUL && tieneLlaveAzul) ||
			(m->getCanal() == CANAL_PUERTA_AMARILLA && tieneLlaveAmarilla) ||
			(m->getCanal() == CANAL_PUERTA_VERDE && tieneLlaveVerde);
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

void Nivel::cajaConCristal(sf::Vector2f posicion, int canal) {
	// cuatro bloques (arriba, abajo, izquierda, derecha) y el cristal en el hueco del medio
	_bloques.push_back(Bloque(posicion, sf::Vector2f(60.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(posicion.x, posicion.y + 54.f), sf::Vector2f(60.f, 10.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(posicion.x, posicion.y + 10.f), sf::Vector2f(8.f, 44.f), _texturas));
	_bloques.push_back(Bloque(sf::Vector2f(posicion.x + 52.f, posicion.y + 10.f), sf::Vector2f(8.f, 44.f), _texturas));
	_cristales.push_back(Cristal(sf::Vector2f(posicion.x + 14.f, posicion.y + 10.f), canal, _texturas));
}