#include "parameters.h"    //Módulo con parametros para la simulación.
using namespace std; 

//Generador de números aleatorios en (0,1).
mt19937::result_type seed = time(0);
mt19937 gen(seed);                             //Standard mersenne_twister_engine seeded time(0)
uniform_real_distribution<double> dis(0., 1.); // dis(gen), número aleatorio real entre 0 y 1. 

/*Definimos la clase partículas y sus métodos */
/*texto sobre la clase*/
class particle{
	private:
		//Estado interno del agente.  
		int state;
	public:
		//Estado dinámico del agente. 
		double x,y;
		double velocity;
		double angle;

	//Constuctores:
	particle(); 
	particle(double x1, double y1, double vel, double ang);

	//Métodos:
	bool is_healthy()    {return (state <  1);}
	bool is_infected()   {return (state == 1);}
	bool is_refractary() {return (state == 2);}

	int get_state()      {return state;}

	void set_healthy()    {state = 0;}
	void set_infected()   {state = 1;}
	void set_refractary() {state = 2;}
};


/***************************************************************************************/
/***************************************************************************************/

/* Constructor de partícula genérica*/
particle::particle(){
	velocity = 0;
	angle    = 0;
	x        = 0;
	y        = 0;  
}

/*Constructor of a particle in a given phase-state (x,p) of the system */
particle::particle(double x1, double y1, double vel, double ang){
	velocity = vel;
	angle    = ang;
	x        = x1;
	y        = y1;
}


/***************************************************************************************/

/* Create a particle in tha random phase-state  */
/* Falta agregar un string que elija la distribución para las velocidades.*/
/*PONERLE AGENT EN VEZ DE A*/
particle create_particle(void){
	double x,y,velocity,angle;

	x     = dis(gen)*L;
	y     = dis(gen)*L;
	angle = dis(gen)*dos_Pi;

	//Tres distribuciones para asiganar la velocidad al azar:

	//velocity = -active_velocity*log(1. - dis(gen)); //distribución exponencial
	//velocity = pow( dis(gen)*( pow(v_max, 1- k_powerl) - pow(v_min,1-k_powerl))+pow(v_min,1-k_powerl), 1./(1.-k_powerl)); //power_law
	velocity = active_velocity;

	//Creación de la partícula:
	particle A(x,y,velocity,angle);
	
	//Setting del estado interno de la partícula:
	if   (dis(gen) < p_init){ A.set_infected();} //Agrega un porcentaje p_init de partículas infectadas.
	else A.set_healthy();

	//Agrega un pocentaje p_rinit de partículas en estado refractario.
	//No reasigna una previamente infectada.  
	if (dis(gen) < p_rinit and !A.is_infected() ){ A.set_refractary();}

	return A;
}


/*FUNCIONES AUXILIARES PARA LA CLASE*/
/*Real boundary condition  and integer boundary condition functions*/
double b_condition(double a){
    return fmod((fmod(a,L)+L),L);   
}

int my_mod(int a, int b){
	return ((a%b)+b)%b;
}

/*Distancia entre partículas*/
double distance(particle A, particle B){
	double x1, x2, y1, y2, res;
	res = infinity;
	x2  = B.x; y2 = B.y;
	for(int i=-1; i<2; i++) for(int j=-1; j<2; j++){
		x1 = A.x + i*L;
		x2 = A.y + j*L;
		res = min(res, pow(x1-x2,2) + pow(y1-y2,2));
	}
	return sqrt(res);
}

double distance_x(particle A, particle B){
		double x1, x2, res;
		int j = 0;
		vector<double> dx;

		dx.resize(3,0);
		res = infinity;
		x2  = B.x;
		for(int i=-1; i<2; i++){
			x1      = A.x + i*L; 
			dx[i+1] = x1 - x2;

			if (abs(dx[i+1]) < res ){
				res = abs(dx[i+1]);
				j = i;   
			} //if           
		}//for
		return dx[j+1];
}

double distance_y(particle A, particle B){   
		double y1, y2, res;
		int j = 0;
		vector<double> dy;

		dy.resize(3,0);
		res = infinity;
		y2  = B.y;
		for(int i=-1; i<2; i++){
			y1      = A.y + i*L; 
			dy[i+1] = y1 - y2;

			if (abs(dy[i+1]) < res ) {
				res = abs(dy[i+1]);
				j = i;   
			} //if       
		}//for
		return dy[j+1];
}

double distance1(double dx, double dy){
    return sqrt(pow(dx,2) + pow(dy,2));
}

/* Interact */
bool interact(particle A, particle B){

	return (distance(A,B) < diameter);
} //repensar esta función


/*INTERACTION FUNCTIONS*/
/* Evolution time step function of the particle */
/* Campo de interacción */

/*Agregar un vector<double> interaction_particle_distances de tamaño 2*index*/
vector<double> campo( vector<particle> system, vector<int> &index){ 
	vector<double> field; //Campo de salida
	vector<double> potencial;
    
	field.resize(2);
	potencial.resize(2,0); //inicia vector tamaño 2 en 0.
	for(int i=1; i < index.size(); i++){
		double dx_0i = distance_x(system[index[0]], system[index[i]]),
			   dy_0i = distance_y(system[index[0]], system[index[i]]),
			    d_0i = distance1(dx_0i, dy_0i);

		potencial[0] = pow(d_0i,-3)*dx_0i + potencial[0];
		potencial[1] = pow(d_0i,-3)*dy_0i + potencial[1];
	}//for
    for(int i=0; i<potencial.size(); i++) potencial[i] = gamma_friction*potencial[i];
    field[0] = system[index[0]].velocity*cos(system[index[0]].angle) + potencial[0]; 
    field[1] = system[index[0]].velocity*sin(system[index[0]].angle) + potencial[1]; 
    return field;
}

/***************************************************************************************/

particle evolution(vector<particle> &system, vector<int> &index, bool inter){
	particle Agent = system[index[0]];
	/* Dinámica espacial del agente*/
	if (inter){
		vector<double> k; 
		k.resize(2);
		k = campo(system,index);  //campo del sistema
		Agent.x = b_condition(Agent.x + delta_time*k[0]);
		Agent.y = b_condition(Agent.y + delta_time*k[1]);        
	}//if
    else{
		Agent.x = b_condition(Agent.x + Agent.velocity*cos(Agent.angle)*delta_time);
		Agent.y = b_condition(Agent.y + Agent.velocity*sin(Agent.angle)*delta_time);
	}//else
	/*El agente cambia de dirección en A.angle +/- pi/2*/ 
	if (dis(gen) < p_rotation){
		double ruido = (dis(gen)-0.5)*Pi;
		Agent.angle += ruido; 
	}   

    /*Dinámica de la epedemia*/
	bool flag = true; //Flag de infección.
	for (int i=1; i<index.size(); i++){
		if (Agent.is_healthy() && system[index[i]].is_infected()){
			if (dis(gen) < p_transmision){
				Agent.set_infected();
				flag = false; //no puede volverse refractaria en esta instancia de evolución.
			}
		}
	}//for
    //if (A.is_refractary() && (dis(gen) < p_recfractary) ) A.set_healthy(); //SIRS 
    if (Agent.is_infected() && flag && (dis(gen) < p_infection) ) Agent.set_refractary();
    return Agent;      
}