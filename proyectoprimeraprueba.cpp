#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>

const int MAX_REPOSITORIOS=10;
const int MAX_COMMITS=50; 
const int MAX_CAMBIOS=20;
const int MAX_LONGITUDNOMBRE=30;
const int MIN_LONGITUDMENSAJE=10;
const int MAX_LONGITUDMENSAJE=60;

void mostrarMenuPrincipal (std::string nombreRepositorio[], int repositorioActivo){
	std::cout<<"\n        				 << MINI GIT >>\n";
	if (repositorioActivo== -1){
	std::cout << "					<<Repositorio activo: Niguno>>\n\n";
	}
	else{
	std::cout << "					<<Repositorio activo: " << nombreRepositorio[repositorioActivo] << ">>\n\n";
	}
	
	std::cout<<"					<<1. Crear Repositorio>>\n";
	std::cout<<"					<<2. Listar repositorios>>\n";
	std::cout<<"					<<3. Seleccionar repositorio activo>>\n";
	std::cout<<"					<<4. Ver información del repositorio activo>>\n";
	std::cout<<"					<<5. Simular cambios>>\n";
	std::cout<<"					<<6. Ver cambios pendientes>>\n";
	std::cout<<"					<<7. Crear commit>>\n";
	std::cout<<"					<<8. Ver ultimo commit>>\n";
	std::cout<<"					<<9. Ver historial de commits>>\n";
	std::cout<<"					<<10. Ver estado general y estadisticas>>\n";
	std::cout<<"					<11. Eliminar repositorio>>\n";
	std::cout<<"					<<12. Guardar datos >>\n";
	std::cout<<"					<<13. Ver bitacora>>\n";
	std::cout<<"					<<14. Salir>>\n\n";
	std::cout<<"					Seleccione una opcion: ";	
	
}

void mostrarSubmenuCambios(){
		std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
        std::cout<<"          1. Agregar Archivo\n";
        std::cout<<"          2. Modificar Archivo\n";
        std::cout<<"          3. Eliminar Archivo\n";
        std::cout<<"          4. Cancelar\n";
		std::cout<<"		Seleccione una opcion: ";	
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

std::string leerNombreArchivo (){
	std::string nombre;
std::cout<<"Nombre del archivo: ";
	std::getline(std::cin, nombre);

	while (soloEspacios(nombre)){
		std::cout << "El nombre del archivo no puede estar vacio ni tener solo espacios.\nDigitelo nuevamente: ";
		std::getline(std::cin, nombre);
	}
	return nombre;
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

int leerNumeroValidado(int minvalido, int maxvalido){
	std::string texto;
	int valor = 0;
	bool valido = false;

	while (!valido){
		std::getline(std::cin, texto);

		if (todosDigitos(texto)){
			try{
				valor = std::stoi(texto);
				if (valor >= minvalido && valor <= maxvalido){
					valido = true;
				}
			}
			catch (const std::exception&){
				valido = false;
			}
		}

		if (!valido){
			std::cout<<"Opcion no valida. Escriba solo un numero entre "<<minvalido<<" y "<<maxvalido<<" (sin espacios ni letras).\n";
			std::cout<<"Seleccione una opcion: ";
		}
	}
	return valor;
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

for (int i=0; i < MAX_REPOSITORIOS; i=i+1){ // los arreglos de enteros empiezan con basuras, hay que ponerlos en 0... algo asi
	cantidadCommits[i]=0;
	cantidadCambiosPendientes[i]=0;
}

do {
	system("cls");
	mostrarMenuPrincipal(nombreRepositorio, repositorioActivo);	
	opcion=leerNumeroValidado(1,14);
	
	if (opcion >= 4 && opcion <= 9 && repositorioActivo == -1){ // del case 4 al 9 necesita un repo activo
			std::cout<<"\nDebe seleccionar un repositorio activo antes de realizar esta operacion.\n";
			system("pause");
			continue;
		}
	
		switch(opcion){
			
			case 1:
				system("cls");
				std::cout<<"Opcion digitada: 1\nEntrando a: Crear repositorio\n\n";
			
				system("pause");
				break;

			case 2:
				system("cls");
				std::cout<<"Opcion digitada: 2\nEntrando a: Listar repositorios\n\n";
	
				system("pause");
				break;

			case 3:
				system("cls");
				std::cout<<"Opcion digitada: 3\nEntrando a: Seleccionar repositorio activo\n\n";
				
				system("pause");
				break;

			case 4:
				system("cls");
				std::cout<<"Opcion digitada: 4\nEntrando a: Ver informacion del repositorio activo\n\n";

				system("pause");
				break;

			case 5:
				system("cls");
				std::cout<<"Opcion digitada: 5\nEntrando a: Simular cambios\n\n";

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
