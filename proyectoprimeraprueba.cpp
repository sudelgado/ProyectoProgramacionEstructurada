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

// Submenu de "Simular cambios". La opcion se lee con leerNumeroValidado.
void mostrarSubmenuCambios(){
	std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
	std::cout<<"          1. Agregar Archivo\n";
	std::cout<<"          2. Modificar Archivo\n";
	std::cout<<"          3. Eliminar Archivo\n";
	std::cout<<"          4. Cancelar\n\n";
}



// Agrega una linea al final de bitacora.txt.
// std::ios::app = modo agregar: nunca sobrescribe lo anterior.
// Si no se puede escribir, el catch queda vacio a proposito:
// el enunciado dice que el programa debe continuar sin interrumpirse.
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



// Devuelve true si el texto esta vacio o solo tiene espacios/tabs.
bool soloEspacios(const std::string& texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i = i + 1){
		if (texto[i] != ' ' && texto[i] != '\t'){
			return false;
		}
	}
	return true;
}

// Devuelve true si el texto tiene al menos un ';'.
// El ';' no se permite porque es el separador de los archivos CSV.
bool contienePuntoYComa(const std::string& texto){
	int largo = texto.length();
	for (int i = 0; i < largo; i = i + 1){
		if (texto[i] == ';'){
			return true;
		}
	}
	return false;
}

// Cuenta los caracteres del texto sin contar los espacios/tabs del inicio y del final.
// Sirve para que un mensaje como "   hola   " no pase el minimo de longitud solo por los espacios.
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

// Revisa un texto con TODAS las reglas de texto del enunciado:
//  - no puede estar vacio ni tener solo espacios
//  - no puede contener ';'
//  - su largo debe estar entre minLongitud y maxLongitud
// Retorna "" si el texto es valido, o el mensaje del problema encontrado.
// Se usa tanto al leer del teclado (leerTextoValido) como al leer de los archivos CSV,
// asi la validacion se escribe una sola vez.
std::string errorDeTexto(const std::string& texto, int minLongitud, int maxLongitud){
	int largo = texto.length();

	if (soloEspacios(texto)){
		return "El texto no puede estar vacio ni tener solo espacios.";
	}
	if (contienePuntoYComa(texto)){
		return "El texto no puede contener el caracter ';'.";
	}
	if (largo > maxLongitud){
		return "El texto debe tener entre " + std::to_string(minLongitud) + " y " + std::to_string(maxLongitud) + " caracteres.";
	}
	if (largoSinEspaciosExtremos(texto) < minLongitud){
		return "El texto debe tener entre " + std::to_string(minLongitud) + " y " + std::to_string(maxLongitud)
		       + " caracteres (los espacios al inicio o al final no cuentan).";
	}
	return "";
}

// Lee un texto del teclado y lo vuelve a pedir hasta que sea valido (ver errorDeTexto).
// Se usa para TODOS los textos: repositorio, autor, archivo y mensaje del commit.
std::string leerTextoValido(const std::string& etiqueta, int minLongitud, int maxLongitud){
	std::string texto;
	bool valido = false;

	while (!valido){
		std::cout << etiqueta;
		std::getline(std::cin, texto);

		std::string error = errorDeTexto(texto, minLongitud, maxLongitud);
		if (error == ""){
			valido = true;
		}
		else{
			std::cout << error << "\n";
		}
	}
	return texto;
}

// Devuelve true solo si TODOS los caracteres son digitos (0-9).
// Rechaza "12abc", "-5", "3.5" y el texto vacio antes de intentar convertir.
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

// Convierte un texto a entero con try/catch. Retorna true si la conversion fue exitosa.
// Se usa para las opciones del teclado Y para los numeros de commit leidos de los archivos.
//  - todosDigitos descarta letras, signos, espacios y vacio.
//  - stoi puede lanzar invalid_argument u out_of_range (ej: un numero gigante): se capturan.
//  - se verifica que TODO el texto se haya convertido (posicion == largo).
bool convertirEntero(const std::string& texto, int& valor){
	if (!todosDigitos(texto)){
		return false;
	}
	try{
		std::size_t posicion = 0;
		int convertido = std::stoi(texto, &posicion);
		if (posicion != texto.length()){
			return false;
		}
		valor = convertido;
		return true;
	}
	catch (const std::exception& error){
		registrarBitacora("ERROR | Conversion de numero | Valor: " + texto + " | No se pudo convertir (" + std::string(error.what()) + ")");
		return false;
	}
}

// Lee un numero como TEXTO y lo convierte (ver convertirEntero).
// Repite la lectura hasta que sea numerico y este entre minvalido y maxvalido.
int leerNumeroValidado(const std::string& etiqueta, int minvalido, int maxvalido){
	std::string texto;
	int valor = 0;
	bool valido = false;

	while (!valido){
		std::cout << etiqueta;
		std::getline(std::cin, texto);

		if (convertirEntero(texto, valor) && valor >= minvalido && valor <= maxvalido){
			valido = true;
		}

		if (!valido){
			std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
		}
	}
	return valor;
}



// Busqueda lineal por nombre. Retorna la posicion o -1 si no existe.
int buscarRepositorio(const std::string nombreRepositorio[], int cantidadRepositorios, const std::string& nombreBuscado){
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
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



// Opcion 1: registra un repositorio nuevo en la primera posicion libre.
// Valida la capacidad maxima y que no exista otro con el mismo nombre.
// El repositorio nuevo queda como repositorio activo.
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

	// Como los repositorios nunca dejan huecos, la primera posicion libre
	// siempre es igual a la cantidad de repositorios actuales.
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

// Opcion 2: muestra la tabla de repositorios. El activo se marca con un asterisco (*).
// Los numeros se muestran desde 1 (i + 1) aunque el arreglo empiece en 0.
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

// Opcion 3: muestra la lista y pide el numero del repositorio con el que se quiere trabajar.
// El valor 0 cancela la operacion sin modificar el repositorio activo.
void seleccionarRepositorio(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                            const int cantidadCommits[], const int cantidadCambiosPendientes[],
                            int cantidadRepositorios, int& repositorioActivo){

	if (cantidadRepositorios == 0){
		std::cout << "No hay repositorios registrados.\n\n";
		return;
	}

	listarRepositorios(nombreRepositorio, autorRepositorio, cantidadCommits,
	                   cantidadCambiosPendientes, cantidadRepositorios, repositorioActivo);

	// El rango valido va de 0 (cancelar) hasta la cantidad de repositorios.
	int numero = leerNumeroValidado("Numero del repositorio (0 para cancelar): ", 0, cantidadRepositorios);

	if (numero == 0){
		std::cout << "\nOperacion cancelada. El repositorio activo no cambio.\n\n";
		return;
	}

	repositorioActivo = numero - 1;   // el usuario ve 1..N, el arreglo usa 0..N-1
	std::cout << "\nRepositorio activo: " << nombreRepositorio[repositorioActivo] << ".\n\n";
}

// Opcion 4: muestra la informacion general del repositorio activo.
// El "ultimo commit" es igual a la cantidad de commits (el numero del commit mas reciente).
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

// Opcion 5: registra un cambio pendiente (A, M o E) en el repositorio activo.
// Valida la capacidad maxima y que el mismo cambio no este repetido.
// La opcion Cancelar no modifica nada.
void simularCambio(const std::string nombreRepositorio[], std::string archivosCambios[][MAX_CAMBIOS],
                   char tipoCambioPendiente[][MAX_CAMBIOS], int cantidadCambiosPendientes[],
                   int repositorioActivo){

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
	cantidadCambiosPendientes[repositorioActivo] = cantidadCambiosPendientes[repositorioActivo] + 1;

	registrarBitacora("SIMULAR_CAMBIO | " + nombreRepositorio[repositorioActivo] + " | " + std::string(1, tipo) + " " + archivo);

	std::cout << "\nCambio registrado: [" << nombreTipoCambio(tipo) << "] " << archivo << "\n\n";
}

// Opcion 6: recorre la fila del repositorio activo en las matrices de cambios y los muestra.
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

// Opcion 7: crea un commit en el repositorio activo.
// Se verifican las condiciones ANTES de pedir el mensaje:
//   1. debe haber cambios pendientes
//   2. no se debe haber llegado al maximo de commits
// Al terminar: guarda el mensaje, sube el contador y deja en cero los cambios pendientes.
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

	// La columna j guarda el commit #(j+1), asi que el nuevo commit va en la
	// columna igual a la cantidad actual de commits.
	int columna = cantidadCommits[repositorioActivo];
	mensajesCommits[repositorioActivo][columna] = mensaje;
	cantidadCommits[repositorioActivo] = cantidadCommits[repositorioActivo] + 1;
	int numeroCommit = cantidadCommits[repositorioActivo];

	// Se limpian los cambios pendientes de ESTE repositorio (texto vacio y tipo ' ')
	// para no dejar datos residuales en la matriz.
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

// Opcion 8: muestra el ultimo commit (el de la ultima columna usada de la fila).
void mostrarUltimoCommit(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                         const std::string mensajesCommits[][MAX_COMMITS], const int cantidadCommits[],
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

// Opcion 10: tabla con TODOS los repositorios y estadisticas calculadas recorriendo los arreglos:
//   - acumulador: total de commits
//   - contador: repositorios con cambios pendientes
//   - mayor: repositorio con mas commits (en empate se queda el primero encontrado)
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

	int totalCommits = 0;        // acumulador
	int posicionMayor = 0;       // posicion del repositorio con mas commits
	int reposConCambios = 0;     // contador

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
	std::cout << "Repositorio con mas commits: " << nombreRepositorio[posicionMayor]
	          << " (" << cantidadCommits[posicionMayor] << ")\n";
	std::cout << "Repositorios con cambios pendientes: " << reposConCambios << "\n\n";
}

// Muestra una pregunta con opciones 1. Si / 2. No. Retorna true si el usuario elige Si.
// Se reutiliza al eliminar un repositorio y al salir sin poder guardar.
bool confirmarAccion(const std::string& pregunta){
	std::cout << pregunta << "\n";
	std::cout << "1. Si\n";
	std::cout << "2. No\n";
	int respuesta = leerNumeroValidado("Seleccione una opcion: ", 1, 2);
	return respuesta == 1;
}

// Elimina el repositorio de "posicion" desplazando hacia ARRIBA los siguientes
// en TODOS los arreglos y matrices, para no dejar huecos.
// Despues limpia la ultima posicion, que quedo repetida/libre.
void desplazarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[],
                           int cantidadCommits[], int cantidadCambiosPendientes[],
                           std::string mensajesCommits[][MAX_COMMITS],
                           std::string archivosCambios[][MAX_CAMBIOS],
                           char tipoCambioPendiente[][MAX_CAMBIOS],
                           int& cantidadRepositorios, int posicion){

	// Cada fila i copia a la fila i + 1 (por eso el for llega hasta cantidad - 1).
	for (int i = posicion; i < cantidadRepositorios - 1; i = i + 1){
		nombreRepositorio[i] = nombreRepositorio[i + 1];
		autorRepositorio[i] = autorRepositorio[i + 1];
		cantidadCommits[i] = cantidadCommits[i + 1];
		cantidadCambiosPendientes[i] = cantidadCambiosPendientes[i + 1];

		// En las matrices se copia la fila completa, columna por columna.
		for (int j = 0; j < MAX_COMMITS; j = j + 1){
			mensajesCommits[i][j] = mensajesCommits[i + 1][j];
		}
		for (int j = 0; j < MAX_CAMBIOS; j = j + 1){
			archivosCambios[i][j] = archivosCambios[i + 1][j];
			tipoCambioPendiente[i][j] = tipoCambioPendiente[i + 1][j];
		}
	}

	// La ultima posicion ocupada quedo duplicada: se limpia para no dejar datos residuales.
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

// Opcion 11: elimina un repositorio. Pide el numero (0 cancela) y confirmacion.
// Tras eliminar NO queda ningun repositorio activo (los indices cambiaron).
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
	// Se guarda el nombre antes de eliminar, porque despues esa posicion ya tendra otros datos.
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


// ===================== GUARDAR DATOS =====================

// Abre un archivo para escritura. Si no se puede, LANZA una excepcion (throw)
// con un mensaje descriptivo; la captura la funcion que coordina el guardado (guardarDatos).
// Al abrir sin std::ios::app el contenido anterior se sobrescribe, como pide el enunciado.
void abrirParaEscribir(std::ofstream& archivo, const std::string& nombreArchivo){
	archivo.open(nombreArchivo);
	if (!archivo.is_open()){
		throw std::runtime_error("No se pudo abrir " + nombreArchivo + " para escritura.");
	}
}

// repositorios.csv: encabezado + un registro por repositorio (nombre;autor).
void escribirRepositorios(const std::string nombreRepositorio[], const std::string autorRepositorio[],
                          int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_REPOSITORIOS);

	archivo << ENCABEZADO_REPOSITORIOS << "\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		archivo << nombreRepositorio[i] << ";" << autorRepositorio[i] << "\n";
	}
	archivo.close();
}

// commits.csv: un registro por commit (repositorio;numero;mensaje).
// La cantidad de commits NO se guarda: al cargar se calcula contando registros.
void escribirCommits(const std::string nombreRepositorio[], const std::string mensajesCommits[][MAX_COMMITS],
                     const int cantidadCommits[], int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_COMMITS);

	archivo << ENCABEZADO_COMMITS << "\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		for (int j = 0; j < cantidadCommits[i]; j = j + 1){
			archivo << nombreRepositorio[i] << ";" << (j + 1) << ";" << mensajesCommits[i][j] << "\n";
		}
	}
	archivo.close();
}

// cambios.csv: un registro por cambio pendiente (repositorio;tipo;archivo).
void escribirCambios(const std::string nombreRepositorio[], const std::string archivosCambios[][MAX_CAMBIOS],
                     const char tipoCambioPendiente[][MAX_CAMBIOS], const int cantidadCambiosPendientes[],
                     int cantidadRepositorios){
	std::ofstream archivo;
	abrirParaEscribir(archivo, ARCHIVO_CAMBIOS);

	archivo << ENCABEZADO_CAMBIOS << "\n";
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		for (int j = 0; j < cantidadCambiosPendientes[i]; j = j + 1){
			archivo << nombreRepositorio[i] << ";" << tipoCambioPendiente[i][j] << ";" << archivosCambios[i][j] << "\n";
		}
	}
	archivo.close();
}

// Funcion que COORDINA el guardado: llama a las tres funciones de escritura dentro de un try.
// Si alguna lanza una excepcion, aqui se captura, se avisa al usuario, se registra
// en la bitacora y el programa sigue funcionando.
// Retorna true si todo se guardo bien y false si hubo error (lo usa confirmarSalida).
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


// ===================== CARGAR DATOS =====================

// Deja TODOS los arreglos y matrices en su estado inicial (vacios) y la cantidad en 0.
// Se usa al iniciar el programa y cuando la carga falla (para no conservar datos parciales).
// Los int y char sin inicializar tienen basura, por eso se ponen en 0 / ' '.
void reiniciarDatos(std::string nombreRepositorio[], std::string autorRepositorio[],
                    int cantidadCommits[], int cantidadCambiosPendientes[],
                    std::string mensajesCommits[][MAX_COMMITS],
                    std::string archivosCambios[][MAX_CAMBIOS],
                    char tipoCambioPendiente[][MAX_CAMBIOS],
                    int& cantidadRepositorios){

	for (int i = 0; i < MAX_REPOSITORIOS; i = i + 1){
		nombreRepositorio[i] = "";
		autorRepositorio[i] = "";
		cantidadCommits[i] = 0;
		cantidadCambiosPendientes[i] = 0;
		for (int j = 0; j < MAX_COMMITS; j = j + 1){
			mensajesCommits[i][j] = "";
		}
		for (int j = 0; j < MAX_CAMBIOS; j = j + 1){
			archivosCambios[i][j] = "";
			tipoCambioPendiente[i][j] = ' ';
		}
	}
	cantidadRepositorios = 0;
}

// Arma el mensaje de error de un archivo con el formato: "archivo | Linea N: problema".
// Asi todos los errores de carga indican el archivo y la linea, como pide el enunciado.
std::string mensajeLinea(const std::string& nombreArchivo, int numeroLinea, const std::string& problema){
	return nombreArchivo + " | Linea " + std::to_string(numeroLinea) + ": " + problema;
}

// Si la linea termina en '\r' (archivos editados en Windows) se lo quita.
void quitarRetornoCarro(std::string& linea){
	int largo = linea.length();
	if (largo > 0 && linea[largo - 1] == '\r'){
		linea = linea.substr(0, largo - 1);
	}
}

// Separa una linea por ';' y guarda los primeros maxCampos campos en el arreglo "campos".
// RETORNA cuantos campos tiene la linea en total (aunque sean mas que maxCampos),
// para poder detectar lineas con una cantidad incorrecta de campos.
int dividirCampos(const std::string& linea, std::string campos[], int maxCampos){
	int cantidad = 0;
	std::string actual = "";
	int largo = linea.length();

	for (int i = 0; i < largo; i = i + 1){
		if (linea[i] == ';'){
			if (cantidad < maxCampos){
				campos[cantidad] = actual;
			}
			cantidad = cantidad + 1;
			actual = "";
		}
		else{
			actual = actual + linea[i];
		}
	}
	if (cantidad < maxCampos){
		campos[cantidad] = actual;
	}
	cantidad = cantidad + 1;
	return cantidad;
}

// Abre un CSV para lectura y valida su encabezado (primera linea).
//  - Si el archivo NO existe retorna false: en la primera ejecucion no es un error.
//  - Si existe pero esta vacio o tiene un encabezado distinto, LANZA una excepcion.
// numeroLinea queda en 1 (la linea del encabezado ya fue leida).
bool abrirParaLectura(std::ifstream& archivo, const std::string& nombreArchivo,
                      const std::string& encabezadoEsperado, int& numeroLinea){
	archivo.open(nombreArchivo);
	if (!archivo.is_open()){
		return false;
	}

	std::string linea;
	if (!std::getline(archivo, linea)){
		throw std::runtime_error(mensajeLinea(nombreArchivo, 1, "el archivo esta vacio o no se pudo leer (falta el encabezado)"));
	}
	numeroLinea = 1;
	quitarRetornoCarro(linea);

	if (linea != encabezadoEsperado){
		throw std::runtime_error(mensajeLinea(nombreArchivo, 1, "encabezado incorrecto, se esperaba: " + encabezadoEsperado));
	}
	return true;
}

// Lee el siguiente registro del archivo (salta las lineas completamente vacias).
// Va aumentando numeroLinea para poder reportar en que linea esta el problema.
// Retorna false cuando se llega al final del archivo.
// Si el archivo falla a la mitad de la lectura, lanza una excepcion.
bool leerSiguienteRegistro(std::ifstream& archivo, const std::string& nombreArchivo,
                           std::string& linea, int& numeroLinea){
	while (std::getline(archivo, linea)){
		numeroLinea = numeroLinea + 1;
		quitarRetornoCarro(linea);
		if (linea.length() > 0){
			return true;
		}
	}
	if (archivo.bad()){
		throw std::runtime_error(nombreArchivo + " | Error de lectura del archivo.");
	}
	return false;
}

// Carga repositorios.csv (nombre;autor). Se carga PRIMERO porque commits y cambios
// hacen referencia a los repositorios por su nombre.
// Valida: cantidad de campos, textos validos, repositorio duplicado y capacidad maxima.
// Retorna false si el archivo no existe. Ante cualquier dato malo lanza una excepcion.
bool cargarRepositorios(std::string nombreRepositorio[], std::string autorRepositorio[],
                        int& cantidadRepositorios){
	std::ifstream archivo;
	int numeroLinea = 0;

	if (!abrirParaLectura(archivo, ARCHIVO_REPOSITORIOS, ENCABEZADO_REPOSITORIOS, numeroLinea)){
		return false;
	}

	std::string linea;
	std::string campos[2];

	while (leerSiguienteRegistro(archivo, ARCHIVO_REPOSITORIOS, linea, numeroLinea)){

		int cantidadCampos = dividirCampos(linea, campos, 2);
		if (cantidadCampos != 2){
			throw std::runtime_error(mensajeLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			      "se esperaban 2 campos y se encontraron " + std::to_string(cantidadCampos)));
		}

		std::string errorNombre = errorDeTexto(campos[0], 1, MAX_LONGITUDNOMBRE);
		if (errorNombre != ""){
			throw std::runtime_error(mensajeLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			      "nombre de repositorio invalido. " + errorNombre));
		}

		std::string errorAutor = errorDeTexto(campos[1], 1, MAX_LONGITUDNOMBRE);
		if (errorAutor != ""){
			throw std::runtime_error(mensajeLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			      "autor invalido. " + errorAutor));
		}

		if (buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]) != -1){
			throw std::runtime_error(mensajeLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			      "el repositorio " + campos[0] + " esta duplicado"));
		}

		if (cantidadRepositorios >= MAX_REPOSITORIOS){
			throw std::runtime_error(mensajeLinea(ARCHIVO_REPOSITORIOS, numeroLinea,
			      "se supera la capacidad maxima de " + std::to_string(MAX_REPOSITORIOS) + " repositorios"));
		}

		nombreRepositorio[cantidadRepositorios] = campos[0];
		autorRepositorio[cantidadRepositorios] = campos[1];
		cantidadRepositorios = cantidadRepositorios + 1;
	}
	return true;
}

// Carga commits.csv (repositorio;numero;mensaje).
// Valida: cantidad de campos, repositorio existente, numero convertible (try/catch),
// capacidad maxima, numeros CONSECUTIVOS (1, 2, 3...) y mensaje valido (10 a 60 caracteres).
// La cantidad de commits de cada repositorio NO viene en el archivo: se calcula contando registros.
bool cargarCommits(const std::string nombreRepositorio[], int cantidadRepositorios,
                   std::string mensajesCommits[][MAX_COMMITS], int cantidadCommits[]){
	std::ifstream archivo;
	int numeroLinea = 0;

	if (!abrirParaLectura(archivo, ARCHIVO_COMMITS, ENCABEZADO_COMMITS, numeroLinea)){
		return false;
	}

	std::string linea;
	std::string campos[3];

	while (leerSiguienteRegistro(archivo, ARCHIVO_COMMITS, linea, numeroLinea)){

		int cantidadCampos = dividirCampos(linea, campos, 3);
		if (cantidadCampos != 3){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos)));
		}

		int posicion = buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]);
		if (posicion == -1){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "el repositorio " + campos[0] + " no existe"));
		}

		int numeroCommit = 0;
		if (!convertirEntero(campos[1], numeroCommit)){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "el numero de commit '" + campos[1] + "' no es un numero valido"));
		}

		if (cantidadCommits[posicion] >= MAX_COMMITS){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_COMMITS) + " commits"));
		}

		int numeroEsperado = cantidadCommits[posicion] + 1;
		if (numeroCommit != numeroEsperado){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "el numero de commit " + campos[1] + " no es consecutivo (se esperaba " + std::to_string(numeroEsperado) + ")"));
		}

		std::string errorMensaje = errorDeTexto(campos[2], MIN_LONGITUDMENSAJE, MAX_LONGITUDMENSAJE);
		if (errorMensaje != ""){
			throw std::runtime_error(mensajeLinea(ARCHIVO_COMMITS, numeroLinea,
			      "mensaje de commit invalido. " + errorMensaje));
		}

		mensajesCommits[posicion][cantidadCommits[posicion]] = campos[2];
		cantidadCommits[posicion] = cantidadCommits[posicion] + 1;
	}
	return true;
}

// Carga cambios.csv (repositorio;tipo;archivo).
// Valida: cantidad de campos, repositorio existente, tipo A/M/E, archivo valido,
// cambio repetido (mismo tipo y archivo) y capacidad maxima de cambios pendientes.
// La cantidad de cambios pendientes tampoco se guarda: se calcula contando registros.
bool cargarCambios(const std::string nombreRepositorio[], int cantidadRepositorios,
                   std::string archivosCambios[][MAX_CAMBIOS], char tipoCambioPendiente[][MAX_CAMBIOS],
                   int cantidadCambiosPendientes[]){
	std::ifstream archivo;
	int numeroLinea = 0;

	if (!abrirParaLectura(archivo, ARCHIVO_CAMBIOS, ENCABEZADO_CAMBIOS, numeroLinea)){
		return false;
	}

	std::string linea;
	std::string campos[3];

	while (leerSiguienteRegistro(archivo, ARCHIVO_CAMBIOS, linea, numeroLinea)){

		int cantidadCampos = dividirCampos(linea, campos, 3);
		if (cantidadCampos != 3){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "se esperaban 3 campos y se encontraron " + std::to_string(cantidadCampos)));
		}

		int posicion = buscarRepositorio(nombreRepositorio, cantidadRepositorios, campos[0]);
		if (posicion == -1){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "el repositorio " + campos[0] + " no existe"));
		}

		// El tipo debe ser exactamente un caracter: A, M o E.
		bool tipoValido = (campos[1].length() == 1) &&
		                  (campos[1][0] == 'A' || campos[1][0] == 'M' || campos[1][0] == 'E');
		if (!tipoValido){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "tipo de cambio '" + campos[1] + "' no valido (debe ser A, M o E)"));
		}
		char tipo = campos[1][0];

		std::string errorArchivo = errorDeTexto(campos[2], 1, MAX_LONGITUDNOMBRE);
		if (errorArchivo != ""){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "nombre de archivo invalido. " + errorArchivo));
		}

		if (existeCambioPendiente(archivosCambios, tipoCambioPendiente, cantidadCambiosPendientes,
		                          posicion, tipo, campos[2])){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "el cambio " + campos[1] + " " + campos[2] + " esta repetido en el repositorio " + campos[0]));
		}

		if (cantidadCambiosPendientes[posicion] >= MAX_CAMBIOS){
			throw std::runtime_error(mensajeLinea(ARCHIVO_CAMBIOS, numeroLinea,
			      "el repositorio " + campos[0] + " supera el maximo de " + std::to_string(MAX_CAMBIOS) + " cambios pendientes"));
		}

		int columna = cantidadCambiosPendientes[posicion];
		tipoCambioPendiente[posicion][columna] = tipo;
		archivosCambios[posicion][columna] = campos[2];
		cantidadCambiosPendientes[posicion] = cantidadCambiosPendientes[posicion] + 1;
	}
	return true;
}

// Funcion que COORDINA la carga al iniciar (se llama antes de mostrar el menu).
// Orden: repositorios, commits y cambios (los dos ultimos usan el nombre del repositorio).
//  - Si ningun archivo existe (primera ejecucion) NO es un error: inicia vacio y lo informa.
//  - Si todo carga bien, muestra cuantos repositorios se recuperaron.
//  - Si ocurre CUALQUIER error, se captura aqui, NO se conservan datos parciales
//    (se vacian todos los arreglos), se informa el problema y se advierte que
//    guardar sobrescribe los archivos existentes. El programa sigue funcionando.
void cargarDatos(std::string nombreRepositorio[], std::string autorRepositorio[],
                 int cantidadCommits[], int cantidadCambiosPendientes[],
                 std::string mensajesCommits[][MAX_COMMITS],
                 std::string archivosCambios[][MAX_CAMBIOS],
                 char tipoCambioPendiente[][MAX_CAMBIOS],
                 int& cantidadRepositorios){

	reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	               mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);

	try{
		bool existenRepositorios = cargarRepositorios(nombreRepositorio, autorRepositorio, cantidadRepositorios);
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
	catch (const std::exception& error){
		// Sin datos parciales: se vacia todo lo que alcanzo a cargarse.
		reiniciarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
		               mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);

		std::cout << "Error al cargar los datos: " << error.what() << "\n";
		std::cout << "El sistema inicia vacio.\n";
		std::cout << "ADVERTENCIA: si guarda los datos (opcion 12 o al salir), se sobrescribiran los archivos existentes.\n\n";

		registrarBitacora("ERROR | " + std::string(error.what()));
		registrarBitacora("CARGAR_DATOS | La carga fallo. El sistema inicia vacio");
	}
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

// Opcion 14: intenta guardar automaticamente antes de salir.
// Retorna true si se puede salir (se guardo bien, o el usuario acepto salir de todas formas).
// Retorna false si el guardado fallo y el usuario prefiere NO salir.
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

	// Antes de mostrar el menu se cargan los archivos.
	// cargarDatos tambien deja todos los arreglos inicializados (vacios).
	cargarDatos(nombreRepositorio, autorRepositorio, cantidadCommits, cantidadCambiosPendientes,
	            mensajesCommits, archivosCambios, tipoCambioPendiente, cantidadRepositorios);
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
				// Guarda automaticamente. Si falla y el usuario NO quiere salir,
				// opcion = 0 evita que termine el do-while y se vuelve al menu.
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
