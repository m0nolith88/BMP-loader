#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "bmp.c"

struct BMP_BMP picture;

static GLuint texName;

void init(int n,char *filename,char *mode,struct BMP_BMP bmp){
    glClearColor(0.0,0.0,0.0,0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);

    
    unsigned char R,G,B;
    bmp.header=loadHeader(filename,mode,bmp);
    bmp.info=loadInfoHeader(filename,mode,bmp);
    bmp.data=loadRasterData(filename,mode,bmp);
    int index;
    for(int i = 0; i < 512*512 ; ++i){
        index = i*3;
        B = bmp.data[index]; G = bmp.data[index+1]; R = bmp.data[index+2];
        bmp.data[index] = R; bmp.data[index+1] = G; bmp.data[index+2] = B;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    glGenTextures(n,&texName);
    glBindTexture(GL_TEXTURE_2D,texName);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE,bmp.data);

}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glBindTexture(GL_TEXTURE_2D,texName);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void reshape(int w,int h){
    glViewport(0,0,(GLsizei)w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h){
        glOrtho(-3.5,3.5,-3.5*(GLfloat)h/(GLfloat)w,3.5*(GLfloat)h/(GLfloat)w,-3.5,3.5);
    } else {
        glOrtho(-3.5*(GLfloat)w/(GLfloat)h,3.5*(GLfloat)w/(GLfloat)h,-3.5,3.5,-3.5,3.5);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc,char *argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("oglApp");
    init(1,"brick.bmp","rb",picture);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}