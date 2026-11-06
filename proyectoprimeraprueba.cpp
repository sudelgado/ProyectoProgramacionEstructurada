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

void mostrarMenuPrincipal (const std::string nombreRepositorio[], int repositorioActivo){
	std::cout<<"\n        				 << MINI GIT >>\n";
	if (repositorioActivo== -1){
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
	std::cout<<"					<<12. Guardar datos >>\n";
	std::cout<<"					<<13. Ver bitacora>>\n";
	std::cout<<"					<<14. Salir>>\n\n";
}

// Muestra las opciones del submenu de "Simular cambios".
// La lectura de la opcion se hace con leerNumeroValidado.
void mostrarSubmenuCambios(){
	std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
	std::cout<<"          1. Agregar Archivo\n";
	std::cout<<"          2. Modificar Archivo\n";
	std::cout<<"          3. Eliminar Archivo\n";
	std::cout<<"          4. Cancelar\n\n";
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

// Lee un texto y lo vuelve a pedir hasta que sea valido:
//  - no puede estar vacio ni tener solo espacios
//  - no puede contener ';'
//  - su largo debe estar entre minLongitud y maxLongitud
// Se usa para TODOS los textos: nombre del repositorio, autor, archivo y mensaje del commit.
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
		else if (largo < minLongitud || largo > maxLongitud){
			std::cout << "El texto debe tener entre " << minLongitud << " y " << maxLongitud << " caracteres.\n";
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

// Lee un numero como texto y lo convierte con try/catch.
// Repite la lectura hasta que el dato sea numerico y este entre minvalido y maxvalido.
// "etiqueta" es el mensaje que se muestra antes de cada lectura.
int leerNumeroValidado(const std::string& etiqueta, int minvalido, int maxvalido){
	std::string texto;
	int valor = 0;
	bool valido = false;

	while (!valido){
		std::cout << etiqueta;
		std::getline(std::cin, texto);

		if (todosDigitos(texto)){
			try{
				std::size_t posicion = 0;
				valor = std::stoi(texto, &posicion);
				// verifica que TODO el texto se haya convertido
				if (posicion == texto.length() && valor >= minvalido && valor <= maxvalido){
					valido = true;
				}
			}
			catch (const std::exception&){   // invalid_argument y out_of_range
				valido = false;
			}
		}

		if (!valido){
			std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
		}
	}
	return valor;
}

// Agrega una linea al final de bitacora.txt (modo agregar, nunca sobrescribe).
// Si no se puede escribir, el programa continua sin interrumpirse.
void registrarBitacora(const std::string& evento){
	try{
		std::ofstream archivo(ARCHIVO_BITACORA.c_str(), std::ios::app);
		if (!archivo.is_open()){
			throw std::runtime_error("No se pudo abrir " + ARCHIVO_BITACORA);
		}
		archivo << evento << "\n";
		archivo.close();
	}
	catch (const std::exception&){
		// Si la bitacora falla, no se interrumpe el programa.
	}
}

// Busca un repositorio por su nombre recorriendo el arreglo.
// Retorna la posicion donde esta, o -1 si no existe.
int buscarRepositorio(const std::string nombreRepositorio[], int cantidadRepositorios, const std::string& nombreBuscado){
	for (int i = 0; i < cantidadRepositorios; i = i + 1){
		if (nombreRepositorio[i] == nombreBuscado){
			return i;
		}
	}
	return -1;
}

// Convierte el tipo de cambio ('A', 'M' o 'E') en su nombre completo.
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

// Devuelve true si en el repositorio indicado ya hay un cambio pendiente
// con el mismo tipo y el mismo archivo.
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

	// la primera posicion libre siempre es cantidadRepositorios (no hay huecos)
	int posicion = cantidadRepositorios;
	nombreRepositorio[posicion] = nombre;
	autorRepositorio[posicion] = autor;
	cantidadCommits[posicion] = 0;              // empieza sin commits
	cantidadCambiosPendientes[posicion] = 0;    // y sin cambios pendientes

	cantidadRepositorios = cantidadRepositorios + 1;
	repositorioActivo = posicion;               // queda seleccionado

	registrarBitacora("CREAR_REPOSITORIO | " + nombre + " | Autor: " + autor);

	std::cout << "\nRepositorio " << nombre << " creado correctamente.\n\n";
}

// Opcion 2: muestra la tabla de repositorios registrados.
// El repositorio activo se marca con un asterisco (*).
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

	int numero = leerNumeroValidado("Numero del repositorio (0 para cancelar): ", 0, cantidadRepositorios);

	if (numero == 0){
		std::cout << "\nOperacion cancelada. El repositorio activo no cambio.\n\n";
		return;
	}

	repositorioActivo = numero - 1;   // el usuario ve 1..N, el arreglo usa 0..N-1
	std::cout << "\nRepositorio activo: " << nombreRepositorio[repositorioActivo] << ".\n\n";
}

// Opcion 4: muestra la informacion general del repositorio activo.
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
// Valida capacidad maxima y que el mismo cambio (tipo + archivo) no este repetido.
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

// los arreglos de enteros y de char empiezan con basura, hay que inicializarlos
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
	
	if (opcion >= 4 && opcion <= 9 && repositorioActivo == -1){ // del case 4 al 9 necesita un repo activo
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

				system("pause");
				break;

			case 7:
				system("cls");
				std::cout<<"Opcion digitada: 7\nEntrando a: Crear commit\n\n";

				system("pause");
				break;

			case 8:
				system("cls");
				std::cout<<"Opcion digitada: 8\nEntrando a: Ver ultimo commit\n\n";

				system("pause");
				break;

			case 9:
				system("cls");
				std::cout<<"Opcion digitada: 9\nEntrando a: Ver historial de commits\n\n";

				system("pause");
				break;

			case 10:
				system("cls");
				std::cout<<"Opcion digitada: 10\nEntrando a: Ver estado general y estadisticas\n\n";

				system("pause");
				break;

			case 11:
				system("cls");
				std::cout<<"Opcion digitada: 11\nEntrando a: Eliminar repositorio\n\n";

				system("pause");
				break;

			case 12:
				system("cls");
				std::cout<<"Opcion digitada: 12\nEntrando a: Guardar datos\n\n";

				system("pause");
				break;

			case 13:
				system("cls");
				std::cout<<"Opcion digitada: 13\nEntrando a: Ver bitacora\n\n";

				system("pause");
				break;

			case 14:
				system("cls");
				std::cout<<"Saliendo....\n";

				break;
		}

	} while (opcion != 14);

	return 0;
}
