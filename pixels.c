#include <GL/glut.h>
#include <time.h>
#include <stdio.h>

const unsigned int W = 700;
const unsigned int H = 700;
enum mode{preset, rando, hogwild};
int* theWorld;

//Conway's Helper Functions
int* get_map(int w, int h) {
    return malloc((w * h) * sizeof(int));
}

void random_map(int* map, int w, int h) {
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            ////////////////////////////////////////////////
            //  (rand() %  (upper - lower + 1)) + lower;  //
            ////////////////////////////////////////////////
            int r = rand() % (1-0 + 1) + 0;
            map[y*w + x] = r;
        }
    }   
}

void zero_map(int* world, int w, int h) {
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            world[y * w + x] = 0;
        }
    }
}

void update_world(int* map, int w, int h) {
    int nb_map[w * h];
    
    //Update neighbor map
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {

            int ul, u, ur, l, r, dl, d, dr;

            //check ul
            if (x-1 >= 0 && y-1 >= 0) {
                ul = map[(y-1)*w + (x-1)];
            } else { ul = 0;}

            //check u edge
            if (y-1 >= 0) {
                u = map[(y-1)*w + x];
            } else { u = 0;}
        
            //check ur
             if (x+1 < w && y-1 >= 0) {
                 ur = map[(y-1)*w + (x+1)];
            } else { ur = 0; }

		    //check r
            if (x+1 < w) {
                r = map[(y)*w + (x+1)];
            } else { r = 0; }

            //check dr
            if(y+1 < h && x+1 < w) {
                dr = map[(y+1)*w + (x+1)];
            } else { dr = 0; }

            //check d
            if (y+1 < h) {
            d = map[(y+1)*w + x];
            } else { d = 0; }

            //check dl
            if (y+1 < h && x-1 >= 0) {
                dl = map[(y+1)*w + (x-1)];
            } else { dl = 0; }
		
            //check l
            if (x-1 >= 0) {
                l = map[(y)*w + (x-1)];
            } else { l = 0; }
		
		    int num_neighbors = ul + u + ur + l + r + dl + d + dr;
            nb_map[y*w + x] = num_neighbors;
        }
    }

    //print_nb_map(nb_map, w, h);

    //update map based on neighbors
    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            if(map[y*w + x] == 1)
            {
                int n = nb_map[y*w + x];
                if ( n == 2 || n == 3) {
                    map[y*w + x] = 1;
                } else {
                    map[y*w + x] = 0;
                }

            } else if(map[y*w+x] == 0 ) {
                if (nb_map[y*w +x] == 3) {
                    map[y*w + x] = 1;
                }
            }
        } 
    }
}

void draw_world(int* world, int w, int h) {
    for(int y = 0; y < h; y++) {
        for(int x=0; x < w; x++) {
            int val = world[y * w + x];
            if (val == 1) { 
                //printf("* ");
            } else if (val == 0) {
               //printf("_ ");
            }
        }
        //printf("\n");
    }
}

//OpenGL Stuff


void display()
{
    int* map = theWorld;
    int w = W;
    int h = H;
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    /*This is the original random RGB array code
    unsigned int data[H][W][3];
    for( size_t y = 0; y < H; ++y )
    {
        for( size_t x = 0; x < W; ++x )
        {
            data[y][x][0] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][1] = ( rand() % 256 ) * 256 * 256 * 256;
            data[y][x][2] = ( rand() % 256 ) * 256 * 256 * 256;
        }
    }
    */

    unsigned int data[H][W][3];
    for( size_t y = 0; y < H; ++y )
    {
        for( size_t x = 0; x < W; ++x )
        {
            int foo = map[y*W + x];
            //int try = foo * 1191182336;
            int try = foo * 2291492336;
            int florp = ( rand() % 256 ) * 256 * 256 * 256;
            //printf("%d, %d, %d\n",foo, try, florp);
            data[y][x][0] = 0;
            data[y][x][1] = try;
            data[y][x][2] = 0;
            //data[y][x][2] = map[y*W + x] * 256 * 256 * 256 + 200;
        }
    }

    glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_INT, data );

    glutSwapBuffers();

    update_world(map, w, h);

    theWorld = map;
}

// DELP******** cool this works but why does it work? lol
// I have no idea why this is apparently recursive and why incrementing the 16 to something else
// leads to stuttering bullshit.
void timer(int value) {
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

int main( int argc, char **argv )
{
    //Conway Init
    srand(time(NULL));   

    int w = W;
    int h = H;
    int* map = get_map(w, h);

    //enum mode currentMode = preset;
    enum mode currentMode = rando;
    
    if (currentMode == preset) {
        zero_map(map, w, h);
    } else if (currentMode == rando) {
        zero_map(map, w, h);
        random_map(map, w, h);
    }

    theWorld = map;

    //OpenGL Init
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( W, H );
    glutCreateWindow( "lordGod w'ere not evne out of the boat" );
    // DELP******************
    glutTimerFunc(0, timer, 0);
     // DELP******************
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}
