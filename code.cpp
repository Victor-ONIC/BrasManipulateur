#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <math.h>

// à activer si sa merde
/*
#pragma warning(disable:4996)
*/

int ecrireBinaire(const char* emplacement, unsigned char* contenu, const unsigned int taille) {
	std::ofstream f(emplacement, std::ios::in | std::ios::binary);
	if (f.is_open()) {
		for (unsigned int i = 0; i < taille; i++) f.write((char*)&contenu[i], sizeof(char));
		return 0;
	}
	return 1;
}

unsigned int tailleStrTableau(const char* contenu) {
	unsigned int i = 0;
	while (contenu[i] != '\0') i++;
	return i;
}

int calculAngles(double d1, double d2, double d, double h, int & a1, int & a2, int & a3) {
    if (h == 0) return 1;
    if (d == 0) return 2;
	double a = - (d / h);
	double b = pow(d, 2) / (2 * h) + h / 2 + pow(d1, 2) / (2 * h) - pow(d2, 2) / (2 * h);
	double delta = 4 * pow(a, 2) * pow(b, 2) - 4 * (pow(a, 2) + 1) * (pow(b, 2) - pow(d1, 2));
	if (delta < 0) return 3;
	double x;
	if (d > 0 && h > 0 || d < 0 && h < 0) x = (- 2 * a * b - sqrt(delta)) / (2 * (pow(a, 2) + 1));
	else x = (- 2 * a * b + sqrt(delta)) / (2 * (pow(a, 2) + 1));
	double alpha1 = acos(x / d1);
	double alpha2 = asin(x / d1) + asin((d - x) / d2);
	double alpha3 = asin((d - x) / d2);
	a1 = 100 * (180 * alpha1 / M_PI + 45) / 9;
	a2 = - 100 * (180 * alpha2 / M_PI - 225) / 9;
	a3 = - 100 * (180 * alpha3 / M_PI - 135) / 9;
	std::cout << 180 * alpha3 / M_PI << std::endl;
	std::cout << a3 << std::endl;
	return 0;
}

// Cette fonction ainsi que les fonctions "ecrireBinaire" et "tailleStrTableau" ont été originellement faites par Sic. Vous avez tout les droits sur lesdites fonctions, veuillez cependant laisser et ne pas modifier cette ligne, merci.
int ordre(const char* contenu) {
	const char* emplacement = "/dev/ttyUSB0"; //mettre l'emplacement du fichier du robot
	if (!ecrireBinaire(emplacement, (unsigned char*)contenu, tailleStrTableau(contenu))) return 0;
	return 1; //erreur lors de l'ouverture du fichier du robot
}

int calcul(double d, double h, int & a1, int & a2, int & a3) {
    if (!calculAngles(14.5, 18.5, d, h, a1, a2, a3)) return 0;
    return 1;
}

int mouvement(int a1, int a2, int a3) {
	char valeur_char[5];
	char commande[20];
	for(int i = 0; i < 20; i++) commande[i] = '\0';
	sprintf(valeur_char, "%d", a1);
	strcat(commande, "#1P");
	strcat(commande, valeur_char);
	strcat(commande, "T2000\r");
	ordre(commande);
	for(int i = 0; i < 20; i++) commande[i] = '\0';
	sprintf(valeur_char, "%d", a2);
	strcat(commande, "#2P");
	strcat(commande, valeur_char);
	strcat(commande, "T2000\r");
	ordre(commande);
	for(int i = 0; i < 20; i++) commande[i] = '\0';
	sprintf(valeur_char, "%d", a3);
	strcat(commande, "#3P");
	strcat(commande, valeur_char);
	strcat(commande, "T2000\r");
	ordre(commande);
	usleep(2000000);
	return 0;
}

int main() {
	int d, h, a0, a1, a2, a3;
	char valeur_char[5];
	char commande[20];
	
	ordre("#0P1500T1000\r");
	ordre("#1P1500T1000\r");
	ordre("#2P1500T1000\r");
	ordre("#3P1500T1000\r");
	usleep(1000000);
	
	d = 10;
	h = 5;
	
	while (d <= 30) {
		calcul(d, h, a1, a2, a3);
		
		ordre("#0P500T2000\r");
		
		mouvement(a1, a2, a3);
		
		a0 = 500;
		while (a0 <= 2500) {
			for(int i = 0; i < 20; i++) commande[i] = '\0';
			sprintf(valeur_char, "%d", a0);
			strcat(commande, "#0P");
			strcat(commande, valeur_char);
			strcat(commande, "T1000\r");
			ordre(commande);
			usleep(1000000);
			
			if (false) { //fonction ya un objet ?
				calcul(d, -2.5, a1, a2, a3);
				
				mouvement(a1, a2, a3);
				
				//fonction on pompe
				
				calcul(d, h, a1, a2, a3);
				
				mouvement(a1, a2, a3);
				
				if (false) { //fonction C rouge ?
					ordre("#0P500T2000\r");
				}
				else {
					ordre("#0P2500T2000\r");
				}
				
				calcul(10, h, a1, a2, a3);
				
				mouvement(a1, a2, a3);
				
				//fonction on dépompe
				
				for(int i = 0; i < 20; i++) commande[i] = '\0';
				sprintf(valeur_char, "%d", a0);
				strcat(commande, "#0P");
				strcat(commande, valeur_char);
				strcat(commande, "T2000\r");
				ordre(commande);
				
				calcul(d, h, a1, a2, a3);
				
				mouvement(a1, a2, a3);
			}
			
			a0 += 100;
		}
		d += 1;
	}
	
	ordre("#0P1500T1000\r");
	ordre("#1P1500T1000\r");
	ordre("#2P1500T1000\r");
	ordre("#3P1500T1000\r");
}
