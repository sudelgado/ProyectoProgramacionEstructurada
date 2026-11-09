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

// Encabezados de CVV. Se usarán al guardar y al validar la carga.
const std::string ENCABEZADO_REPOSITORIOS = "nombre;autor";
const std::string ENCABEZADO_COMMITS = "repositorio;numero;mensaje";
const std::string ENCABEZADO_CAMBIOS = "repositorio;tipo;archivo";


void mostrarMenuPrincipal(std::string nombreRepositorio[], int repositorioActivo){
	std::cout<<"\n        				 << MINI GIT >>\n";
	std::cout << "					<<Repositorio activo: "
	          << (repositorioActivo == -1 ? "Ninguno" : nombreRepositorio[repositorioActivo]) << ">>\n\n";

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

void registrarBitacora(std::string evento){
	try{
		std::ofstream archivo(ARCHIVO_BITACORA, std::ios::app);
		if (!archivo.is_open()){
			throw std::runtime_error("No se pudo abrir " + ARCHIVO_BITACORA);
		}
		archivo << evento << "\n";
		archivo.close();
	}
	catch (std::runtime_error){ // El catch queda vacio si no puede escribir a proposito, así el programa continua
	}
}

bool soloEspacios(std::string texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i++){
		if (texto[i] != ' ' && texto[i] != '\t'){ // \t representa que el usuario escribió tab, que cuenta como texto vacío
			return false;
		}
	}
	return true;
}

bool contienePuntoYComa(std::string texto){ // evitar que el texto tenga ;
	int largo = texto.length();
	for (int i = 0; i < largo; i++){
		if (texto[i] == ';'){
			return true;
		}
	}
	return false;
}

std::string errorDeTexto(std::string texto, int minLongitud, int maxLongitud){
	int largo = texto.length();

	if (soloEspacios(texto)){
		return "El texto no puede estar vacio ni tener solo espacios.";
	}
	if (contienePuntoYComa(texto)){
		return "El texto no puede contener el caracter ';'.";
	}
	if (largo < minLongitud || largo > maxLongitud){
		return "El texto debe tener entre " + std::to_string(minLongitud) + " y " + std::to_string(maxLongitud) + " caracteres.";
	}
	return "";
}

std::string leerTextoValido(std::string etiqueta, int minLongitud, int maxLongitud){ // Lee texto del teclado y lo vuelve a pedir hasta que sea valido 
	std::string texto;
	while (true){
		std::cout << etiqueta;
		std::getline(std::cin, texto);
		std::string error = errorDeTexto(texto, minLongitud, maxLongitud);
		if (error == ""){
			return texto;
		}
		std::cout << error << "\n";
	}
}

bool todosDigitos(std::string texto){// Rechaza 12abc, -9, 4.4 y el texto vacio antes de intentar convertir.
	int largo = texto.length();
	if (largo == 0){
		return false;
	}
	for (int i = 0; i < largo; i++){
		if (texto[i] < '0' || texto[i] > '9'){
			return false;
		}
	}
	return true;
}

int convertirEntero(std::string texto){ // Convierte un texto a entero con try/catch 
	if (!todosDigitos(texto)){
		return -1;
	}
	try{
		return std::stoi(texto);
	}
	catch (std::out_of_range error){       // el numero es demasiado grande para un int
		registrarBitacora("ERROR | Conversion de numero | Valor: " + texto + " | No se pudo convertir (" + std::string(error.what()) + ")");
		return -1;
	}
	catch (std::invalid_argument error){   // el texto no es un numero
		registrarBitacora("ERROR | Conversion de numero | Valor: " + texto + " | No se pudo convertir (" + std::string(error.what()) + ")");
		return -1;
	}
}
int leerNumeroValidado(std::string etiqueta, int minvalido, int maxvalido){
	std::string texto;
	while (true){
		std::cout << etiqueta;
		std::getline(std::cin, texto);
		int valor = convertirEntero(texto);   
		if (valor != -1 && valor >= minvalido && valor <= maxvalido){
			return valor;
		}
		std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
	}
}

int buscarRepositorio(std::string nombreRepositorio[], int cantidadRepositorios, std::string nombreBuscado){
	for (int i = 0; i < cantidadRepositorios; i++){
		if (nombreRepositorio[i] == nombreBuscado){
			return i;
		}
	}
	return -1;
}

std::string nombreTipoCambio(char tipo){ // Convierte lo guardado en la matriz ('A', 'M', 'E') en texto para mostrar al usuario
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

bool existeCambioPendiente(std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[], int repositorio, char tipo, std::string archivo){
	for (int j = 0; j < cantidadCambiosPendientes[repositorio]; j++){
		if (tipoCambioPendiente[repositorio][j] == tipo && archivosCambios[repositorio][j] == archivo){ 
			return true;
		}
	}
	return false;
}

int crearRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios){ // Opcion 1

	if (cantidadRepositorios >= MAX_REPOSITORIOS){
		std::cout << "Se alcanzo el maximo de " << MAX_REPOSITORIOS << " repositorios.\n\n";
		return -1;
	}

	std::string nombre = leerTextoValido("Nombre del repositorio: ", 1, MAX_LONGITUDNOMBRE);

	if (buscarRepositorio(nombreRepositorio, cantidadRepositorios, nombre) != -1){
		std::cout << "\nYa existe un repositorio con el nombre " << nombre << ".\n\n";
		return -1;
	}

	std::string autor = leerTextoValido("Nombre del autor: ", 1, MAX_LONGITUDNOMBRE);

	int posicion = cantidadRepositorios;
	nombreRepositorio[posicion] = nombre;
	autorRepositorio[posicion] = autor;
	cantidadCommits[posicion] = 0;
	cantidadCambiosPendientes[posicion] = 0;

	registrarBitacora("CREAR_REPOSITORIO | " + nombre + " | Autor: " + autor);

	std::cout << "\nRepositorio " << nombre << " creado correctamente.\n\n";
	return posicion;
}
 
void listarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios, int repositorioActivo){ // muestra la tabla de repositorios.

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return;
	}

	std::cout << "  " << std::left << std::setw(5) << "No." << std::setw(32) << "Repositorio"
	          << std::setw(32) << "Autor" << std::setw(9) << "Commits" << "Pendientes\n";

	for (int i = 0; i < cantidadRepositorios; i++){
		std::cout << (i == repositorioActivo ? "* " : "  ") << std::left << std::setw(5) << (i + 1)
		          << std::setw(32) << nombreRepositorio[i] << std::setw(32) << autorRepositorio[i]
		          << std::setw(9) << cantidadCommits[i] << cantidadCambiosPendientes[i] << "\n";
	}
	std::cout << "\n";
}

int pedirNumeroRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios, int repositorioActivo, std::string etiqueta, std::string mensajeCancelar){
	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return -1;
	}
	listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);

	int numero = leerNumeroValidado(etiqueta, 0, cantidadRepositorios);
	if (numero == 0){
		std::cout << "\n" << mensajeCancelar << "\n\n";
		return -1;
	}
	return numero - 1;   
}


int seleccionarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios, int repositorioActivo){
	int posicion = pedirNumeroRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo, "Numero del repositorio (0 para cancelar): ", "Operacion cancelada. El repositorio activo no cambio.");
	if (posicion == -1){
		return repositorioActivo;
	}
	std::cout << "\nRepositorio activo: " << nombreRepositorio[posicion] << ".\n\n";
	return posicion;
}

void mostrarInformacion(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int repositorioActivo){

	int commits = cantidadCommits[repositorioActivo];
	int pendientes = cantidadCambiosPendientes[repositorioActivo];

	std::cout << "Repositorio: " << nombreRepositorio[repositorioActivo] << "\n";
	std::cout << "Autor: " << autorRepositorio[repositorioActivo] << "\n";
	std::cout << "Estado: Inicializado\n";
	std::cout << "Cantidad de commits: " << commits << "\n";
	std::cout << "Ultimo commit: " << (commits == 0 ? "Ninguno" : "#" + std::to_string(commits)) << "\n";
	std::cout << "Cambios pendientes: " << (pendientes == 0 ? "No" : "Si (" + std::to_string(pendientes) + ")") << "\n\n";
}

void simularCambio(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[], int repositorioActivo){

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

	char tipo = 'A'; 	// Opcion 1 = Agregado, 2 = Modificado, 3 = Eliminado
	if (opcionCambio == 2){
		tipo = 'M';
	}
	else if (opcionCambio == 3){
		tipo = 'E';
	}

	std::string archivo = leerTextoValido("Nombre del archivo (ejemplo: main.cpp): ", 1, MAX_LONGITUDNOMBRE);

	if (existeCambioPendiente(archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes, repositorioActivo, tipo, archivo)){
		std::cout << "\nEse cambio ya esta pendiente: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
		return;
	}

	int posicion = cantidadCambiosPendientes[repositorioActivo];
	tipoCambioPendiente[repositorioActivo][posicion] = tipo;
	archivosCambios[repositorioActivo][posicion] = archivo;
	cantidadCambiosPendientes[repositorioActivo]++;

	registrarBitacora("SIMULAR_CAMBIO | " + nombreRepositorio[repositorioActivo] + " | " + std::string(1, tipo) + " " + archivo);

	std::cout << "\nCambio registrado: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
}

void mostrarCambiosPendientes(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[], int repositorioActivo){

	if (cantidadCambiosPendientes[repositorioActivo] == 0){
		std::cout << "No hay cambios pendientes.\n\n";
		return;
	}

	std::cout << "Cambios pendientes de " << nombreRepositorio[repositorioActivo] << ":\n";

	for (int j = 0; j < cantidadCambiosPendientes[repositorioActivo]; j++){
		std::cout << (j + 1) << ". [" << nombreTipoCambio(tipoCambioPendiente[repositorioActivo][j]) << "] " << archivosCambios[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

void crearCommit(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCommits[], int cantidadCambiosPendientes[], int repositorioActivo){

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
	cantidadCommits[repositorioActivo]++;
	int numeroCommit = cantidadCommits[repositorioActivo];

	int cambiosIncluidos = cantidadCambiosPendientes[repositorioActivo]; // Se limpian los cambios pendientes de este repositorio 
	for (int j = 0; j < cambiosIncluidos; j++){
		archivosCambios[repositorioActivo][j] = "";
		tipoCambioPendiente[repositorioActivo][j] = ' ';
	}
	cantidadCambiosPendientes[repositorioActivo] = 0;

	registrarBitacora("CREAR_COMMIT | " + nombreRepositorio[repositorioActivo] + " | Commit #" + std::to_string(numeroCommit));

	std::cout << "\nCommit #" << numeroCommit << " creado correctamente. "  << (cambiosIncluidos == 1 ? "Se incluyo 1 cambio." : "Se incluyeron " + std::to_string(cambiosIncluidos) + " cambios.") << "\n\n";
}

void mostrarUltimoCommit(std::string nombreRepositorio[], std::string autorRepositorio[], std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[], int repositorioActivo){

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

void mostrarHistorial(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[], int repositorioActivo){

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "El repositorio " << nombreRepositorio[repositorioActivo] << " aun no tiene commits.\n\n";
		return;
	}

	std::cout << "Historial de " << nombreRepositorio[repositorioActivo] << " (" << cantidadCommits[repositorioActivo] << (cantidadCommits[repositorioActivo] == 1 ? " commit)\n" : " commits)\n");

	for (int j = cantidadCommits[repositorioActivo] - 1; j >= 0; j = j - 1){
		std::cout << "#" << (j + 1) << " - " << mensajesCommits[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

void mostrarEstadoGeneral(std::string nombreRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados. No se pueden mostrar estadisticas.\n\n";
		return;
	}

	std::cout << std::left << std::setw(32) << "Repositorio" << std::setw(42) << "Estado" << "Ultimo commit\n";

	int totalCommits = 0;        
	int posicionMayor = 0;       
	int reposConCambios = 0;     

	for (int i = 0; i < cantidadRepositorios; i++){

		std::string pendientes = "Cambios pendientes (" + std::to_string(cantidadCambiosPendientes[i]) + ")";
		std::string estado = "Inicializado | " + (cantidadCambiosPendientes[i] > 0 ? pendientes : "Sin cambios pendientes");
		std::string ultimoCommit = (cantidadCommits[i] > 0 ? "#" + std::to_string(cantidadCommits[i]) : "Ninguno");
		std::cout << std::setw(32) << nombreRepositorio[i] << std::setw(42) << estado << ultimoCommit << "\n";

		totalCommits = totalCommits + cantidadCommits[i];   
		if (cantidadCambiosPendientes[i] > 0){
			reposConCambios++;                              
		}

		if (cantidadCommits[i] > cantidadCommits[posicionMayor]){
			posicionMayor = i;
		}
	}

	double promedio = totalCommits;
	promedio = promedio / cantidadRepositorios;

	std::cout << "\nTotal de repositorios registrados: " << cantidadRepositorios << "\n";
	std::cout << "Total de commits: " << totalCommits << "\n";
	std::cout << "Promedio de commits por repositorio: " << std::fixed << std::setprecision(2) << promedio << "\n";
	std::cout << "Repositorio con mas commits: " << nombreRepositorio[posicionMayor] << " (" << cantidadCommits[posicionMayor] << ")\n";
	std::cout << "Repositorios con cambios pendientes: " << reposConCambios << "\n\n";
}

bool confirmarAccion(std::string pregunta){
	std::cout << pregunta << "\n";
	std::cout << "1. Si\n";
	std::cout << "2. No\n";
	int respuesta = leerNumeroValidado("Seleccione una opcion: ", 1, 2);
	return respuesta == 1;
}

void limpiarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int posicion){ // Vacia todos los datos de la fila "posicion" en los arreglos y matrices 
	nombreRepositorio[posicion] = "";
	autorRepositorio[posicion] = "";
	cantidadCommits[posicion] = 0;
	cantidadCambiosPendientes[posicion] = 0;
	for (int j = 0; j < MAX_COMMITS; j++){
		mensajesCommits[posicion][j] = "";
	}
	for (int j = 0; j < MAX_CAMBIOS; j++){
		archivosCambios[posicion][j] = "";
		tipoCambioPendiente[posicion][j] = ' ';
	}
}

void desplazarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios, int posicion){

	for (int i = posicion; i < cantidadRepositorios - 1; i++){
		nombreRepositorio[i] = nombreRepositorio[i + 1];
		autorRepositorio[i] = autorRepositorio[i + 1];
		cantidadCommits[i] = cantidadCommits[i + 1];
		cantidadCambiosPendientes[i] = cantidadCambiosPendientes[i + 1];

		for (int j = 0; j < MAX_COMMITS; j++){
			mensajesCommits[i][j] = mensajesCommits[i + 1][j];
		}
		for (int j = 0; j < MAX_CAMBIOS; j++){
			archivosCambios[i][j] = archivosCambios[i + 1][j];
			tipoCambioPendiente[i][j] = tipoCambioPendiente[i + 1][j];
		}
	}
	
	limpiarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios - 1); 	// La ultima posicion ocupada quedo duplicada: se limpia para no dejar datos residuales.
}

bool eliminarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios, int repositorioActivo){

	int posicion = pedirNumeroRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo, "Numero del repositorio a eliminar (0 para cancelar): ", "Operacion cancelada. No se elimino ningun repositorio.");
	if (posicion == -1){
		return false;
	}
	
	std::string nombre = nombreRepositorio[posicion];

	std::cout << "\n";
	if (!confirmarAccion("Desea eliminar el repositorio " + nombre + "?")){
		std::cout << "\nOperacion cancelada. El repositorio " << nombre << " no fue eliminado.\n\n";
		return false;
	}

	desplazarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios, posicion);

	registrarBitacora("ELIMINAR_REPOSITORIO | " + nombre);

	std::cout << "\nRepositorio " << nombre << " eliminado correctamente.\n\n";
	return true;
}

void verificarApertura(bool abierto, std::string nombreArchivo){
	if (!abierto){
		throw std::runtime_error("No se pudo abrir " + nombreArchivo + " para escritura.");
	}
}


void escribirRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadRepositorios){
	
	std::ofstream archivo(ARCHIVO_REPOSITORIOS);
	verificarApertura(archivo.is_open(), ARCHIVO_REPOSITORIOS);

	archivo << ENCABEZADO_REPOSITORIOS << "\n";
	for (int i = 0; i < cantidadRepositorios; i++){
		archivo << nombreRepositorio[i] << ";" << autorRepositorio[i] << "\n";
	}
	archivo.close();
}

void escribirCommits(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[], int cantidadRepositorios){ // La cantidad de commits no se guarda. al cargar se calcula contando registros.
	
	std::ofstream archivo(ARCHIVO_COMMITS);
	verificarApertura(archivo.is_open(), ARCHIVO_COMMITS);

	archivo << ENCABEZADO_COMMITS << "\n";
	for (int i = 0; i < cantidadRepositorios; i++){
		for (int j = 0; j < cantidadCommits[i]; j++){
			archivo << nombreRepositorio[i] << ";" << (j + 1) << ";" << mensajesCommits[i][j] << "\n";
		}
	}
	archivo.close();
}

void escribirCambios(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[], int cantidadRepositorios){

	std::ofstream archivo(ARCHIVO_CAMBIOS);
	verificarApertura(archivo.is_open(), ARCHIVO_CAMBIOS);

	archivo << ENCABEZADO_CAMBIOS << "\n";
	for (int i = 0; i < cantidadRepositorios; i++){
		for (int j = 0; j < cantidadCambiosPendientes[i]; j++){
			archivo << nombreRepositorio[i] << ";" << tipoCambioPendiente[i][j] << ";" << archivosCambios[i][j] << "\n";
		}
	}
	archivo.close();
}

bool guardarDatos(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios){
	
	try{
		escribirRepositorios(nombreRepositorio, autorRepositorio, cantidadRepositorios);
		escribirCommits(nombreRepositorio, mensajesCommits, cantidadCommits, cantidadRepositorios);
		escribirCambios(nombreRepositorio, archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes, cantidadRepositorios);
	}
	catch (std::runtime_error error){
		std::cout << "Error al guardar los datos: " << error.what() << "\n";
		std::cout << "Los datos siguen en memoria; puede intentar guardar de nuevo.\n\n";
		registrarBitacora("ERROR | GUARDAR_DATOS | " + std::string(error.what()));
		return false;
	}

	registrarBitacora("GUARDAR_DATOS | Datos guardados correctamente");
	std::cout << "Datos guardados correctamente.\n\n";
	return true;
}

void reiniciarDatos(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS]){

	for (int i = 0; i < MAX_REPOSITORIOS; i++){
		limpiarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, i);
	}
}

void lanzarErrorLinea(std::string nombreArchivo, int numeroLinea, std::string problema){
	
	throw std::runtime_error(nombreArchivo + " | Linea " + std::to_string(numeroLinea) + ": " + problema);
}

int dividirCampos(std::string linea, std::string campos[], int maxCampos){
	int cantidad = 0;
	std::string actual = "";
	int largo = linea.length();

	for (int i = 0; i < largo; i++){
		if (linea[i] == ';'){
			if (cantidad < maxCampos){
				campos[cantidad] = actual;
			}
			cantidad++;
			actual = "";
		}
		else{
			actual = actual + linea[i];
		}
	}
	if (cantidad < maxCampos){
		campos[cantidad] = actual;
	}
	cantidad++;
	return cantidad;
}

void validarEncabezado(bool seLeyo, std::string linea, std::string encabezadoEsperado, std::string nombreArchivo){ // Si el archivo esta vacio o el encabezado es distinto, lanza una excepcion.
	if (!seLeyo){
		lanzarErrorLinea(nombreArchivo, 1, "el archivo esta vacio o no se pudo leer (falta el encabezado)");
	}
	if (linea != encabezadoEsperado){
		lanzarErrorLinea(nombreArchivo, 1, "encabezado incorrecto, se esperaba: " + encabezadoEsperado);
	}
}

int cargarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[]){
	
    std::ifstream archivo(ARCHIVO_REPOSITORIOS);
    if (!archivo.is_open()){
        return -1;   
    }

    std::string linea;
    std::getline(archivo, linea);
    validarEncabezado(!archivo.fail(), linea, ENCABEZADO_REPOSITORIOS, ARCHIVO_REPOSITORIOS);
    int numeroLinea = 1;   
    int cantidadRepositorios = 0;
    std::string campos[2];

    while (std::getline(archivo, linea)){
        numeroLinea++;
        if (linea == ""){
            continue;   
        }

        int cantidadCampos = dividirCampos(linea, campos, 2);
        if (cantidadCampos != 2){
            lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea, "se esperaban 2 campos y se encontraron " + std::to_string(cantidadCampos));
        }

        std::string errorNombre = errorDeTexto(campos[0], 1, MAX_LONGITUDNOMBRE);
        if (errorNombre != ""){
            lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea, "nombre de repositorio invalido. " + errorNombre);
        }

        std::string errorAutor = errorDeTexto(campos[1], 1, MAX_LONGITUDNOMBRE);
        if (errorAutor != ""){
            lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea, "autor invalido. " + errorAutor);
        }

        if (buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]) != -1){
            lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea, "el repositorio " + campos[0] + " esta duplicado");
        }

        if (cantidadRepositorios >= MAX_REPOSITORIOS){
            lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea, "se supera la capacidad maxima de " + std::to_string(MAX_REPOSITORIOS) + " repositorios");
        }

        nombreRepositorio[cantidadRepositorios] = campos[0];
        autorRepositorio[cantidadRepositorios] = campos[1];
        cantidadRepositorios++;
    }
    return cantidadRepositorios;
}

bool cargarCommits(std::string nombreRepositorio[], int cantidadRepositorios, std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[]){
	
    std::ifstream archivo(ARCHIVO_COMMITS);
    if (!archivo.is_open()){
        return false;  
    }

    std::string linea;
    std::getline(archivo, linea);
    validarEncabezado(!archivo.fail(), linea, ENCABEZADO_COMMITS, ARCHIVO_COMMITS);
    int numeroLinea = 1;  
    std::string campos[3];

    while (std::getline(archivo, linea)){
        numeroLinea++;
        if (linea == ""){
            continue;  
        }

        int cantidadCampos = dividirCampos(linea, campos, 3);
        if (cantidadCampos != 3){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos));
        }

        int posicion = buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]);
        if (posicion == -1){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "el repositorio " + campos[0] + " no existe");
        }

        int numeroCommit = convertirEntero(campos[1]);
        if (numeroCommit == -1){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "el numero de commit '" + campos[1] + "' no es un numero valido");
        }

        if (cantidadCommits[posicion] >= MAX_COMMITS){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_COMMITS) + " commits");
        }

        int numeroEsperado = cantidadCommits[posicion] + 1;
        if (numeroCommit != numeroEsperado){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "el numero de commit " + campos[1] + " no es consecutivo (se esperaba " + std::to_string(numeroEsperado) + ")");
        }

        std::string errorMensaje = errorDeTexto(campos[2], MIN_LONGITUDMENSAJE, MAX_LONGITUDMENSAJE);
        if (errorMensaje != ""){
            lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea, "mensaje de commit invalido. " + errorMensaje);
        }

        mensajesCommits[posicion][cantidadCommits[posicion]] = campos[2];
        cantidadCommits[posicion]++;
    }
    return true;
}

bool cargarCambios(std::string nombreRepositorio[], int cantidadRepositorios, std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[]){
    std::ifstream archivo(ARCHIVO_CAMBIOS);
    if (!archivo.is_open()){
        return false;
    }

    std::string linea;
    std::getline(archivo, linea);
    validarEncabezado(!archivo.fail(), linea, ENCABEZADO_CAMBIOS, ARCHIVO_CAMBIOS);
    int numeroLinea = 1;
    std::string campos[3];

    while (std::getline(archivo, linea)){
        numeroLinea++;
        if (linea == ""){
            continue;
        }

        int cantidadCampos = dividirCampos(linea, campos, 3);
        if (cantidadCampos != 3){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos));
        }

        int posicion = buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]);
        if (posicion == -1){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "el repositorio " + campos[0] + " no existe");
        }

        bool tipoValido = (campos[1].length() == 1) && (campos[1][0] == 'A' || campos[1][0] == 'M' || campos[1][0] == 'E');
        if (!tipoValido){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "tipo de cambio '" + campos[1] + "' no valido (debe ser A, M o E)");
        }
        char tipo = campos[1][0];

        std::string errorArchivo = errorDeTexto(campos[2], 1, MAX_LONGITUDNOMBRE);
        if (errorArchivo != ""){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "nombre de archivo invalido. " + errorArchivo);
        }

        if (existeCambioPendiente(archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes, posicion, tipo, campos[2])){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "el cambio " + campos[1] + " " + campos[2] + " esta repetido en el repositorio " + campos[0]);
        }

        if (cantidadCambiosPendientes[posicion] >= MAX_CAMBIOS){
            lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_CAMBIOS) + " cambios pendientes");
        }

        int columna = cantidadCambiosPendientes[posicion];
        tipoCambioPendiente[posicion][columna] = tipo;
        archivosCambios[posicion][columna] = campos[2];
        cantidadCambiosPendientes[posicion]++;
    }
    return true;
}

int cargarDatos(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS]){

	int cantidadRepositorios = 0;
	reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente);

	try{
		int cargados = cargarRepositorios(nombreRepositorio, autorRepositorio);
		bool existenRepositorios = (cargados != -1);
		if (existenRepositorios){
			cantidadRepositorios = cargados;
		}
		bool existenCommits = cargarCommits(nombreRepositorio, cantidadRepositorios, mensajesCommits, cantidadCommits);
		bool existenCambios = cargarCambios(nombreRepositorio, cantidadRepositorios, archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes);

		if (!existenRepositorios && !existenCommits && !existenCambios){
			std::cout << "No se encontraron archivos de datos (primera ejecucion). El sistema inicia vacio.\n\n";
			registrarBitacora("CARGAR_DATOS | No se encontraron archivos. El sistema inicia vacio");
		}
		else{
			std::cout << "Datos cargados correctamente. Repositorios recuperados: " << cantidadRepositorios << "\n\n";
			registrarBitacora("CARGAR_DATOS | Repositorios recuperados: " + std::to_string(cantidadRepositorios));
		}
	}
	catch (std::runtime_error error){

		reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente);
		cantidadRepositorios = 0;

		std::cout << "Error al cargar los datos: " << error.what() << "\n";
		std::cout << "El sistema inicia vacio.\n";
		std::cout << "ADVERTENCIA: si guarda los datos (opcion 12 o al salir), se sobrescribiran los archivos existentes.\n\n";

		registrarBitacora("ERROR | " + std::string(error.what()));
		registrarBitacora("CARGAR_DATOS | La carga fallo. El sistema inicia vacio");
	}
	return cantidadRepositorios;
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

bool confirmarSalida(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[], int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios){

	bool guardado = guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);

	if (guardado == true){
		return true;
	}

	std::cout << "ADVERTENCIA: los datos no se pudieron guardar. Si sale ahora se perderan los cambios de esta sesion.\n\n";
	return confirmarAccion("Desea salir de todas formas?");
}

void mostrarEncabezado(int opcion, std::string titulo){
	system("cls");
	std::cout << "Opcion digitada: " << opcion << "\nEntrando a: " << titulo << "\n\n";
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
	int nuevo=-1;                

	cantidadRepositorios = cargarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente);
	system("pause");

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
				mostrarEncabezado(1, "Crear repositorio");
				nuevo = crearRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits,cantidadCambiosPendientes, cantidadRepositorios);
				if (nuevo != -1){                 
					cantidadRepositorios++;
					repositorioActivo = nuevo;
				}
				break;
				
			case 2:
				mostrarEncabezado(2, "Listar repositorios");
				listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				                   cantidadRepositorios, repositorioActivo);
				break;
				
			case 3:
				mostrarEncabezado(3, "Seleccionar repositorio activo");
				repositorioActivo = seleccionarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);
				break;
				
			case 4:
				mostrarEncabezado(4, "Ver informacion del repositorio activo");
				mostrarInformacion(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, repositorioActivo);
				break;
				
			case 5:
				mostrarEncabezado(5, "Simular cambios");
				simularCambio(nombreRepositorio, archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes, repositorioActivo);
				break;
				
			case 6:
				mostrarEncabezado(6, "Ver cambios pendientes");
				mostrarCambiosPendientes(nombreRepositorio, archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes, repositorioActivo);
				break;
				
			case 7:
				mostrarEncabezado(7, "Crear commit");
				crearCommit(nombreRepositorio, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadCommits, cantidadCambiosPendientes, repositorioActivo);
				break;
				
			case 8:
				mostrarEncabezado(8, "Ver ultimo commit");
				mostrarUltimoCommit(nombreRepositorio, autorRepositorio, mensajesCommits, cantidadCommits, repositorioActivo);
				break;
				
			case 9:
				mostrarEncabezado(9, "Ver historial de commits");
				mostrarHistorial(nombreRepositorio, mensajesCommits, cantidadCommits, repositorioActivo);
				break;
				
			case 10:
				mostrarEncabezado(10, "Ver estado general y estadisticas");
				mostrarEstadoGeneral(nombreRepositorio, cantidadCommits, cantidadCambiosPendientes, cantidadRepositorios);
				break;
				
			case 11:
				mostrarEncabezado(11, "Eliminar repositorio");
				if (eliminarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios, repositorioActivo)){
					cantidadRepositorios = cantidadRepositorios - 1;   
					repositorioActivo = -1;                            
				}
				break;
				
			case 12:
				mostrarEncabezado(12, "Guardar datos");
				guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);
				break;
				
			case 13:
				mostrarEncabezado(13, "Ver bitacora");
				mostrarBitacora();
				break;
				
			case 14:
				system("cls");
				if (confirmarSalida(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes, mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios)){
					std::cout<<"Saliendo....\n\n";
				}
				else{
					opcion = 0;
				}
				break;
		}

		system("pause");

	} while (opcion != 14);

	return 0;
}
