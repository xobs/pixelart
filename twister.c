#include <stdint.h>
#include <math.h>

#define WIDTH 1280
#define HEIGHT 720
#define MOD_MAX 10
#define MOD_MIN -10


float u( float x ) { return (x>0.0)?1.0:0.0; }

void get_next_pixel(uint32_t x, uint32_t y, uint32_t frame,
                    uint8_t *r, uint8_t *g, uint8_t *b)
{
    float time = 1000.0/(frame * 30);
    float mov0 = x+y+cosf(sinf(time)*2.)*100.+sinf(x/100.)*1000.;
    float mov1 = y / HEIGHT / 0.2 + time;
    float mov2 = x / WIDTH / 0.2;
    float c1 = fabs(sinf(mov1+time)/2.+mov2/2.-mov1-mov2+time);
    float c2 = fabs(sinf(c1+sinf(mov0/1000.+time)+sinf(y/40.+time)+sinf((x+y)/100.)*3.));
    float c3 = fabs(sinf(c2+cosf(mov1+mov2+c2)+cosf(mov2)+sinf(x/1000.)));
    *r = c1*255;
    *g = c2*255;
    *b = c3*255;

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

