/*************************************************************************
	> File Name: first_triangle.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月30日 星期五 13时56分13秒
 ************************************************************************/

#include<iostream>
#include<GLTools.h>
#include<GLShaderManager.h>
#include<GLFrustum.h>

#define FREEGLUT_STATIC
#include<GL/glut.h>

GLShaderManager shaderManager;
GLFrustum viewFrustum;
GLBatch smallStarBatch;
GLBatch mediumStarBatch;
GLBatch bigStarBatch;
GLBatch moonBatch;
GLBatch mountainBatch;

const int ciSmallStarNum = 100;
const int ciMediumStarNum = 40;
const int ciBigStarNum = 15;

const int ciScreenWidth = 800;
const int ciScreenHeight = 600;

void ProcessMenu(int value)
{
    switch(value)
    {
        case 1:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        break;
        case 2:
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POINT_SMOOTH);
        break;
        default:
        break;
    }
    glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetOrthographic(0.0f, ciScreenWidth, 0.0f, ciScreenHeight, -1.0f, 1.0f);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    shaderManager.InitializeStockShaders();
    //TODO
    M3DVector3f smallStarPos[ciSmallStarNum];
    M3DVector3f mediumStarPos[ciMediumStarNum];
    M3DVector3f bigStarPos[ciBigStarNum];
    
    for( int i=0; i<ciSmallStarNum; i++ )
    {
        smallStarPos[i][0] = (GLfloat)(rand()%ciScreenWidth);
        smallStarPos[i][1] = (GLfloat)(rand()% (ciScreenHeight - 100)) + 100.0f;
    }
    smallStarBatch.Begin(GL_POINTS, ciSmallStarNum);
    smallStarBatch.CopyVertexData3f(smallStarPos);
    smallStarBatch.End();
    
    for( int i=0; i<ciMediumStarNum; i++ )
    {
        mediumStarPos[i][0] = (GLfloat)(rand()%ciScreenWidth);
        mediumStarPos[i][1] = (GLfloat)(rand()%(ciScreenHeight - 100)) + 100.0f ;
    }
    mediumStarBatch.Begin(GL_POINTS, ciMediumStarNum);
    mediumStarBatch.CopyVertexData3f(mediumStarPos);
    mediumStarBatch.End();

    for( int i=0; i<ciBigStarNum; i++ )
    {
        bigStarPos[i][0] = (GLfloat)(rand()%ciScreenWidth);
        bigStarPos[i][1] = (GLfloat)(rand()%(ciScreenHeight - 100)) + 100.0f;
    }
    bigStarBatch.Begin(GL_POINTS, ciBigStarNum);
    bigStarBatch.CopyVertexData3f(bigStarPos);
    bigStarBatch.End();

    M3DVector3f vMountains[12] = {  0.0f,   25.0f, 0.0f,
                                    50.0f,  80.0f, 0.0f,
                                    100.0f, 60.0f, 0.0f,
                                    150.0f, 15.0f, 0.0f,
                                    210.0f, 52.0f, 0.0f,
                                    240.0f, 70.0f, 0.0f,
                                    300.0f, 63.0f, 0.0f,
                                    320.0f, 99.0f, 0.0f,
                                    400.0f, 10.0f, 0.0f,
                                    460.0f, 65.0f, 0.0f,
                                    500.0f, 72.0f, 0.0f,
                                    800.0f, 89.0f, 0.0f,
                                    };
    mountainBatch.Begin(GL_LINE_STRIP, 12);
    mountainBatch.CopyVertexData3f(vMountains);
    mountainBatch.End();

    const int AngleNum = 34;
    M3DVector3f anglePos[AngleNum];
    GLfloat fAngle = 0.0f;
    int iN = 0;
    anglePos[iN][0] = 500.0f;
    anglePos[iN][1] = 400.0f;
    anglePos[iN][2] = 0.0f;
    for(;fAngle < 2.0f*3.1415926f; fAngle += 0.2f)
    {
        iN++;
        anglePos[iN][0] = 50.0f*float(cos(fAngle)) + 500.0f;
        anglePos[iN][1] = 50.0f*float(sin(fAngle)) + 400.0f;
        anglePos[iN][2] = 0.0f;
    }
    iN++;
    anglePos[iN][0] = 550.0f;
    anglePos[iN][1] = 400.0f;
    anglePos[iN][2] = 0.0f;
    moonBatch.Begin(GL_TRIANGLE_FAN, AngleNum);
    moonBatch.CopyVertexData3f(anglePos);
    moonBatch.End();
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 
    GLfloat vColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_FLAT, viewFrustum.GetProjectionMatrix(), vColor);
    
    glPointSize(1.0f);
    smallStarBatch.Draw();

    glPointSize(4.0f);
    mediumStarBatch.Draw();

    glPointSize(8.0f);
    bigStarBatch.Draw();
    
    glLineWidth(1.0f);
    mountainBatch.Draw();

    moonBatch.Draw();
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

    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Antialiased Rendering", 1);
    glutAddMenuEntry("Normal Rendering", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

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
