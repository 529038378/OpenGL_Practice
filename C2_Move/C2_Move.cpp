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

#include <vector>


GLBatch squareBatch;
GLShaderManager shaderManager;


GLfloat  vfPos[] = {0.0f, 0.0f, 0.0f};
GLfloat  vvfSquare[4][3];

const GLfloat  ciStep = 0.05f;

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void UpdateVert()
{

    vvfSquare[0][0] = vfPos[0] - 0.1f;
    vvfSquare[0][1] = vfPos[1] - 0.1f;
    vvfSquare[1][0] = vfPos[0] - 0.1f;
    vvfSquare[1][1] = vfPos[1] + 0.1f;
    vvfSquare[2][0] = vfPos[0] + 0.1f;
    vvfSquare[2][1] = vfPos[1] + 0.1f;
    vvfSquare[3][0] = vfPos[0] + 0.1f;
    vvfSquare[3][1] = vfPos[1] - 0.1f;
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO
    vvfSquare[0][0] = vfPos[0] - 0.1f;
    vvfSquare[0][1] = vfPos[1] - 0.1f;
    vvfSquare[1][0] = vfPos[0] - 0.1f;
    vvfSquare[1][1] = vfPos[1] + 0.1f;
    vvfSquare[2][0] = vfPos[0] + 0.1f;
    vvfSquare[2][1] = vfPos[1] + 0.1f;
    vvfSquare[3][0] = vfPos[0] + 0.1f;
    vvfSquare[3][1] = vfPos[1] - 0.1f;

    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    squareBatch.CopyVertexData3f(vvfSquare);
    squareBatch.End();

}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 
    squareBatch.CopyVertexData3f(vvfSquare);
    GLfloat  vColor[] = {1.0f, 0.0f, 0.0f, 0.0f };
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vColor);
    squareBatch.Draw();
    glutSwapBuffers();
    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
    //TODO
    if( GLUT_KEY_UP == key )    vfPos[1] += ciStep; 
    if( GLUT_KEY_DOWN == key )  vfPos[1] -= ciStep; 
    if( GLUT_KEY_LEFT == key )  vfPos[0] -= ciStep; 
    if( GLUT_KEY_RIGHT == key )  vfPos[0] += ciStep; 
    if( vfPos[1] > 0.9f ) vfPos[1] = 0.9f;
    if( vfPos[1] < -0.9f ) vfPos[1] = -0.9f;
    if( vfPos[0] > 0.9f ) vfPos[0] = 0.9f;
    if( vfPos[0] < -0.9f ) vfPos[0] = -0.9f;

    UpdateVert();
    squareBatch.CopyVertexData3f(vvfSquare);
    glutPostRedisplay();
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
