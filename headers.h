#ifndef HEADERS_H_INCLUDED
#define HEADERS_H_INCLUDED

/* Estrutura de dados para armazenar um ponto. */
typedef struct point* Point;
struct point {
    GLsizei x;
    GLsizei y;
};


void init(void);
void reshape(int, int);
void display(void);
void displayInfo(void);
void displayText(GLsizei, GLsizei, char*);
void displayControlPoints(void);
void displayBezierCurves(void);
void bezierCurve1(int);
void bezierCurve2(int);
void bezierCurve3(int);
void key(unsigned char, int, int);
void removeLastPoint(void);
void clearWindow(void);
void idle(void);
void mouseClick(int, int, int, int);
void setPoint(int, int);
void mouseMotion(int, int);

#define SIZE 1000               // quantidade máxima de pontos que serão usados
Point P[SIZE];                  // vetor de pontos
int n;                          // índice do vetor de pontos
char mouseLocation[10] = "";    // vetor que guarda as coordenadas do cursor

#endif
