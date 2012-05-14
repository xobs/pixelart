#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 1280
#define HEIGHT 720
#define MOD_MAX 10
#define MOD_MIN -10

static uint32_t sinlut[314];
static uint32_t coslut[314];
static int initted;
static int max_c1;
static int max_c2;
static int max_c3;

static void init() {
    int i;
    for (i=0; i<314; i++) {
        sinlut[i] = 32768+sinf(i/50.0)*32768;
        coslut[i] = 32768+cosf(i/50.0)*32768;
    }
    initted = 1;
}
//float u( float x ) { return (x>0.0)?1.0:0.0; }
static uint32_t getsin(uint32_t v) { return sinlut[v%314]; }
static uint32_t getcos(uint32_t v) { return coslut[v%314]; }

void get_next_pixel(uint32_t x, uint32_t y, uint32_t frame,
                    uint8_t *r, uint8_t *g, uint8_t *b)
{
    if (!initted)
        init();

    /*
       float time = 1000.0/(frame * 30);
       float mov0 = x+y+cosf(sinf(time)*2.)*100.+sinf(x/100.)*1000.;
       float mov1 = y / HEIGHT / 0.2 + time;
       float mov2 = x / WIDTH / 0.2;
       float c1 = fabs(sinf(mov1+time)/2.+mov2/2.-mov1-mov2+time);
       float c2 = fabs(sinf(c1+sinf(mov0/1000.+time)+sinf(y/40.+time)+sinf((x+y)/100.)*3.));
       float c3 = fabs(sinf(c2+cosf(mov1+mov2+c2)+cosf(mov2)+sinf(x/1000.)));
       *r = 0;//c1*255;
       *g = c2*255;
       *b = 0;//c3*255;
       */
    uint32_t time = frame;
    uint32_t mov0 = x+y+getcos(getsin(time)*2);//x+y+cosf(sinf(time)*2.)*100.+sinf(x/100.)*1000.;
    uint32_t mov1 = y+time;// / HEIGHT / 0.2 + time;
    uint32_t mov2 = x;// / WIDTH / 0.2;
    uint32_t c1 = abs(getsin(mov1+time*8)+mov2-mov1-mov2*512+time*2)/16;
    uint32_t c2 =
        (getsin(c1/16+getsin(mov0*32+time)/4096+getsin(y*32+time)/4096+x*16+y/64+getsin((x*512+y*512)/32)/512));
    uint32_t c3 = getsin(c2+getcos(mov1+mov2+c2)+getcos(mov2)+getsin(x/1000.));

    if (c1 > max_c1) {
        max_c1 = c1;
//        printf("max_c1: %d\n", max_c1);
    }
    if (c2 > max_c2) {
        max_c2 = c2;
//        printf("max_c2: %d\n", max_c2);
    }
    if (c3 > max_c3) {
        max_c3 = c3;
        printf("max_c3: %d\n", max_c3);
    }

    c1 >>= 8;
    c2 >>= 8;
    c3 >>= 8;
    *r = c1;
    *g = c2;
    *b = c3;

    /*
    float time = 1000.0/(frame * 30);
    float px = (2.0*x-WIDTH)/HEIGHT;
    //float py = (2.0*y-HEIGHT)/HEIGHT;

    float a = atanf(px);//, py);
    float ru = 200;//length(p)*.75;

    float w = cosf(3.1415927*time-ru*2.0);
    float h = 0.5+0.5*cosf(12.0*a-w*7.0+ru*8.0);
    float d = 0.25+0.75*powf(h,1.0*ru)*(0.7+0.3*w);

    float col = u( d-ru ) * sqrt(1.0-ru/d)*ru*2.5;
    col *= 1.25+0.25*cosf((12.0*a-w*7.0+ru*8.0)/2.0);
    col *= 1.0 - 0.35*(0.5+0.5*sinf(ru*30.0))*(0.5+0.5*cosf(12.0*a-w*7.0+ru*8.0));

    *r = col*255,
    *g = (col-h*0.5+ru*.2 + 0.35*h*(1.0-ru))*255;
    *b = (col-h*ru + 0.1*h*(1.0-ru))*255;
    */
    return;
}

