#include <GL/glut.h>
#include <math.h>
#include <string>

#define PI 3.1415926535898

double ball_x, ball_y, ball_dir_x, ball_dir_y;
double sx, sy, squash;
double ball_radius;
double speed;
int window_width = 640; // Tamaño de ventana adecuado
int window_height = 480; // Tamaño de ventana adecuado
bool is_colliding = false;
int collision_timer = 0;
bool is_paused = false; // Variable de estado para pausar el movimiento de la pelota
int score_left = 0; // Puntuación de la paleta izquierda
int score_right = 0; // Puntuación de la paleta derecha

// Variables para las paletas
double paddle1_y, paddle2_y;
double paddle_width;
double paddle_height;
const double paddle_margin = 20.0; // Distancia constante desde el borde de la ventana
const double paddle_speed = 10.0; // Velocidad de movimiento de las paletas

// Estado de las teclas
bool key_w = false;
bool key_s = false;
bool key_up = false;
bool key_down = false;

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

void draw_paddle(double x, double y) {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(x - paddle_width / 2, y - paddle_height / 2);
    glVertex2f(x + paddle_width / 2, y - paddle_height / 2);
    glVertex2f(x + paddle_width / 2, y + paddle_height / 2);
    glVertex2f(x - paddle_width / 2, y + paddle_height / 2);
    glEnd();
}

void draw_text(const char* text, int x, int y) {
    glRasterPos2i(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text); // Fuente más grande
        text++;
    }
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Dibujar la pelota
    draw_ball();

    // Dibujar las paletas
    draw_paddle(paddle_margin, paddle1_y); // Paleta del jugador 1
    draw_paddle(window_width - paddle_margin, paddle2_y); // Paleta del jugador 2

    // Dibujar la puntuación de la paleta izquierda
    std::string score_text_left = std::to_string(score_left);
    int text_width_left = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)score_text_left.c_str());
    int text_x_left = (window_width / 4) - (text_width_left / 2); // Centrar en la mitad izquierda
    draw_text(score_text_left.c_str(), text_x_left, window_height - 30);

    // Dibujar la puntuación de la paleta derecha
    std::string score_text_right = std::to_string(score_right);
    int text_width_right = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)score_text_right.c_str());
    int text_x_right = (3 * window_width / 4) - (text_width_right / 2); // Centrar en la mitad derecha
    draw_text(score_text_right.c_str(), text_x_right, window_height - 30);

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
    if (!is_paused) {
        // Actualizar la posición de la pelota
        ball_x += ball_dir_x * speed;
        ball_y += ball_dir_y * speed;

        // Asegurarse de que la pelota se mantenga dentro de los límites
        if (ball_y - ball_radius < 0) {
            ball_y = ball_radius;
            ball_dir_y = -ball_dir_y;
        } else if (ball_y + ball_radius > window_height) {
            ball_y = window_height - ball_radius;
            ball_dir_y = -ball_dir_y;
        }

        // Colisiones con las paletas
        if (ball_x - ball_radius < paddle_margin + paddle_width / 2 &&
            ball_y > paddle1_y - paddle_height / 2 &&
            ball_y < paddle1_y + paddle_height / 2) {
            ball_dir_x = -ball_dir_x;
            speed += 0.5; // Incrementar la velocidad de la pelota
        } else if (ball_x - ball_radius < 0) {
            // La pelota pasó la paleta del jugador 1
            ball_x = window_width / 2;
            ball_y = window_height / 2;
            is_paused = true;
            speed = 2.0; // Reiniciar la velocidad de la pelota
            score_right++; // Incrementar la puntuación de la paleta derecha
        }

        if (ball_x + ball_radius > window_width - paddle_margin - paddle_width / 2 &&
            ball_y > paddle2_y - paddle_height / 2 &&
            ball_y < paddle2_y + paddle_height / 2) {
            ball_dir_x = -ball_dir_x;
            speed += 0.5; // Incrementar la velocidad de la pelota
        } else if (ball_x + ball_radius > window_width) {
            // La pelota pasó la paleta del jugador 2
            ball_x = window_width / 2;
            ball_y = window_height / 2;
            is_paused = true;
            speed = 2.0; // Reiniciar la velocidad de la pelota
            score_left++; // Incrementar la puntuación de la paleta izquierda
        }

        // Mover las paletas
        if (key_w) {
            paddle1_y += paddle_speed;
            if (paddle1_y + paddle_height / 2 > window_height) {
                paddle1_y = window_height - paddle_height / 2;
            }
        }
        if (key_s) {
            paddle1_y -= paddle_speed;
            if (paddle1_y - paddle_height / 2 < 0) {
                paddle1_y = paddle_height / 2;
            }
        }
        if (key_up) {
            paddle2_y += paddle_speed;
            if (paddle2_y + paddle_height / 2 > window_height) {
                paddle2_y = window_height - paddle_height / 2;
            }
        }
        if (key_down) {
            paddle2_y -= paddle_speed;
            if (paddle2_y - paddle_height / 2 < 0) {
                paddle2_y = paddle_height / 2;
            }
        }
    }

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

    // Ajustar las dimensiones de las paletas
    paddle_width = window_width * 0.03; // 3% del ancho de la ventana
    paddle_height = window_height * 0.25; // 25% de la altura de la ventana

    // Ajustar el radio de la pelota
    ball_radius = window_width * 0.02; // 2% del ancho de la ventana

    // Ajustar las posiciones de las paletas
    paddle1_y = window_height / 2;
    paddle2_y = window_height / 2;
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
    speed = 2.0; // Velocidad inicial de la pelota

    // Inicializar las posiciones de las paletas
    paddle1_y = window_height / 2;
    paddle2_y = window_height / 2;

    // Inicializar las dimensiones de las paletas
    paddle_width = window_width * 0.03; // 3% del ancho de la ventana
    paddle_height = window_height * 0.25; // 25% de la altura de la ventana

    // Inicializar el radio de la pelota
    ball_radius = window_width * 0.02; // 2% del ancho de la ventana
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            key_w = true;
            break;
        case 's':
            key_s = true;
            break;
        case ' ':
            if (is_paused) {
                is_paused = false;
                ball_dir_x = (rand() % 2 == 0) ? 1 : -1; // Dirección aleatoria
                ball_dir_y = (rand() % 2 == 0) ? 1 : -1; // Dirección aleatoria
            }
            break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            key_w = false;
            break;
        case 's':
            key_s = false;
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            key_up = true;
            break;
        case GLUT_KEY_DOWN:
            key_down = true;
            break;
    }
}

void specialKeysUp(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            key_up = false;
            break;
        case GLUT_KEY_DOWN:
            key_down = false;
            break;
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Pong");
    init();
    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glutTimerFunc(16, update, 0); // Iniciar el temporizador
    glutMainLoop();
    return 0;
}