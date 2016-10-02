/*************************************************************************
	> File Name: first_triangle.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月30日 星期五 13时56分13秒
 ************************************************************************/

#include<iostream>
#include<GLTools.h>
#include<GLShaderManager.h>

#define FREEGLUT_STATIC
#include<GL/glut.h>

GLBatch triangleBatch;
GLShaderManager shaderManager;


void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 
    glutSwapBuffers();
}

void SpecialKey(int key, int x, int y)
{
    //TODO
    
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKey);
    GLenum err = glewInit();
    if( GLEW_OK !=  err )
    {
        std::cerr<<"GLEW error: "<<glewGetErrorString(err)<<std::endl; 
        return 1;
    }
    
    SetupRC();
    glutMainLoop();
    return 0;
}
