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

const GLfloat fTexSize = 100.0f;
GLuint uiTexture[3];

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
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    //TODO

//    M3DVector3f floorPos[ciFloorLineNum*4];
//    for(int i = 0; i<ciFloorLineNum; i+=2 )
//    {
//        floorPos[i][0] = -50.0f;
//        floorPos[i][1] = -1.0f;
//        floorPos[i][2] = 50.0f - i;
//
//        floorPos[i+1][0] = 50.0f;
//        floorPos[i+1][1] = -1.0f;
//        floorPos[i+1][2] = floorPos[i][2];
//
//        floorPos[i+ciFloorLineNum][0] = -floorPos[i][2];
//        floorPos[i+ciFloorLineNum][1] = -1.0f;
//        floorPos[i+ciFloorLineNum][2] = 50.0f;
//
//        floorPos[i+ciFloorLineNum+1][0] = floorPos[i+ciFloorLineNum][0];
//        floorPos[i+ciFloorLineNum+1][1] = -1.0f;
//        floorPos[i+ciFloorLineNum+1][2] = -50.0f;
//    }

//
//    floorBatch.Begin(GL_LINES, 200);
//    floorBatch.CopyVertexData3f(floorPos);
    floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
    floorBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
    floorBatch.Vertex3f(-20.0f, -1.0f, 20.0f);
    
    floorBatch.MultiTexCoord2f(0, fTexSize, 0.0f);
    floorBatch.Vertex3f(20.0f, -1.0f, 20.0f);

    floorBatch.MultiTexCoord2f(0, fTexSize, fTexSize);
    floorBatch.Vertex3f(20.0f, -1.0f, -20.0f);

    floorBatch.MultiTexCoord2f(0, 0.0f, fTexSize);
    floorBatch.Vertex3f(-20.0f, -1.0f, -20.0f);
    

    floorBatch.End();
    


    gltMakeTorus(torusBatch, 0.6f, 0.2f, 30, 30);
    gltMakeSphere(sphereBatch, 0.1f, 26, 13);

    glGenTextures(3, uiTexture);
   
    glBindTexture(GL_TEXTURE_2D, uiTexture[0]);
    if(!LoadTGATexture("Marbel.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT)) 
    {
        std::cout<<"fail to load floor texture"<<std::endl;
    };
    
    glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
    if(!LoadTGATexture("Marslike.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE))
    {
        std::cout<<"fail to load torus texture"<<std::endl;
    };

    glBindTexture(GL_TEXTURE_2D, uiTexture[2]);
    if(!LoadTGATexture("Moonlike.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE))
    {
        std::cout<<"fail to load sphere texture"<<std::endl;
    };


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
    
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    
    M3DVector4f vLightPos = {0.0f, 10.0f, 5.0f, 1.0f};
    M3DVector4f vLightEyePos;
    m3dTransformVector4(vLightEyePos, vLightPos, mCamera);
    static GLfloat vFloorColor[] = {1.0f, 1.0f, 1.0f, 0.75f};
    
    modelViewMatrix.PushMatrix(mCamera);
        modelViewMatrix.Scale(1.0f, -1.0f, 1.0f);
        modelViewMatrix.Translate(0.0f, 2.0f, 0.0f);
        glFrontFace(GL_CW);
        modelViewMatrix.Translate(0.0f, 0.0f, -5.0f);
        modelViewMatrix.Rotate(fYRot, 0.0f, 1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vFloorColor, 0);
        torusBatch.Draw();
        glFrontFace(GL_CCW);
    modelViewMatrix.PopMatrix();


    modelViewMatrix.PushMatrix(mCamera);
    
    
    

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor, 0);
      floorBatch.Draw();
      glDisable(GL_BLEND);

        static GLfloat vTorusColor[] = {1.0f, 0.0f, 0.0f, 0.75f};
        modelViewMatrix.Translate(0.0f, 0.0f, -5.0f);
        modelViewMatrix.PushMatrix();
            modelViewMatrix.Rotate(fYRot, 0.0f, 1.0f, 0.0f);
//           shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vTorusColor);
//           shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vTorusColor);
            glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
//           shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor, 0);
            shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vFloorColor, 0);
            torusBatch.Draw();
        modelViewMatrix.PopMatrix();
        static GLfloat vSphereColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
        modelViewMatrix.Rotate(fYRot*-2.0f, 0.0f, 0.1f, 0.0f);
        modelViewMatrix.Translate(2.0f, 0.0f, 0.0f);
//      shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vSphereColor);
//      shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vSphereColor);
        glBindTexture(GL_TEXTURE_2D, uiTexture[1]);
//      shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor, 0);
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightEyePos, vFloorColor, 0);
        sphereBatch.Draw();
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
