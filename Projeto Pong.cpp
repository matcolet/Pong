#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

GLfloat tam;

GLfloat windowXmin, windowXmax;
GLfloat windowYmin, windowYmax;

GLfloat raqueteLar, raqueteAlt;

GLfloat raqEsq, raqDir; 

//variaveis da bolinha 
GLfloat Tx;
GLfloat Ty;

GLfloat xStep;
GLfloat yStep;

GLfloat minX, maxX;
GLfloat minY, maxY;

GLint golEsq, golDir;

GLint velBola;  

char placar[10];


void DesenhaTexto(void *font, char *string, int placarEsq, int placarDir) {
	
	sprintf(placar, "%i  |  %i", placarEsq, placarDir);
	
	while(*string)
		glutBitmapCharacter(font,*string++);

}

void DesenhaRaquete( GLfloat x, GLfloat y, GLfloat lar, GLfloat alt){
	
  glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f((x + lar), y);
        glVertex2f((x + lar), (y + alt));
        glVertex2f(x, (y + alt));
    glEnd();
	
	
}

void Desenha(void){
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
	// posição centro e 10% menos que o total da window
	glRasterPos2f(0, windowYmax - (windowYmax*0.1)); 	
	DesenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24,placar, golEsq, golDir);
		
	glPushMatrix();
		glTranslated(0.0, raqEsq, 0.0); 
		//desenha raquete da esquerda
		DesenhaRaquete( (windowXmin - (windowXmin* 0.04)), 0, raqueteLar, raqueteAlt );
	glPopMatrix(); 
	
	glPushMatrix();
		glTranslated(0.0, raqDir, 0.0); 
		//desenha raquete da direita
		DesenhaRaquete( (windowXmax - (windowXmax* 0.04)), 0, -raqueteLar,-raqueteAlt); 
	glPopMatrix();
	
	//desenha bola 
	glPushMatrix();
		glTranslatef(Tx, Ty, 0.0f);
		glPointSize(10.0);
		glBegin(GL_POINTS);
			glVertex2f(0.0, 0.0);
		glEnd();
	glPopMatrix(); 
	
	//raquete direita 
	//glPointSize(2.0);
	//glColor3f(1.0,0.0,0.0);
	//glBegin(GL_POINTS);
	//	glVertex2f(0, 0 +raqDir);
	//	glVertex2f(0, (-raqueteAlt +raqDir) );
	//glEnd();
	
	//raquete esquerda 
	//glPointSize(2.0);
	//glColor3f(1.0,0.0,0.0);
	//glBegin(GL_POINTS);
	//	glVertex2f(0, 0 +raqEsq);
	//	glVertex2f(0, raqueteAlt + raqEsq );
	//glEnd();
	
	glFlush();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h){
	GLsizei largura, altura;
	
	if(h == 0) h = 1;
	
	largura = w;
	altura = h;

	glViewport(0, 0, largura, altura);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (largura <= altura) {
		gluOrtho2D (-tam, tam, -tam*altura/largura, tam*altura/largura);
		windowXmin = -tam;
		windowXmax =  tam;
		windowYmin = -tam*altura/largura;
		windowYmax = tam*altura/largura;
	}
	else{ 
		gluOrtho2D (-tam*largura/altura, tam*largura/altura, -tam, tam);
		windowXmin = -tam*largura/altura;
		windowXmax =  tam*largura/altura;
		windowYmin = -tam;
		windowYmax =  tam;
	}
}

void Teclado (unsigned char key, int x, int y){
		
	switch(key){
		   
		case 'a': case 'A':
			if(raqEsq > windowYmin)
				raqEsq -= 5.0;  
			
			break; 
		
		case 'q': case 'Q':
			if(raqEsq < windowYmax - raqueteAlt)
				raqEsq += 5.0;  
			
			break; 	   		
		
	/*
		case 'o': case 'O':
			if(raqDir < windowYmax)
				raqDir += 5.0;  
			
			break; 
		
		case 'l': case 'L':
			if(raqDir > windowYmin + raqueteAlt)
				raqDir -= 5.0;  
			
			break; 	
		*/
	}
		  
	glutPostRedisplay();
		
	if (key == 27)
		exit(0);
}

void TeclasEspeciais( GLint key , GLint x , GLint y ) {
	
	switch(key){
		
	case GLUT_KEY_PAGE_UP:
		if(raqDir < windowYmax)
			raqDir += 5.0; 
		
		break;
		

	case GLUT_KEY_PAGE_DOWN:
		if(raqDir > windowYmin + raqueteAlt)
			raqDir -= 5.0; 	
		
		break;
	}
	
	glutPostRedisplay();
	
}


void Anima(int value){

				
	/*
	* teste de colisão 
	*/
	
	// teste de colisão top e button 
	if( (Ty+maxY) > windowYmax || (Ty+minY) < windowYmin )
		yStep = -yStep;
	
		
	//teste de colisão na raquete direita 	
	if( (Ty+maxY) <= 0 +raqDir && (Ty+maxY) >= (-raqueteAlt +raqDir) )
		if( (Tx+maxX) > (windowXmax - (windowXmax* 0.04) - raqueteLar) ){
			xStep = -xStep;
			if( velBola > 10) // pros alunos fazer 
				velBola -=  velBola *0.2;   
		}
					
		
	//teste de colisão na raquete esquerda  	
	if( (Ty+maxY) >= 0 +raqEsq && (Ty+maxY) <= (raqueteAlt + raqEsq) )
		if( (Tx+minX) < (windowXmin - (windowXmin* 0.04) + raqueteLar) ){
			xStep = -xStep;
			if( velBola > 10) // pros alunos fazer 
				velBola -=  velBola *0.2;  
		}
				
    
	Tx += xStep;
	Ty += yStep;


	//gol da esquerda 
	if( (Tx+maxX) > windowXmax ){
		//Tx = Ty = 0.0; 
		
		Tx = 0.0; 
		//Ty = rand() % (int) tam;
		Ty = rand() % ((int)tam*2) + (-(int)tam); // PROS ALUNOS
		golEsq++;
		velBola = 70.0;  	
	}
	
	//gol da direita 
	if( (Tx+minX) < windowXmin  ){
		Tx = 0.0; 
		Ty = rand() % ((int)tam*2)  + (-(int)tam);
		golDir++;
		velBola = 70.0;  	
	}
	
		

	glutPostRedisplay();
	glutTimerFunc(velBola,Anima, 1);
}

void Inicializa (void){   

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	tam = 50.0f;
	
	windowXmin = windowYmin = -tam;
	windowXmax = windowYmax = tam;
	
	raqueteLar = 4.0f; 
	raqueteAlt = 20.0f; 
	
	raqEsq = 0.0; 
	raqDir = 0.0;
	
	golEsq = 0; 
	golDir = 0; 
	
	Tx = Ty = 0.0f;
	xStep = yStep = 2.0f;
	
	minX = -2.0f;
	maxX =  2.0f;
	minY = -2.0f;
	maxY =  2.0f;
	
	velBola = 70; 
	
}

int main(void){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(5,5); 
	glutInitWindowSize(1200, 700); 
	glutCreateWindow("Projeto Pong");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutKeyboardFunc(Teclado);
	glutSpecialFunc( TeclasEspeciais ); 
    glutTimerFunc(50, Anima, 1);
	Inicializa();
	glutMainLoop();
	return 0;
}

