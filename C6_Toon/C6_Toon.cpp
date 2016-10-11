/*************************************************************************
	> File Name: first_triangle.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月30日 星期五 13时56分13秒
 ************************************************************************/

#include<iostream>
#include<GLTools.h>
#include<GLShaderManager.h>
#include<GLMatrixStack.h>
#include<GLGeometryTransform.h>
#include<GLFrame.h>
#include<GLFrustum.h>
#include<StopWatch.h>

#define FREEGLUT_STATIC
#include<GL/glut.h>

GLTriangleBatch sphereBatch;
GLShaderManager shaderManager;


GLFrustum viewFrustum;
GLFrame objectFrame;
GLFrame cameraFrame;
GLMatrixStack modelviewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipline;

GLuint uiTexture;
GLint uiShaderProgram;
GLint MVP;
GLint MV;
GLint NormalMatrix;
GLint textureSampler;

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
    GLbyte *pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;

    // Read the texture bits
    pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
    if(pBits == NULL) 
        return false;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, nWidth, nHeight, 0,
                 eFormat, GL_UNSIGNED_BYTE, pBits);

    free(pBits);

    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
            
    return true;
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(45.0f, float(w) / float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipline.SetMatrixStacks(modelviewMatrix, projectionMatrix);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO
    
    glEnable(GL_DEPTH_TEST);
    cameraFrame.MoveForward(-4.0f);
    uiShaderProgram = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp", 3, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_NORMAL, "vNormal", GLT_ATTRIBUTE_TEXTURE0, "vTexture0");
    MVP = glGetUniformLocation(uiShaderProgram, "MVP");
    MV = glGetUniformLocation(uiShaderProgram, "MV");
    NormalMatrix = glGetUniformLocation(uiShaderProgram, "Normalmatrix");
    textureSampler = glGetUniformLocation(uiShaderProgram, "textureSampler");

    gltMakeSphere(sphereBatch, 1.0f, 26, 13);
    
    glGenTextures(1, &uiTexture);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
    if( !LoadTGATexture("CoolTexture.tga", GL_LINEAR, GL_LINEAR, GL_TEXTURE_WRAP_S) )
    {
        std::cout<<"fail to load texture"<<std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 
    static CStopWatch rotTimer;
    float fYRot = rotTimer.GetElapsedSeconds()*10.0f;
    
    M3DMatrix44f cameraMatrix;
    cameraFrame.GetCameraMatrix(cameraMatrix);
    modelviewMatrix.PushMatrix(cameraMatrix);
        modelviewMatrix.Rotate(fYRot, 0.0f, 1.0f, 0.0f);

        glUseProgram(uiShaderProgram);
        glBindTexture(GL_TEXTURE_2D, uiTexture);
        glUniformMatrix4fv(MVP, 1, GL_FALSE, transformPipline.GetModelViewProjectionMatrix());
        glUniformMatrix4fv(MV, 1, GL_FALSE, transformPipline.GetModelViewMatrix());
        glUniformMatrix3fv(NormalMatrix, 1, GL_FALSE, transformPipline.GetNormalMatrix());
        glUniform1i(textureSampler, 0);
        sphereBatch.Draw();
    modelviewMatrix.PopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
    //TODO
    GLfloat fStep = 1.0f;
    GLfloat fAng = m3dDegToRad(5.0f);
    if( GLUT_KEY_DOWN == key ) cameraFrame.MoveForward(fStep);
    if( GLUT_KEY_UP ==  key ) cameraFrame.MoveForward(-fStep);
    if( GLUT_KEY_LEFT == key ) cameraFrame.RotateWorld(fAng, 0.0f, 1.0f, 0.0f);
    if( GLUT_KEY_RIGHT == key ) cameraFrame.RotateWorld(-fAng, 0.0f, 1.0f, 0.0f);
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
