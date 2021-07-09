/*
 * Universidade Federal da Grande Dourados - UFGD
 * Faculdade de Ciências Exatas e Tecnologia - FACET
 * Curso de Bacharelado em Sistemas de Informação - BSI
 *
 * Disciplina: LABORATÓRIO DE COMPUTAÇÃO GRÁFICA - LCG
 * Prof.: Adailton José Alves da Cruz
 * Período: 2020.05 - Módulo IV - RAE
 *
 * Atividade 01: desenvolvimento de uma aplicação para desenhar
 *               curvas, utilizando-se das equações de curvas de
 *               Bezier.
 *
 * Desenvolvido por: Jeferson Willian Vieira Silva.
 */

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>

#include "headers.h"    /* arquivo de cabeçalho contendo definições, declarações etc */

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Portuguese");

    glutInit(&argc, argv);
    glutInitWindowSize(1000, 700);
    glutInitWindowPosition(300,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("LCG – Atividade 01 – Curvas de Bézier | por Jeferson W. V. Silva");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouseClick);


    init();
    glutMainLoop();

    return 0;
}

/*
* Procedimento que inicializa as configurações e recursos necessários,
* para que o programa funcione corretamente.
*/
void init(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < SIZE; i++) {
        P[i] = NULL;
    }
    n = 0;
    sprintf(mouseLocation, "(%03d,%03d)", 0, 0);
}

/*
* Procedimento que redimensiona a tela, configurando width como a nova
* largura e height a nova altura.
*/
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, -height, 0);
}

/*
* Procedimento que renderiza os elementos na tela, fazendo um contínuo redesenho.
*/
void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    displayInfo();
    displayControlPoints();
    displayBezierCurves();

    glutSwapBuffers();
}

/*
* Procedimento que renderiza as informações textuais na tela.
*/
void displayInfo(void)
{
    glColor3f(1.0, 1.0, 1.0);
    char text1[60] = "MOUSE (LEFT BUTTON) - Insere um ponto de controle";
    char text2[60] = "BACKSPACE - Remove o ultimo ponto de controle inserido";
    char text3[20] = "ESC - Limpa a tela";
    displayText(20, -20, text1);
    displayText(20, -40, text2);
    displayText(20, -60, text3);
    displayText(20, -80, mouseLocation);
}

/*
* Procedimento que renderiza o texto text na posição (x,y).
*/
void displayText(GLsizei x, GLsizei y, char* text)
{
    glPushMatrix();

    glRasterPos2f(x, y);
    int length = strlen(text);
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
	glPopMatrix();
}

/*
* Procedimento que renderiza tanto os pontos quanto as retas de controle,
* a fim de visualizar os elementos que suportam o desenho das curvas.
*/
void displayControlPoints()
{
    char label[5] = "";
    glPointSize(7.5f);
    glColor3f(0.0f, 1.0f, 0.0f);

    for (int i = 0; i < n; i++) {
    /* Desenho e rotulagem dos n pontos de controle */
        glBegin(GL_POINTS);
            glVertex2i(P[i]->x, P[i]->y);
        glEnd();
        sprintf(label, "P%d", i);
        displayText(P[i]->x, P[i]->y+10, label);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 1; i < n; i++) {
    /* Desenho das n-1 retas de controle */
        for (float j = 0.0; j <= 1.0; j += 0.05) {
        /* forma pontilhada */
            GLsizei x = (1-j)*(P[i-1]->x)  +  j*P[i]->x;
            GLsizei y = (1-j)*(P[i-1]->y)  +  j*P[i]->y;
            glPointSize(1.5f);
            glBegin(GL_POINTS);
                glVertex2i(x, y);
            glEnd();
        }
    }
    glFlush();
}

/*
* Procedimento que renderiza as curvas de Bézier, de acordo com o número de pontos.
* Ou seja, seleciona um determinado número de pontos e desenha sua respectiva curva.
*/
void displayBezierCurves(void)
{
    int i;      // índice dos pontos desenhados na tela
    int n2;     // pontos que ainda não foram usados para construir uma curva
    int curve;  // tipo de curva de acordo com quantidade de pontos usados para construí-la

    i = 0;
    n2 = n;

    while (i < n) {

        /* Caso forem dados mais do que 4 pontos, a curva será construída através das demais. */
        curve = (n2 > 4) ? 4 : n2;

        switch (curve) {
            case 2:                 /* Curva do tipo 1 - com base em dois pontos de controle. Neste caso, é uma reta. */
                bezierCurve1(i);    /* Procedimento que desenha uma reta com origem no ponto P[i]. */
                n2 -= 1;            // Descarte do ponto de controle que não será mais usado.
                i++;                // Itera para o próximo ponto de controle que ainda pode ser usado.
                break;
            case 3:                 /* Curva do tipo 2 - com base em três pontos de controle. */
                bezierCurve2(i);    /* Procedimento que desenha uma curva do tipo 2 com origem no ponto P[i]. */
                n2 -= 2;            // Descarte dos dois pontos de controle que não serão mais usados.
                i += 2;             // Itera para o próximo ponto de controle que ainda pode ser usado.
                break;
            case 4:                 /* Curva do tipo 3 - com base em quatro pontos de controle. */
                bezierCurve3(i);    /* Procedimento que desenha uma curva do tipo 3 com origem no ponto P[i]. */
                n2 -= 3;            // Descarte dos três pontos de controle que não serão mais usados.
                i += 3;             // Itera para o próximo ponto de controle que ainda pode ser usado.
                break;
            default:                /* Um único ponto. */
                i++;
                break;
        }
    }

}

/*
* Procedimento que desenha uma curva do tipo 1 (cor branca), isto é, uma reta.
* P[i]: ponto de origem da reta.
*/
void bezierCurve1(int i)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.5f);

    for (float t = 0.0; t <= 1.0; t += 0.001) {
        /* Cálculo da curva C(t) */
        GLfloat x = (1-t)*(P[i]->x)  +  t*P[i+1]->x;
        GLfloat y = (1-t)*(P[i]->y)  +  t*P[i+1]->y;

        glBegin(GL_POINTS);
            glVertex2f(x, y);
        glEnd();
    }
    glFlush();
}

/*
* Procedimento que desenha uma curva do tipo 2 (cor vermelha), curva com três pontos de controle.
* P[i]: ponto de origem da curva.
*/
void bezierCurve2(int i)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(1.5f);

    for (float t = 0.0; t <= 1.0; t += 0.001) {
        /* Cálculo da curva C(t) */
        GLfloat x = pow(1-t, 2)*(P[i]->x)  +  2*t*(1-t)*P[i+1]->x  +  pow(t, 2)*P[i+2]->x;
        GLfloat y = pow(1-t, 2)*(P[i]->y)  +  2*t*(1-t)*P[i+1]->y  +  pow(t, 2)*P[i+2]->y;

        glBegin(GL_POINTS);
            glVertex2f(x, y);
        glEnd();
    }
    glFlush();
}

/*
* Procedimento que desenha uma curva do tipo 3 (cor azul), curva com quatro pontos de controle.
* P[i]: ponto de origem da curva.
*/
void bezierCurve3(int i)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 1.0f);
    glPointSize(1.5f);

    for (float t = 0.0; t <= 1.0; t += 0.001) {
        /* Cálculo da curva C(t)*/
        GLfloat x = pow(1-t, 3)*(P[i]->x)  +  3*t*pow(1-t, 2)*P[i+1]->x  +  3*pow(t, 2)*(1-t)*P[i+2]->x  +  pow(t, 3)*P[i+3]->x;
        GLfloat y = pow(1-t, 3)*(P[i]->y)  +  3*t*pow(1-t, 2)*P[i+1]->y  +  3*pow(t, 2)*(1-t)*P[i+2]->y  +  pow(t, 3)*P[i+3]->y;

        glBegin(GL_POINTS);
            glVertex2f(x, y);
        glEnd();
    }
    glFlush();
}

/*
* Procedimento que trata os eventos de teclado.
*/
void key(unsigned char key, int x, int y)
{
    switch (key) {
        case 8:     // backspace
            removeLastPoint();
            break;
        case 27:    // ESC
            clearWindow();
            break;
        default:
            break;
    }
    if (key == 27) {
        clearWindow();
    }
}

/*
* Procedimento que remove o último ponto colocado pelo usuário.
*/
void removeLastPoint()
{
    if (0 <= n-1) {
    /* Caso ainda não tenha removido todos os pontos */
        free(P[n-1]);
        P[n-1] = NULL;
        n--;
    }
}

/*
* Procedimento que limpa a tela, fazendo a remoção de todos os pontos
* de uma única vez.
*/
void clearWindow()
{
    for (int i = 0; i < n; i++) {
        free(P[i]);
    }
    init();
}

/*
* Procedimento que faz a atualização contínua da tela, para quando
* o programa não está disparando eventos.
*/
void idle(void)
{
    glutPostRedisplay();
}

/*
* Procedimento que trata os eventos de mouse.
*/
void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        setPoint(x, y);
    }
    glutPostRedisplay();
}

/*
* Procedimento que insere um ponto na coordenada (x,y).
*/
void setPoint(int x, int y)
{
    if (n < SIZE) {
    /* Condição para guardar apenas a quantidade de pontos suportada pelo vetor*/
        P[n] = (Point) malloc(sizeof(Point));
        if (P[n] != NULL) {
            P[n]->x = x;
            P[n]->y = -y;// transformando da coordenada de viewport para a coordenada da projeção ortográfica
            printf("P%02d = (%03d, %03d)\n", n, x, y);
            n++;
        } else {
            printf("Problema de memória!");
            system("pause");
            exit(1);
        }
    } else {
        printf("A aplicação só suporta %d pontos.\n", SIZE);
    }
}

/*
* Procedimento que registra as coordenadas do cursor conforme o mouse
* movimenta-se.
*/
void mouseMotion(int x, int y)
{
    sprintf(mouseLocation, "(%03d, %03d)", x, y);
    glutPostRedisplay();
}
