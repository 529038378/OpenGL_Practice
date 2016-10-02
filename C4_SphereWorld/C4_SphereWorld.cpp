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
#include<GLFrustum.h>
#include<GLMatrixStack.h>
#include<GLGeometryTransform.h>
#include<GLFrame.h>
#include<StopWatch.h>

const int ciFloorLineNum = 100;

GLBatch floorBatch;
GLTriangleBatch torusBatch;
GLTriangleBatch sphereBatch;
GLShaderManager shaderManager;
GLFrame objectFrame;
GLFrame cameraFrame;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLFrustum viewFrustum;

bool bFillMode = true;
bool bLineMode = false;
bool bPointMode = false;

void ProcessMenu(int value)
{
    switch(value)
    {
        case 1:
            bFillMode = true;
            bLineMode = false;
            bPointMode = false;
        break;
        case 2:
            bFillMode = false;
            bLineMode = true;
            bPointMode = false;
        break;
        case 3:
            bFillMode = false;
            bLineMode = false;
            bPointMode = true;
        break;
        default:
        break;
    }
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void SetupRC()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    //TODO

    M3DVector3f floorPos[ciFloorLineNum*4];
    for(int i = 0; i<ciFloorLineNum; i+=2 )
    {
        floorPos[i][0] = -50.0f;
        floorPos[i][1] = -1.0f;
        floorPos[i][2] = 50.0f - i;

        floorPos[i+1][0] = 50.0f;
        floorPos[i+1][1] = -1.0f;
        floorPos[i+1][2] = floorPos[i][2];

        floorPos[i+ciFloorLineNum][0] = -floorPos[i][2];
        floorPos[i+ciFloorLineNum][1] = -1.0f;
        floorPos[i+ciFloorLineNum][2] = 50.0f;

        floorPos[i+ciFloorLineNum+1][0] = floorPos[i+ciFloorLineNum][0];
        floorPos[i+ciFloorLineNum+1][1] = -1.0f;
        floorPos[i+ciFloorLineNum+1][2] = -50.0f;
    }

    floorBatch.Begin(GL_LINES, 200);
    floorBatch.CopyVertexData3f(floorPos);
    floorBatch.End();
    
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 30, 30);
    gltMakeSphere(sphereBatch, 0.1f, 26, 13);

}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO
    
    if(bFillMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(bLineMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(bPointMode) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    static CStopWatch rotTimer;
    float fYRot = rotTimer.GetElapsedSeconds()*60.0f;
    

    modelViewMatrix.PushMatrix();
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);
    
    M3DVector4f vLightPos = {0.0f, 10.0f, 5.0f, 1.0f};
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);

    static GLfloat vFloorColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor);
    floorBatch.Draw();


    static GLfloat vTorusColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    modelViewMatrix.Translate(0.0f, 0.0f, -5.0f);
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Rotate(fYRot, 0.0f, 1.0f, 0.0f);
//    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vTorusColor);
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vTorusColor);
    torusBatch.Draw();
    modelViewMatrix.PopMatrix();

    static GLfloat vSphereColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
    modelViewMatrix.Rotate(fYRot*-2.0f, 0.0f, 0.1f, 0.0f);
    modelViewMatrix.Translate(2.0f, 0.0f, 0.0f);
//    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vSphereColor);
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vSphereColor);
    sphereBatch.Draw();

    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
    //TODO
    float fStep = 0.2f;
    float fAng = float(m3dDegToRad(5.0f));
    if( GLUT_KEY_UP == key )  cameraFrame.MoveForward(fStep);
    if( GLUT_KEY_DOWN == key ) cameraFrame.MoveForward(-fStep);
    if( GLUT_KEY_LEFT == key ) cameraFrame.RotateWorld(fAng, 0.0f, 1.0f, 0.0f);
    if( GLUT_KEY_RIGHT == key ) cameraFrame.RotateWorld(-fAng, 0.0f, 1.0f, 0.0f);

//    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("SphereWorld");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKey);

    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Fill Mode", 1);
    glutAddMenuEntry("Line Mode", 2);
    glutAddMenuEntry("Point Mode", 3);
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
