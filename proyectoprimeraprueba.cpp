#include <iostream>
#include <string>
#include <iomanip>
int main(){

int i=0, cantidadCommits=0, opcion=0;
std::string nombrerepositorio, autor, estado;
bool repositorioCreado = false, cambiosPendientes= false;

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
		
if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(1000, '\n');

    std::cout << "Opcion no valida. Debe ingresar un numero.\n";
    system("pause");
    continue;
}
		
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
   		}
			case 2:
			    system("cls");
				std::cout<<"Opcion digitada: 2...\nEntrando a: Informacion del Repositorio\n\n";
				if (!repositorioCreado){ //quite el ==true porque es redundante 
						
						std::cout<<"No se ha creado ningun repositorio...\n";
						std::cout<<"\n";
						system("pause");
					}
				else {
					estado = "Inicializado";
					estado += (cambiosPendientes) ? " | Cambios pendientes" : " | Sin cambios pendientes"; // aqui cambia el valor de estado dependiendo del valor de cambios pendientes
					
					std::cout << "					Repositorio: " << nombrerepositorio << "\n";
					std::cout << "					Autor del repositorio: " << autor << "\n";
					std::cout << "					Estado: " << estado << "\n"; //revisar como seria en caso de que no este inicializado ya que no use variable
					std::cout << "					Cantidad de commits: " << cantidadCommits << "\n";
					std::cout << "					Ultimo commit: #" << cantidadCommits << "\n";
					std::cout << "					Cambios pendientes: " << (cambiosPendientes ? "Si": "No") << "\n";
				}
				
					system("pause");
				
				break;
				system("cls");
			case 3:
				
				system("cls");
			    std::cout<<"Opcion digitada: 3...\nEntrando a: Simulador de Cambios\n";
				
				std::cout << "\n        				 << SIMULADOR DE CAMBIOS >>\n";
				
				std::cout<<" 					<<1. Agregar Archivo>>\n";
				std::cout<<"					<<2. Modificar archivo>>\n";
				std::cout<<"					<<3. Eliminar archivo>>\n";
				std::cout<<"					<<4. Cancelar>>\n";
				
				std::cin>> opcion; //revisen si hay que cambiar el nombre de la variable para mas adelante
		
				if (std::cin.fail()) { //meti esta estructura por si daba el mismo bug que antes
   					std::cin.clear();
    				std::cin.ignore(1000, '\n');

    				std::cout << "Opcion no valida. Debe ingresar un numero.\n";
    				system("pause");
  					  continue;
			}
				switch (opcion) {
					
					case 1:
						system("cls");
						std::cout <<"Opcion digitada: 1...\n Entrando a: Agregar archivo\n\n";
						system("pause");
        				break;

    				case 2:
       					system("cls");
       					std::cout << "Opcion digitada: 2...\nEntrando a: Modificar Archivo\n\n";
						system("pause");
       					break;

   					case 3:
        				system("cls");
       					std::cout << "Opcion digitada: 3...\nEntrando a: Eliminar Archivo\n\n";

       					system("pause");
       					break;

   					case 4:
       					system("cls");
      					std::cout << "Opcion digitada: 4...\nCancelando...\n";
       					system("pause");
       					break;

   					default:
        				std::cout << "Opcion no valida. Digite una opcion del SUBMENU.\n";
       					system("pause");
        				break;
				}

				system("pause");
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
				std::cout<<"Opcion no valida. Digite una opcion del MENU.\n";
				system("pause");
				break;
		}

	} while (i!=8);
	
	

		return 0;
		
		//agregue la variable estado para cuando mas adelante se necesite revisar si hay cambios pendientes en el repo, entonces es bueno trabajarla por aparte para no usarla como string si no se cambie sola por el estado de la variable

}
