#include <iostream>
#include <string>
int main(){

int i=0;
std::string nombrerepositorio, autor;
bool repositorioCreado = false;

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
	
{
    		system("cls");

    		if (repositorioCreado==true)  {
        	std::cout << "ADVERTENCIA: Ya existe un repositorio activo.\n";
        	std::cout << "No se puede crear otro repositorio.\n";
        	system("pause");
        	break;
    }

    		std::cout << "Opcion digitada: 1...\n";
    		std::cout << "Entrando a: Inicializando Repositorio\n\n";

    		std::cin.ignore();

    		std::cout << "Digite el nombre del repositorio: ";
    		std::getline(std::cin, nombrerepositorio);

    	while (nombrerepositorio == "")
    {
        		std::cout << "El nombre del repositorio no puede estar vacio. "
                  << "Digite nuevamente: ";
        		std::getline(std::cin, nombrerepositorio);
    }

    		std::cout << "Digite el nombre del autor: ";
    		std::getline(std::cin, autor);

    	while (autor == "")
    {
        	std::cout << "El nombre del autor no puede estar vacio. "
                  << "Digite nuevamente: ";
        	std::getline(std::cin, autor);
    }

			    repositorioCreado = true;

 			   std::cout << "\nRepositorio creado correctamente.\n";

    		system("pause");
   			 break;
			case 2:
			    system("cls");
				std::cout<<"Opcion digitada: 2...\nEntrando a: Informacion del Repositorio\n";
				if (!repositorioCreado==true){
						
						std::cout<<"No se a creado ningun repositorio\n";
						std::cout<<"\n";
						system("pause");
					}
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
}	while (i!=8);
	
	

		return 0;

}
