#include <stdlib.h>
#include <math.h>
#include <map>
#include <GL/glut.h>

using namespace std;

unsigned char prevKey;

class GrilaCarteziana {
public:
	int numarLinii;
	int numarColoane;
	double cx;
	double cy;
	double dc;
	double de;
	double epsilon;
	double radius;


	GrilaCarteziana(int numarLinii, int numarColoane) {
		this->numarLinii = numarLinii;
		this->numarColoane = numarColoane;
		this->epsilon = 0.1;
		this->cx = -1 + this->epsilon;
		this->cy = -1 + this->epsilon;
		this->dc = (2 - 2 * this->epsilon) / (numarColoane - 1);
		this->de = (2 - 2 * this->epsilon) / (numarLinii - 1);

		this->radius = this->dc / 3;
		this->deseneazaColoane();
		this->deseneazaLinii();
	}


	void deseneazaLinii() {
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		double p1_x = this->cx;
		double p_y = this->cy;
		double p2_x = -this->cx;
		for (int i = 1; i <= this->numarLinii; i++) {
			glVertex2f(p1_x, p_y);
			glVertex2f(p2_x, p_y);
			p_y += this->de;
		}
		glEnd();
	}


	void deseneazaColoane() {
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		double p_x = this->cx;
		double p1_y = this->cy;
		double p2_y = 1 - this->epsilon;
		for (int i = 1; i <= this->numarColoane; i++) {
			glVertex2f(p_x, p1_y);
			glVertex2f(p_x, p2_y);
			p_x += this->dc;
		}
		glEnd();
	}
	void writePixel(int i, int j) {
		double x = this->cx + i * this->dc;
		double y = this->cy + j * this->de;
		deseneaaCerc(x, y, radius, 10000);	
	}
	void afisareSegmentDreapta3(int x0, int y0, int xn, int yn) {
		double dx, dy;
		//panta dreptei m = rise/run (cat urc/cobor pe linii supra cat merg la stg/dreapta pe coloane)
		dx = abs(xn - x0);//run
		dy = abs(yn - y0);//rise

		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);
		int x = x0, y = y0;
		map<int, int> m;
		m[x] = y;
		printf("X:%d Y:%d\n", x, y);
		if (y < this->numarLinii) {
			this->writePixel(x, y);
		}
		while (x < xn) {
			printf("d:%d\n", d);
			if (d <= 0) {
				d += dE;
				x++;
			}
			else {
				d += dNE;
				x++;
				if (x0 < xn && y0 < yn) {
					y++;
				}
				else {
					y--;
				}
			}
			printf("X:%d Y:%d\n", x, y);
			if (y < this->numarLinii) {
				this->writePixel(x, y);
			}
			m[x] = y;
		}
	}

	void afisareSegmentDreapta3_1(int x0, int y0, int xn, int yn) {
		double dx, dy;
		if (x0 < xn && y0 < yn) {
			dx = xn - x0;
			dy = yn - y0;
		}
		else {
			dx = abs(xn - x0);
			dy = abs(yn - y0);
		}
		int d = 2 * dy - dx;
		int dE = 2 * dy;
		int dNE = 2 * (dy - dx);
		int x = x0, y = y0;
		map<int, int> m;
		m[x] = y;
		printf("X:%d Y:%d\n", x, y);
		this->writePixel(x, y);
		while (x > xn) {
			printf("d:%d\n", d);
			if (d <= 0) {
				d += dE;
				x++;
			}
			else {
				d += dNE;
				x++;
				if (x0 < xn && y0 < yn) {
					y++;
				}
				else {
					y--;
				}
			}
			printf("X:%d Y:%d\n", x, y);
			this->writePixel(x, y);
			m[x] = y;
		}

		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINES); // 0 15 15 10
		double x1, y1;
		x1 = this->cx + x0 * this->dc;
		y1 = this->cy + y0 * this->de;
		glVertex2f(x1, y1);
		x1 = this->cx + xn * this->dc;
		y1 = this->cy + yn * this->de;
		glVertex2f(x1, y1);
		glEnd();
	}


	void deseneaaCerc(double x, double y, double r, int numberOfSegments) {

		double pi = 4 * atan(1.0);

		glColor3ub(71, 71, 71);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);

		for (int i = 0; i < numberOfSegments; i++) {
			float x_aux = x + (this->radius*cos(i * 2 * pi / numberOfSegments));
			float y_aux = y + (this->radius*sin(i * 2 * pi / numberOfSegments));
			glVertex2f(x_aux, y_aux);
		}

		glEnd();
	}
	void afisareCerc4() {
		int x = 0;
		float y = this->radius;
		int d = 1 - this->radius;
		int dE = 3, dSE = 2 - this->radius + 5;

		map<double, double> m;
		m[x] = y;
		m[-x] = -y;
		m[-x] = y;
		m[x] = -y;
		if (x != y) {
			m[y] = x;
			m[-y] = -x;
			m[-y] = x;
			m[y] = -x;
		}



		while (y > x) {
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;
			}
			else {
				d += dSE;
				dE += 2;
				dSE += 4;
				y--;
			}
			x++;



			m[x] = y;
			m[-x] = -y;
			m[-x] = y;
			m[x] = -y;
			if (x != y) {
				m[y] = x;
				m[-y] = -x;
				m[-y] = x;
				m[y] = -x;
			}

		}

		glColor3f(10, 0.1, 0.1);
		glLineWidth(4);
		glBegin(GL_LINES);

		map<double, double> ::iterator it;
		for (it = m.begin(); it != m.end(); it++) {
			glVertex2f(this->cx + it->first*this->dc, this->cy + it->second*this->de);
		}
		glEnd();
	}


	void afisarePuncteCerc3(double x, double y) {
		map<double, double> m;
		m[x] = y;
		m[-x] = -y;
		m[-x] = y;
		m[x] = -y;
		if (x != y) {
			m[y] = x;
			m[-y] = -x;
			m[-y] = x;
			m[y] = -x;
		}
	}


	void deseneazaSegmentMartor(int x0, int y0, int xn, int yn) {
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINES);
		double x1, y1;
		x1 = this->cx + x0 * this->dc;
		y1 = this->cy + y0 * this->de;
		glVertex2f(x1, y1);
		x1 = this->cx + xn * this->dc;
		y1 = this->cy + yn * this->de;
		glVertex2f(x1, y1);
		glEnd();
	}
};

void Init(void) {
	// specifica culoarea unui buffer dupa ce acesta
	// a fost sters utilizand functia glClear. Ultimul
	// argument reprezinta transparenta (1 - opacitate
	// completa, 0 - transparenta totala)
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// grosimea liniilor
	glLineWidth(1);

	// dimensiunea punctelor
	glPointSize(4);

	// functia void glPolygonMode (GLenum face, GLenum mode)
	// controleaza modul de desenare al unui poligon
	// mode : GL_POINT (numai vf. primitivei) GL_LINE (numai
	//        muchiile) GL_FILL (poligonul plin)
	// face : tipul primitivei geometrice dpdv. al orientarii
	//        GL_FRONT - primitive orientate direct
	//        GL_BACK  - primitive orientate invers
	//        GL_FRONT_AND_BACK  - ambele tipuri
	glPolygonMode(GL_FRONT, GL_LINE);
}

void display1() {
	int numarLinii = 16;
	int numarColoane = 16;
	GrilaCarteziana* grilaCarteziana = new GrilaCarteziana(numarLinii, numarColoane);
	printf("Linia1\n");
	grilaCarteziana->afisareSegmentDreapta3(0, 15, 15, 10);
	grilaCarteziana->afisareSegmentDreapta3(0, 14, 15, 9);
	grilaCarteziana->afisareSegmentDreapta3(0, 16, 15, 11);
	grilaCarteziana->deseneazaSegmentMartor(0, 15, 15, 10);
	grilaCarteziana->afisareSegmentDreapta3(0, 0, 15, 7);
	grilaCarteziana->deseneazaSegmentMartor(0, 15, 15, 10);
	grilaCarteziana->deseneazaSegmentMartor(0, 0, 15, 7);

}

void Display(void) {
	printf("Call Display\n");

	// sterge buffer-ul indicat
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {

	case '1':
		display1();

	default:
		break;
	}

	// forteaza redesenarea imaginii
	glFlush();
}

/*
Parametrii w(latime) si h(inaltime) reprezinta noile
dimensiuni ale ferestrei
*/
void Reshape(int w, int h) {
	printf("Call Reshape : latime = %d, inaltime = %d\n", w, h);

	// functia void glViewport (GLint x, GLint y,
	//                          GLsizei width, GLsizei height)
	// defineste poarta de afisare : acea suprafata dreptunghiulara
	// din fereastra de afisare folosita pentru vizualizare.
	// x, y sunt coordonatele pct. din stg. jos iar 
	// width si height sunt latimea si inaltimea in pixeli.
	// In cazul de mai jos poarta de afisare si fereastra coincid
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*
Parametrul key indica codul tastei iar x, y pozitia
cursorului de mouse
*/
void KeyboardFunc(unsigned char key, int x, int y) {
	printf("Ati tastat <%c>. Mouse-ul este in pozitia %d, %d.\n",
		key, x, y);
	// tasta apasata va fi utilizata in Display ptr.
	// afisarea unor imagini
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

/*
Codul butonului poate fi :
GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
Parametrul state indica starea: "apasat" GLUT_DOWN sau
"eliberat" GLUT_UP
Parametrii x, y : coordonatele cursorului de mouse
*/
void MouseFunc(int button, int state, int x, int y) {
	printf("Call MouseFunc : ati %s butonul %s in pozitia %d %d\n",
		(state == GLUT_DOWN) ? "apasat" : "eliberat",
		(button == GLUT_LEFT_BUTTON) ?
		"stang" :
		((button == GLUT_RIGHT_BUTTON) ? "drept" : "mijlociu"),
		x, y);
}

int main(int argc, char** argv) {
	// Initializarea bibliotecii GLUT. Argumentele argc
	// si argv sunt argumentele din linia de comanda si nu 
	// trebuie modificate inainte de apelul functiei 
	// void glutInit(int *argcp, char **argv)
	// Se recomanda ca apelul oricarei functii din biblioteca
	// GLUT sa se faca dupa apelul acestei functii.
	glutInit(&argc, argv);

	// Argumentele functiei
	// void glutInitWindowSize (int latime, int latime)
	// reprezinta latimea, respectiv inaltimea ferestrei
	// exprimate in pixeli. Valorile predefinite sunt 300, 300.
	glutInitWindowSize(300, 300);

	// Argumentele functiei
	// void glutInitWindowPosition (int x, int y)
	// reprezinta coordonatele varfului din stanga sus
	// al ferestrei, exprimate in pixeli. 
	// Valorile predefinite sunt -1, -1.
	glutInitWindowPosition(100, 100);

	// Functia void glutInitDisplayMode (unsigned int mode)
	// seteaza modul initial de afisare. Acesta se obtine
	// printr-un SAU pe biti intre diverse masti de display
	// (constante ale bibliotecii GLUT) :
	// 1. GLUT_SINGLE : un singur buffer de imagine. Reprezinta
	//    optiunea implicita ptr. nr. de buffere de
	//    de imagine.
	// 2. GLUT_DOUBLE : 2 buffere de imagine.
	// 3. GLUT_RGB sau GLUT_RGBA : culorile vor fi afisate in
	//    modul RGB.
	// 4. GLUT_INDEX : modul indexat de selectare al culorii.
	// etc. (vezi specificatia bibliotecii GLUT)
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Functia int glutCreateWindow (char *name)
	// creeaza o fereastra cu denumirea data de argumentul
	// name si intoarce un identificator de fereastra.
	glutCreateWindow(argv[0]);

	Init();

	// Functii callback : functii definite in program si 
	// inregistrate in sistem prin intermediul unor functii
	// GLUT. Ele sunt apelate de catre sistemul de operare
	// in functie de evenimentul aparut

	// Functia 
	// void glutReshapeFunc (void (*Reshape)(int width, int height))
	// inregistreaza functia callback Reshape care este apelata
	// oridecate ori fereastra de afisare isi modifica forma.
	glutReshapeFunc(Reshape);

	// Functia 
	// void glutKeyboardFunc (void (*KeyboardFunc)(unsigned char,int,int))
	// inregistreaza functia callback KeyboardFunc care este apelata
	// la actionarea unei taste.
	glutKeyboardFunc(KeyboardFunc);

	// Functia 
	// void glutMouseFunc (void (*MouseFunc)(int,int,int,int))
	// inregistreaza functia callback MouseFunc care este apelata
	// la apasarea sau la eliberarea unui buton al mouse-ului.
	glutMouseFunc(MouseFunc);

	// Functia 
	// void glutDisplayFunc (void (*Display)(void))
	// inregistreaza functia callback Display care este apelata
	// oridecate ori este necesara desenarea ferestrei: la 
	// initializare, la modificarea dimensiunilor ferestrei
	// sau la apelul functiei
	// void glutPostRedisplay (void).
	glutDisplayFunc(Display);

	// Functia void glutMainLoop() lanseaza bucla de procesare
	// a evenimentelor GLUT. Din bucla se poate iesi doar prin
	// inchiderea ferestrei aplicatiei. Aceasta functie trebuie
	// apelata cel mult o singura data in program. Functiile
	// callback trebuie inregistrate inainte de apelul acestei
	// functii.
	// Cand coada de evenimente este vida atunci este executata
	// functia callback IdleFunc inregistrata prin apelul functiei
	// void glutIdleFunc (void (*IdleFunc) (void))
	glutMainLoop();

	return 0;
}