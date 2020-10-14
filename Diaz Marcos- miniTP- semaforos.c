#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#define LIMITE 50

//inicializo los mutex
pthread_mutex_t salero = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cocina = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t horno = PTHREAD_MUTEX_INITIALIZER;
int verifica;

struct semaforos {
                sem_t sem_mezclar;
                sem_t sem_salarMezcla;
                sem_t sem_armarMedallones;
                sem_t sem_cocinarHamburguesa;
                sem_t sem_hornearPan;
                sem_t sem_cortarExtras;
                sem_t sem_armarHamburguesa;

};

struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
};


struct parametro {
        int equipo_param;
        struct semaforos semaforos_param;
        struct paso pasos_param[8];
};


void* imprimirAccion(void *data, char *accionIn) {
	        FILE* competencia=fopen("resultado.txt","a");
        char equipo[3];
        char line1[100];
        char line2[100];
        char line3[100];
        struct parametro *mydata = data;
        int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
        int i;
        for(i = 0; i < sizeArray; i ++){
                if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
                        sprintf(equipo,"%d",mydata->equipo_param);
                        strcat(line1,"\n Equipo ");
                        strcat(line1,equipo);
                        strcat(line1,":");
                        strcat(line1,mydata->pasos_param[i].accion);
                        strcat(line1,"\n");
                        fputs(line1,competencia);

                //calculo la longitud del array de ingredientes
                int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
				int h;
                fputs(line2,competencia);
                for(h = 0; h < sizeArrayIngredientes; h++) {
                                //consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
                                if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
                                                        //Concateno en line3 un vacio, para limpiar la variable
                                                        strcpy(line3,"");
                                                        //Guardo en line3 distintas cadenas
                                                        strcat(line3,"-");
                                                        strcat(line3,mydata->pasos_param[i].ingredientes[h]);
                                                        strcat(line3,"\n");
                                                        //Subo al archivo competencia los strings que se encuentran en line3
                                                        fputs(line3,competencia);
                                }
                        }
                }
        }
        fclose(competencia);
}
void* cortar(void *data) {
        char *accion = "cortar";
        struct parametro *mydata = data;
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
        sem_post(&mydata->semaforos_param.sem_mezclar);
                pthread_exit(NULL);
}


void* mezclar(void *data) {
        char *accion = "mezclar";
        struct parametro *mydata = data;
                sem_wait(&mydata->semaforos_param.sem_mezclar);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
                sem_post(&mydata->semaforos_param.sem_salarMezcla);
                pthread_exit(NULL);
}
void* salar(void *data) {
        pthread_mutex_lock(&salero);
        char *accion = "salar";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_salarMezcla);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
        sem_post(&mydata->semaforos_param.sem_armarMedallones);
        pthread_mutex_unlock(&salero);
        pthread_exit(NULL);
}
void* armarMedallones(void *data) {
        char *accion = "armar";
        struct parametro *mydata = data;
                sem_wait(&mydata->semaforos_param.sem_armarMedallones);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
                sem_post(&mydata->semaforos_param.sem_cocinarHamburguesa);
        pthread_exit(NULL);
}


void* cocinarHamburguesa(void *data) {
        pthread_mutex_lock(&cocina);
        char *accion = "cocinar";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_cocinarHamburguesa);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
        pthread_mutex_unlock(&cocina);
        sem_post(&mydata->semaforos_param.sem_hornearPan);
        pthread_exit(NULL);
}
void* hornearPan(void *data) {
        pthread_mutex_lock(&horno);
        char *accion = "hornear";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_hornearPan);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
        pthread_mutex_unlock(&horno);
        sem_post(&mydata->semaforos_param.sem_cortarExtras);
        pthread_exit(NULL);
}


void* cortarExtras(void *data) {
        char *accion = "cortarExtras";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_cortarExtras);
        imprimirAccion(mydata,accion);
        usleep( 2000000 );
        sem_post(&mydata->semaforos_param.sem_armarHamburguesa);
        pthread_exit(NULL);
}
void* armarHamburguesa(void *data) {
        char *accion = "armarHamburguesa";
        struct parametro *mydata = data;
                sem_wait(&mydata->semaforos_param.sem_armarHamburguesa);
        imprimirAccion(mydata,accion);
                verifica++;

                //Me subira al archivo cual es el equipo ganador
                if(verifica==1){
                        FILE* competencia=fopen("resultado.txt","a");
                        char lineaganador[100];
                        char equipo[3];
                        sprintf(equipo,"%d",mydata->equipo_param);

                        strcat(lineaganador,"\nEl equipo ");
                        strcat(lineaganador,equipo);
                        strcat(lineaganador,"¡Es el ganador!\n");
                        fputs(lineaganador,competencia);
                        fclose(competencia);
                }
                else if(verifica==2) {
                        FILE* competencia=fopen("resultado.txt","a");
                        char segundopuesto[100];
                        char equipo[3];
                        sprintf(equipo,"%d",mydata->equipo_param);
                        strcat(segundopuesto,"\nEl equipo ");
                        strcat(segundopuesto,equipo);
                        strcat(segundopuesto,"¡ Salio segundo!\n");
                        fputs(segundopuesto,competencia);
                        fclose(competencia);
                        }
                else{
                        FILE* competencia=fopen("resultado.txt","a");
                        char tercerpuesto[100];
                        char equipo[3];
                        sprintf(equipo,"%d",mydata->equipo_param);
                        strcat(tercerpuesto,"\nEl equipo ");
                        strcat(tercerpuesto,equipo);
						strcat(tercerpuesto,"¡Salio tercero!\n");
                        fputs(tercerpuesto,competencia);
                        fclose(competencia);
                }
                usleep( 2000000 );
        pthread_exit(NULL);
}
void* ejecutarReceta(void *i){
        //Defino los semaforos
        sem_t sem_mezclar;
        sem_t sem_cortarExtras;
        sem_t sem_hornearPan;
        sem_t sem_armarMedallones;
        sem_t sem_salarMezcla;
        sem_t sem_cocinarHamburguesa;
        sem_t sem_armarHamburguesa;

        pthread_t p1;
        pthread_t p2;
        pthread_t p3;
        pthread_t p4;
        pthread_t p5;
        pthread_t p6;
        pthread_t p7;
        pthread_t p8;

        int p = *((int *) i);


        struct parametro *pthread_data = malloc(sizeof(struct parametro));

        pthread_data->equipo_param = p;
 		FILE* receta=fopen("receta.txt","r");
        char delimitador[]=",:";
        int largo=8;
        char *sub;
        char line[100];
        //Recorro la cantidad de lineas del archivo hasta el final
        for (int i=0;i<largo;i++){
                //Leo la linea
                fgets(line,100,receta);
                sub=strtok(line,delimitador);
                strcpy(pthread_data->pasos_param[i].accion,sub);
                while(sub!=NULL){
                                //Recorro el resto de la linea
                                for(int j=0;j<4;j++){
                                sub=strtok(NULL,delimitador);
                                if(sub!=NULL){
                                        strcpy(pthread_data->pasos_param[i].ingredientes[j],sub);
                                }
                        }
                }
        }
        fclose(receta);
        
 		pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
        pthread_data->semaforos_param.sem_salarMezcla = sem_salarMezcla;
        pthread_data->semaforos_param.sem_armarMedallones = sem_armarMedallones;
        pthread_data->semaforos_param.sem_cocinarHamburguesa = sem_cocinarHamburguesa;
        pthread_data->semaforos_param.sem_cortarExtras = sem_cortarExtras;
        pthread_data->semaforos_param.sem_hornearPan= sem_hornearPan;
        pthread_data->semaforos_param.sem_armarHamburguesa = sem_armarHamburguesa;


        sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_salarMezcla),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armarMedallones),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_cocinarHamburguesa),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_cortarExtras),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_hornearPan),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armarHamburguesa),0,0);

        int rc;
        
        rc = pthread_create(&p1,NULL,cortar,pthread_data);
        rc = pthread_create(&p2,NULL,mezclar,pthread_data);
        rc = pthread_create(&p3,NULL,salar,pthread_data);
        rc = pthread_create(&p4,NULL,armarMedallones,pthread_data);
        rc = pthread_create(&p5,NULL,cocinarHamburguesa,pthread_data);
        rc = pthread_create(&p6,NULL,hornearPan,pthread_data);
        rc = pthread_create(&p7,NULL,cortarExtras,pthread_data);
        rc = pthread_create(&p8,NULL,armarHamburguesa,pthread_data);



        pthread_join (p1,NULL);
        pthread_join (p2,NULL);
        pthread_join (p3,NULL);
        pthread_join (p4,NULL);
        pthread_join (p5,NULL);
        pthread_join (p6,NULL);
        pthread_join (p7,NULL);
        pthread_join (p8,NULL);
        
        
		//destruccion de los semaforos
		sem_destroy(&sem_mezclar);
		sem_destroy(&sem_salarMezcla);
		sem_destroy(&sem_armarMedallones);
		sem_destroy(&sem_cocinarHamburguesa);
		sem_destroy(&sem_cortarExtras);
		sem_destroy(&sem_hornearPan);
		sem_destroy(&sem_armarHamburguesa);
		
		//destruccion de los mutex
        pthread_mutex_destroy(&salero);
        pthread_mutex_destroy(&cocina);
        pthread_mutex_destroy(&horno);
        pthread_exit(NULL);
        return NULL;
}



int main ()
{
    int rc;
    int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
        int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
        int *equipoNombre3 =malloc(sizeof(*equipoNombre3));

    *equipoNombre1 = 1;
        *equipoNombre2 = 2;
        *equipoNombre3 = 3;


    pthread_t equipo1;
        pthread_t equipo2;
        pthread_t equipo3;


        rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1);
        rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);
        rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);
  		
		pthread_join (equipo1,NULL);
        pthread_join (equipo2,NULL);
        pthread_join (equipo3,NULL);

    pthread_exit(NULL);

}




