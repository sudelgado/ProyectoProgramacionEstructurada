#include <iostream>
#include <string>
int main(){

int i=0;
std::string nombrerepositorio, autor;


do {
	system("cls");
		std::cout<<"\n        				 << MINI GIT >>\n";
		std::cout<<"					<<1. Inicializar Repositorio>>\n";
		std::cout<<"					<<2. Ver Informacion del Repositorio>>\n";
		std::cout<<"					<<3. Simular Cambios>>\n";
		std::cout<<"					<<4. Crear Commit>>\n";
		std::cout<<"					<<5. Ver Ultimo Commit>>\n";
		std::cout<<"					<<6. Ver Estado del Repositorio>>\n";
		std::cout<<"					<<7. Reiniciar Repositorio>>\n";
		std::cout<<"					<<8. Salir>>\n";
		std::cin>> i;
		
		switch(i){
			
			case 1:
				system("cls");
				std::cout<<"Opcion digitada: 1...\nEntrando a: Inicializando Repositorio\n";
				std::cin.ignore()
				std::cout<<"Digite el nombre del repositorio:      \n";
				std::getline(cin, nombrerepositorio);

				std::cout<<"Digite el nombre del autor:      \n";
				std::getline(cin,autor);

				while (nombrerepositorio= "") {
				cout<< "El nombre del reposotorio no puede estar vacío. Digitelo nuevamnete:      ";
				std::getline(cin, nombre repositorio." }
				
				
				break;
				system("cls");
			case 2:
			    system("cls");
				std::cout<<"Opcion digitada: 2...\nEntrando a: Informacion del Repositorio\n";
					
				break;
				system("cls");
			case 3:
				system("cls");
			    std::cout<<"Opcion digitada: 3...\nEntrando a: Simulador de Cambios\n";
				
				break;
				system("cls");
			case 4:
				system("cls");
			    std::cout<<"Opcion digitada: 4...\nEntrando a: Crear Commit\n";
				
				break;
				system("cls");
			case 5:
				system("cls");
			    std::cout<<"Opcion digitada: 5...\nEntrando a: Ver el Ultimo Commit\n";
				
				break;
				system("cls");
			case 6:
				system("cls");
			    std::cout<<"Opcion digitada: 6...\nEntrando a: Estado del Repositorio\n";
				
				break;
				system("cls");
			case 7:
				system("cls");
			    std::cout<<"Opcion digitada: 7...\nEntrando a: Reiniciar Repositorio\n";
				
				break;
				system("cls");
			case 8:
				system("cls");
			std::cout<<"Saliendo....";
			
			break;
				
			default:
				std::cout<<"Opcion no valida. Ingrese otra: ";
				break;
		}

	}
	while (i!=8);
		return 0;
}
