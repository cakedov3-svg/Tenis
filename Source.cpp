#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <fstream>

enum tPuntosJuego { NADA, QUINCE, TREINTA, CUARENTA, VENTAJA, GANADOR };

//Constantes utilizadas
const int LIM_INF_HAB = 1, LIM_SUP_HAB = 4, LIM_INF_VEL = 1, LIM_SUP_VEL = 4, ANCHO_PISTA = 7, CALLE_INICIAL = (ANCHO_PISTA / 2) + 1, JUEGOS_SET = 3, LARGO_PISTA = 3, LARGO_NOMBRE = 3, MAX_JUG = 10;
const bool JUEGO_ALEATORIO = true, MODO_DEBUG = false;
const std::string NOM_FICHERO = "pruebaFinal.txt";

struct tjugador { //Datos del jugador
	std::string siglas = "";
	int habilidad = 0;
	int velocidad = 0;
	tPuntosJuego puntuacion = NADA;
	int pos = CALLE_INICIAL;
	int juegos = 0;
	int tConteoGolpes[ANCHO_PISTA + 2];
	int golpesGanadores = 0;
	int golpesTotales = 0;
	int partidosGanados = 0;
	int partidosPerdidos = 0;
};

struct tListaJug {
	tjugador tenistas[MAX_JUG];
	int cont;
};

void inicializarJugador(tjugador& jugador) { //Inicializa las características del jugador indicado
	jugador.siglas = "";
	jugador.habilidad = 0;
	jugador.velocidad = 0;
	jugador.puntuacion = NADA;
	jugador.pos = CALLE_INICIAL;
	jugador.juegos = 0;
	for (int i = 0; i < ANCHO_PISTA + 2; i++) {
		jugador.tConteoGolpes[i] = 0;
	}
	jugador.golpesGanadores = 0;
	jugador.golpesTotales = 0;
	jugador.partidosGanados = 0;
	jugador.partidosPerdidos = 0;
}

std::string marcador(tPuntosJuego a) { //Transforma los puntos a nivel marcador
	std::string pt;
	if (a == NADA) pt = "00";
	else if (a == QUINCE) pt = "15";
	else if (a == TREINTA) pt = "30";
	else if (a == CUARENTA) pt = "40";
	else if (a == VENTAJA) pt = "Adv";

	return pt;
}

std::string escribeNombreJugador(tjugador aTenistas[], int j) { //Escribe el nombre del jugador indicado
	return aTenistas[j].siglas;
}

void escribirMarcador(tjugador aTenistas[], int saque) { //Escribe el marcador
	if (saque == 0) {
		std::cout << "    " << escribeNombreJugador(aTenistas, 0) << "  " << aTenistas[0].juegos << " : " << marcador(aTenistas[0].puntuacion) << " *\n";
		std::cout << "    " << escribeNombreJugador(aTenistas, 1) << "  " << aTenistas[1].juegos << " : " << marcador(aTenistas[1].puntuacion) << '\n';
	}
	else {
		std::cout << "    " << escribeNombreJugador(aTenistas, 0) << "  " << aTenistas[0].juegos << " : " << marcador(aTenistas[0].puntuacion) << '\n';
		std::cout << "    " << escribeNombreJugador(aTenistas, 1) << "  " << aTenistas[1].juegos << " : " << marcador(aTenistas[1].puntuacion) << " *\n";
	}
}

void escribeEstadisticas(tjugador aTenistas[], int indic) { //Escribe las estadísticas del juego al final de cada juego
	std::cout << "Estadisticas de " << escribeNombreJugador(aTenistas, indic) << '\n';
	std::cout << "   Golpes totales: " << aTenistas[indic].golpesTotales << '\n';
	std::cout << "   Golpes ganadores: " << aTenistas[indic].golpesGanadores << '\n';
	std::cout << "   Errores no forzados: " << (aTenistas[indic].tConteoGolpes[0] + aTenistas[indic].tConteoGolpes[ANCHO_PISTA + 1]) << '\n';
	std::cout << "   Distribucion de los golpes en la pista \n";
	std::cout << "      Calle ";
	for (int i = 0; i <= (ANCHO_PISTA + 1); ++i) {
		std::cout << std::setw(8) << i;
	}
	std::cout << '\n' << std::setw(12) << "% ";
	for (int i = 0; i <= (ANCHO_PISTA + 1); ++i) {
		float porcentajePista = (float(aTenistas[indic].tConteoGolpes[i]) / aTenistas[indic].golpesTotales) * 100; // Calcula el porcentaje de tiros en cada pista
		std::cout << std::setw(8) << std::fixed << std::setprecision(2) << porcentajePista;
	}
	std::cout << '\n' << '\n';
}

tjugador leerDatosTenistas() { //Pide y guarda los datos de los tenistas
	bool correcto = false, letras = false;
	tjugador jugador;
	inicializarJugador(jugador);
	std::cout << " - Introduce sus iniciales (" << LARGO_NOMBRE << " letras mayusculas): ";
	std::cin >> jugador.siglas;
	while (jugador.siglas.length() != LARGO_NOMBRE || !letras) {
		letras = false;
		int cierto = 0;

		for (int i = 0; i < jugador.siglas.length(); i++) {
			if (jugador.siglas[i] > 'A' && jugador.siglas[i] < 'Z') cierto++;
		}

		if (cierto == jugador.siglas.length() && jugador.siglas.length() == LARGO_NOMBRE) letras = true;
		else {
			std::cout << "Ha introducido las siglas de manera erronea. Por favor, intentelo de nuevo\n";
			std::cout << " - Introduce sus iniciales (" << LARGO_NOMBRE << " letras mayusculas): ";
			std::cin >> jugador.siglas;
		}
	}

	while (!correcto) {
		std::cout << " - Introduce su habilidad (valor entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
		std::cin >> jugador.habilidad;
		if (jugador.habilidad >= LIM_INF_HAB && jugador.habilidad <= LIM_SUP_HAB)
			correcto = true;
		else {
			std::cout << "El dato de la habilidad ha sido introducido de forma erronea, por favor repitalo." << '\n';
		}
	}
	correcto = false;

	while (!correcto) {
		std::cout << " - Introduce su velocidad (valor entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
		std::cin >> jugador.velocidad;
		if (jugador.velocidad >= LIM_INF_VEL && jugador.velocidad <= LIM_SUP_VEL)
			correcto = true;
		else {
			std::cout << "El dato de la velocidad ha sido introducido de forma erronea, por favor repitalo." << '\n';
		}
	}

	jugador.partidosGanados = 0;
	jugador.partidosPerdidos = 0;
	return jugador;
}

void escribirPista(tjugador aTenistas[], int indic, int posBola) { //Dibuja el campo, los jugadores y la bola en su correspondiente posicion
	std::cout << '\n';
	std::cout << std::setw((2 * aTenistas[0].pos) - 1 + 1 / aTenistas[0].siglas.size()) << std::setfill(' ') << " ";
	std::cout << escribeNombreJugador(aTenistas, 0);
	for (int j = (2 * aTenistas[0].pos) - 1 + 1 / aTenistas[0].siglas.size(); j < 2 * (ANCHO_PISTA + 2) - aTenistas[0].siglas.size(); j++) std::cout << " ";
	std::cout << '\n';

	std::cout << "  ";
	for (int i = 0; i < ANCHO_PISTA; i++) std::cout << "- ";
	std::cout << " \n";

	for (int i = 0; i < (ANCHO_PISTA + 1); ++i) {
		if (indic == 1 && posBola == i) std::cout << "o|";
		else std::cout << " |";
	}
	if (indic == 1 && posBola == ANCHO_PISTA + 1) std::cout << "o";
	std::cout << '\n';

	for (int i = 1; i < LARGO_PISTA; i++) {
		for (int i = 0; i < (ANCHO_PISTA + 1); ++i) {
			std::cout << " |";
		}
		std::cout << '\n';
	}

	std::cout << "--";
	for (int i = 1; i <= ANCHO_PISTA; i++) std::cout << i << "-";
	std::cout << "-\n";

	for (int i = 1; i < LARGO_PISTA; i++) {
		for (int i = 0; i < (ANCHO_PISTA + 1); ++i) {
			std::cout << " |";
		}
		std::cout << '\n';
	}

	for (int i = 0; i < (ANCHO_PISTA + 1); ++i) {
		if (indic == 0 && posBola == i) std::cout << "o|";
		else std::cout << " |";
	}
	if (indic == 0 && posBola == ANCHO_PISTA + 1) std::cout << "o";
	std::cout << '\n';

	std::cout << "  ";
	for (int i = 0; i < ANCHO_PISTA; i++) std::cout << "- ";
	std::cout << " \n";

	std::cout << std::setw((2 * aTenistas[1].pos) - 1 + 1 / aTenistas[1].siglas.size()) << " ";

	std::cout << escribeNombreJugador(aTenistas, 1);
	for (int j = (2 * aTenistas[1].pos) - 1 + 1 / aTenistas[1].siglas.size(); j < 2 * (ANCHO_PISTA + 2) - aTenistas[1].siglas.size(); j++) std::cout << " ";
	std::cout << "\n\n";
}

int golpeoBola(int pos, int hab) { //Calcula y devuelve el destino de la bola
	int objetivo, posibilidad, resultado;
	if (JUEGO_ALEATORIO) {
		objetivo = rand() % ANCHO_PISTA + 1;
	}
	else {
		std::cout << "- Introduce el destino deseado para la bola (valor entre 1 y " << ANCHO_PISTA << "): ";
		std::cin >> objetivo;
		while (objetivo > ANCHO_PISTA || objetivo <= 0) {
			std::cout << "Has escogido un objetivo fuera del campo. Introduce un valor comprendido dentro del campo: ";
			std::cin >> objetivo;
		}
	}
	if (MODO_DEBUG) std::cout << "\nEl jugador dispara hacia la calle " << objetivo << '\n';

	posibilidad = 100 - (((abs(objetivo - pos) - hab) * 100) / ((ANCHO_PISTA - 1) - LIM_INF_HAB));
	resultado = (rand() % 100 + 1);
	if (abs(pos - objetivo) <= hab) {
		if (MODO_DEBUG) std::cout << "Ese ha sido un tiro sencillo." << '\n';
		return objetivo;
	}
	else if (resultado <= posibilidad) {
		if (MODO_DEBUG) std::cout << "Tiro complicado que... (probab_exito = " << posibilidad << " y resultado = " << resultado << ") LLega a su destino!" << '\n';
		return objetivo;
	}
	else {
		if (MODO_DEBUG)	std::cout << "Tiro complicado que... (probab_exito = " << posibilidad << " y resultado = " << resultado << ") No ha sido preciso!" << '\n';
		if (rand() % 2 == 0) return (objetivo - 1);
		else return (objetivo + 1);
	}
}

int correTenista(int pos, int vel, int obj) { //Calcula si el tenista llega al objetivo o no
	if (abs(pos - obj) <= vel) {
		return obj;
	}
	else {
		if (pos > obj) return (pos - vel);
		else return (pos + vel);
	}
}

int punto(tjugador aTenistas[], int indic) { //Se juega el punto
	int obj = CALLE_INICIAL, ganador;
	aTenistas[0].pos = aTenistas[1].pos = CALLE_INICIAL;
	bool punto = false;

	escribirPista(aTenistas, (indic + 1) % 2, obj);
	system("pause");

	while (!punto) {

		std::cout << "Golpea " << escribeNombreJugador(aTenistas, indic) << '\n';
		obj = golpeoBola(aTenistas[indic].pos, aTenistas[indic].habilidad);


		aTenistas[indic].tConteoGolpes[obj]++;
		aTenistas[indic].golpesTotales++;

		if (MODO_DEBUG) std::cout << "La bola llega a la casilla " << obj << '\n';
		if (obj > 0 && obj <= ANCHO_PISTA) aTenistas[(indic + 1) % 2].pos = correTenista(aTenistas[(indic + 1) % 2].pos, aTenistas[(indic + 1) % 2].velocidad, obj);

		escribirPista(aTenistas, indic, obj);
		system("pause");

		if (obj > ANCHO_PISTA || obj <= 0) {
			if (MODO_DEBUG) std::cout << "La bola ha salido fuera del campo." << '\n';
			ganador = (indic + 1) % 2;
			punto = true;
		}
		else if (aTenistas[(indic + 1) % 2].pos != obj) {
			if (MODO_DEBUG) std::cout << "Su rival no llega a la bola." << '\n';
			punto = true;
			ganador = indic;
			aTenistas[indic].golpesGanadores++;
		}
		else {
			if (MODO_DEBUG) std::cout << "Su rival llega a la bola." << '\n' << '\n';
		}

		indic = (indic + 1) % 2;

	}
	return ganador;
}

int juego(tjugador aTenistas[], int saque) { //Controla el juego, los puntos y la puntuacion en el marcador
	int indic;
	aTenistas[0].puntuacion = NADA;
	aTenistas[1].puntuacion = NADA;
	std::cout << "\nServicio para " << escribeNombreJugador(aTenistas, saque) << '\n';
	std::cout << '\n';
	indic = saque;
	while (aTenistas[0].puntuacion < GANADOR && aTenistas[1].puntuacion < GANADOR) {
		escribirMarcador(aTenistas, saque);
		std::cout << '\n';
		system("pause");
		indic = punto(aTenistas, saque);

		if (aTenistas[indic].puntuacion == CUARENTA && aTenistas[(indic + 1) % 2].puntuacion < CUARENTA) aTenistas[indic].puntuacion = GANADOR; //Caso 40 y el otro menos de 40
		else if (aTenistas[indic].puntuacion == CUARENTA && aTenistas[(indic + 1) % 2].puntuacion > CUARENTA) aTenistas[(indic + 1) % 2].puntuacion = CUARENTA; //Caso 40 - Adv
		else aTenistas[indic].puntuacion = tPuntosJuego(int(aTenistas[indic].puntuacion) + 1); //Resto de casos

		if (aTenistas[indic].puntuacion < GANADOR) std::cout << "Punto para " << escribeNombreJugador(aTenistas, indic) << "!!!" << '\n' << '\n';
		else std::cout << "Juego para " << escribeNombreJugador(aTenistas, indic) << "!!!" << '\n' << '\n';
	}
	aTenistas[indic].juegos++;
	return indic;
}

int set(tjugador aTenistas[]) { //Controla el desarrollo del set
	int ganador = 0, saque = rand() % 2; //Decide quien empieza sacando
	while (aTenistas[0].juegos < JUEGOS_SET && aTenistas[1].juegos < JUEGOS_SET || abs(aTenistas[0].juegos - aTenistas[1].juegos) < 2) {

		ganador = juego(aTenistas, saque);

		escribeEstadisticas(aTenistas, 0);
		escribeEstadisticas(aTenistas, 1);

		saque = (saque + 1) % 2; //Cambia el saque entre juegos

		//Reinicia los parametros puntuacion y golpes entre juegos
		aTenistas[0].puntuacion = NADA;
		aTenistas[1].puntuacion = NADA;
		for (int i = 0; i < ANCHO_PISTA + 2; i++) {
			aTenistas[0].tConteoGolpes[i] = 0;
		}
		aTenistas[0].golpesGanadores = 0;
		aTenistas[0].golpesTotales = 0;
		for (int i = 0; i < ANCHO_PISTA + 2; i++) {
			aTenistas[1].tConteoGolpes[i] = 0;
		}
		aTenistas[1].golpesGanadores = 0;
		aTenistas[1].golpesTotales = 0;

		if ((aTenistas[0].juegos < JUEGOS_SET || aTenistas[0].juegos - aTenistas[1].juegos < 2) && (aTenistas[1].juegos < JUEGOS_SET || aTenistas[1].juegos - aTenistas[0].juegos < 2)) {
			if (aTenistas[0].puntuacion != NADA || aTenistas[1].puntuacion != NADA) system("pause");
		}
	}
	std::cout << "El ganador del partido es " << escribeNombreJugador(aTenistas, ganador) << "\n\n";
	std::cout << "    " << escribeNombreJugador(aTenistas, 0) << "  " << aTenistas[0].juegos << '\n';
	std::cout << "    " << escribeNombreJugador(aTenistas, 1) << "  " << aTenistas[1].juegos << "\n\n";
	std::cout << escribeNombreJugador(aTenistas, ganador) << " se hizo con el partido\n\n" << "Gracias por jugar\n";

	return ganador;
}

void mostrar(const tListaJug& listaJug) { //Muestra los datos de todos los tenistas en formato tabla
	std::cout << "  INI  HAB  VEL   PG   PP\n";
	for (int i = 0; i < listaJug.cont; i++) {
		std::cout << std::setfill(' ') << std::setw(5) << listaJug.tenistas[i].siglas << std::setw(5) << listaJug.tenistas[i].habilidad << std::setw(5) << listaJug.tenistas[i].velocidad << std::setw(5) << listaJug.tenistas[i].partidosGanados << std::setw(5) << listaJug.tenistas[i].partidosPerdidos << '\n';
	}
}

void mostrarIniciales(const tListaJug& listaJug) { //Redacta las iniciales en fila
	std::cout << "Iniciales de los tenistas: " << listaJug.tenistas[0].siglas;
	for (int i = 1; i < listaJug.cont; i++) std::cout << " - " << listaJug.tenistas[i].siglas;
	std::cout << '\n';
}

int buscarIniciales(const tListaJug& listaJug, std::string ini) { //Devuelve la posicion del tenista buscado o pasado el limite si no existe
	int i = 0;
	bool encontrado = false;
	while (i < listaJug.cont && !encontrado) {
		if (listaJug.tenistas[i].siglas == ini) {
			encontrado = true;
		}
		else ++i;
	}
	return i;
}

bool eliminarTenista(tListaJug& listaJug, std::string siglas) { //Borra un jugador y desplaza el resto para mantener el orden
	int eliminado = buscarIniciales(listaJug, siglas);
	if (eliminado == listaJug.cont) return false;
	listaJug.cont--;
	for (int i = eliminado; i < listaJug.cont; i++) {
		listaJug.tenistas[i].siglas = listaJug.tenistas[i + 1].siglas;
		listaJug.tenistas[i].habilidad = listaJug.tenistas[i + 1].habilidad;
		listaJug.tenistas[i].velocidad = listaJug.tenistas[i + 1].velocidad;
		listaJug.tenistas[i].partidosGanados = listaJug.tenistas[i + 1].partidosGanados;
		listaJug.tenistas[i].partidosPerdidos = listaJug.tenistas[i + 1].partidosPerdidos;
	}
	return true;
}

bool cargar(tListaJug& listaJug, bool& abierto) { //Comprueba que se abrio bien el fichero,lo lee y guarda los datos de los tenistas
	std::ifstream entrada;
	int i = 0;
	entrada.open(NOM_FICHERO);
	abierto = entrada.is_open();
	if (!abierto) return false;
	while (!entrada.eof() && i < MAX_JUG) {
		inicializarJugador(listaJug.tenistas[i]);
		entrada >> listaJug.tenistas[i].siglas;
		entrada >> listaJug.tenistas[i].habilidad;
		entrada >> listaJug.tenistas[i].velocidad;
		entrada >> listaJug.tenistas[i].partidosGanados;
		entrada >> listaJug.tenistas[i].partidosPerdidos;
		i++;
	}
	listaJug.cont = i;
	if (!entrada.eof() && listaJug.cont == MAX_JUG) {
		std::cout << "\nHay mas tenistas de los maximos.\n\n";
		return false;
	}
	entrada.close();
	return true;
}

void guardar(const tListaJug& listaJug) { //Antes de cerrar el programa actualiza el fichero
	std::ofstream salida;
	salida.open(NOM_FICHERO);
	for (int i = 0; i < listaJug.cont - 1; i++) {
		salida << listaJug.tenistas[i].siglas << ' ' << listaJug.tenistas[i].habilidad << ' ' << listaJug.tenistas[i].velocidad << ' ' << listaJug.tenistas[i].partidosGanados << ' ' << listaJug.tenistas[i].partidosPerdidos << '\n';
	}
	salida << listaJug.tenistas[listaJug.cont - 1].siglas << ' ' << listaJug.tenistas[listaJug.cont - 1].habilidad << ' ' << listaJug.tenistas[listaJug.cont - 1].velocidad << ' ' << listaJug.tenistas[listaJug.cont - 1].partidosGanados << ' ' << listaJug.tenistas[listaJug.cont - 1].partidosPerdidos;
	salida.close();
}

void introducirTenista(tListaJug& listaJug) { //Añade un jugador a la lista de tenistas
	if (listaJug.cont == MAX_JUG) std::cout << "El numero de tenistas en la lista es el maximo. No puede introducir mas tenistas\n";
	else {
		tjugador añadido;
		añadido = leerDatosTenistas();
		int existe = buscarIniciales(listaJug, añadido.siglas);
		if (existe != listaJug.cont) std::cout << "Ya existe un tenista con esas siglas. Regresando al menu...\n\n";
		else {
			listaJug.tenistas[listaJug.cont] = añadido;
			++listaJug.cont;
		}
	}
}

void jugarPartido(tListaJug& listaJug) { //Controla el juego de los partidos
	std::string ini;
	int jug0, jug1;
	mostrarIniciales(listaJug);

	std::cout << "Introduce las iniciales del tenista 1: "; //Lee quien es el primer jugador
	std::cin >> ini;
	jug0 = buscarIniciales(listaJug, ini);
	while (jug0 == listaJug.cont) {
		std::cout << "No existe ningún tenista con esas iniciales\n";
		std::cout << "Introduce las iniciales del tenista 1: ";
		std::cin >> ini;
		jug0 = buscarIniciales(listaJug, ini);
	}

	std::cout << "Introduce las iniciales del tenista 2: "; //Lee quien es el segundo jugador
	std::cin >> ini;
	jug1 = buscarIniciales(listaJug, ini);
	while (jug1 == listaJug.cont || jug1 == jug0) {
		if (jug1 == listaJug.cont) std::cout << "No existe ningun tenista con esas iniciales\n";
		else if (jug1 == jug0) std::cout << "Tenista repetido. Elija otro.\n"; //Indica si el jugador esta repetido
		std::cout << "Introduce las iniciales del tenista 2: ";
		std::cin >> ini;
		jug1 = buscarIniciales(listaJug, ini);
	}

	tjugador aTenistas[2];
	aTenistas[0] = listaJug.tenistas[jug0];
	aTenistas[1] = listaJug.tenistas[jug1];

	int ganador = set(aTenistas);

	if (ganador == 0) { //Actualiza los datos de los tenistas
		listaJug.tenistas[jug0].partidosGanados++;
		listaJug.tenistas[jug1].partidosPerdidos++;
	}
	else {
		listaJug.tenistas[jug1].partidosGanados++;
		listaJug.tenistas[jug0].partidosPerdidos++;
	}
}


int menu() { //Muestra el menu
	int opcion;
	std::cout << std::setw(22) << std::setfill('-') << '\n';
	std::cout << "SIMULADOR DE TENIS V3 \n";
	std::cout << std::setw(22) << std::setfill('-') << '\n';
	std::cout << "<1> Ver datos tenistas \n";
	std::cout << "<2> Nuevo Tenista \n";
	std::cout << "<3> Eliminar tenista \n";
	std::cout << "<4> Jugar partido \n";
	std::cout << "<0> Salir \n\n";
	std::cout << "Opcion: ";
	std::cin >> opcion;
	return opcion;
}

int main() { //Main del programa que regula el resto de funciones
	srand(0);
	int opcion, continuar;
	bool cargado, abierto, fin = false;
	tListaJug listaJug;
	cargado = cargar(listaJug, abierto);
	while (!fin) { //Fichero abierto correctamente y no supera el maximo de jugadores
		if (cargado) {
			opcion = menu();
			while (opcion != 0) {
				switch (opcion) {
				case 1: {
					mostrar(listaJug);
					break;
				}
				case 2: {
					introducirTenista(listaJug);
					break;
				}
				case 3: {
					std::string ini;
					mostrarIniciales(listaJug);
					std::cout << "Introduce las iniciales del tenista a eliminar: ";
					std::cin >> ini;
					if (eliminarTenista(listaJug, ini)) {
						std::cout << "Tenista eliminado correctamente.\n";
					}
					else {
						std::cout << "No se ha podido eliminar correctamente el tenista. Volviendo al menu...\n";
					}
					break;
				}
				case 4: {
					if (listaJug.cont < 2) {
						std::cout << "No hay jugadores suficientes para simular un partido.\n";
					}
					else {
						jugarPartido(listaJug);
					}
				}
				}
				opcion = menu();
			}
			guardar(listaJug);
			fin = true;
		}
		else {
			if (!abierto) { //Fichero no abierto correctamente
				std::cout << "No se logro abrir el fichero tenistas.txt. Cerrando programa...";
				fin = true;
			}
			else { //Fichero abierto correctamente pero se supera el maximo de jugadores
				std::cout << "Si desea continuar, el programa borrara todos los jugadores exceptuando los " << MAX_JUG << " primeros del fichero.\nIntroduzca un 1 si desea continuar y un 0 si quiere cerrar el programa : ";
				std::cin >> continuar;
				if (continuar == 1) cargado = true; //Continuar guardando el maximo de jugadores permitidos y eliminando a los restantes del fichero
				if (continuar == 0) fin = true; //Cerrar el programa y conservar todos los jugadores en el fichero
			}
		}
	}
	return 0;
}

