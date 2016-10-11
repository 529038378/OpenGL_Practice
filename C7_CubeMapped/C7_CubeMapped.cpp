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
#include<GLFrame.h>
#include<GLMatrixStack.h>
#include<GLGeometryTransform.h>
#include<StopWatch.h>

#define FREEGLUT_STATIC
#include<GL/glut.h>

GLTriangleBatch sphereBatch;
GLShaderManager shaderManager;
GLFrustum   viewFrustum;
GLFrame     cameraFrame;
GLFrame     objectFrame;
GLMatrixStack modelviewMatrix;
GLMatrixStack projectionMatrix;
glGeometryTransform transformPipeline;

GLuint reflectProgram;
GLuint boxProgram;
GLuint cubeTexture;

const char* pccFileName[6] = {"pos_x.tga", "pos_y.tga", "pos_z.tga", "neg_x.tga", "neg_y.tga", "neg_z.tga"};


GLenum enTextType[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
                        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                       };


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
    viewFrustum.SetPerspective(45.0f, float(w)/float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjection());
    transformPipeline.SetMatrixStacks(modelviewMatrix, projectionMatrix);
}

void SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    //TODO
    glEnable(GL_DEPTH_TEST);
    gltMakeSphere(sphereBatch, 1.0f, 52, 26);
    
    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i=0; i<6; i++)
    {
        GLbyte* pBytes;
        int nWidth, nHeight, nComponents;
        GLenum eFormat;
        pBits = gltReadTGABits(pccFileName[i], &nWidth, &nHeight, &nComponents, &eFormat);
        if(pBits == NULL) return false;
        glTexImage2D(enTextType[i], 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE,pBytes);
        
        free(pBytes);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);    
    
    reflectProgram = gltLoadShaderPairWithAttributes("reflect.vp", "reflect.fp", 2, 
                                                    GLT_ATTRIBUTE_VERTEX, "vVertex",
                                                    GLT_ATTRIBUTE_NORMAL, "vNormal");
    
    boxProgram = gltLoadShaderPairWithAttributes("box.vp","box.fp", 2, 
                                                 GLT_ATTRIBUTE_VERTEX, "vVertex", 
                                                 GLT_ATTRIBUTE_NORMAL, "vNormal");
}

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //TODO 
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelviewMatrix.PushMatrix(mCamera);
       glUseProgram(boxProgram);

    modelviewMatrix.PopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
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
    glutCreateWindow("CubeMapped");
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
