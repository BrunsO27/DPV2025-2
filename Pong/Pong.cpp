#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535898

double ball_x, ball_y, ball_dir_x, ball_dir_y;
const double ball_radius = 15.0;
const double speed = 0.5;
int window_width = 320;
int window_height = 240;

void draw_ball() {
    glColor3f(0.6, 0.3, 0.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        double angle = 2 * PI * i / 100;
        glVertex2f(ball_x + ball_radius * cos(angle), ball_y + ball_radius * sin(angle));
    }
    glEnd();
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibujar la pelota
    draw_ball();

    // Colisiones con las paredes
    if (ball_y + ball_radius > window_height || ball_y - ball_radius < 0) {
        ball_dir_y = -ball_dir_y;
    }
    if (ball_x + ball_radius > window_width || ball_x - ball_radius < 0) {
        ball_dir_x = -ball_dir_x;
    }

    glutSwapBuffers();
}

void update(int value) {
    // Actualizar la posición de la pelota
    ball_x += ball_dir_x * speed;
    ball_y += ball_dir_y * speed;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Llamar a update cada 16 ms (~60 FPS)
}

void reshape(int w, int h) {
    window_width = w;
    window_height = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init(void) {
    glClearColor(0.0, 0.8, 0.0, 1.0);
    ball_x = window_width / 2;
    ball_y = window_height / 2;
    ball_dir_x = 1;
    ball_dir_y = 1;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Bouncing Ball");
    init();
    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0); // Iniciar el temporizador
    glutMainLoop();
    return 0;
}