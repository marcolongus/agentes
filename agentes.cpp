/*
Autor: Benjamín R. Marcolongo. FAMAF-UNC.

Programa para evolucionar un sistema de N agentes en el tiempo.

Agentes:
		i. Las velocidades de los N agentes pueden ser uniformes o tomadas aleatoriamente. 
			1. Las distribuciones por defecto son de tipo exponencial o de ley de potencias.  			
		ii. Pueden interactuar a través de un potencial u otra forma (una red neuronal i.e).
			1. El potencial por defecto es de esferas blandas.
		ii.  Están confinados a un toroide topológico. 
			1. El tamaño característico del toro se denota con L (lado del cuadrado). 
		iii. Poseen un estado interno, caracterizado por un número entero.	 
			1. Este estado puede o no interactaur con la dinámica espacial.
		iV. El estado interno puede evolucionar en el tiempo.
			1. Esta dinámica está regulada, genéricamente, con una distribución de poisson.  


Red compleja:
			i. El programa trackea todo el sistema de interacciones y guarda la red compleja resultante.
			ii. La red compleja puede ser la asociada a la propagación del estado interno o la de contactos
			para alguna escala temporal. 

*/
#include <bits/stdc++.h>
#include "classparticle.h" //Módulo con la clase definida para los agentes. 

#define forn(i,a,b) for(int i=a; i<b; i++)

using namespace std;

int main(void){

	/*DEFINICIÓN DE ARCHIVOS DE SALIDA DEL PROGRAMA*/
	//Para modelado de epidemias:
	ofstream FinalState ("evolution.txt");
	ofstream epidemic   ("epidemia.txt") ;//Estado de la epidemia en cada instante modulo m.
	ofstream anim       ("animacion.txt");
	ofstream imax       ("imax.txt")     ;//Máxima cantidad de infectados.
	ofstream mips       ("mips.txt")     ;//Busqueda de mobility induced phase-separetion.


	//Red compleja:
	//Comentario sobre cómo está guardada esta información. 
	ofstream topology   ("topology.txt") ; //Se guarda la red compleja. 


	/*DECLARACIÓN DE VARIABLES*/
	vector<particle> system    , 
					 system_new;
	vector<bool>     inter    ,   //Flag de interacción. 	
					 inter_old;   //Flag de interacción terminada.
	vector<int>      state_vector;//En cada lugar contiene la población de cada estado. 

	/*Estuctura de datos para optimizar la búsqueda de interacciones entre agentes:
		1. Utiliza un red-and-black tree implementado en c++ como set.
		2. Cada agente está indexado por un int que representa su posición en 
		   los vectores system y system_new.
		3. Se construye una grilla con cuadrículas de tamaño 1 y cada a una se le asigna un set.
		4. Cada set contiene los agentes que están en cada cuadrícula.  
	*/

	vector<vector<set<int>>> box;
	int num_boxes = floor(L);

	//Inicializamos los vectores declarados previamente:
	inter.resize(N,false);
	inter_old.resize(N,false);

	box.resize(num_boxes);
	for (int i=0; i<box.size(); i++) box[i].resize(num_boxes);

	state_vector.resize(spin,0);
	
	/*CONDICIÓN INICIAL*/
	for(int p = 0; p < N; p++){
		particle Agent;       
		bool accepted = false;
		while(!accepted){
			accepted = true;
			Agent = create_particle();
			int i_index = floor(Agent.x),
				j_index = floor(Agent.y);
			//Si interactúa con otra partícula cambiamos la condición a no aceptada.
			forn(l,-2,3){
				forn(m,-2,3){
					int i = b_condition(i_index + l),
						j = b_condition(j_index + m);
					if (!box[i][j].empty()){
						for (auto element: box[i][j]){
							if (interact(Agent,system[element])) accepted = false;						
						}//for auto
					}//if not empty
				}//for m
			}//for l
			if (accepted) box[i_index][j_index].insert(p);
		}//while		
		system.push_back(Agent);
		state_vector[Agent.get_state()]++;
		anim << Agent.x <<" "<< Agent.y <<" "<< Agent.get_state() << endl;	
	}//for N


	/*ESCRITURA DE RESULTADOS*/
	cout << "--------------------" << endl;
	cout << "Experimento data:"    << endl;
	cout << "--------------------" << endl;
	cout << "Healthy, Infected, Refractary:" << endl;
	for (auto element: state_vector) cout << element << endl;
	cout << endl;
	
	//Reinicializar el vector.
	state_vector = {0,0,0};

	//Cerramos los archivos: 
	FinalState.close();
	epidemic.close();
	anim.close();
	imax.close();
	mips.close();
	topology.close();

	return 0;
}