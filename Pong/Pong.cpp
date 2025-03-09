#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535898

double ball_x, ball_y, ball_dir_x, ball_dir_y;
double sx, sy, squash;
const double ball_radius = 15.0;
const double speed = 1.0;
int window_width = 320;
int window_height = 240;
bool is_colliding = false;
int collision_timer = 0;

void draw_ball() {
    glColor3f(0.6, 0.3, 0.0);
    glPushMatrix();
    glTranslatef(ball_x, ball_y, 0);
    glScalef(sx, sy, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        double angle = 2 * PI * i / 100;
        glVertex2f(ball_radius * cos(angle), ball_radius * sin(angle));
    }
    glEnd();
    glPopMatrix();
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibujar la pelota
    draw_ball();

    // Colisiones con las paredes
    if (ball_y + ball_radius * sy > window_height || ball_y - ball_radius * sy < 0) {
        ball_dir_y = -ball_dir_y;
        sy = sy * squash;
        if (sy < 0.8) {
            squash = 1.1;
        } else if (sy > 1.0) {
            sy = 1.0;
            squash = 0.9;
        }
        sx = 1.0 / sy;
        is_colliding = true;
        collision_timer = 0;
    }
    if (ball_x + ball_radius * sx > window_width || ball_x - ball_radius * sx < 0) {
        ball_dir_x = -ball_dir_x;
        sx = sx * squash;
        if (sx < 0.8) {
            squash = 1.1;
        } else if (sx > 1.0) {
            sx = 1.0;
            squash = 0.9;
        }
        sy = 1.0 / sx;
        is_colliding = true;
        collision_timer = 0;
    }

    // Restaurar la escala de la pelota rápidamente
    if (is_colliding) {
        collision_timer++;
        if (collision_timer > 5) { // Ajusta este valor para controlar la duración de la transición
            sx += (1.0 - sx) * 0.5;
            sy += (1.0 - sy) * 0.5;
            if (fabs(sx - 1.0) < 0.01 && fabs(sy - 1.0) < 0.01) {
                sx = 1.0;
                sy = 1.0;
                is_colliding = false;
            }
        }
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
    sx = 1.0;
    sy = 1.0;
    squash = 0.9;
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