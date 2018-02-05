
#define GLUT_DISABLE_ATEXIT_HACK

#define BALL_QUANTITY 9 //����
#define MAIN_BALL balls[BALL_QUANTITY-1] //����
#define MAIN_BALL_RADIUS 10.0f //����뾶

#define SIZE_X 350.0f //���������
#define SIZE_Y 200.0f //����������
#define SIZE_H 10.0f //����ڰ�߶�

#define A_RUB 0.000040f //���滬��Ħ�����ٶ�

#define CUE_LENTH 150.0f //��˳���
#define CUE_DISTANCE_INIT MAIN_BALL_RADIUS+30.0f //��˳�ʼ����
#define CUE_DISTANCE_MAX 80.0f //���������
#define CUE_DISTANCE_CNAHGE 2.0f //һ�ΰ�����˾���ı���
#define CUE_ANGLE_CNAHGE 1.0f //һ�ΰ�����˽Ƕȸı���
#define POWER_MAX 0.4f //�������
#define CUE_MOVE_FRAME 200 //����˶�֡��

#define PI 3.1415926536f

#include<gl/glut.h>
#include<stdio.h>
#include<math.h>
#include<windows.h>

struct ball{
	float mass;//����
	float radius;//��뾶
	float vx, vy;//�ٶ�x��y����
	float px, py;//λ������
	float colorR, colorG, colorB;//��ɫ
}balls[BALL_QUANTITY] = {
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 80.8f, .py = 0.0f,
		.colorR = 0.8, .colorG = 0.0, .colorB = 0.0
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 60.6f, .py = 0.0f,
		.colorR = 0.0, .colorG = 0.8, .colorB = 0.0
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 40.4f, .py = 0.0f,
		.colorR = 0.0, .colorG = 0.0, .colorB = 0.8
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 20.2f, .py = 0.0f,
		.colorR = 0.8, .colorG = 0.8, .colorB = 0.0
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 80.8f, .py = 20.2f,
		.colorR = 0.0, .colorG = 0.8, .colorB = 0.8
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 60.6f, .py = 20.2f,
		.colorR = 0.8, .colorG = 0.0, .colorB = 0.8
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 40.4f, .py = 20.2f,
		.colorR = 0.0, .colorG = 0.0, .colorB = 0.0
	},
	{
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = 20.2f, .py = 20.2f,
		.colorR = 0.2, .colorG = 0.5, .colorB = 0.7
	},
	{//MAIN_BALL
		.mass = 1.0f,
		.radius = MAIN_BALL_RADIUS,
		.vx = 0.0f, .vy = 0.0f,
		.px = -80.8f, .py = 10.1f,
		.colorR = 1.0, .colorG = 1.0, .colorB = 1.0
	}
};

int controllable = 1; //�ɲٿ�
int viewType = 0; //�ӽ�����
int cueIsMoving = 0; //����˶���
float cueDistance = CUE_DISTANCE_INIT;  //��˾������
float cueAngle = 180.0f; //��˽Ƕ�
float hitDistance = 0.0f; //��˻������
int windowsWidth = 600; //��ʼ���ڿ��
int windowsHeight = 480; //��ʼ���ڿ��

//���Ƕ�ת��Ϊ����
float toRadian(float f){
	return f / 180 * PI;
}

//����˾���ת��Ϊ���ȣ��ٶȣ�
float distanceToPower(float distance){
	return (distance - MAIN_BALL.radius) / (CUE_DISTANCE_MAX - MAIN_BALL.radius)*POWER_MAX;
}

//�����������
float distance(float x1, float y1, float x2, float y2){
	return pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
}

//����
void drawBall(struct ball *b){
	glColor3f(b->colorR, b->colorG, b->colorB);
	glPushMatrix();
	glTranslatef(b->px, b->py, 0.0f);
	glutSolidSphere(b->radius, 20, 16);
	glPopMatrix();
}

//�ƶ���
void moveBall(struct ball *b){
	b->px += b->vx;
	b->py += b->vy;
}

//�߽���ײ�ж�
void sideCrash(struct ball *b){
	if (b->px >= SIZE_X - b->radius || b->px <= b->radius - SIZE_X){
		b->vx = -b->vx;
	}
	if (b->py >= SIZE_Y - b->radius || b->py <= b->radius - SIZE_Y){
		b->vy = -b->vy;
	}
	while (b->px >= SIZE_X - b->radius || b->px <= b->radius - SIZE_X || b->py >= SIZE_Y - b->radius || b->py <= b->radius - SIZE_Y){
		moveBall(b);
	}
}

//������ײ�ж�
void ballCrash(struct ball *p, struct ball *q){
	float vertical1, parallel1, vertical2, parallel2;
	float vertical1_crash, vertical2_crash;
	float angleSin, angleCos;
	float ballDistance;
	ballDistance = distance(p->px, p->py, q->px, q->py);
	if (ballDistance <= p->radius + q->radius){
		angleCos = (p->px - q->px) / ballDistance;
		angleSin = (p->py - q->py) / ballDistance;

		vertical1 = p->vx*angleCos + p->vy*angleSin;
		vertical2 = q->vx*angleCos + q->vy*angleSin;
		parallel1 = p->vx*(-angleSin) + p->vy*angleCos;
		parallel2 = q->vx*(-angleSin) + q->vy*angleCos;

		vertical1_crash = ((p->mass - q->mass)*vertical1 + 2 * q->mass * vertical2) / (p->mass + q->mass);
		vertical2_crash = ((q->mass - p->mass)*vertical2 + 2 * p->mass * vertical1) / (p->mass + q->mass);

		p->vx = vertical1_crash*angleCos - parallel1*angleSin;
		p->vy = vertical1_crash*angleSin + parallel1*angleCos;
		q->vx = vertical2_crash*angleCos - parallel2*angleSin;
		q->vy = vertical2_crash*angleSin + parallel2*angleCos;

		while (distance(p->px, p->py, q->px, q->py) <= p->radius + q->radius){
			moveBall(p);
			moveBall(q);
		}
	}
}

//����Ħ��
void accelarateOfRub(struct ball *b){
	float main_v = pow(pow(b->vx, 2) + pow(b->vy, 2), 0.5);
	if (main_v == 0) main_v = 1;
	if (b->vx > 0){
		b->vx -= A_RUB*b->vx / main_v;
		if (b->vx < 0) b->vx = 0;
	}
	else{
		b->vx -= A_RUB*b->vx / main_v;
		if (b->vx > 0) b->vx = 0;
	}

	if (b->vy > 0){
		b->vy -= A_RUB*b->vy / main_v;
		if (b->vy < 0) b->vy = 0;
	}
	else{
		b->vy -= A_RUB*b->vy / main_v;
		if (b->vy > 0) b->vy = 0;
	}
}

//�ƶ����
void cueMove(){
	cueDistance -= (hitDistance - MAIN_BALL.radius) / CUE_MOVE_FRAME;
	if (cueDistance <= MAIN_BALL.radius){
		cueDistance = MAIN_BALL.radius;
		cueIsMoving = 0;
	}
}

//===================================================
//===================================================

void OnDisplay(void){
	int i;
	//Sleep(100);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (viewType == 0){
		glTranslatef(0.0f, 0.0f, -100.0f);
	}
	else if (viewType == 1){
		glTranslatef(0.0, 50.0f, -100.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-cueAngle - 90, 0.0f, 0.0f, 1.0f);
		glTranslatef(-MAIN_BALL.px, -MAIN_BALL.py, -100.0f);
	}

	glBegin(GL_QUADS);
	glColor3f(0.1, 0.1, 0.2);
	glVertex3f(-SIZE_X, -SIZE_Y, SIZE_H);
	glVertex3f(-SIZE_X, SIZE_Y, SIZE_H);
	glVertex3f(-SIZE_X, SIZE_Y, -SIZE_H);
	glVertex3f(-SIZE_X, -SIZE_Y, -SIZE_H);

	glVertex3f(SIZE_X, -SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, SIZE_Y, -SIZE_H);
	glVertex3f(SIZE_X, -SIZE_Y, -SIZE_H);

	glVertex3f(-SIZE_X, SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, SIZE_Y, -SIZE_H);
	glVertex3f(-SIZE_X, SIZE_Y, -SIZE_H);

	glVertex3f(-SIZE_X, -SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, -SIZE_Y, SIZE_H);
	glVertex3f(SIZE_X, -SIZE_Y, -SIZE_H);
	glVertex3f(-SIZE_X, -SIZE_Y, -SIZE_H);

	glColor3f(0.1, 0.5, 0.2);
	glVertex3f(-SIZE_X, SIZE_Y, -SIZE_H);
	glVertex3f(SIZE_X, SIZE_Y, -SIZE_H);
	glVertex3f(SIZE_X, -SIZE_Y, -SIZE_H);
	glVertex3f(-SIZE_X, -SIZE_Y, -SIZE_H);
	glEnd();

	if (controllable == 1 || cueIsMoving){
		if (viewType == 0) glLineWidth(5);
		if (viewType == 1) glLineWidth(10);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(MAIN_BALL.px + cueDistance*cosf(toRadian(cueAngle)), MAIN_BALL.py + cueDistance*sinf(toRadian(cueAngle)), MAIN_BALL_RADIUS);
		glColor3f(1.0f, 1.0f, 1.0f);   glVertex3f(MAIN_BALL.px + (cueDistance + CUE_LENTH)*cosf(toRadian(cueAngle)), MAIN_BALL.py + (cueDistance + CUE_LENTH)*sinf(toRadian(cueAngle)), MAIN_BALL_RADIUS);
		glEnd();
	}

	for (i = 0; i < BALL_QUANTITY; i++){
		drawBall(&balls[i]);
	}
	glutSwapBuffers();
}

void OnReShape(int w, int h){
	GLfloat aspect = (float)w / (float)h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (0 == h) h = 1;
	if (viewType == 0){//��ͶӰ
		if (aspect >= SIZE_X / SIZE_Y)  glOrtho(-SIZE_Y*aspect, SIZE_Y*aspect, -SIZE_Y, SIZE_Y, 0.0f, 250.0f);
		else  glOrtho(-SIZE_X, SIZE_X, -SIZE_X / aspect, SIZE_X / aspect, 0.0f, 250.0f);
	}
	else if (viewType == 1){//͸��ͶӰ
		gluPerspective(60.0f, aspect, 1.0f, 1000.0f);
	}
	windowsWidth = w;
	windowsHeight = h;
}

void OnIdle(void){
	int i, j;
	int isStopped = 1;
	if (cueIsMoving){
		cueMove();
	}
	else{
		for (i = 0; i < BALL_QUANTITY; i++){
			accelarateOfRub(&balls[i]);
			if (balls[i].vx != 0 || balls[i].vy != 0)  isStopped = 0;
			moveBall(&balls[i]);
			for (j = i - 1; j >= 0; j--){
				ballCrash(&balls[i], &balls[j]);
			}
			sideCrash(&balls[i]);

		}
		if (isStopped){
			controllable = 1;
		}
	}
	OnDisplay();
}

void SetupLights(){
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };//������
	GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };//������
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };//�����
	GLfloat lightPos[] = { 50.0f, 80.0f, 60.0f, 1.0f };//��Դλ��

	glEnable(GL_LIGHTING);			//���ù���
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);	//���û�����Դ
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);	//�����������Դ
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);	//���þ����Դ
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);		//���õƹ�λ��
	glEnable(GL_LIGHT0);			//�򿪵�һ���ƹ�

	glEnable(GL_COLOR_MATERIAL);	//���ò��ʵ���ɫ����
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	//ָ��������ɫ����
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //ָ�����϶Ծ����ķ�Ӧ
	glMateriali(GL_FRONT, GL_SHININESS, 100);			//ָ������ϵ��
}

void OnKey(unsigned char key, int x, int y){
	switch (key){
	case GLUT_KEY_UP:
		if (controllable){
			cueDistance += CUE_DISTANCE_CNAHGE;
			if (cueDistance > CUE_DISTANCE_MAX) cueDistance = CUE_DISTANCE_MAX;
			break;
		}
	case GLUT_KEY_DOWN:
		if (controllable){
			cueDistance -= CUE_DISTANCE_CNAHGE;
			if (cueDistance < MAIN_BALL.radius) cueDistance = MAIN_BALL.radius;
			break;
		}
	case GLUT_KEY_LEFT:
		if (controllable){
			cueAngle += CUE_ANGLE_CNAHGE;
			if (cueAngle > 360) cueAngle -= 360;
			break;
		}
	case GLUT_KEY_RIGHT:
		if (controllable){
			cueAngle -= CUE_ANGLE_CNAHGE;
			if (cueAngle < 0) cueAngle += 360;
			break;
		}
	}
}

void OnMouse(int button, int state, int x, int y){
	float power = distanceToPower(cueDistance);
	float radian = toRadian(cueAngle);
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && controllable){
			controllable = 0;
			hitDistance = cueDistance;
			MAIN_BALL.vx = -power*cosf(radian);
			MAIN_BALL.vy = -power*sinf(radian);
			cueIsMoving = 1;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			if (viewType == 0) {
				viewType = 1;
				OnReShape(windowsWidth, windowsHeight);
			}
			else if (viewType == 1) {
				viewType = 0;
				OnReShape(windowsWidth, windowsHeight);
			}
		}
		break;
	}
}

void main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowsWidth, windowsHeight);
	glutCreateWindow("̨����ʾ");

	glutReshapeFunc(OnReShape);//���ڸı�
	glutDisplayFunc(OnDisplay);//�������
	glutIdleFunc(OnIdle);//����ı�
	glutSpecialFunc(OnKey);//���̰���
	glutMouseFunc(OnMouse);//��갴��
	SetupLights();

	glutMainLoop();
}
