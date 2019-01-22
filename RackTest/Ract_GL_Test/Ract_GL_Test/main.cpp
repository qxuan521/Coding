// 
// #include<stdlib.h>
// #include<stdio.h>
// #include<windows.h>
// #include <tchar.h>
// 
// 
// 
// 
// #include "glut.h"
// //#include<gl/GL.h>
// //#include<gl/GLU.h>
// 
// 
// void display(void)
// {
// 	glClear(GL_COLOR_BUFFER_BIT);//清除所有像素
// 	static const GLfloat vertex_list[][3] = {
// 	 -0.5f, -0.5f, -0.5f,
// 	  0.5f, -0.5f, -0.5f,
// 	 -0.5f,   0.5f, -0.5f,
// 	  0.5f,   0.5f, -0.5f,
// 	 -0.5f, -0.5f,   0.5f,
// 	  0.5f, -0.5f,   0.5f,
// 	 -0.5f,   0.5f,   0.5f,
// 	  0.5f,   0.5f,   0.5f,
// 	};
// 	static const GLint index_list[][4] = {
// 	 0, 2, 3, 1,
// 	 0, 4, 6, 2,
// 	 0, 1, 5, 4,
// 	 4, 5, 7, 6,
// 	 1, 3, 7, 5,
// 	 2, 6, 7, 3,
// 	};
// 	/*int i, j;*/
// 
// 	// 绘制的时候代码很简单
// 	glFrontFace(GL_CCW);
// 	glCullFace(GL_BACK);
// 	glEnable(GL_CULL_FACE);
// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 	glBegin(GL_QUADS);
// // 	for (i = 0; i < 6; ++i)
// // 	{// 有六个面，循环六次
// // 		for (j = 0; j < 4; ++j)  // 每个面有四个顶点，循环四次
// // 		{
// // 			glVertex3fv(vertex_list[index_list[i][j]]);
// // 		}
// // 	}
// 	glEnableClientState(GL_VERTEX_ARRAY);
// 	glVertexPointer(3, GL_FLOAT, 0, vertex_list);
// 	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, index_list);
// 	glEnd();
// 	glFlush();//用于强制刷新缓冲，保证绘图命令将被执行，而不是存储在缓冲区中等待其他的OpenGL命令
// }
// 
// 
// void init(void)
// {
// 	glClearColor(0.0, 0.0, 0.0, 0.0);//将清空颜色设置为黑
// 
// 
// 	glMatrixMode(GL_PROJECTION);//glMatrixMode：告诉计算机“我接下来要做什么”，GL_PROJECTION：要对投影相关进行操作
// 	glLoadIdentity();//将当前的用户坐标系的原点移到了屏幕中心
// 	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);//投影类型：正交投影。不会因为离屏幕的远近而产生大小的变换的情况
// }
// 
// 
// int _tmain(int argc, _TCHAR* argv[])
// {
// 	glutInit(&argc, (char**)argv);
// 	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
// 	glutInitWindowSize(250, 250);
// 	glutInitWindowPosition(100, 100);
// 	glutCreateWindow("hello");
// 	init();
// 	glutDisplayFunc(display);
// 	glutMainLoop();
// 	return 0;
// }


#include <windows.h> // FOR Sleep
#include "glut.h"    // and glut32.lib ,glut32.dll
///////////////////////////////////////////////////////
static GLfloat xrot = 0.0;
static GLfloat yrot = 0.0;
static GLfloat zrot = 0.0;
void cube()//绘制立方体
{
	glBegin(GL_LINES);//绘制坐标轴
	glPointSize(8);
	glLineWidth(2);
	glColor3f(1, 0.5, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	//绘制立方体
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); //红 y轴          
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(0.5, 0.5, 0.5);

	glColor3f(0.0, 1.0, 0.0); //绿          
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);

	glColor3f(0.0, 0.0, 1.0); //蓝 x轴          
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);

	glColor3f(1.0, 1.0, 0.0); //黄           
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);

	glColor3f(0.6, 0.2, 0.2); //棕           
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, -0.5, 0.5);

	glColor3f(0.6, 0.1, 0.9); //紫 z轴          
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glEnd();
}

void display(void)//显示
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//glPushMatrix();
	glTranslatef(0, 0, -5);
	//glTranslatef(0,0,0);
	glRotatef(xrot, 0.5, 0, 0);
	//glTranslatef(0,0,0);
	//glTranslatef(0,0.5,0);
	glRotatef(yrot, 0, 0.5, 0);
	//glTranslatef(0,-0.5,0);
	//glTranslatef(0,0,0.5);
	glRotatef(zrot, 0, 0, 0.5);
	//glTranslatef(0,0,-0.5);
	cube();
	glutSwapBuffers();
	glPopMatrix();
	//
	xrot += 0.5;
	yrot += 0.5;
	zrot += 0.5;
	// speed control
	Sleep(20);
}

void reshape(int w, int h)//重新绘制窗口
{
	if (h == 0) h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 4.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void init(int width, int height)
{
	if (height == 0) height = 1;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int w, int h)
{
	if (key == 'f') // 进入全屏
		glutFullScreen();
	if (key == 'F') // 退出全屏
	{
		glutReshapeWindow(640, 480); // 设置窗口大小（不能用初始化的函数）
		glutPositionWindow(400, 100); // 设置窗口位置（不能用初始化的函数）
	}
	if (key == 27) // ESC退出程序
		exit(0);
	if (key == 'x') xrot = xrot + 10;
	if (key == 'c') xrot = xrot - 10;
	if (key == 'y') yrot = yrot + 10;
	if (key == 'z') zrot = zrot + 10;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("");

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	init(640, 480);
	glutMainLoop();
	return 0;
}