#include <stdlib.h> // standard library: funciones como atoi, malloc serán utilizadas y se importarán de dicha biblioteca 
#include <string.h> //contiene es de utilidad para trabajar con cadenas de caracteres y algunas operaciones de manipulación de memoria
#include <stdio.h> //standard input-output header, es la biblioteca estándar del lenguaje de programación C, el archivo de cabecera que contiene las definiciones de macros, las constantes, las declaraciones de funciones y la definición de tipos usados por varias operaciones estándar de entrada y salida.
#include <arpa/inet.h> // Funciones para el manejo de directorios.
#include <unistd.h>// Proporciona acceso al POSIX

/*Función encargada del envio de archivos  */

int cliente(int argc, char *argv[]) {
	printf("\033[33m** Habilitado el envio de archivos** \n \033[0m");
	int respuesta;
	sleep(1);
	printf("\033[33m Digite 1 para enviar archivos \n \033[0m");
	scanf("%d",&respuesta);
					
	if (respuesta==1){
		if (argc!=4){
			perror("No se ingresó la cantidad de datos solicitados");
			return -1;
			}
		
		char archivo;
		printf ("\033[33m Digite el nombre del archivo: \033[0m");
		scanf ("%s",&archivo);

 
	//  descriptor Socket: SOCK_STREAM es para TCP y SOCK_DGRAM para UDP 
		int desc = socket(AF_INET, SOCK_STREAM, 0);//desc contiene el descriptor del socket
 
     // Direccion Local
		struct sockaddr_in *direcc_loc = malloc(sizeof(struct sockaddr_in));
			{  (*direcc_loc).sin_family = AF_INET;
			(*direcc_loc).sin_addr.s_addr = INADDR_ANY;
			(*direcc_loc).sin_port = htons(atoi(argv[3]));
 
			bind(desc, (struct sockaddr *)direcc_loc, sizeof(struct sockaddr_in));
			}
  
     // Direccion remota (se desea conectar a) 
		struct sockaddr_in *direcc_rem = malloc(sizeof(struct sockaddr_in));
				{       
					(*direcc_rem).sin_family = AF_INET;
					(*direcc_rem).sin_addr.s_addr = inet_addr(argv[1]);
					(*direcc_rem).sin_port = htons(atoi(argv[2])); }
 
      //Conectarse al servidor
		connect(desc, (struct sockaddr *)direcc_rem , sizeof(struct sockaddr_in) );
		
		//Archivo a enviar 
		FILE *archivoEnviar;
		const char *archiv=&archivo;
		char buf[30]; // enviar un mensaje conexion establecida
		char barchivo;
	  
	  //Ejemplo:"/home/luisd,erick,mariam/Desktop/..."
		archivoEnviar=fopen(archiv,"rb");  // rb= read binaria
	  
		if (archivoEnviar==NULL){
			perror("No se puede abrir el archivo, proceso terminado \n");
			return -1;}
		  
		fseek(archivoEnviar, 0, SEEK_END);
		int len = ftell(archivoEnviar);
		sprintf(buf,"%d",len);

		rewind(archivoEnviar);
 
       //Se envía un mensaje de conexión establecida con el servidor
		send( desc,"\033[34m Conexión establecida \n \033[0m",30,0);
		printf("\033[33m Enviando Archivo...\n \033[0m");
		send(desc,buf,30, 0);
       
		sleep(1); // Para imprimir en orden
		while (!feof( archivoEnviar )){		   
			fread (&barchivo, 1, 1, archivoEnviar);
			send(desc,&barchivo, 1,0);           
        }
      
	   // Cerrar el socket y la conexion
		fclose(archivoEnviar);
		close(desc);
		free(direcc_loc);
		free(direcc_rem);
		return 0;
				
	}
	else{//if(respuesta!=1){  // Si el usuario digita Finalizar
		exit(-1);}
return 0;	
}

/*Función encargada de la recepción de archivos  */
int servidor(int argc, char *argv[]){
	sleep(1);
	printf("\033[32m** Habilitada la recepción de archivos** \n \033[0m");
	int tam_direccion = sizeof(struct sockaddr_in);
 
    //  descriptor Socket: SOCK_STREAM es para TCP y SOCK_DGRAM para UDP 
    int desc = socket(AF_INET, SOCK_STREAM, 0);
         
    // Direccion Local 
    struct sockaddr_in *direcc_loc = malloc(sizeof(struct sockaddr_in));

        { // Puerto local se utilizará el 3650 
          (*direcc_loc).sin_family = AF_INET;
          (*direcc_loc).sin_addr.s_addr = INADDR_ANY;// inet_addr(argv[1]);
          (*direcc_loc).sin_port = htons(atoi(argv[2]));
           bind(desc, (struct sockaddr *)direcc_loc , sizeof(struct sockaddr_in));
        }
  
	// Va a contener maximo 5 conexiones
    listen(desc, 5);
    
    // Direccion remota (se desea conectar a) 
    struct sockaddr_in *direcc_rem = malloc(sizeof(struct sockaddr_in));
   
        
    //Conexión entrante
    int clientremoto= accept(desc, (struct sockaddr *)direcc_rem, (void *)&tam_direccion);
 
    // Se imprimirá un mensaje de conexión establecida
    
    char barchiv;  
    int len; 
    
    int cont=1;
	char cantidad;
    char buf_bienvenida[30];   // 
    recv(clientremoto,buf_bienvenida , 30, 0);
    printf("%s \n",buf_bienvenida);
    
		    
    //Archivo enviado 
    FILE *archivoRecibido;    
	archivoRecibido = fopen ("ArchivoNuevo", "wb");
		
		if (archivoRecibido==NULL){
			perror("No se puede abrir el archivo,proceso terminado \n");
			return -1;}
		 
		recv(clientremoto, &cantidad, 30, 0);	   
		char*cantid=&cantidad;
		
		
		while (cont!=atoi(cantid)){
			memset( &barchiv, '\0', sizeof(barchiv) );// memset: sobrescribe  en un área de memoria con un patrón de bytes dado 
			
        if( (len = recv ( clientremoto, &barchiv, 1, 0 )) == -1 ){ 
			return -1;}
        else if ( len > 0 ){ 
                fwrite (&barchiv, 1, 1, archivoRecibido);
                cont++;
                			
            }
        }
			
            
		fclose(archivoRecibido);
		printf("\033[32mArchivo recibido \n \033[0m");

        //Cerrar el socket 
        close(desc);
     
		//se libera el emisor y el receptor
        free(direcc_loc ); 
        free(direcc_rem);  
	
        return 0;
         
}


int main(int argc, char *argv[]){
	int hijo=fork();  // Se clona el proceso y se utilizara el PID para diferenciarlos	
	
	if (hijo ==0){ // Si es el proceso hijo entrará aca
		cliente( argc,argv);
		kill(0); // Terminamos el proceso 
	

	}
	else{  // Si es el proceso padre, tomará este camino
		servidor(argc,argv);   // Se llama a servidor
		sleep(1);
		printf("\033[30m\"El que no posee el don de maravillarse,ni de entusiasmarse\n ");
		printf(" más le valdría estar muerto,porque sus ojos están cerrados.\"\n \033[0m");
		sleep(1);		
		printf("Salida con éxito \n");	
		}	
				
		
	return 0;
	kill(hijo);
	
}
