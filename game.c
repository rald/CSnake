#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include <GL/glfw.h>
#include <GL/gl2d.h>

#include "uvcoord_sprites.h"

#define GAME_TITLE "CSnakeGB" 

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define WORLD_WIDTH 16
#define WORLD_HEIGHT 16

#define SNAKES_MAX (WORLD_WIDTH*WORLD_HEIGHT)

typedef struct Point Point;

struct Point {
	int x,y;
};

Point snakes[SNAKES_MAX];
int snakesLen=0;

Point snake;

Point food;

Point display[SNAKES_MAX];




void newFood() {
	food.x=rand()%16;
	food.y=rand()%16;
}

int cmp(const void *a,const void *b) {
	Point l=*(Point*)a;
	Point r=*(Point*)b;

	if(l.y<r.y) {
		return -1;
	} else if(l.y>r.y) {
		return 1;
	} else if(l.x<r.x) {
		return -1;
	} else if(l.x>r.x) {
		return 1;
	}
	
	return 0;
}

int main(int argc, char *argv[]) {

	glImage sprites_image[SPRITES_NUM_IMAGES];

	GLuint sprites_texture;

	int i,j;

	int x,y;

	int cx,cy;

	bool quit = false;

	int xi=1,yi=0;

	int d=0,delay=10;


	srand(time(NULL));

	glScreenInit(SCREEN_WIDTH,SCREEN_HEIGHT);
	glfwSetWindowTitle(GAME_TITLE);
	glfwSwapInterval(1);

	sprites_texture = glLoadSpriteset(
			"images/snake.tga",
			sprites_image,
			SPRITES_BITMAP_WIDTH,
			SPRITES_BITMAP_HEIGHT,
			SPRITES_NUM_IMAGES,
			sprites_texcoords,
			GL_NEAREST
	); 

	cx=(SCREEN_WIDTH-32)/2;
	cy=(SCREEN_HEIGHT-16*16)/2;

	snake.x=0;
	snake.y=0;

	snakes[0]=snake;
	snakesLen=1;

	newFood();

	while(!quit) {

    glClearColor( 0xCA/255.0f, 0xDC/255.0f, 0x9F/255.0f, 0xFF/255.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		for(j=0;j<16;j++) {
			for(i=0;i<16;i++) {
				x=(i-j)*16+cx;
				y=(i+j)*8+cy;
				glSprite(x,y,GL2D_FLIP_NONE,&sprites_image[0]);
			}
		}

		for(i=0;i<snakesLen;i++) {
			display[i]=snakes[i];
		}
		display[snakesLen]=food;

		qsort(display,snakesLen+1,sizeof(Point),cmp);

		for(i=0;i<snakesLen+1;i++) {
			x=(display[i].x-display[i].y)*16+cx;
			y=(display[i].x+display[i].y)*8+cy;
			glSprite(x,y,GL2D_FLIP_NONE,&sprites_image[1]);
		}

		if(glfwGetKey('W')) {
			xi=0; yi=-1;
		} else if(glfwGetKey('S')) {
			xi=0; yi=1;
		} else if(glfwGetKey('A')) {
			xi=-1; yi=0;
		} else if(glfwGetKey('D')) {
			xi=1; yi=0;
		}

		d++;
		if(d>=delay) {
			d=0;
			snake.x+=xi;
			snake.y+=yi;

			if(snake.x<0) snake.x=15;
			if(snake.y<0) snake.y=15;
			if(snake.x>15) snake.x=0;
			if(snake.y>15) snake.y=0;

			if(snake.x==food.x && snake.y==food.y) {
				if(snakesLen<SNAKES_MAX-1) {
					snakesLen++;
					newFood();
				}
			}

			for(i=snakesLen-1;i>0;i--) {
				snakes[i]=snakes[i-1];
			}
			snakes[0]=snake;
		}

		float TimeStart=glfwGetTime();
		while((glfwGetTime()-TimeStart)<(1.0/100.0)){};
	
		glfwSwapBuffers();
	
		quit=glfwGetKey(GLFW_KEY_ESC) | !glfwGetWindowParam(GLFW_OPENED);
	}
	
	glfwTerminate();

	return 0;
}

