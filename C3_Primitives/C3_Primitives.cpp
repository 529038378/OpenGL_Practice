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

#include<GLMatrixStack.h>
#include<GLFrustum.h>
#include<GLFrame.h>
#include<GLGeometryTransform.h>
#include<math.h>



GLBatch         pyramidBatch;
GLShaderManager shaderManager;

GLMatrixStack   modelViewMatrix;
GLMatrixStack   projectionMatrix;
GLFrame         cameraFrame;
GLFrame         objectFrame;
GLFrustum       viewFrustum;

GLGeometryTransform transformPipeline;
M3DMatrix44f        shadowMatrix;

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    modelViewMatrix.LoadIdentity();
}
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();


    //TODO
    glEnable(GL_DEPTH_TEST);
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    cameraFrame.MoveForward(-15.0f);

    GLfloat vPyramid[12][3] = { -2.0f, 0.0f, -2.0f, 
                                2.0f, 0.0f, -2.0f, 
                                0.0f, 4.0f, 0.0f,
                                
                                2.0f, 0.0f, -2.0f,
                                2.0f, 0.0f, 2.0f,
                                0.0f, 4.0f, 0.0f,
                                
                                -2.0f, 0.0f, 2.0f,
                                -2.0f, 0.0f, -2.0f,
                                 0.0f, 4.0f, 0.0f};
    
    pyramidBatch.Begin(GL_TRIANGLES, 12);
    pyramidBatch.CopyVertexData3f(vPyramid);
    pyramidBatch.End();


}
void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 

    modelViewMatrix.PushMatrix();
    M3DMatrix44f viewMatrix;
    cameraFrame.GetCameraMatrix(viewMatrix);
    modelViewMatrix.MultMatrix(viewMatrix);

    M3DMatrix44f modelMatrix;
    objectFrame.GetMatrix(modelMatrix);
    modelViewMatrix.MultMatrix(modelMatrix);
    
    GLfloat fColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), fColor);
    pyramidBatch.Draw();
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
}
void SpecialKey(int key, int x, int y)
{
    //TODO
    if( GLUT_KEY_UP == key ) objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    if( GLUT_KEY_DOWN == key ) objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    if( GLUT_KEY_LEFT == key ) objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    if( GLUT_KEY_RIGHT == key ) objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
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
