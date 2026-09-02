#include <iostream>
#include <string>
#include <iomanip>
int main(){

int i=0, cantidadCommits=0, opcioncase3=0, totalArchivos3=0;
std::string nombreRepositorio, autor, estado, nombreArchivo3, mensajeCommit, ultimoMensajeCommit;
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
        	std::cout<<"ADVERTENCIA: Ya existe un repositorio activo.\n";
        	std::cout<<"No se puede crear otro repositorio.\n\n";
        	system("pause");
        	break;
    }

    		std::cout<<"Opcion digitada: 1\nEntrando a: Inicializando Repositorio\n\n";

    		std::cin.ignore();

    		std::cout<<"Digite el nombre del repositorio: ";
    		std::getline(std::cin, nombreRepositorio);

    	while (nombreRepositorio=="")
    {
    	        system("cls");
        		std::cout<<"El nombre del repositorio no puede estar vacio.\nDigitelo nuevamente: ";
        		std::getline(std::cin, nombreRepositorio);
    }

    		std::cout<<"\nDigite el nombre del autor: ";
    		std::getline(std::cin, autor);

    	while (autor=="")
    {
    		system("cls");
    		std::cout<<"Nombre del repositorio: "<<nombreRepositorio;
        	std::cout<<"\n\nEl nombre del autor no puede estar vacio.\nDigitelo nuevamente: ";
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
					
					std::cout << "					Repositorio: " << nombreRepositorio << "\n";
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
	std::cout<<"Opcion digitada: 3...\nEntrando a: Simulador de Cambios\n\n";
	
		if (!(repositorioCreado)){
		std::cout<<"No existen registros de ningun repositorio.\n";
		std::cout<<"Debe inicializar un repositorio antes de simular cambios.\n\n";
		system("pause");
		break;
	}

    do {
        std::cout<<"        << Seleccione una de las siguientes opciones: >>\n\n";
        std::cout<<"          1. Agregar Archivo\n";
        std::cout<<"          2. Modificar Archivo\n";
        std::cout<<"          3. Eliminar Archivo\n";
        std::cout<<"          4. Cancelar\n";

        std::cin >> opcioncase3;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            
 			system("cls");
            std::cout << "Opcion no valida. Debe ingresar un numero.\n\n";
            system("pause");
            system("cls");
            opcioncase3 = 0;
        }
        else {
            switch (opcioncase3) {

                case 1:
                    system("cls");
                    std::cout<<"Opcion digitada: 1\nEntrando a: Agregar archivo\n\n";
                    std::cout<<"Nombre del archivo: ";
                    std::cin.ignore();
                    std::getline(std::cin, nombreArchivo3);
                    
                    while (nombreArchivo3 == ""){
			        system("cls");
			        std::cout << "El nombre del archivo no puede estar vacio.\nDigitelo nuevamente: ";
			        std::getline(std::cin, nombreArchivo3);
			        }
                    
                    totalArchivos3++;
                    cambiosPendientes=true;
                    system("cls");
                    std::cout<<"El archivo fue agregado correctamente.\n\n";
					system("pause");
					system("cls");
                    break;

                case 2:
                    system("cls");
                    std::cout <<"Opcion digitada: 2\nEntrando a: Modificar Archivo\n\n";
                    std::cout<<"Nombre del archivo: ";
                    std::cin>>nombreArchivo3;
                    cambiosPendientes=true;
                    std::cout<<"El archivo fue modificado correctamente.\n\n";
                    system("pause");
                    system("cls");
                    break;

                case 3:
                    system("cls");
                    std::cout <<"Opcion digitada: 3\nEntrando a: Eliminar Archivo\n\n";
                    std::cout<<"Nombre del archivo: ";
                    std::cin>>nombreArchivo3;
                    if (totalArchivos3 > 0){
                    	totalArchivos3--;
                    	cambiosPendientes=true;
                    	std::cout<<"El archivo: "<<nombreArchivo3<<" ha sido eliminado correctamente.\n\n";
                    	} else {
                    		std::cout<<"El archivo: "<<nombreArchivo3<<" no existe.\n\n";
						}
                    system("pause");
                    system("cls");
                    break;

                case 4:
                    system("cls");
                    std::cout <<"Opcion digitada: 4\nCancelando...\n\nRegresando al menu principal.\n\n";
                    system("pause");
                    system("cls");
                    break;

                default:
                    system("cls");
                    std::cout << "Opcion no valida. Digite una opcion del SUBMENU.\n\n";
                    system("pause");
                    system("cls");
                    break;
            }
        }

    } while (opcioncase3!=4);

    break;
			case 4:
				system("cls");
			    std::cout<<"Opcion digitada: 4\nEntrando a: Crear Commit\n\n";
			    
			    if (!(repositorioCreado)){
			    	std::cout<<"Debe inicializar un repositorio para crear commits.\nRegresando al menu principal.\n\n";
			    	system("pause");
			    	system("cls");
			    	break;
				}
				
				if (!(cambiosPendientes)){
					std::cout<<"No existen cambios para guardar.\nRegresando al menu principal.\n\n";
				system("pause");
				system("cls");	
				break;
				}
				
				std::cin.ignore();
				
				std::cout<<"Ingrese el mensaje del commit: ";
				std::getline(std::cin, mensajeCommit);
				
				while (mensajeCommit=="" || mensajeCommit.length()<10 || mensajeCommit.length()>60){
					system("cls");
					std::cout<<"Mensaje del commit con error(es): "<<mensajeCommit;
					
					if (mensajeCommit==""){
						std::cout<<"\n\nEl mensaje del commit NO puede estar vacio. Digitelo nuevamente: ";
					}else{
						std::cout<<"\n\nEl mensaje del commit debe tener entre 10 y 60 caracteres. Digitelo nuevamente: ";
					} 
					std::getline(std::cin, mensajeCommit);
				}
				
				cantidadCommits++;
				ultimoMensajeCommit=mensajeCommit;
				cambiosPendientes=false;
				
				system("cls");
				std::cout<<"Commit #"<<cantidadCommits<<" creado correctamente.\n\n";
				std::cout<<"No existen cambios pendientes sin guardar.\n\n";
				std::cout<<"Regresando al menu principal.\n\n";
				system("pause");
				break;
				
			case 5:
				system("cls");
			    std::cout<<"Opcion digitada: 5\nEntrando a: Ver el Ultimo Commit\n\n\n";
			    
			    if (cantidadCommits==0){
			    	std::cout<<"Aun no existe ningun commit registrado.\n\n";
				} else {

					std::cout<<"		Commit #"<<cantidadCommits;
					std::cout<<"		\nRepositorio: "<<nombreRepositorio;
					std::cout<<"		\nAutor: "<<autor;
					std::cout<<"		\nMensaje: "<<ultimoMensajeCommit;
					std::cout<<"		\nEstado: Guardado correctamente.\n\n\n";
				}
				system("pause");
				break;

			case 6:
				 system("cls");

    std::cout << "Opcion digitada: 6...\n";
    std::cout << "Entrando a: Estado del Repositorio\n\n";

    if (!repositorioCreado) {
        std::cout << "No se ha creado ningun repositorio.\n";
    }
    else {

        std::cout << "Repositorio: " << nombreRepositorio << "\n";

        if (cambiosPendientes) {
            std::cout << "Estado: Inicializado | Cambios pendientes\n";
        }
        else {
            std::cout << "Estado: Inicializado | Sin cambios pendientes\n";
        }

        if (cantidadCommits == 0) {
            std::cout << "Ultimo commit: Ninguno\n";
        }
        else {
            std::cout << "Ultimo commit: #" << cantidadCommits << "\n";
        }
    }

   		 system("pause");
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
