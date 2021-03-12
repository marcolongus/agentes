//Parametros principales:

const int    N = 100; //Cantidad de agentes.
const double L = 20; //Largo del sistema. Area = L*L.


//Parametros de evolución del sistema. 
//Pre-condición para estos parámetros (dt*v<0.1):

const double delta_time      = 0.01;
const double active_velocity = 0.05; //Velocidad de las partículas cuando no interactúan. 


//Condición inicial SIR

const double p_init  = 0.1; //Infectadas iniciales. 
const double p_rinit = 0.1; //Refractarias iniciales (elegidas al azar).
const double p_dinit = 0.0; //Refractarias iniciales (elegidas a partir de lista de velocidades).

const int spin = 3; //Estados internos.

//Constantes:

const double Pi       = 3.14159265358979323846;
const double dos_Pi   = 2*Pi;
const double infinity = 1000000000;

//Características de agentes.

const double radio    = 1.;
const double diameter = 2.*radio;