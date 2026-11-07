#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>

const int MAX_REPOSITORIOS=10;
const int MAX_COMMITS=50;
const int MAX_CAMBIOS=20;
const int MAX_LONGITUDNOMBRE=30;
const int MIN_LONGITUDMENSAJE=10;
const int MAX_LONGITUDMENSAJE=60;

const std::string ARCHIVO_BITACORA = "bitacora.txt";
const std::string ARCHIVO_REPOSITORIOS = "repositorios.csv";
const std::string ARCHIVO_COMMITS = "commits.csv";
const std::string ARCHIVO_CAMBIOS = "cambios.csv";

void mostrarMenuPrincipal(const std::string nombreRepositorio[], int repositorioActivo){
	std::cout<<"\n        				 << MINI GIT >>\n";
	if (repositorioActivo == -1){
		std::cout << "					<<Repositorio activo: Ninguno>>\n\n";
	}
	else{
		std::cout << "					<<Repositorio activo: " << nombreRepositorio[repositorioActivo] << ">>\n\n";
	}

	std::cout<<"					<<1. Crear Repositorio>>\n";
	std::cout<<"					<<2. Listar repositorios>>\n";
	std::cout<<"					<<3. Seleccionar repositorio activo>>\n";
	std::cout<<"					<<4. Ver informacion del repositorio activo>>\n";
	std::cout<<"					<<5. Simular cambios>>\n";
	std::cout<<"					<<6. Ver cambios pendientes>>\n";
	std::cout<<"					<<7. Crear commit>>\n";
	std::cout<<"					<<8. Ver ultimo commit>>\n";
	std::cout<<"					<<9. Ver historial de commits>>\n";
	std::cout<<"					<<10. Ver estado general y estadisticas>>\n";
	std::cout<<"					<<11. Eliminar repositorio>>\n";
	std::cout<<"					<<12. Guardar datos>>\n";
	std::cout<<"					<<13. Ver bitacora>>\n";
	std::cout<<"					<<14. Salir>>\n\n";
}

void mostrarSubmenuCambios(){
	std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
	std::cout<<"          1. Agregar Archivo\n";
	std::cout<<"          2. Modificar Archivo\n";
	std::cout<<"          3. Eliminar Archivo\n";
	std::cout<<"          4. Cancelar\n\n";
}

void registrarBitacora(const std::string& evento){
	try{
		std::ofstream archivo(ARCHIVO_BITACORA, std::ios::app);
		if (!archivo.is_open()){
			throw std::runtime_error("No se pudo abrir " + ARCHIVO_BITACORA);
		}
		archivo << evento << "\n";
		archivo.close();
	}
	catch (const std::exception&){
	}
}

bool soloEspacios(const std::string& texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i = i + 1){
		if (texto[i] != ' ' && texto[i] != '\t'){
			return false;
		}
	}
	return true;
}

bool contienePuntoYComa(const std::string& texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i = i + 1){
		if (texto[i] == ';'){
			return true;
		}
	}
	return false;
}

int largoSinEspaciosExtremos(const std::string& texto){
	int inicio = 0;
	int fin = texto.length() - 1;

	while (inicio <= fin && (texto[inicio] == ' ' || texto[inicio] == '\t')){
		inicio = inicio + 1;
	}
	while (fin >= inicio && (texto[fin] == ' ' || texto[fin] == '\t')){
		fin = fin - 1;
	}
	return fin - inicio + 1;
}

std::string leerTextoValido(const std::string& etiqueta, int minLongitud, int maxLongitud){
	std::string texto;
	bool valido = false;

	while (!valido){
		std::cout << etiqueta;
		std::getline(std::cin, texto);
		int largo = texto.length();

		if (soloEspacios(texto)){
			std::cout << "El texto no puede estar vacio ni tener solo espacios.\n";
		}
		else if (contienePuntoYComa(texto)){
			std::cout << "El texto no puede contener el caracter ';'.\n";
		}
		else if (largo > maxLongitud){
			std::cout << "El texto debe tener entre " << minLongitud << " y " << maxLongitud << " caracteres.\n";
		}
		else if (largoSinEspaciosExtremos(texto) < minLongitud){
			std::cout << "El texto debe tener entre " << minLongitud << " y " << maxLongitud
			          << " caracteres (los espacios al inicio o al final no cuentan).\n";
		}
		else {
			valido = true;
		}
	}
	return texto;
}

bool todosDigitos(const std::string& texto){
	int largo = texto.length();
	if (largo == 0){
		return false;
	}
	for (int i = 0; i < largo; i = i + 1){
		if (texto[i] < '0' || texto[i] > '9'){
			return false;
		}
	}
	return true;
}

int leerNumeroValidado(const std::string& etiqueta, int minvalido, int maxvalido){
	std::string texto;
	int valor = 0;
	bool valido = false;

	while (!valido){
		std::cout << etiqueta;
		std::getline(std::cin, texto);

		if (todosDigitos(texto)){
			try{
				valor = std::stoi(texto);
				if (valor >= minvalido && valor <= maxvalido){
					valido = true;
				}
			}
			catch (const std::exception& error){
				valido = false;
				registrarBitacora("ERROR | Conversion de numero | Valor: " + texto + " | No se pudo convertir (" + std::string(error.what()) + ")");
			}
		}

		if (!valido){
			std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
		}
	}
	return valor;
}

int buscarRepositorio(const std::string nombreRepositorio[], int cantidadRepositorios, const std::string& nombreBuscado){
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		if (nombreRepositorio[i] == nombreBuscado){
			return i;
		}
	}
	return -1;
}

std::string nombreTipoCambio(char tipo){
	if (tipo == 'A'){
		return "Agregado";
	}
	if (tipo == 'M'){
		return "Modificado";
	}
	if (tipo == 'E'){
		return "Eliminado";
	}
	return "Desconocido";
}

bool existeCambioPendiente(const std::string archivosCambios[][MAX_CAMBIOS],
                           const char tipoCambioPendiente[][MAX_CAMBIOS],
                           const int cantidadCambiosPendientes[],
                           int repositorio, char tipo, const std::string& archivo){
	for (int j = 0; j < cantidadCambiosPendientes[repositorio]; j = j + 1){
		if (tipoCambioPendiente[repositorio][j] == tipo && archivosCambios[repositorio][j] == archivo){
			return true;
		}
	}
	return false;
}

void crearRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[],
                      int cantidadCommits[], int cantidadCambiosPendientes[],
                      int& cantidadRepositorios, int& repositorioActivo){

	if (cantidadRepositorios >= MAX_REPOSITORIOS){
		std::cout << "Se alcanzo el maximo de " << MAX_REPOSITORIOS << " repositorios.\n\n";
		return;
	}

	std::string nombre = leerTextoValido("Nombre del repositorio: ", 1, MAX_LONGITUDNOMBRE);

	if (buscarRepositorio(nombreRepositorio, cantidadRepositorios, nombre) != -1){
		std::cout << "\nYa existe un repositorio con el nombre " << nombre << ".\n\n";
		return;
	}

	std::string autor = leerTextoValido("Nombre del autor: ", 1, MAX_LONGITUDNOMBRE);

	int posicion = cantidadRepositorios;
	nombreRepositorio[posicion] = nombre;
	autorRepositorio[posicion] = autor;
	cantidadCommits[posicion] = 0;
	cantidadCambiosPendientes[posicion] = 0;

	cantidadRepositorios = cantidadRepositorios + 1;
	repositorioActivo = posicion;

	registrarBitacora("CREAR_REPOSITORIO | " + nombre + " | Autor: " + autor);

	std::cout << "\nRepositorio " << nombre << " creado correctamente.\n\n";
}

void listarRepositorios(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                        const int cantidadCommits[], const int cantidadCambiosPendientes[],
                        int cantidadRepositorios, int repositorioActivo){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return;
	}

	std::cout << "  " << std::left
	          << std::setw(5)  << "No."
	          << std::setw(32) << "Repositorio"
	          << std::setw(32) << "Autor"
	          << std::setw(9)  << "Commits"
	          << "Pendientes\n";

	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		if (i == repositorioActivo){
			std::cout << "* ";
		}
		else{
			std::cout << "  ";
		}
		std::cout << std::left
		          << std::setw(5)  << (i + 1)
		          << std::setw(32) << nombreRepositorio[i]
		          << std::setw(32) << autorRepositorio[i]
		          << std::setw(9)  << cantidadCommits[i]
		          << cantidadCambiosPendientes[i] << "\n";
	}
	std::cout << "\n";
}

void seleccionarRepositorio(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                            const int cantidadCommits[], const int cantidadCambiosPendientes[],
                            int cantidadRepositorios, int& repositorioActivo){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return;
	}

	listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits,
	                   cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);

	int numero = leerNumeroValidado("Numero del repositorio (0 para cancelar): ", 0, cantidadRepositorios);

	if (numero == 0){
		std::cout << "\nOperacion cancelada. El repositorio activo no cambio.\n\n";
		return;
	}

	repositorioActivo = numero - 1;
	std::cout << "\nRepositorio activo: " << nombreRepositorio[repositorioActivo] << ".\n\n";
}

void mostrarInformacion(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                        const int cantidadCommits[], const int cantidadCambiosPendientes[],
                        int repositorioActivo){

	std::cout << "Repositorio: " << nombreRepositorio[repositorioActivo] << "\n";
	std::cout << "Autor: " << autorRepositorio[repositorioActivo] << "\n";
	std::cout << "Estado: Inicializado\n";
	std::cout << "Cantidad de commits: " << cantidadCommits[repositorioActivo] << "\n";

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "Ultimo commit: Ninguno\n";
	}
	else{
		std::cout << "Ultimo commit: #" << cantidadCommits[repositorioActivo] << "\n";
	}

	if (cantidadCambiosPendientes[repositorioActivo] == 0){
		std::cout << "Cambios pendientes: No\n\n";
	}
	else{
		std::cout << "Cambios pendientes: Si (" << cantidadCambiosPendientes[repositorioActivo] << ")\n\n";
	}
}

void simularCambio(const std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS],
                   char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[],
                   int repositorioActivo){

	mostrarSubmenuCambios();
	int opcionCambio = leerNumeroValidado("		Seleccione una opcion: ", 1, 4);

	if (opcionCambio == 4){
		std::cout << "\nOperacion cancelada. No se realizaron cambios.\n\n";
		return;
	}

	if (cantidadCambiosPendientes[repositorioActivo] >= MAX_CAMBIOS){
		std::cout << "\nSe alcanzo el maximo de " << MAX_CAMBIOS << " cambios pendientes para este repositorio.\n\n";
		return;
	}

	char tipo = 'A';
	if (opcionCambio == 2){
		tipo = 'M';
	}
	else if (opcionCambio == 3){
		tipo = 'E';
	}

	std::string archivo = leerTextoValido("Nombre del archivo (ejemplo: main.cpp): ", 1, MAX_LONGITUDNOMBRE);

	if (existeCambioPendiente(archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes,
	                          repositorioActivo, tipo, archivo)){
		std::cout << "\nEse cambio ya esta pendiente: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
		return;
	}

	int posicion = cantidadCambiosPendientes[repositorioActivo];
	tipoCambioPendiente[repositorioActivo][posicion] = tipo;
	archivosCambios[repositorioActivo][posicion] = archivo;
	cantidadCambiosPendientes[repositorioActivo] = cantidadCambiosPendientes[repositorioActivo] + 1;

	registrarBitacora("SIMULAR_CAMBIO | " + nombreRepositorio[repositorioActivo] + " | " + std::string(1, tipo) + " " + archivo);

	std::cout << "\nCambio registrado: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
}

void mostrarCambiosPendientes(const std::string nombreRepositorio[], const std::string archivosCambios[][MAX_CAMBIOS],
                              const char tipoCambioPendiente[][MAX_CAMBIOS],
                              const int cantidadCambiosPendientes[], int repositorioActivo){

	if (cantidadCambiosPendientes[repositorioActivo] == 0){
		std::cout << "No hay cambios pendientes.\n\n";
		return;
	}

	std::cout << "Cambios pendientes de " << nombreRepositorio[repositorioActivo] << ":\n";

	for (int j = 0; j < cantidadCambiosPendientes[repositorioActivo]; j = j + 1){
		std::cout << (j + 1) << ". [" << nombreTipoCambio(tipoCambioPendiente[repositorioActivo][j]) << "] "
		          << archivosCambios[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

void crearCommit(const std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS],
                 std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS],
                 int cantidadCommits[], int cantidadCambiosPendientes[], int repositorioActivo){

	if (cantidadCambiosPendientes[repositorioActivo] == 0){
		std::cout << "No existen cambios para guardar.\n\n";
		return;
	}

	if (cantidadCommits[repositorioActivo] >= MAX_COMMITS){
		std::cout << "Se alcanzo el maximo de " << MAX_COMMITS << " commits para este repositorio.\n\n";
		return;
	}

	std::string mensaje = leerTextoValido("Ingrese el mensaje del commit: ", MIN_LONGITUDMENSAJE, MAX_LONGITUDMENSAJE);

	int columna = cantidadCommits[repositorioActivo];
	mensajesCommits[repositorioActivo][columna] = mensaje;
	cantidadCommits[repositorioActivo] = cantidadCommits[repositorioActivo] + 1;
	int numeroCommit = cantidadCommits[repositorioActivo];

	int cambiosIncluidos = cantidadCambiosPendientes[repositorioActivo];
	for (int j = 0; j < cambiosIncluidos; j = j + 1){
		archivosCambios[repositorioActivo][j] = "";
		tipoCambioPendiente[repositorioActivo][j] = ' ';
	}
	cantidadCambiosPendientes[repositorioActivo] = 0;

	registrarBitacora("CREAR_COMMIT | " + nombreRepositorio[repositorioActivo] + " | Commit #" + std::to_string(numeroCommit));

	std::cout << "\nCommit #" << numeroCommit << " creado correctamente. ";
	if (cambiosIncluidos == 1){
		std::cout << "Se incluyo 1 cambio.\n\n";
	}
	else{
		std::cout << "Se incluyeron " << cambiosIncluidos << " cambios.\n\n";
	}
}

void mostrarUltimoCommit(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                         const std::string mensajesCommits[][MAX_COMMITS], const int cantidadCommits[],
                         int repositorioActivo){

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "El repositorio " << nombreRepositorio[repositorioActivo] << " aun no tiene commits.\n\n";
		return;
	}

	int ultimaColumna = cantidadCommits[repositorioActivo] - 1;

	std::cout << "Commit #" << cantidadCommits[repositorioActivo] << "\n";
	std::cout << "Repositorio: " << nombreRepositorio[repositorioActivo] << "\n";
	std::cout << "Autor: " << autorRepositorio[repositorioActivo] << "\n";
	std::cout << "Mensaje: " << mensajesCommits[repositorioActivo][ultimaColumna] << "\n";
	std::cout << "Estado: Guardado correctamente\n\n";
}

void mostrarHistorial(const std::string nombreRepositorio[], const std::string mensajesCommits[][MAX_COMMITS],
                      const int cantidadCommits[], int repositorioActivo){

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "El repositorio " << nombreRepositorio[repositorioActivo] << " aun no tiene commits.\n\n";
		return;
	}

	std::cout << "Historial de " << nombreRepositorio[repositorioActivo]
	          << " (" << cantidadCommits[repositorioActivo];
	if (cantidadCommits[repositorioActivo] == 1){
		std::cout << " commit)\n";
	}
	else{
		std::cout << " commits)\n";
	}

	for (int j = cantidadCommits[repositorioActivo] - 1; j >= 0; j = j - 1){
		std::cout << "#" << (j + 1) << " - " << mensajesCommits[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

void mostrarEstadoGeneral(const std::string nombreRepositorio[], const int cantidadCommits[],
                          const int cantidadCambiosPendientes[], int cantidadRepositorios){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados. No se pueden mostrar estadisticas.\n\n";
		return;
	}

	std::cout << std::left
	          << std::setw(32) << "Repositorio"
	          << std::setw(42) << "Estado"
	          << "Ultimo commit\n";

	int totalCommits = 0;
	int posicionMayor = 0;
	int reposConCambios = 0;

	for (int i = 0; i < cantidadRepositorios; i = i + 1){

		std::string estado = "Inicializado | ";
		if (cantidadCambiosPendientes[i] > 0){
			estado = estado + "Cambios pendientes (" + std::to_string(cantidadCambiosPendientes[i]) + ")";
			reposConCambios = reposConCambios + 1;
		}
		else{
			estado = estado + "Sin cambios pendientes";
		}

		std::string ultimoCommit = "Ninguno";
		if (cantidadCommits[i] > 0){
			ultimoCommit = "#" + std::to_string(cantidadCommits[i]);
		}

		std::cout << std::setw(32) << nombreRepositorio[i]
		          << std::setw(42) << estado
		          << ultimoCommit << "\n";

		totalCommits = totalCommits + cantidadCommits[i];

		if (cantidadCommits[i] > cantidadCommits[posicionMayor]){
			posicionMayor = i;
		}
	}

	double promedio = totalCommits;
	promedio = promedio / cantidadRepositorios;

	std::cout << "\nTotal de repositorios registrados: " << cantidadRepositorios << "\n";
	std::cout << "Total de commits: " << totalCommits << "\n";
	std::cout << "Promedio de commits por repositorio: " << std::fixed << std::setprecision(2) << promedio << "\n";
	std::cout << "Repositorio con mas commits: " << nombreRepositorio[posicionMayor]
	          << " (" << cantidadCommits[posicionMayor] << ")\n";
	std::cout << "Repositorios con cambios pendientes: " << reposConCambios << "\n\n";
}

bool confirmarAccion(const std::string& pregunta){
	std::cout << pregunta << "\n";
	std::cout << "1. Si\n";
	std::cout << "2. No\n";
	int respuesta = leerNumeroValidado("Seleccione una opcion: ", 1, 2);
	return respuesta == 1;
}

void desplazarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[],
                           int cantidadCommits[], int cantidadCambiosPendientes[],
                           std::string mensajesCommits[][MAX_COMMITS],
                           std::string archivosCambios[][MAX_CAMBIOS],
                           char tipoCambioPendiente[][MAX_CAMBIOS],
                           int& cantidadRepositorios, int posicion){

	for (int i = posicion; i < cantidadRepositorios - 1; i = i + 1){
		nombreRepositorio[i] = nombreRepositorio[i + 1];
		autorRepositorio[i] = autorRepositorio[i + 1];
		cantidadCommits[i] = cantidadCommits[i + 1];
		cantidadCambiosPendientes[i] = cantidadCambiosPendientes[i + 1];

		for (int j = 0; j < MAX_COMMITS; j = j + 1){
			mensajesCommits[i][j] = mensajesCommits[i + 1][j];
		}
		for (int j = 0; j < MAX_CAMBIOS; j = j + 1){
			archivosCambios[i][j] = archivosCambios[i + 1][j];
			tipoCambioPendiente[i][j] = tipoCambioPendiente[i + 1][j];
		}
	}

	int ultima = cantidadRepositorios - 1;
	nombreRepositorio[ultima] = "";
	autorRepositorio[ultima] = "";
	cantidadCommits[ultima] = 0;
	cantidadCambiosPendientes[ultima] = 0;
	for (int j = 0; j < MAX_COMMITS; j = j + 1){
		mensajesCommits[ultima][j] = "";
	}
	for (int j = 0; j < MAX_CAMBIOS; j = j + 1){
		archivosCambios[ultima][j] = "";
		tipoCambioPendiente[ultima][j] = ' ';
	}

	cantidadRepositorios = cantidadRepositorios - 1;
}

void eliminarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[],
                         int cantidadCommits[], int cantidadCambiosPendientes[],
                         std::string mensajesCommits[][MAX_COMMITS],
                         std::string archivosCambios[][MAX_CAMBIOS],
                         char tipoCambioPendiente[][MAX_CAMBIOS],
                         int& cantidadRepositorios, int& repositorioActivo){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return;
	}

	listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits,
	                   cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);

	int numero = leerNumeroValidado("Numero del repositorio a eliminar (0 para cancelar): ", 0, cantidadRepositorios);

	if (numero == 0){
		std::cout << "\nOperacion cancelada. No se elimino ningun repositorio.\n\n";
		return;
	}

	int posicion = numero - 1;
	std::string nombre = nombreRepositorio[posicion];

	std::cout << "\n";
	if (!confirmarAccion("Desea eliminar el repositorio " + nombre + "?")){
		std::cout << "\nOperacion cancelada. El repositorio " << nombre << " no fue eliminado.\n\n";
		return;
	}

	desplazarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                      mensajesCommits, archivosCambios, tipoCambioPendiente,
	                      cantidadRepositorios, posicion);

	repositorioActivo = -1;

	registrarBitacora("ELIMINAR_REPOSITORIO | " + nombre);

	std::cout << "\nRepositorio " << nombre << " eliminado correctamente.\n\n";
}

void abrirParaEscribir(std::ofstream& archivo, const std::string& nombreArchivo){
	archivo.open(nombreArchivo);
	if (!archivo.is_open()){
		throw std::runtime_error("No se pudo abrir " + nombreArchivo + " para escritura.");
	}
}

void escribirRepositorios(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                          int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_REPOSITORIOS);

	archivo << "nombre;autor\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		archivo << nombreRepositorio[i] << ";" << autorRepositorio[i] << "\n";
	}
	archivo.close();
}

void escribirCommits(const std::string nombreRepositorio[], const std::string mensajesCommits[][MAX_COMMITS],
                     const int cantidadCommits[], int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_COMMITS);

	archivo << "repositorio;numero;mensaje\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		for (int j = 0; j < cantidadCommits[i]; j = j + 1){
			archivo << nombreRepositorio[i] << ";" << (j + 1) << ";" << mensajesCommits[i][j] << "\n";
		}
	}
	archivo.close();
}

void escribirCambios(const std::string nombreRepositorio[], const std::string archivosCambios[][MAX_CAMBIOS],
                     const char tipoCambioPendiente[][MAX_CAMBIOS], const int cantidadCambiosPendientes[],
                     int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_CAMBIOS);

	archivo << "repositorio;tipo;archivo\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		for (int j = 0; j < cantidadCambiosPendientes[i]; j = j + 1){
			archivo << nombreRepositorio[i] << ";" << tipoCambioPendiente[i][j] << ";" << archivosCambios[i][j] << "\n";
		}
	}
	archivo.close();
}

bool guardarDatos(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                  const int cantidadCommits[], const int cantidadCambiosPendientes[],
                  const std::string mensajesCommits[][MAX_COMMITS],
                  const std::string archivosCambios[][MAX_CAMBIOS],
                  const char tipoCambioPendiente[][MAX_CAMBIOS],
                  int cantidadRepositorios){
	try{
		escribirRepositorios(nombreRepositorio, autorRepositorio, cantidadRepositorios);
		escribirCommits(nombreRepositorio, mensajesCommits, cantidadCommits, cantidadRepositorios);
		escribirCambios(nombreRepositorio, archivosCambios, tipoCambioPendiente,
		                cantidadCambiosPendientes, cantidadRepositorios);
	}
	catch (const std::exception& error){
		std::cout << "Error al guardar los datos: " << error.what() << "\n";
		std::cout << "Los datos siguen en memoria; puede intentar guardar de nuevo.\n\n";
		registrarBitacora("ERROR | GUARDAR_DATOS | " + std::string(error.what()));
		return false;
	}

	registrarBitacora("GUARDAR_DATOS | Datos guardados correctamente");
	std::cout << "Datos guardados correctamente.\n\n";
	return true;
}
	void mostrarBitacora(){
	std::ifstream archivo(ARCHIVO_BITACORA);

	if (!archivo.is_open()){
		std::cout << "La bitacora esta vacia.\n\n";
		return;
	}

	std::string linea;
	bool hayLineas = false;

	while (std::getline(archivo, linea)){
		std::cout << linea << "\n";
		hayLineas = true;
	}
	archivo.close();

	if (!hayLineas){
		std::cout << "La bitacora esta vacia.\n";
	}
	std::cout << "\n";
}

bool confirmarSalida(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                     const int cantidadCommits[], const int cantidadCambiosPendientes[],
                     const std::string mensajesCommits[][MAX_COMMITS],
                     const std::string archivosCambios[][MAX_CAMBIOS],
                     const char tipoCambioPendiente[][MAX_CAMBIOS],
                     int cantidadRepositorios){

	bool guardado = guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                             mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);

	if (guardado){
		return true;
	}

	std::cout << "ADVERTENCIA: los datos no se pudieron guardar. Si sale ahora se perderan los cambios de esta sesion.\n\n";
	return confirmarAccion("Desea salir de todas formas?");
}
int main(){
	int cantidadCommits[MAX_REPOSITORIOS];
	int cantidadCambiosPendientes[MAX_REPOSITORIOS];
	std::string nombreRepositorio[MAX_REPOSITORIOS];
	std::string autorRepositorio[MAX_REPOSITORIOS];

	std::string mensajesCommits[MAX_REPOSITORIOS][MAX_COMMITS];
	std::string archivosCambios[MAX_REPOSITORIOS][MAX_CAMBIOS];
	char tipoCambioPendiente[MAX_REPOSITORIOS][MAX_CAMBIOS];

	int cantidadRepositorios=0;
	int repositorioActivo=-1;
	int opcion=0;

	for (int i=0; i < MAX_REPOSITORIOS; i=i+1){
		cantidadCommits[i]=0;
		cantidadCambiosPendientes[i]=0;
		for (int j=0; j < MAX_CAMBIOS; j=j+1){
			tipoCambioPendiente[i][j]=' ';
		}
	}

	do {
		system("cls");
		mostrarMenuPrincipal(nombreRepositorio, repositorioActivo);
		opcion=leerNumeroValidado("					Seleccione una opcion: ", 1, 14);

		if (opcion >= 4 && opcion <= 9 && repositorioActivo == -1){
			std::cout<<"\nDebe seleccionar un repositorio activo antes de realizar esta operacion.\n";
			system("pause");
			continue;
		}

		switch(opcion){

			case 1:
				system("cls");
				std::cout<<"Opcion digitada: 1\nEntrando a: Crear repositorio\n\n";
				crearRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits,
				                 cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);
				system("pause");
				break;

			case 2:
				system("cls");
				std::cout<<"Opcion digitada: 2\nEntrando a: Listar repositorios\n\n";
				listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits,
				                   cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);
				system("pause");
				break;

			case 3:
				system("cls");
				std::cout<<"Opcion digitada: 3\nEntrando a: Seleccionar repositorio activo\n\n";
				seleccionarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits,
				                       cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);
				system("pause");
				break;

			case 4:
				system("cls");
				std::cout<<"Opcion digitada: 4\nEntrando a: Ver informacion del repositorio activo\n\n";
				mostrarInformacion(nombreRepositorio, autorRepositorio, cantidadCommits,
				                   cantidadCambiosPendientes, repositorioActivo);
				system("pause");
				break;

			case 5:
				system("cls");
				std::cout<<"Opcion digitada: 5\nEntrando a: Simular cambios\n\n";
				simularCambio(nombreRepositorio, archivosCambios, tipoCambioPendiente,
				              cantidadCambiosPendientes, repositorioActivo);
				system("pause");
				break;

			case 6:
				system("cls");
				std::cout<<"Opcion digitada: 6\nEntrando a: Ver cambios pendientes\n\n";
				mostrarCambiosPendientes(nombreRepositorio, archivosCambios, tipoCambioPendiente,
				                         cantidadCambiosPendientes, repositorioActivo);
				system("pause");
				break;

			case 7:
				system("cls");
				std::cout<<"Opcion digitada: 7\nEntrando a: Crear commit\n\n";
				crearCommit(nombreRepositorio, mensajesCommits, archivosCambios, tipoCambioPendiente,
				            cantidadCommits, cantidadCambiosPendientes, repositorioActivo);
				system("pause");
				break;

			case 8:
				system("cls");
				std::cout<<"Opcion digitada: 8\nEntrando a: Ver ultimo commit\n\n";
				mostrarUltimoCommit(nombreRepositorio, autorRepositorio, mensajesCommits,
				                    cantidadCommits, repositorioActivo);
				system("pause");
				break;

			case 9:
				system("cls");
				std::cout<<"Opcion digitada: 9\nEntrando a: Ver historial de commits\n\n";
				mostrarHistorial(nombreRepositorio, mensajesCommits, cantidadCommits, repositorioActivo);
				system("pause");
				break;

			case 10:
				system("cls");
				std::cout<<"Opcion digitada: 10\nEntrando a: Ver estado general y estadisticas\n\n";
				mostrarEstadoGeneral(nombreRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios);
				system("pause");
				break;

			case 11:
				system("cls");
				std::cout<<"Opcion digitada: 11\nEntrando a: Eliminar repositorio\n\n";
				eliminarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				                    mensajesCommits, archivosCambios, tipoCambioPendiente,
				                    cantidadRepositorios, repositorioActivo);
				system("pause");
				break;

			case 12:
				system("cls");
				std::cout<<"Opcion digitada: 12\nEntrando a: Guardar datos\n\n";
				guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				             mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);
				system("pause");
				break;

			case 13:
				system("cls");
				std::cout<<"Opcion digitada: 13\nEntrando a: Ver bitacora\n\n";
				mostrarBitacora();
				system("pause");
				break;

			case 14:
				system("cls");
				std::cout<<"Saliendo....\n";
				if (confirmarSalida(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				               		mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios)){
					std::cout<<"Saliendo....\n\n";
				}
				else{
					opcion = 0;   
				}
				system("pause");
				break;
		}

	} while (opcion != 14);

	return 0;
}
