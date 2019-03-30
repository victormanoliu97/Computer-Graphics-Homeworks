#include <stdlib.h>
#include <math.h>
#include <map>
#include <GL/glut.h>

using namespace std;

unsigned char prevKey;

double xs=1.0, ys=1.0;

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
		printf("%d %d", xs, ys);
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
			glVertex2f(p1_x / xs, p_y / ys);
			glVertex2f(p2_x / xs, p_y / ys);
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
			glVertex2f(p_x / xs, p1_y / ys);
			glVertex2f(p_x / xs, p2_y / ys);
			p_x += this->dc;
		}
		glEnd();
	}

	void writePixel(int i, int j) {
		double x = this->cx + i * this->dc;
		double y = this->cy + j * this->de;
		deseneaaCerc(x, y, radius, 10000);	
	}

	void afisareSegmentDreapta3(int x0, int y0, int xn, int yn, int grosime) {
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
		//printf("X:%d Y:%d\n", x, y);
		for (int i = 0; i <= (int)grosime / 2; i++) {
			if (y + i < this->numarLinii)
				this->writePixel(x, y + i);
			if (y - i > 0)
				this->writePixel(x, y - i);
		}
		while (x < xn) {
			//printf("d:%d\n", d);
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
			//printf("X:%d Y:%d\n", x, y);
			for (int i = 0; i <= (int)grosime / 2; i++) {
				if (y + i < this->numarLinii)
					this->writePixel(x, y + i);
				if (y - i > 0)
					this->writePixel(x, y - i);
			}
			m[x] = y;
		}
	}


	void deseneaaCerc(double x, double y, double r, int numberOfSegments) {

		double pi = 4 * atan(1.0);

		glColor3ub(71, 71, 71);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x / xs, y / ys);

		for (int i = 0; i < numberOfSegments; i++) {
			float x_aux = x + (this->radius*cos(i * 2 * pi / numberOfSegments));
			float y_aux = y + (this->radius*sin(i * 2 * pi / numberOfSegments));
			glVertex2f(x_aux / xs, y_aux / ys);
		}

		glEnd();
	}


	void deseneazaSegmentMartor(int x0, int y0, int xn, int yn) {
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINES);
		double x1, y1;
		x1 = this->cx + x0 * this->dc;
		y1 = this->cy + y0 * this->de;
		glVertex2f(x1 / xs, y1 / ys);
		x1 = this->cx + xn * this->dc;
		y1 = this->cy + yn * this->de;
		glVertex2f(x1 / xs, y1 / ys);
		glEnd();
	}
};

void Init(void) {
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void display1() {
	int numarLinii = 16;
	int numarColoane = 16;
	GrilaCarteziana* grilaCarteziana = new GrilaCarteziana(numarLinii, numarColoane);
	
	grilaCarteziana->afisareSegmentDreapta3(0, 15, 15, 10,3);
	grilaCarteziana->deseneazaSegmentMartor(0, 15, 15, 10);
	grilaCarteziana->afisareSegmentDreapta3(0, 0, 15, 7, 1);
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

	glFlush();
}

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

	double ratio = (double)w / (double)h;
	if (ratio != 1.0) {
		if (w > h)
			xs = (double)w / (double)h;
		else
			ys = (double)h / (double)w;
	}
	//printf("Modificate %f %f - ratia %f\n", xs, ys, ratio);

}

void KeyboardFunc(unsigned char key, int x, int y) {
	printf("Ati tastat <%c>. Mouse-ul este in pozitia %d, %d.\n",
		key, x, y);

	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
	printf("Call MouseFunc : ati %s butonul %s in pozitia %d %d\n",
		(state == GLUT_DOWN) ? "apasat" : "eliberat",
		(button == GLUT_LEFT_BUTTON) ?
		"stang" :
		((button == GLUT_RIGHT_BUTTON) ? "drept" : "mijlociu"),
		x, y);
}

int main(int argc, char** argv) {
	
	glutInit(&argc, argv);

	glutInitWindowSize(300, 300);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}