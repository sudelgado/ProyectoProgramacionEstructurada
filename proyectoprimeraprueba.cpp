#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>


// Los arreglos tienen tamano fijo, por eso todas las capacidades son constantes.
const int MAX_REPOSITORIOS=10;
const int MAX_COMMITS=50;
const int MAX_CAMBIOS=20;
const int MAX_LONGITUDNOMBRE=30;
const int MIN_LONGITUDMENSAJE=10;
const int MAX_LONGITUDMENSAJE=60;

// Nombres de los archivos de persistencia (misma carpeta del programa)
const std::string ARCHIVO_BITACORA = "bitacora.txt";
const std::string ARCHIVO_REPOSITORIOS = "repositorios.csv";
const std::string ARCHIVO_COMMITS = "commits.csv";
const std::string ARCHIVO_CAMBIOS = "cambios.csv";

// Primera linea (encabezado) de cada CSV. Se usa al guardar y al validar la carga.
const std::string ENCABEZADO_REPOSITORIOS = "nombre;autor";
const std::string ENCABEZADO_COMMITS = "repositorio;numero;mensaje";
const std::string ENCABEZADO_CAMBIOS = "repositorio;tipo;archivo";


// Muestra el menu principal e indica cual es el repositorio activo.
// -1 significa que no hay ningun repositorio seleccionado.
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

// Submenu de "Simular cambios". La opcion se lee con leerNumeroValidado.
void mostrarSubmenuCambios(){
	std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
	std::cout<<"          1. Agregar Archivo\n";
	std::cout<<"          2. Modificar Archivo\n";
	std::cout<<"          3. Eliminar Archivo\n";
	std::cout<<"          4. Cancelar\n\n";
}


// Agrega una linea al final de bitacora.txt (std::ios::app = modo agregar, nunca sobrescribe).
// Si no se puede escribir, el catch queda vacio a proposito para que el programa continue.
void registrarBitacora(std::string evento){
	try{
		std::ofstream archivo(ARCHIVO_BITACORA, std::ios::app);
		if (!archivo.is_open()){
			throw std::runtime_error("No se pudo abrir " + ARCHIVO_BITACORA);
		}
		archivo << evento << "\n";
		archivo.close();
	}
	catch (std::runtime_error){
	}
}


// Devuelve true si el texto esta vacio o solo tiene espacios/tabs.
bool soloEspacios(std::string texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i++){
		if (texto[i] != ' ' && texto[i] != '\t'){
			return false;
		}
	}
	return true;
}

// Devuelve true si el texto tiene al menos un ';'.
// El ';' no se permite porque es el separador de los archivos CSV.
bool contienePuntoYComa(std::string texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i++){
		if (texto[i] == ';'){
			return true;
		}
	}
	return false;
}

// Revisa un texto: no vacio, sin ';' y con largo entre minLongitud y maxLongitud.
// Retorna "" si es valido o el mensaje del error. Se usa con el teclado y con los CSV.
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

// Lee un texto del teclado y lo vuelve a pedir hasta que sea valido (ver errorDeTexto).
// Se usa para TODOS los textos: repositorio, autor, archivo y mensaje del commit.
std::string leerTextoValido(std::string etiqueta, int minLongitud, int maxLongitud){
	std::string texto;
	while (true){
		std::cout << etiqueta;
		std::getline(std::cin, texto);
		std::string error = errorDeTexto(texto, minLongitud, maxLongitud);
		if (error == ""){
			return texto;   // valido: se sale de la funcion
		}
		std::cout << error << "\n";
	}
}

// Devuelve true solo si TODOS los caracteres son digitos (0-9).
// Rechaza "12abc", "-5", "3.5" y el texto vacio antes de intentar convertir.
bool todosDigitos(std::string texto){
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

// Convierte un texto a entero con try/catch (stoi puede lanzar out_of_range con numeros gigantes).
// Retorna el numero, o -1 si no se pudo (los numeros validos nunca son negativos).
// todosDigitos ya garantiza que TODO el texto son digitos. Se usa con el teclado y con los archivos.
int convertirEntero(std::string texto){
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

// Lee un numero como TEXTO y lo convierte (ver convertirEntero).
// Repite la lectura hasta que sea numerico y este entre minvalido y maxvalido.
int leerNumeroValidado(std::string etiqueta, int minvalido, int maxvalido){
	std::string texto;
	while (true){
		std::cout << etiqueta;
		std::getline(std::cin, texto);
		int valor = convertirEntero(texto);   // -1 si no es un numero valido
		if (valor != -1 && valor >= minvalido && valor <= maxvalido){
			return valor;
		}
		std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
	}
}


// Busqueda lineal por nombre. Retorna la posicion o -1 si no existe.
int buscarRepositorio(std::string nombreRepositorio[], int cantidadRepositorios, std::string nombreBuscado){
	for (int i = 0; i < cantidadRepositorios; i++){
		if (nombreRepositorio[i] == nombreBuscado){
			return i;
		}
	}
	return -1;
}

// Convierte el codigo guardado en la matriz ('A', 'M', 'E') en texto para mostrar.
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

// Devuelve true si el repositorio ya tiene pendiente el mismo cambio (mismo tipo Y mismo archivo).
// Solo recorre hasta cantidadCambiosPendientes, que son las posiciones realmente usadas.
bool existeCambioPendiente(std::string archivosCambios[][MAX_CAMBIOS],
                           char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[],
                           int repositorio, char tipo, std::string archivo){
	for (int j = 0; j < cantidadCambiosPendientes[repositorio]; j++){
		if (tipoCambioPendiente[repositorio][j] == tipo && archivosCambios[repositorio][j] == archivo){
			return true;
		}
	}
	return false;
}


// Opcion 1: registra un repositorio nuevo (valida capacidad y nombre repetido).
// Retorna la posicion del nuevo repositorio, o -1 si no se creo.
// El main usa ese valor para sumar 1 a la cantidad y dejarlo como repositorio activo.
int crearRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                     int cantidadCambiosPendientes[], int cantidadRepositorios){

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

	// Como los repositorios nunca dejan huecos, la primera posicion libre
	// siempre es igual a la cantidad de repositorios actuales.
	int posicion = cantidadRepositorios;
	nombreRepositorio[posicion] = nombre;
	autorRepositorio[posicion] = autor;
	cantidadCommits[posicion] = 0;
	cantidadCambiosPendientes[posicion] = 0;

	registrarBitacora("CREAR_REPOSITORIO | " + nombre + " | Autor: " + autor);

	std::cout << "\nRepositorio " << nombre << " creado correctamente.\n\n";
	return posicion;
}

// Opcion 2: muestra la tabla de repositorios. El activo se marca con un asterisco (*).
// Los numeros se muestran desde 1 (i + 1) aunque el arreglo empiece en 0.
void listarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[],
                        int cantidadCommits[], int cantidadCambiosPendientes[], int cantidadRepositorios,
                        int repositorioActivo){

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

// Si no hay repositorios lo informa. Si hay, muestra la lista y pide el numero (0 cancela).
// Retorna la posicion elegida (0..N-1), o -1 si no hay repositorios o si se cancelo.
// La usan seleccionarRepositorio y eliminarRepositorio, que hacen lo mismo al inicio.
int pedirNumeroRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                           int cantidadCambiosPendientes[], int cantidadRepositorios, int repositorioActivo,
                           std::string etiqueta, std::string mensajeCancelar){
	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return -1;
	}
	listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits,
	                   cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);

	int numero = leerNumeroValidado(etiqueta, 0, cantidadRepositorios);
	if (numero == 0){
		std::cout << "\n" << mensajeCancelar << "\n\n";
		return -1;
	}
	return numero - 1;   // el usuario ve 1..N, el arreglo usa 0..N-1
}

// Opcion 3: pide el numero del repositorio con el que se quiere trabajar.
// Retorna la posicion del nuevo repositorio activo, o el mismo de antes si se cancela.
int seleccionarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[],
                           int cantidadCommits[], int cantidadCambiosPendientes[],
                           int cantidadRepositorios, int repositorioActivo){
	int posicion = pedirNumeroRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                                      cantidadRepositorios, repositorioActivo, "Numero del repositorio (0 para cancelar): ",
	                                      "Operacion cancelada. El repositorio activo no cambio.");
	if (posicion == -1){
		return repositorioActivo;
	}
	std::cout << "\nRepositorio activo: " << nombreRepositorio[posicion] << ".\n\n";
	return posicion;
}

// Opcion 4: muestra la informacion general del repositorio activo.
// El "ultimo commit" es igual a la cantidad de commits (el numero del commit mas reciente).
void mostrarInformacion(std::string nombreRepositorio[], std::string autorRepositorio[],
                        int cantidadCommits[], int cantidadCambiosPendientes[], int repositorioActivo){

	int commits = cantidadCommits[repositorioActivo];
	int pendientes = cantidadCambiosPendientes[repositorioActivo];

	std::cout << "Repositorio: " << nombreRepositorio[repositorioActivo] << "\n";
	std::cout << "Autor: " << autorRepositorio[repositorioActivo] << "\n";
	std::cout << "Estado: Inicializado\n";
	std::cout << "Cantidad de commits: " << commits << "\n";
	std::cout << "Ultimo commit: " << (commits == 0 ? "Ninguno" : "#" + std::to_string(commits)) << "\n";
	std::cout << "Cambios pendientes: " << (pendientes == 0 ? "No" : "Si (" + std::to_string(pendientes) + ")") << "\n\n";
}

// Opcion 5: registra un cambio pendiente (A, M o E) en el repositorio activo.
// Valida capacidad y cambio repetido. Cancelar no modifica nada.
void simularCambio(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS],
                   char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[], int repositorioActivo){

	mostrarSubmenuCambios();
	int opcionCambio = leerNumeroValidado("		Seleccione una opcion: ", 1, 4);

	if (opcionCambio == 4){
		std::cout << "\nOperacion cancelada. No se realizaron cambios.\n\n";
		return;
	}

	// Se revisa la capacidad ANTES de pedir el archivo, para no hacer escribir al usuario en vano.
	if (cantidadCambiosPendientes[repositorioActivo] >= MAX_CAMBIOS){
		std::cout << "\nSe alcanzo el maximo de " << MAX_CAMBIOS << " cambios pendientes para este repositorio.\n\n";
		return;
	}

	// Opcion 1 = Agregado, 2 = Modificado, 3 = Eliminado
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

	// El cambio se guarda en la siguiente columna libre de la FILA del repositorio activo.
	int posicion = cantidadCambiosPendientes[repositorioActivo];
	tipoCambioPendiente[repositorioActivo][posicion] = tipo;
	archivosCambios[repositorioActivo][posicion] = archivo;
	cantidadCambiosPendientes[repositorioActivo]++;

	registrarBitacora("SIMULAR_CAMBIO | " + nombreRepositorio[repositorioActivo] + " | " + std::string(1, tipo) + " " + archivo);

	std::cout << "\nCambio registrado: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
}

// Opcion 6: recorre la fila del repositorio activo en las matrices de cambios y los muestra.
void mostrarCambiosPendientes(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS],
                              char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[],
                              int repositorioActivo){

	if (cantidadCambiosPendientes[repositorioActivo] == 0){
		std::cout << "No hay cambios pendientes.\n\n";
		return;
	}

	std::cout << "Cambios pendientes de " << nombreRepositorio[repositorioActivo] << ":\n";

	for (int j = 0; j < cantidadCambiosPendientes[repositorioActivo]; j++){
		std::cout << (j + 1) << ". [" << nombreTipoCambio(tipoCambioPendiente[repositorioActivo][j]) << "] "
		          << archivosCambios[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

// Opcion 7: crea un commit si hay cambios pendientes y no se llego al maximo.
// Guarda el mensaje, sube el contador y deja en cero los cambios pendientes.
void crearCommit(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS],
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

	// La columna j guarda el commit #(j+1), asi que el nuevo commit va en la
	// columna igual a la cantidad actual de commits.
	int columna = cantidadCommits[repositorioActivo];
	mensajesCommits[repositorioActivo][columna] = mensaje;
	cantidadCommits[repositorioActivo]++;
	int numeroCommit = cantidadCommits[repositorioActivo];

	// Se limpian los cambios pendientes de ESTE repositorio (texto vacio y tipo ' ')
	// para no dejar datos residuales en la matriz.
	int cambiosIncluidos = cantidadCambiosPendientes[repositorioActivo];
	for (int j = 0; j < cambiosIncluidos; j++){
		archivosCambios[repositorioActivo][j] = "";
		tipoCambioPendiente[repositorioActivo][j] = ' ';
	}
	cantidadCambiosPendientes[repositorioActivo] = 0;

	registrarBitacora("CREAR_COMMIT | " + nombreRepositorio[repositorioActivo] + " | Commit #" + std::to_string(numeroCommit));

	std::cout << "\nCommit #" << numeroCommit << " creado correctamente. "
	          << (cambiosIncluidos == 1 ? "Se incluyo 1 cambio." : "Se incluyeron " + std::to_string(cambiosIncluidos) + " cambios.")
	          << "\n\n";
}

// Opcion 8: muestra el ultimo commit (el de la ultima columna usada de la fila).
void mostrarUltimoCommit(std::string nombreRepositorio[], std::string autorRepositorio[],
                         std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[],
                         int repositorioActivo){

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "El repositorio " << nombreRepositorio[repositorioActivo] << " aun no tiene commits.\n\n";
		return;
	}

	// Si hay N commits, el ultimo esta en la columna N - 1.
	int ultimaColumna = cantidadCommits[repositorioActivo] - 1;

	std::cout << "Commit #" << cantidadCommits[repositorioActivo] << "\n";
	std::cout << "Repositorio: " << nombreRepositorio[repositorioActivo] << "\n";
	std::cout << "Autor: " << autorRepositorio[repositorioActivo] << "\n";
	std::cout << "Mensaje: " << mensajesCommits[repositorioActivo][ultimaColumna] << "\n";
	std::cout << "Estado: Guardado correctamente\n\n";
}

// Opcion 9: muestra el historial del MAS RECIENTE al MAS ANTIGUO.
// Por eso el for va hacia atras: empieza en la ultima columna usada y baja hasta la 0.
void mostrarHistorial(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS],
                      int cantidadCommits[], int repositorioActivo){

	if (cantidadCommits[repositorioActivo] == 0){
		std::cout << "El repositorio " << nombreRepositorio[repositorioActivo] << " aun no tiene commits.\n\n";
		return;
	}

	std::cout << "Historial de " << nombreRepositorio[repositorioActivo] << " (" << cantidadCommits[repositorioActivo]
	          << (cantidadCommits[repositorioActivo] == 1 ? " commit)\n" : " commits)\n");

	for (int j = cantidadCommits[repositorioActivo] - 1; j >= 0; j = j - 1){
		std::cout << "#" << (j + 1) << " - " << mensajesCommits[repositorioActivo][j] << "\n";
	}
	std::cout << "\n";
}

// Opcion 10: tabla de todos los repositorios y estadisticas (acumulador de commits,
// contador de repositorios con pendientes y el de mas commits; en empate gana el primero).
void mostrarEstadoGeneral(std::string nombreRepositorio[], int cantidadCommits[],
                          int cantidadCambiosPendientes[], int cantidadRepositorios){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados. No se pueden mostrar estadisticas.\n\n";
		return;
	}

	std::cout << std::left << std::setw(32) << "Repositorio" << std::setw(42) << "Estado" << "Ultimo commit\n";

	int totalCommits = 0;        // acumulador
	int posicionMayor = 0;       // posicion del repositorio con mas commits
	int reposConCambios = 0;     // contador

	for (int i = 0; i < cantidadRepositorios; i++){

		std::string pendientes = "Cambios pendientes (" + std::to_string(cantidadCambiosPendientes[i]) + ")";
		std::string estado = "Inicializado | " + (cantidadCambiosPendientes[i] > 0 ? pendientes : "Sin cambios pendientes");
		std::string ultimoCommit = (cantidadCommits[i] > 0 ? "#" + std::to_string(cantidadCommits[i]) : "Ninguno");
		std::cout << std::setw(32) << nombreRepositorio[i] << std::setw(42) << estado << ultimoCommit << "\n";

		totalCommits = totalCommits + cantidadCommits[i];   // acumulador
		if (cantidadCambiosPendientes[i] > 0){
			reposConCambios++;                              // contador
		}

		// Se usa ">" (no ">=") para que en caso de empate gane el primero encontrado.
		if (cantidadCommits[i] > cantidadCommits[posicionMayor]){
			posicionMayor = i;
		}
	}

	// Se pasa a double ANTES de dividir; si no, la division seria entera y perderia los decimales.
	double promedio = totalCommits;
	promedio = promedio / cantidadRepositorios;

	std::cout << "\nTotal de repositorios registrados: " << cantidadRepositorios << "\n";
	std::cout << "Total de commits: " << totalCommits << "\n";
	std::cout << "Promedio de commits por repositorio: " << std::fixed << std::setprecision(2) << promedio << "\n";
	std::cout << "Repositorio con mas commits: " << nombreRepositorio[posicionMayor] << " (" << cantidadCommits[posicionMayor] << ")\n";
	std::cout << "Repositorios con cambios pendientes: " << reposConCambios << "\n\n";
}

// Muestra una pregunta con opciones 1. Si / 2. No. Retorna true si el usuario elige Si.
// Se reutiliza al eliminar un repositorio y al salir sin poder guardar.
bool confirmarAccion(std::string pregunta){
	std::cout << pregunta << "\n";
	std::cout << "1. Si\n";
	std::cout << "2. No\n";
	int respuesta = leerNumeroValidado("Seleccione una opcion: ", 1, 2);
	return respuesta == 1;
}

// Vacia todos los datos de la fila "posicion" en los arreglos y matrices (0 / ' ' / "").
// La usan desplazarRepositorios (para la ultima fila) y reiniciarDatos (para todas).
void limpiarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                        int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS],
                        std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS], int posicion){
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

// Elimina el repositorio de "posicion" subiendo los siguientes en TODOS los arreglos
// y matrices (sin huecos). Despues limpia la ultima posicion, que quedo repetida.
// La resta de la cantidad de repositorios se hace en el main.
void desplazarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                           int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS],
                           std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS],
                           int cantidadRepositorios, int posicion){

	// Cada fila i copia a la fila i + 1 (por eso el for llega hasta cantidad - 1).
	for (int i = posicion; i < cantidadRepositorios - 1; i++){
		nombreRepositorio[i] = nombreRepositorio[i + 1];
		autorRepositorio[i] = autorRepositorio[i + 1];
		cantidadCommits[i] = cantidadCommits[i + 1];
		cantidadCambiosPendientes[i] = cantidadCambiosPendientes[i + 1];

		// En las matrices se copia la fila completa, columna por columna.
		for (int j = 0; j < MAX_COMMITS; j++){
			mensajesCommits[i][j] = mensajesCommits[i + 1][j];
		}
		for (int j = 0; j < MAX_CAMBIOS; j++){
			archivosCambios[i][j] = archivosCambios[i + 1][j];
			tipoCambioPendiente[i][j] = tipoCambioPendiente[i + 1][j];
		}
	}

	// La ultima posicion ocupada quedo duplicada: se limpia para no dejar datos residuales.
	limpiarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                   mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios - 1);
}

// Opcion 11: elimina un repositorio. Pide el numero (0 cancela) y confirmacion.
// Retorna true si se elimino; el main entonces resta 1 a la cantidad y deja el activo en -1.
bool eliminarRepositorio(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                         int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS],
                         std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS],
                         int cantidadRepositorios, int repositorioActivo){

	int posicion = pedirNumeroRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                                      cantidadRepositorios, repositorioActivo,
	                                      "Numero del repositorio a eliminar (0 para cancelar): ",
	                                      "Operacion cancelada. No se elimino ningun repositorio.");
	if (posicion == -1){
		return false;
	}

	// Se guarda el nombre antes de eliminar, porque despues esa posicion ya tendra otros datos.
	std::string nombre = nombreRepositorio[posicion];

	std::cout << "\n";
	if (!confirmarAccion("Desea eliminar el repositorio " + nombre + "?")){
		std::cout << "\nOperacion cancelada. El repositorio " << nombre << " no fue eliminado.\n\n";
		return false;
	}

	desplazarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                      mensajesCommits, archivosCambios, tipoCambioPendiente,
	                      cantidadRepositorios, posicion);

	registrarBitacora("ELIMINAR_REPOSITORIO | " + nombre);

	std::cout << "\nRepositorio " << nombre << " eliminado correctamente.\n\n";
	return true;
}


// ===================== GUARDAR DATOS =====================

// Revisa si un archivo para escritura se pudo abrir. Si no, lanza una excepcion
// que se captura en guardarDatos. (Al abrir para escritura se sobrescribe lo anterior.)
void verificarApertura(bool abierto, std::string nombreArchivo){
	if (!abierto){
		throw std::runtime_error("No se pudo abrir " + nombreArchivo + " para escritura.");
	}
}

// repositorios.csv: encabezado + un registro por repositorio (nombre;autor).
void escribirRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[],
                          int cantidadRepositorios){
	std::ofstream archivo(ARCHIVO_REPOSITORIOS);
	verificarApertura(archivo.is_open(), ARCHIVO_REPOSITORIOS);

	archivo << ENCABEZADO_REPOSITORIOS << "\n";
	for (int i = 0; i < cantidadRepositorios; i++){
		archivo << nombreRepositorio[i] << ";" << autorRepositorio[i] << "\n";
	}
	archivo.close();
}

// commits.csv: un registro por commit (repositorio;numero;mensaje).
// La cantidad de commits NO se guarda: al cargar se calcula contando registros.
void escribirCommits(std::string nombreRepositorio[], std::string mensajesCommits[][MAX_COMMITS],
                     int cantidadCommits[], int cantidadRepositorios){
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

// cambios.csv: un registro por cambio pendiente (repositorio;tipo;archivo).
void escribirCambios(std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS],
                     char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[],
                     int cantidadRepositorios){
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

// Coordina el guardado: si alguna escritura falla, el catch avisa, lo registra en la bitacora
// y el programa sigue. Retorna true si se guardo bien (lo usa confirmarSalida).
bool guardarDatos(std::string nombreRepositorio[], std::string autorRepositorio[],
                  int cantidadCommits[], int cantidadCambiosPendientes[],
                  std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS],
                  char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios){
	try{
		escribirRepositorios(nombreRepositorio, autorRepositorio, cantidadRepositorios);
		escribirCommits(nombreRepositorio, mensajesCommits, cantidadCommits, cantidadRepositorios);
		escribirCambios(nombreRepositorio, archivosCambios, tipoCambioPendiente,
		                cantidadCambiosPendientes, cantidadRepositorios);
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


// ===================== CARGAR DATOS =====================

// Deja todos los arreglos vacios (0 / ' ' / "").
// Se usa al iniciar y cuando la carga falla, para no conservar datos parciales.
void reiniciarDatos(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                    int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS],
                    std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS]){

	for (int i = 0; i < MAX_REPOSITORIOS; i++){
		limpiarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
		                   mensajesCommits, archivosCambios, tipoCambioPendiente, i);
	}
}

// Lanza una excepcion con el formato "archivo | Linea N: problema", asi todos los errores
// de carga indican el archivo y la linea, como pide el enunciado.
void lanzarErrorLinea(std::string nombreArchivo, int numeroLinea, std::string problema){
	throw std::runtime_error(nombreArchivo + " | Linea " + std::to_string(numeroLinea) + ": " + problema);
}

// Separa una linea por ';' y guarda hasta maxCampos campos. Retorna cuantos campos
// tiene en total, para detectar lineas con una cantidad incorrecta de campos.
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

// Valida la primera linea (encabezado) de un CSV. "seLeyo" indica si se pudo leer esa linea.
// Si el archivo esta vacio o el encabezado es distinto, lanza una excepcion.
void validarEncabezado(bool seLeyo, std::string linea, std::string encabezadoEsperado, std::string nombreArchivo){
	if (!seLeyo){
		lanzarErrorLinea(nombreArchivo, 1, "el archivo esta vacio o no se pudo leer (falta el encabezado)");
	}
	if (linea != encabezadoEsperado){
		lanzarErrorLinea(nombreArchivo, 1, "encabezado incorrecto, se esperaba: " + encabezadoEsperado);
	}
}

// Carga repositorios.csv (va primero porque commits y cambios usan el nombre).
// Valida campos, textos, duplicados y capacidad; ante un dato malo lanza una excepcion.
// Retorna la cantidad de repositorios cargados, o -1 si el archivo no existe.
int cargarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[]){
	std::ifstream archivo(ARCHIVO_REPOSITORIOS);
	if (!archivo.is_open()){
		return -1;   // el archivo no existe: no es un error
	}

	std::string linea;
	std::getline(archivo, linea);
	validarEncabezado(!archivo.fail(), linea, ENCABEZADO_REPOSITORIOS, ARCHIVO_REPOSITORIOS);
	int numeroLinea = 1;   // la linea 1 es el encabezado
	int cantidadRepositorios = 0;
	std::string campos[2];

	while (std::getline(archivo, linea)){
		numeroLinea++;
		if (linea == ""){
			continue;   // se saltan las lineas vacias
		}

		int cantidadCampos = dividirCampos(linea, campos, 2);
		if (cantidadCampos != 2){
			lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			                 "se esperaban 2 campos y se encontraron " + std::to_string(cantidadCampos));
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
			lanzarErrorLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			                 "se supera la capacidad maxima de " + std::to_string(MAX_REPOSITORIOS) + " repositorios");
		}

		nombreRepositorio[cantidadRepositorios] = campos[0];
		autorRepositorio[cantidadRepositorios] = campos[1];
		cantidadRepositorios++;
	}
	return cantidadRepositorios;
}

// Carga commits.csv. Valida campos, repositorio existente, numero (try/catch), capacidad,
// numeros consecutivos y mensaje. La cantidad de commits se calcula contando registros.
bool cargarCommits(std::string nombreRepositorio[], int cantidadRepositorios,
                   std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[]){
	std::ifstream archivo(ARCHIVO_COMMITS);
	if (!archivo.is_open()){
		return false;   // el archivo no existe: no es un error
	}

	std::string linea;
	std::getline(archivo, linea);
	validarEncabezado(!archivo.fail(), linea, ENCABEZADO_COMMITS, ARCHIVO_COMMITS);
	int numeroLinea = 1;   // la linea 1 es el encabezado
	std::string campos[3];

	while (std::getline(archivo, linea)){
		numeroLinea++;
		if (linea == ""){
			continue;   // se saltan las lineas vacias
		}

		int cantidadCampos = dividirCampos(linea, campos, 3);
		if (cantidadCampos != 3){
			lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea,
			                 "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos));
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
			lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea,
			                 "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_COMMITS) + " commits");
		}

		int numeroEsperado = cantidadCommits[posicion] + 1;
		if (numeroCommit != numeroEsperado){
			lanzarErrorLinea(ARCHIVO_COMMITS, numeroLinea,
			                 "el numero de commit " + campos[1] + " no es consecutivo (se esperaba " + std::to_string(numeroEsperado) + ")");
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

// Carga cambios.csv. Valida campos, repositorio existente, tipo A/M/E, archivo, cambio
// repetido y capacidad. La cantidad de cambios pendientes se calcula contando registros.
bool cargarCambios(std::string nombreRepositorio[], int cantidadRepositorios,
                   std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS],
                   int cantidadCambiosPendientes[]){
	std::ifstream archivo(ARCHIVO_CAMBIOS);
	if (!archivo.is_open()){
		return false;   // el archivo no existe: no es un error
	}

	std::string linea;
	std::getline(archivo, linea);
	validarEncabezado(!archivo.fail(), linea, ENCABEZADO_CAMBIOS, ARCHIVO_CAMBIOS);
	int numeroLinea = 1;   // la linea 1 es el encabezado
	std::string campos[3];

	while (std::getline(archivo, linea)){
		numeroLinea++;
		if (linea == ""){
			continue;   // se saltan las lineas vacias
		}

		int cantidadCampos = dividirCampos(linea, campos, 3);
		if (cantidadCampos != 3){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea,
			                 "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos));
		}

		int posicion = buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]);
		if (posicion == -1){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "el repositorio " + campos[0] + " no existe");
		}

		// El tipo debe ser exactamente un caracter: A, M o E.
		bool tipoValido = (campos[1].length() == 1) &&
		                  (campos[1][0] == 'A' || campos[1][0] == 'M' || campos[1][0] == 'E');
		if (!tipoValido){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "tipo de cambio '" + campos[1] + "' no valido (debe ser A, M o E)");
		}
		char tipo = campos[1][0];

		std::string errorArchivo = errorDeTexto(campos[2], 1, MAX_LONGITUDNOMBRE);
		if (errorArchivo != ""){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea, "nombre de archivo invalido. " + errorArchivo);
		}

		if (existeCambioPendiente(archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes,
		                          posicion, tipo, campos[2])){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea,
			                 "el cambio " + campos[1] + " " + campos[2] + " esta repetido en el repositorio " + campos[0]);
		}

		if (cantidadCambiosPendientes[posicion] >= MAX_CAMBIOS){
			lanzarErrorLinea(ARCHIVO_CAMBIOS, numeroLinea,
			                 "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_CAMBIOS) + " cambios pendientes");
		}

		int columna = cantidadCambiosPendientes[posicion];
		tipoCambioPendiente[posicion][columna] = tipo;
		archivosCambios[posicion][columna] = campos[2];
		cantidadCambiosPendientes[posicion]++;
	}
	return true;
}

// Coordina la carga al iniciar (repositorios, commits y cambios). Sin archivos inicia vacio.
// Ante cualquier error lo captura, vacia los arreglos (sin datos parciales) y lo informa.
// Retorna la cantidad de repositorios cargados (0 si no hay archivos o si la carga fallo).
int cargarDatos(std::string nombreRepositorio[], std::string autorRepositorio[], int cantidadCommits[],
                int cantidadCambiosPendientes[], std::string mensajesCommits[][MAX_COMMITS],
                std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS]){

	int cantidadRepositorios = 0;
	reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	               mensajesCommits, archivosCambios, tipoCambioPendiente);

	try{
		int cargados = cargarRepositorios(nombreRepositorio, autorRepositorio);
		bool existenRepositorios = (cargados != -1);
		if (existenRepositorios){
			cantidadRepositorios = cargados;
		}
		bool existenCommits = cargarCommits(nombreRepositorio, cantidadRepositorios, mensajesCommits, cantidadCommits);
		bool existenCambios = cargarCambios(nombreRepositorio, cantidadRepositorios, archivosCambios,
		                                    tipoCambioPendiente, cantidadCambiosPendientes);

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
		// Sin datos parciales: se vacia todo lo que alcanzo a cargarse.
		reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
		               mensajesCommits, archivosCambios, tipoCambioPendiente);
		cantidadRepositorios = 0;

		std::cout << "Error al cargar los datos: " << error.what() << "\n";
		std::cout << "El sistema inicia vacio.\n";
		std::cout << "ADVERTENCIA: si guarda los datos (opcion 12 o al salir), se sobrescribiran los archivos existentes.\n\n";

		registrarBitacora("ERROR | " + std::string(error.what()));
		registrarBitacora("CARGAR_DATOS | La carga fallo. El sistema inicia vacio");
	}
	return cantidadRepositorios;
}


// Opcion 13: lee bitacora.txt linea por linea y la muestra.
// Si el archivo no existe o esta vacio, muestra "La bitacora esta vacia."
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

// Opcion 14: guarda automaticamente. Retorna true si se puede salir (se guardo, o el
// usuario acepta salir sin guardar) y false si el guardado fallo y prefiere no salir.
bool confirmarSalida(std::string nombreRepositorio[], std::string autorRepositorio[],
                     int cantidadCommits[], int cantidadCambiosPendientes[],
                     std::string mensajesCommits[][MAX_COMMITS], std::string archivosCambios[][MAX_CAMBIOS],
                     char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadRepositorios){

	bool guardado = guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                             mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);

	if (guardado){
		return true;
	}

	std::cout << "ADVERTENCIA: los datos no se pudieron guardar. Si sale ahora se perderan los cambios de esta sesion.\n\n";
	return confirmarAccion("Desea salir de todas formas?");
}


// Limpia la pantalla y muestra que opcion se eligio y a donde se entra.
// Asi no se repite el mismo encabezado en cada case del menu.
void mostrarEncabezado(int opcion, std::string titulo){
	system("cls");
	std::cout << "Opcion digitada: " << opcion << "\nEntrando a: " << titulo << "\n\n";
}


// main solo declara los arreglos y coordina el flujo del menu.
// Toda la logica esta en las funciones de arriba.
int main(){
	// Arreglos de una dimension: un dato por repositorio (posicion i = repositorio i).
	int cantidadCommits[MAX_REPOSITORIOS];
	int cantidadCambiosPendientes[MAX_REPOSITORIOS];
	std::string nombreRepositorio[MAX_REPOSITORIOS];
	std::string autorRepositorio[MAX_REPOSITORIOS];

	// Matrices: fila = repositorio, columna = commit o cambio.
	std::string mensajesCommits[MAX_REPOSITORIOS][MAX_COMMITS];
	std::string archivosCambios[MAX_REPOSITORIOS][MAX_CAMBIOS];
	char tipoCambioPendiente[MAX_REPOSITORIOS][MAX_CAMBIOS];

	int cantidadRepositorios=0;   // cuantos repositorios hay registrados
	int repositorioActivo=-1;     // posicion del repositorio activo (-1 = ninguno, nunca se guarda en archivos)
	int opcion=0;
	int nuevo=-1;                 // posicion del repositorio recien creado (-1 = no se creo)

	// Antes de mostrar el menu se cargan los archivos.
	// cargarDatos tambien deja todos los arreglos inicializados (vacios) y retorna la cantidad cargada.
	cantidadRepositorios = cargarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	                                   mensajesCommits, archivosCambios, tipoCambioPendiente);
	system("pause");

	// Menu repetitivo: solo termina cuando opcion vale 14 (Salir).
	do {
		system("cls");
		mostrarMenuPrincipal(nombreRepositorio, repositorioActivo);
		opcion=leerNumeroValidado("					Seleccione una opcion: ", 1, 14);

		// Las opciones 4 a 9 necesitan un repositorio activo.
		// "continue" salta al final del do-while (revisa la condicion y vuelve a mostrar el menu).
		if (opcion >= 4 && opcion <= 9 && repositorioActivo == -1){
			std::cout<<"\nDebe seleccionar un repositorio activo antes de realizar esta operacion.\n";
			system("pause");
			continue;
		}

		switch(opcion){
			case 1:
				mostrarEncabezado(1, "Crear repositorio");
				nuevo = crearRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits,
				                         cantidadCambiosPendientes, cantidadRepositorios);
				if (nuevo != -1){                 // se creo: hay uno mas y queda como activo
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
				repositorioActivo = seleccionarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits,
				                                           cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);
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
				mostrarCambiosPendientes(nombreRepositorio, archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes,
				                         repositorioActivo);
				break;
			case 7:
				mostrarEncabezado(7, "Crear commit");
				crearCommit(nombreRepositorio, mensajesCommits, archivosCambios, tipoCambioPendiente,
				            cantidadCommits, cantidadCambiosPendientes, repositorioActivo);
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
				if (eliminarRepositorio(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				                        mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios,
				                        repositorioActivo)){
					cantidadRepositorios = cantidadRepositorios - 1;   // se elimino: hay uno menos
					repositorioActivo = -1;                            // y no queda ninguno activo
				}
				break;
			case 12:
				mostrarEncabezado(12, "Guardar datos");
				guardarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				             mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);
				break;
			case 13:
				mostrarEncabezado(13, "Ver bitacora");
				mostrarBitacora();
				break;
			case 14:
				system("cls");
				// Guarda automaticamente. Si falla y el usuario NO quiere salir,
				// opcion = 0 evita que termine el do-while y se vuelve al menu.
				if (confirmarSalida(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
				                    mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios)){
					std::cout<<"Saliendo....\n\n";
				}
				else{
					opcion = 0;
				}
				break;
		}

		system("pause");   // una sola pausa para todas las opciones

	} while (opcion != 14);

	return 0;
}
