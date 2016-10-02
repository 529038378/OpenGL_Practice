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

GLBatch squareBatch;
GLShaderManager shaderManager;

GLfloat fPos[] = { 0.1f, 0.0f, 0.0f };
GLfloat fVert[] = { fPos[0],        fPos[1],         0.0f,
                    fPos[0]+0.1f,   fPos[1],         0.0f,
                    fPos[0],        fPos[1]+0.1f,   0.0f,
                    fPos[0]+0.1f,   fPos[1]+0.1f,   0.0f
                  };


GLfloat ciStepX = 0.005f;
GLfloat ciStepY = 0.005f;

void UpdateVert()
{
    if( (fPos[0]+0.1f+ciStepX > 1.0f ) || (fPos[0]+ciStepX < -1.0f ))
    {
        if( ciStepX > 0 ) fPos[0] = 0.9f;
        if( ciStepX < 0 ) fPos[0] = -1.0f;
        ciStepX = -ciStepX;
        
    }
    else
    {
        fPos[0] += ciStepX;   
    }
    if( (fPos[1]+0.1f+ciStepY > 1.0f ) || (fPos[1]+ciStepY < -1.0f ))
    {
        if( ciStepY > 0 ) fPos[1] = 0.9f;
        if( ciStepY < 0 ) fPos[1] = -1.0f;
        ciStepY = -ciStepY;
    }
    else
    {
        fPos[1] += ciStepY;
    }
    
    fVert[0] =  fPos[0];
    fVert[1] =  fPos[1];
    fVert[3] =  fPos[0]+0.1f;
    fVert[4] =  fPos[1];
    fVert[6] =  fPos[0];
    fVert[7] =  fPos[1]+0.1f;
    fVert[9] =  fPos[0]+0.1f;
    fVert[10] =  fPos[1]+0.1f;
    squareBatch.CopyVertexData3f(fVert);
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO 
    squareBatch.Begin(GL_TRIANGLE_STRIP, 4);
    squareBatch.CopyVertexData3f(fVert);
    squareBatch.End();
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO
    GLfloat vColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vColor);
    squareBatch.Draw();
    glutSwapBuffers();
    UpdateVert();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("C2_Bounce");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

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
