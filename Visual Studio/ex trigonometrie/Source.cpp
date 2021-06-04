#include <iostream>
using namespace std;

#include <SDL.h>

//	****************  //
//	window attributs  //
//	****************  //
//	- position and size on screen
constexpr auto POS_X = 1920 + 200, POS_Y = 100;
constexpr auto WIDTH = 400, HEIGHT = 400;

//	include desired header files for libraries
#include "../lib_Point/Point.h"

SDL_Renderer* init_SDL(const char* title) {
#pragma region SDL initialization
	// SDL initialization
	SDL_Window* window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "ERREUR : can't initialize SDL!" << endl;
		exit(0);
	}

	//SDL_ShowCursor(SDL_DISABLE);	//	hide mouse cursor
	SDL_ShowCursor(SDL_ENABLE);	//	show mouse cursor

	//	create the window and its associated renderer
	window = SDL_CreateWindow(title, POS_X, POS_Y, WIDTH, HEIGHT, 0);
	return SDL_CreateRenderer(window, 0, 0);
#pragma endregion
}

void clearWindow(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void showRenderingBuffer(SDL_Renderer* renderer) {
	SDL_RenderPresent(renderer);
}

SDL_Event getNextEvent() {
	SDL_Event event;
	SDL_PollEvent(&event);

	return event;
}

bool keypressed(SDL_Event& event, char key) {
	return event.type == SDL_KEYDOWN && event.key.keysym.sym == key;
}

void quit_SDL() {
#pragma region SDL quit
	//	quit SDL
	SDL_Quit();
#pragma endregion
}


//	1. hauteur d'une tour
//	---------------------
double searchTowerHeight(double horizontalHeadTopAngle, double observerHeight, double distanceToObject) {
	return observerHeight + distanceToObject * tan(horizontalHeadTopAngle * M_PI / 180);
}

//	2. angle forme par 2 vecteurs donnes
//	------------------------------------
double computeAngle(const Vector& v1, const Vector& v2) {
	//	calculer le sinus et le cosinus de l'angle (v1, v2)
	//	calculer un angle avec (par exemple) acos
	//	en observant le signe du sinus, determiner l'angle

	//	calcul du produit scalaire
	double dotProduct = v1.x * v2.x + v1.y * v2.y;			//	= ||v1|| * ||v2|| * cos(v1, v2)
	double normeV1 = sqrt(v1.x * v1.x + v1.y * v1.y);		//	= ||v1||
	double normeV2 = sqrt(v2.x * v2.x + v2.y * v2.y);		//	= ||v2||
	double cosinusV1V2 = dotProduct / (normeV1 * normeV2);	//	= cos(v1, v2)
	double unAngle = acos(cosinusV1V2);						//	cet angle a la meme valeur de cosinus que celle de l'angle (v1, v2)
	//	ATTENTION : cet angle est compris entre 0 et 180 degres !

	//	pour determiner si l'angle precedent est effectivement l'angle (v1, v2), on test son sinus :
	double crossProduct = v1.x * v2.y - v1.y * v2.x;	//	= ||v1|| * ||v2|| * sin(v1, v2) 

	if (crossProduct < 0) {		//	l'angle (v1, v2) est compris entre 180 et 360 degres...
		unAngle = 2 * M_PI - unAngle;
	}

	return unAngle;
}

//	3. distance d'un bateau a la cote
//	---------------------------------
/*
--------------------------------------------
Premier raisonnement (avec la loi des sinus)
--------------------------------------------

loi des sinus

Dans un triangle ABC :
(on note BC=a, AC=b et AB=c)
sin(A)/a = sin(B)/b = sin(C)/c
-------------------------------------

on note teta = 180-(alpha+beta)
on note C le sommet bateau

loi des sinus dans le triangle (ABC) :
sin(alpha)/BC? = sin(beta)/AC? = sin(teta)/L

sin(alpha)/BC? = sin(teta)/L
	 on multiplie par BC des 2 cotes
sin(alpha) = BC*sin(teta)/L
	on divise par (sin(teta)/L) des 2 cotes
sin(alpha)/sin(teta)*L = BC

loi des sinus dans le triangle (BCD)
sin(beta)/d? = sin(180-beta)/BD? = sin(90)/BC

sin(beta)/d? = sin(90)/BC = 1/BC
	on multiplie par d x BC des 2 cotes
BC*sin(beta) = d

on remplace BC par la valeur trouvee plus haut
sin(alpha)/sin(teta)*L*sin(beta) = d

donc d = L * sin(alpha)* sin(beta) / sin(teta)

d = L * sin(alpha)* sin(beta) / sin(180 - (alpha+beta))

---------------------
Deuxieme raisonnement
---------------------

(1) tan(alpha) = d? / L1?
(2) tan(beta) = d? / L2?
(3) L1? + L2? = L

systeme de 3 equations a 3 inconnues

dans (1), on exprime L1 en fonction de alpha et d
(1)  =>  L1 = d/tan(alpha)

on remplace cette valeur de L1 dan (2) et (3)

(2) ne change pas : tan(beta) = d / L2
(3) d/tan(alpha) + L2 = L

dans (2), on exprime L2 en fonction de alpha et d
(2)	 =>  L2 = d/tan(beta)

on remplace cette valeur de L2 dans (3)

(3) => d/tan(alpha) + d/tan(beta) = L

on multiplie par tan(alpha)*tan(beta) des 2 cotes

d.tan(beta) + d.tan(alpha) = L.tan(alpha).tan(beta)
d.(tan(beta) + tan(alpha)) = ...

d = L.tan(alpha).tan(beta) / (tan(beta) + tan(alpha))
*/
double computeDistanceFromCoast(double L, double alpha, double beta) {
	//	conversion des angles en radians
	alpha *= M_PI / 180;
	beta *= M_PI / 180;

	//return L * sin(alpha) * sin(beta) / sin(alpha + beta);

	return L * tan(alpha) * tan(beta) / (tan(beta) + tan(alpha));
}


//	entry point of application
int main(int argc, char** argv) {
	SDL_Renderer* renderer = init_SDL("Trigonometrie");	//	this object will draw in our window

	//	1. height of a tower
	//	--------------------
	//double alpha = 30, h = 1.70, d = 500;
	//double alpha = 60, h = 1.78, d = 15;
	double alpha = 45, h = 1.80, d = 50;
	cout << searchTowerHeight(alpha, h, d) << endl;

	//	2. angle forme par 2 vecteurs donnes
	//	------------------------------------
	Color colors[3] = { Color(255, 0, 0, SDL_ALPHA_OPAQUE),
						Color(0, 255, 0, SDL_ALPHA_OPAQUE),
						Color(0, 0, 255, SDL_ALPHA_OPAQUE) };
	Point points[3] = { Point(WIDTH / 2, HEIGHT / 2, true) ,
						Point(WIDTH * 2 / 3, HEIGHT / 2, true),
						Point(WIDTH / 2 + 50, HEIGHT / 2 - 150, true) };


	//	3. distance d'un bateau a la cote
	//	---------------------------------
	double L = 1000, alpha1 = 45, beta1 = 30;
	//double L = 500, alpha1 = 60, beta1 = 70;
	//double L = 100, alpha1 = 40, beta1 = 40;
	cout << computeDistanceFromCoast(L, alpha1, beta1) << endl;


	//	*********  //
	//	main loop  //
	//	*********  //
	bool endOfGame = false;
	while (!endOfGame) {
		//	******************************  //
		//	draw image in rendering buffer  //
		//	******************************  //
		clearWindow(renderer);

		/*	draw any desired graphical objects here	*/
		for (int i = 0; i < 3; i++) {
			points[i].draw(renderer, colors[i], 10);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[1].x, points[1].y);	//	vector v1
		SDL_RenderDrawLine(renderer, points[0].x, points[0].y, points[2].x, points[2].y);	//	vector v2

		//	compute and print angle
		Vector v1(points[0], points[1]);
		Vector v2(points[0], points[2]);
		//	invesion des y car l'axe des Y sur l'ecran est dirige vers le bas
		v1.y = -v1.y;
		v2.y = -v2.y;
		//cout << computeAngle(v1, v2) * 180 / M_PI << endl;

		//	****************  //
		//	event management  //
		//	****************  //
		SDL_Event event = getNextEvent();

		/*	give event to objects for update if needed here	*/
		for (int i = 0; i < 3; i++) {
			points[i].update(event);
		}

		showRenderingBuffer(renderer);

		endOfGame = keypressed(event, 'q');
	}

	quit_SDL();

	return 0;
}