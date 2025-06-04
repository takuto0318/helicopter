#define _USE_MATH_DEFINES
#define MAX_PARTICLES 200
#define GRAVITY -0.0005
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>


// �u���b�N��
#define	I	21
#define	J	14
#define RAD (M_PI / 180.0)
#define KEY_ESC 27
#define imageWidth 8
#define imageHeight 8

int ReadBitMapData();
void polarview(void);
void resetview(void);
void read_file1(void);
unsigned char* image;

int xBegin = 0, yBegin = 0;
int mButton;
float distance, twist, elevation, azimuth;
float nearw = 1.0, farw = 800.0, fovy = 60.0;
int tnum;
float* r1, * g1, * b1;
float* x, * y, * z;
float* nx, * ny, * nz;
int width, height;
FILE* fp1;


void initTexture(void);


// ���@�̈ʒu
typedef struct {
    float ShiftX;
    float ShiftY;
    float ShiftZ;
    float RotateX;
    float RotateY;
    float RotateZ;
} Geometry;
Geometry Cube;

typedef struct {
    float position[3];
    float velocity[3];
    float color[3];
    float lifespan;
} Particle;

//�ԉ�1
Particle particles1[MAX_PARTICLES];

// �t���O�Ȃǂ̐錾�Ə�����
char  AnimationFlag = 0;
int   xBegin, yBegin;
int   PressButton;
int   i = 0, m = 0, n = 0;
int   jump = 0;
char  left = 0;
char		right = 0;
char		go = 0;
char		back= 0;

char		direction = 0;
char		die_flag = 99;
int		limit_left[I] = { 1 };
int		limit_right[I] = { 1 };
int		limit_go[I] = { 1 };
int		limit_back[I] = { 1 };

int		limit_up[I] = { 1 };
int		limit_down[I] = { 1 };
int		t_limit_left = 1;
int		t_limit_right = 1;
int		t_limit_go = 1;
int		t_limit_back = 1;

int		fall = 1, fall_p = 0;
int		rise = 1, rise_p = 0;
int   Shooting = 0;             /* �e���˒����ǂ��� */
float BulletX = 0.0;            /* �e�̈ʒu�iX���W�j */
float BulletY = 0.0;            /* �e�̈ʒu�iY���W�j */
float BulletZ = 0.0;            /* �e�̈ʒu�iZ���W�j */

double          Rotate = 0;
double		mini = 0, max = 100;
double		Vy;

// ���ԕϐ��̐錾�Ə�����
double		ntime = 0, ptime = 0;
double		die_ntime = 0, die_ptime = 0;

// �u���b�N�ƓG�̐錾�Ə�����
double		block_x[I] = { 4,5,6,7,10,14,16,17,19,12,20,24,27,30,31,33,40};
double		block_y[I] = { 2,3,5,2,2,7,5,6,5,9,10,13,12,14,16,20};
double		block_z[I] = { 1,3,4,3,0,2,5,6,5,9,2,4,6,3 ,2,0};
double		enemy_x[J] = { 0 };
double		enemy_y[J] = { 0 };
double		enemy_z[J] = { 0 };
double		enemy_x2[J] = { 0 };
double		enemy_y2[J] = { 0 };
double		bL[I], bR[I], bU[I], bD[I],bF[I],bB[I], R[J], R2[J];
double		nView[2];
int		jcount = 0;
double          Cv = 0.07;
float CameraAzimuth = 0.0;   /* �J�����̈ʒu�i���ʊp�j */
float CameraElevation = 0.0;    /* �J�����̈ʒu�i�p�j */
float CameraDistance = -7.0;    /* �J�����̈ʒu�i���_����̋����j */
float CameraX = -9.0;            /* �J�����̈ʒu�iX���W�j */
float CameraY = 0.0;            /* �J�����̈ʒu�iY���W�j */
float CameraZ = 5.0;            /* �J�����̈ʒu�iZ���W�j */

// �֐��錾
void display(void);
void geometrySet(Geometry geom);
void keyboard(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseDrag(int x, int y);
void myInit(char* windowTitle);
void myReshape(int width, int height);
void timer(int value);


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    printf(" argc = %d\n", argc);
    if (argc < 2) {
        fprintf(stderr, " Usage: command <bitmap filename>\n");
        exit(-1);
    }

    fp1 = fopen(argv[1], "r");
    printf(" file = %s\n", argv[1]);
    if (fp1 == 0) { // �t�@�C�����J���̂Ɏ��s
        fprintf(stderr, " File open error : %s\n", argv[1]);
        exit(-1);
    }
    else { // ������͐���
        fprintf(stderr, "File open success : %s\n", argv[1]);
    }
    read_file1();

    myInit(argv[0]);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);



    // ���@�ʒu�̏�����
    Cube.ShiftX = 0;
    Cube.ShiftY = 0;
    Cube.ShiftZ = 0;
    Cube.RotateX = 0;
    Cube.RotateY = 0;
    Cube.RotateZ = 0;


    glutMainLoop();

    return(0);
}

void read_file1() {
    int i, j, k;
    int maxi = 100000;
    char buf[10000];
    char key[64], key2[64];;
    unsigned int width1, height1;
    float xx, yy, zz;
    float nxx, nyy, nzz;

    nxx = nyy = nzz = 0.;
    tnum = 0;

    fgets(buf, 10000, fp1); // title //
    printf(" buf = %s\n", buf);
    sscanf(buf, "%s", key);
    if (strcmp(key, "solid") == 0) {
        printf("OK 1\n");
    }
    else {
        printf("NG 1: %s\n", key);
    }
    for (i = 0; i < maxi; i++) {
        fgets(buf, 10000, fp1); // title //
        sscanf(buf, "%s", key);
        if (strcmp(key, "facet") == 0) {
            //			printf("OK: %d\n",i);
        }
        else 	if (strcmp(key, "endsolid") == 0) {
            printf("Normal termination: %s\n", key);
            break;
        }
        else {
            printf("NG: %d, %s\n", i, key);
            exit(-1);
        }
        fgets(buf, 10000, fp1); // outer loop
        fgets(buf, 10000, fp1); // point1
        fgets(buf, 10000, fp1); // point2
        fgets(buf, 10000, fp1); // point3
        fgets(buf, 10000, fp1); // end loop

        fgets(buf, 10000, fp1); // title //
        sscanf(buf, "%s", key);
        if (strcmp(key, "endfacet") == 0) {
            //			printf("OK end \n");
        }
        else {
            printf("NG end: %s\n", key);
            exit(-1);
        }

    }
    tnum = i;
    printf(" tnum = %d\n", tnum);
    x = (float*)malloc(3 * tnum * sizeof(float));
    y = (float*)malloc(3 * tnum * sizeof(float));
    z = (float*)malloc(3 * tnum * sizeof(float));
    nx = (float*)malloc(tnum * sizeof(float));
    ny = (float*)malloc(tnum * sizeof(float));
    nz = (float*)malloc(tnum * sizeof(float));
    rewind(fp1);

    fgets(buf, 10000, fp1); // title //
    printf(" buf = %s\n", buf);
    sscanf(buf, "%s", key);
    if (strcmp(key, "solid") == 0) {
        printf("OK 1\n");
    }
    else {
        printf("NG 1: %s\n", key);
    }
    for (i = 0; i < tnum; i++) {
        fgets(buf, 10000, fp1); // title //
        sscanf(buf, "%s %s %f %f %f", key, key2, &nxx, &nyy, &nzz);
        if (strcmp(key, "facet") == 0) {
            //			printf("OK: %d\n",i);
            nx[i] = nxx;
            ny[i] = nyy;
            nz[i] = nzz;
        }
        else 	if (strcmp(key, "endsolid") == 0) {
            printf("Normal termination: %s\n", key);
            break;
        }
        else {
            printf("NG: %d, %s\n", i, key);
            exit(-1);
        }
        fgets(buf, 10000, fp1); // outer loop
        fgets(buf, 10000, fp1); // point1
        sscanf(buf, "%s %f %f %f", key, &xx, &yy, &zz);
        x[3 * i + 0] = xx;
        y[3 * i + 0] = yy;
        z[3 * i + 0] = zz;
        fgets(buf, 10000, fp1); // point2
        sscanf(buf, "%s %f %f %f", key, &xx, &yy, &zz);
        x[3 * i + 1] = xx;
        y[3 * i + 1] = yy;
        z[3 * i + 1] = zz;
        fgets(buf, 10000, fp1); // point3
        sscanf(buf, "%s %f %f %f", key, &xx, &yy, &zz);
        x[3 * i + 2] = xx;
        y[3 * i + 2] = yy;
        z[3 * i + 2] = zz;
        fgets(buf, 10000, fp1); // end loop

        fgets(buf, 10000, fp1); // title //
        sscanf(buf, "%s", key);
        if (strcmp(key, "endfacet") == 0) {
            //			printf("OK STL data was stored \n");
        }
        else {
            printf("NG end: %s\n", key);
            exit(-1);
        }

    }
    printf("OK STL data was stored \n");

}


// �`��֐�(���܂��Ă�)
void drawString3D(const char* str, float charSize, float lineWidth)
{
    glPushMatrix();
    glPushAttrib(GL_LINE_BIT);
    glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
    glLineWidth(lineWidth);
    while (*str) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
        ++str;
    }
    glPopAttrib();
    glPopMatrix();
}

//�ԉ�1��
void initializeParticle(int i) {
    particles1[i].position[0] = 0.0;
    particles1[i].position[1] = -0.999999;//-0.8
    particles1[i].position[2] = 0.0;

    particles1[i].velocity[0] = (rand() % 100) / 5000.0 - 0.01; // X�����̑��x
    particles1[i].velocity[1] = (rand() % 100) / 5000.0 + 0.05; // Y�����̑��x
    particles1[i].velocity[2] = (rand() % 100) / 5000.0 - 0.01; // Z�����̑��x

    particles1[i].color[0] = (rand() % 100) / 100.0; // R
    particles1[i].color[1] = (rand() % 100) / 100.0; // G
    particles1[i].color[2] = (rand() % 100) / 100.0; // B

    particles1[i].lifespan = 1.0;
}

void updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles1[i].velocity[1] += GRAVITY;

        particles1[i].position[0] += particles1[i].velocity[0];
        particles1[i].position[1] += particles1[i].velocity[1];
        particles1[i].position[2] += particles1[i].velocity[2];

        particles1[i].lifespan -= 0.01;

        if (particles1[i].lifespan < 0.0) {
            initializeParticle(i);
        }
    }
}


void renderParticles() {
    glPointSize(3.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        glColor3fv(particles1[i].color);
        glVertex3fv(particles1[i].position);
    }
    glEnd();
}
//���ԉ�1

void display(void)
{

    float diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    polarview();
    glEnable(GL_DEPTH_TEST);

    for (i = 0; i < tnum; i++) {
        glBegin(GL_POLYGON);
        glNormal3f(nx[i], ny[i], nz[i]);
        glVertex3f(x[3 * i + 0], y[3 * i + 0], z[3 * i + 0]);
        glVertex3f(x[3 * i + 1], y[3 * i + 1], z[3 * i + 1]);
        glVertex3f(x[3 * i + 2], y[3 * i + 2], z[3 * i + 2]);
        glEnd();
    }
    /* ���_�̐ݒ� */
    gluLookAt(Cube.ShiftX + CameraX, Cube.ShiftY + CameraY + 3, Cube.ShiftZ + CameraZ,  /* �J�����̈ʒu */
        Cube.ShiftX, Cube.ShiftY + 3, Cube.ShiftZ,  /* �����_�̈ʒu */
        0.0, 1.0, 0.0); /* �J����������̃x�N�g�� */

    glEnable(GL_DEPTH_TEST);

    // �������(�����̕`��)
    if (die_flag == 99) {
        glPushMatrix();
        glColor3f(0, 1.0, 1.0);
        glTranslatef(-6, 4.0, 0);
        glRotatef(-45.0, 0.0, 1.0, 0.0);
        drawString3D("Shoot Down The Moon", 5.0, 2.0);
        glPopMatrix();

        glPushMatrix();
        glColor3f(1, 0, 0.0);
        glTranslatef(-7, 2.5, 0);
        glRotatef(-45.0, 0.0, 1.0, 0.0);
        drawString3D("Push The S", 5.0, 2.0);
        glPopMatrix();

    }

    glPushMatrix();
    glTranslatef(40.0,21.0, 0.0);
    glutSolidSphere(1.0, 120, 120);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(-40.0, 20.0, -10.0);
    glutSolidSphere(1.0, 300, 300);
    glPopMatrix();


    //�ԉ�1
    glPushMatrix();
    glTranslatef(30.0, 20.0, -1.0);
    glScalef(5.0, 5.0, 5.0);
    updateParticles();
    renderParticles();
    glPopMatrix();



    if (Shooting == 1)
    {
        /* �e�̕`�� */
        glPushMatrix();                /* �`��ʒu��ۑ� */
        glColor3f(1.0, 0.0, 0.0);       /* �`��F��Ԃɂ��� */
        glTranslatef(BulletX, BulletY, BulletZ); /* �`��ʒu��X������1.0�ړ� */

        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(ptime * 100, 0.0, 0.0, 1.0);
        glutWireCone(0.3, 0.5, 10, 2);

        glPopMatrix();                 /* �`��ʒu��߂� */
    }



    // ���@�̕`��
    glPushMatrix();
    glTranslatef(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    if (die_flag == 2||die_flag==3) {
        glColor3f(1.0 - die_ptime, 1.0 - die_ptime, 1.0);
    }
    else if (die_flag == 0) {
        glColor3f(1.0, 1.0 - die_ptime, 1.0 - die_ptime);
    }
    else {
        glColor3f(0.5, 0.0, 0.5);
    }
    for (i = 0; i < tnum; i++) {
        glBegin(GL_LINE_LOOP);
        glNormal3f(nx[i], ny[i], nz[i]);
        glVertex3f(x[3 * i + 0], y[3 * i + 0], z[3 * i + 0]);
        glVertex3f(x[3 * i + 1], y[3 * i + 1], z[3 * i + 1]);
        glVertex3f(x[3 * i + 2], y[3 * i + 2], z[3 * i + 2]);
        glEnd();
    }

    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);/*�n�ʂ̃O���b�h*/
    for (i = -80;i < 81; i += 2) {
        glVertex3f((float)i, 0, -80.0);
        glVertex3f((float)i, 0, 80.0);
        glVertex3f(-80.0, 0, (float)i);
        glVertex3f(80.0, 0, (float)i);
    }
    glEnd();
    
    if( (40 - 0 <= BulletX)&&(BulletX <= 40 + 10)&&(20 - 2 <= BulletY)&&(BulletY <= 20 + 2)&&(-1 <= BulletZ) && (BulletZ <= 1)) {
        die_flag = 3;
        timer(2);
    }
    

    // �u���b�N�̕`��
    for (i = 0; i < I; i++) {
        glPushMatrix();
        glTranslatef(block_x[i], block_y[i], block_z[i]);
        if (i != I - 1) {
            glColor3f(1, 0, 1);
        }else {
            glColor3f(0.5, 1.0, 0.5);
        }
        glColor3f(1.0, 1.0, 0.0);
        glutWireCube(1.5);
        glPopMatrix();
    }

    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    glutSwapBuffers();
}

void makeImage(void)
{
    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            image[0 + 4 * (j + i * width)] = (unsigned char)r1[j + width * i];			// bitmap2�ɕς�����
            image[1 + 4 * (j + i * width)] = (unsigned char)g1[j + width * i];			// bitmap1�̓��e��ς��Ă݂�
            image[2 + 4 * (j + i * width)] = (unsigned char)b1[j + width * i];
            image[3 + 4 * (j + i * width)] = (unsigned char)50;
        }
    }
}

void initTexture3(void)
{
    unsigned char* image3[1];
    int imageHeightb, imageWidthb;

    makeImage();

    glBindTexture(GL_TEXTURE_2D, 3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void timer(int value)
{

    switch (die_flag) {

        // �ʏ펞
    case	1:

        // �u���b�N�Ƃ̓����蔻��
        for (i = 0; i < I; i++) {
            // ���E�㉺�̔���p
            bL[i] = block_x[i] - Cube.ShiftX;
            bR[i] = Cube.ShiftX - block_x[i];
            bU[i] = Cube.ShiftY - block_y[i];
            bD[i] = block_y[i] - Cube.ShiftY;
            bF[i] = block_z[i] - Cube.ShiftZ;
            bB[i] = Cube.ShiftZ - block_z[i];

            // ����
            if (bL[i] < 1 && -0.5 < bL[i] && bU[i] < 1 && bD[i] < 1 && bF[i] < 1 && bB[i] < 1) {
                limit_right[i] = 0;
            }
            else {
                limit_right[i] = 1;
                t_limit_right = 1;
            }

            // �E��
            if (-0.5 < bR[i] && bR[i] < 1 && bU[i] < 1 && bD[i] < 1 && bF[i] < 1 && bB[i] < 1) {
                limit_left[i] = 0;
            }
            else {
                limit_left[i] = 1;
                t_limit_left = 1;
            }

            // ���
            if (-0.75 < bL[i] && bL[i] < 0.75 && -0.75 < bR[i] && bR[i] < 0.75 && bU[i] < 1 && 0 < bU[i] && bF[i] < 1 && bB[i] < 1) {
                mini = block_y[i] + 1;
                fall = 0;
                fall_p = block_x[i];
            }

            // ��������
            if (Cube.ShiftX < fall_p - 0.9 || fall_p + 0.9 < Cube.ShiftX || bU[i] > 1 && -1 > bU[i] || bF[i] > 1 && -1 > bF[i]) {
                fall = 1;
            }

            // ����
            if (-0.75 < bL[i] && bL[i] < 0.75 && -0.75 < bR[i] && bR[i] < 0.75 && 0.75 < bD[i] && 0 < bD[i] && bF[i] < 1 && bB[i] < 1) {
                if (block_y[i] < max) {
                    max = block_y[i] - 1;
                }
                rise = 0;
                rise_p = block_x[i];
            }
            // ��ړ�����
            if (Cube.ShiftX < rise_p - 1.5 || rise_p + 1.5 < Cube.ShiftX) {
                rise = 1;
            }
        }

        // �ړ��t���O
        for (i = 0; i < I; i++) {
            t_limit_right *= limit_right[i];
        }
        for (i = 0; i < I; i++) {
            t_limit_left *= limit_left[i];
        }




        // �ړ��͈͂̐���(�㉺)
        if (fall == 1) {
            mini = 0;
        }
        if (rise == 1) {
            max = 150;
        }

        // ���@�̉�]
        if (direction == 0) {
            Rotate -= 1;
        }
        else {
            Rotate += 1;
        }
        if (Cube.RotateZ > 360.0) {
            Cube.RotateZ -= 360.0;
        }

        // ���@�̈ٓ�(�E)
        if (right == 1 && t_limit_right == 1) {
            Cube.ShiftX += Cv;
            direction = 0;
        }

        // ���@�̈ٓ�(��)
        if (left == 1 && t_limit_left == 1) {
            Cube.ShiftX -= Cv;
            direction = 1;
        }

        // ���@�̈ٓ�(�O)
        if (go == 1 && t_limit_go == 1) {
            Cube.ShiftZ -= Cv;
            direction = 2;
        }
        // ���@�̈ٓ�(���)
        if (back == 1 && t_limit_back == 1) {
            Cube.ShiftZ += Cv;
            direction = 3;
        }

        /* �e���˒��̌v�Z */
        if (Shooting == 1)
        {
            BulletX += 0.2;
            /* �e����ʉ��܂ōs��������� */
            if (BulletX - Cube.ShiftX > 8.0) Shooting = 0;


        }


        // �W�����v�Ɨ����̐���
        Cube.ShiftY += Vy;
        Vy -= 0.0005;
        if (Cube.ShiftY <= mini) {
            Cube.ShiftY = mini;
            Vy = 0;
            jump = 0;
            jcount = 0;
        }
        else if (max <= Cube.ShiftY) {
            Cube.ShiftY = max;
        }

        // ���ԕϐ��̍X�V
        ntime = ntime - (ntime - 0.1);
        ptime += ntime;

        break;

        // ���S���ƃN���A��
    case	0:
    case	2:
        break;
    case    3:
        glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslatef(36, 20.0, 0);
        glRotatef(-90.0, 0.0, 1.0, 0.0);
        drawString3D("Congratulations!!!", 5.0, 2.0);
        glPopMatrix();

        //�ԉ�1
        glPushMatrix();
        glTranslatef(30.0, 20.0, -1.0);
        glScalef(5.0, 5.0, 5.0);
        updateParticles();
        renderParticles();
        glPopMatrix();


        ntime = ntime - (ntime - 0.1);
        ptime += ntime;
        die_ntime = die_ntime - (die_ntime - 0.00004);
        die_ptime += die_ntime;

        if (die_ptime > 1) {
            exit(0);
        }

        break;
    }

    glutPostRedisplay();
    glutTimerFunc(3, timer, 0);

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {

        // �X�y�[�X�ŃW�����v
    case ' ':
        jcount++;
        if (jcount < 3) {
            Vy = 0.065;
        }
        jump = 1;
        break;

        // "s"�ŊJ�n
    case 's':
        die_flag = 1;
        break;

        // ���ړ�
    case 'j':
        PressButton = 0;
        left = 0;
        right = 0;
        go = 1;
        back =0;
        break;

        // �E�ړ�
    case 'l':
        PressButton = 0;
        right = 0;
        left = 0;
        go = 0;
        back = 1;
        break;

        // �O�i
    case 'i':
        PressButton = 0;
        go = 0;
        back = 0;
        left = 0;
        right = 1;
        break;

        // ���
    case 'k':
        PressButton = 0;
        back = 0;
        go = 0;
        left = 1;
        right = 0;
        break;
    case 't':
        if (Shooting == 0)
        {
            BulletX = Cube.ShiftX;
            BulletY = Cube.ShiftY;
            BulletZ = Cube.ShiftZ;
            Shooting = 1;
        }
        break;


    }
    glutPostRedisplay();

    x = y = 0;
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        // 'i'�L�[�𗣂��Ə�ړ���~
    case 'i':
        right = 0;
        break;

        // 'k'�L�[�𗣂��Ɖ��ړ���~
    case 'k':
        left = 0;
        break;
    case 'l':
        back = 0;
        break;
    case 'j':
        go = 0;
        break;


    }
}

void mouseButton(int button, int state, int x, int y)
{
}

void mouseDrag(int x, int y)
{
    int xMove = x - xBegin;
    int yMove = y - yBegin;

    switch (PressButton) {

        // �J�����̈ړ�
    case GLUT_LEFT_BUTTON:
        CameraAzimuth += (float)xMove / 2.0;
        CameraElevation += (float)yMove / 2.0;
        if (CameraElevation > 90.0) {
            CameraElevation = 90.0;
        }
        if (CameraElevation < -90.0) {
            CameraElevation = -90.0;
        }
        break;

    }

    CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
    CameraY = CameraDistance * sin(CameraElevation * RAD);
    CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);

    xBegin = x;
    yBegin = y;

    glutPostRedisplay();
}


/***********************************************************
|  �֐��FmyInit()
|  �����F�E�C���h�E�\���ƕ`��ݒ�̏�����
|  �����Fchar *windowTitle      �E�C���h�E�̃^�C�g���o�[�ɕ\�����镶����
|  �ߒl�F�Ȃ�
***********************************************************/
void myInit(char* windowTitle)
{
    int winWidth = 1500;
    int winHeight = 1200;

    float aspect = (float)winWidth / (float)winHeight;

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(winWidth, winHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(windowTitle);
    glClearColor(0, 0., .0, .0);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseDrag);
    glutDisplayFunc(display);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, aspect, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glutTimerFunc(15, timer, 0);
}

void myReshape(int width, int height)
{
    float aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, nearw, farw);
    glMatrixMode(GL_MODELVIEW);
}

void polarview(void)
{
    glTranslatef(0.0, 0.0, -distance);
    glRotatef(-twist, 0.0, 0.0, 1.0);
    glRotatef(-elevation, 1.0, 0.0, 0.0);
    glRotatef(-azimuth, 0.0, 1.0, 0.0);
}


void resetview(void)
{
    distance = 100.0;
    twist = 0.0;
}
