tarea_programada_1
==================

Repositorio para el desarrollo de la primera tarea programada de LP

/* En esta progra intentaremos comunicar dos equipos de manera que sea posible el intercambio
	de archivos entre sí */


¿ Que debemos hacer ?

Una función servidor() que:
	1- Creará el socket [  con la función socket() ]
	2- "Bindear" el socket a una dirección [ usando bind() ]
	3- Estará atento para escuchar conexiones [ listen() ]
	4- Aceptará conexiones [ con accept() ]
	5- Envia y recibe "archivos" [ read(), write() sytem calls ]

Una función cliente() que:
	1- Creará el socket [  con la función socket() ]
	2- Conectará el socket a la dirección del servidor[ connect () system call]
	3- Envia y recibe "archivos" [ read(), write() sytem calls ]

	
