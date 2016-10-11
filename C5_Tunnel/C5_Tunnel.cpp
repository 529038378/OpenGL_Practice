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
#include<GLMatrixStack.h>
#include<GLFrame.h>
#include<GLGeometryTransform.h>

#define FREEGLUT_STATIC
#include<GL/glut.h>

GLShaderManager shaderManager;

GLFrustum   viewFrustum;
GLFrame     modelViewFrame;
GLFrame     cameraFrame;
GLGeometryTransform transformPipeline;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;

GLuint uiTexture[3];

GLBatch floorBatch;
GLBatch ceilBatch;
GLBatch leftBatch;
GLBatch rightBatch;

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

void ProcessMenu(int value)
{
    GLfloat fLargest;
    switch(value)
    {
        case 1:
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
            glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
        break;
        glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
        case 2:
        break;
        default:
        break;
    }
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(45.0f, float(w)/float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO
    
    glGenTextures(3, uiTexture);
    glBindTexture(GL_TEXTURE_2D, uiTexture[0]);
    if( !LoadTGATexture("floor.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT) )
    {
        std::cout<<"fail to load floor texture"<<std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
    if( !LoadTGATexture("ceiling.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT) )
    {
        std::cout<<"fail to load ceil texture"<<std::endl;
    }
    
    glBindTexture(GL_TEXTURE_2D, uiTexture[2]);
    if( !LoadTGATexture("brick.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT) )
    {
        std::cout<<"fail to load ceil texture"<<std::endl;
    }

    floorBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(int z = 60.0f; z>=0.0f; z -= 10.0f)
    {
        floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        floorBatch.Vertex3f(-10.0f, -10.0f, z);

        floorBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z);

        floorBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        floorBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);

        floorBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);
    }
    floorBatch.End();

    rightBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(int z = 60.0f; z>=0.0f; z -= 10.0f)
    {
        rightBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        rightBatch.Vertex3f(-10.0f, -10.0f, z);

        rightBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        rightBatch.Vertex3f(-10.0f, 10.0f, z);

        rightBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        rightBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);

        rightBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        rightBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);
    }
    rightBatch.End();
    
    ceilBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(int z = 60.0f; z>=0.0f; z -= 10.0f)
    {
        ceilBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        ceilBatch.Vertex3f(-10.0f, 10.0f, z);

        ceilBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        ceilBatch.Vertex3f(10.0f, 10.0f, z);

        ceilBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        ceilBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);

        ceilBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        ceilBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
    }
    ceilBatch.End();

    leftBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(int z = 60.0f; z>=0.0f; z -= 10.0f)
    {
        leftBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        leftBatch.Vertex3f(10.0f, -10.0f, z);

        leftBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        leftBatch.Vertex3f(10.0f, 10.0f, z);

        leftBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        leftBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);

        leftBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        leftBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
    }
    leftBatch.End();
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 

    GLfloat vColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

    M3DMatrix44f camera;
    cameraFrame.GetCameraMatrix(camera);
    modelViewMatrix.PushMatrix(camera);
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vColor, 0);
        glBindTexture(GL_TEXTURE_2D, uiTexture[0]);
        floorBatch.Draw();

        glBindTexture(GL_TEXTURE_2D, uiTexture[2]);
        leftBatch.Draw();
        rightBatch.Draw();

        glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
        ceilBatch.Draw();
    modelViewMatrix.PopMatrix();
    

    glutSwapBuffers();
    glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
    //TODO
    GLfloat fStep = 0.5f;
    GLfloat fAng = m3dDegToRad(5.0f);
    if( GLUT_KEY_DOWN == key ) cameraFrame.MoveForward(-fStep);
    if( GLUT_KEY_UP == key ) cameraFrame.MoveForward(fStep);
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

    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Anisotropic On",1);
    glutAddMenuEntry("Anisotropic off", 2);
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
