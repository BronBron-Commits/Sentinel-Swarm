#include "swarm_render.hpp"
#include "swarm_neighbors.hpp"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdio>

enum class Formation { LINE, WEDGE, ORBIT };
static Formation g_formation = Formation::LINE;

void swarm_render_set_formation(int f){if(f==1)g_formation=Formation::LINE;else if(f==2)g_formation=Formation::WEDGE;else if(f==3)g_formation=Formation::ORBIT;}

static SDL_Window* window=nullptr;static SDL_GLContext gl_ctx=nullptr;

static constexpr float WORLD_MIN_X=-2.f,WORLD_MAX_X=12.f,WORLD_MIN_Y=-6.f,WORLD_MAX_Y=6.f;
static constexpr float NEIGHBOR_RADIUS=3.5f;static constexpr int CIRCLE_SEGMENTS=48;static constexpr int MAX_NEIGHBORS=6;

static float nx(float x){return (x-WORLD_MIN_X)/(WORLD_MAX_X-WORLD_MIN_X)*2.f-1.f;}
static float ny(float y){return (y-WORLD_MIN_Y)/(WORLD_MAX_Y-WORLD_MIN_Y)*2.f-1.f;}

static void center_of_mass(const SwarmState&s,float&cx,float&cy){cx=cy=0;for(auto&a:s.agents){cx+=a.x;cy+=a.y;}cx/=s.agents.size();cy/=s.agents.size();}

static void formation_slot(const SwarmState&s,int i,float&sx,float&sy){float cx,cy;center_of_mass(s,cx,cy);int n=s.agents.size();
 if(g_formation==Formation::LINE){float sp=1.2f;float st=cx-sp*(n-1)*0.5f;sx=st+i*sp;sy=cy;}
 else if(g_formation==Formation::WEDGE){float sp=1.0f;sx=cx+i*sp;sy=cy+(i%2?i:-i)*0.3f;}
 else{float r=2.5f;float t=2.f*3.1415926f*i/n;sx=cx+cosf(t)*r;sy=cy+sinf(t)*r;}}

static void draw_circle(float cx,float cy,float r){glBegin(GL_LINE_LOOP);for(int i=0;i<CIRCLE_SEGMENTS;i++){float t=2.f*3.1415926f*i/CIRCLE_SEGMENTS;glVertex2f(nx(cx+cosf(t)*r),ny(cy+sinf(t)*r));}glEnd();}

static void color_from_density(int n){float t=fminf(n/float(MAX_NEIGHBORS),1.f);glColor3f(t,0.6f*(1.f-t),1.f-t);}

static void compute_convergence(const SwarmState&s,float&avg,float&mx){avg=0;mx=0;for(int i=0;i<s.agents.size();i++){float fx,fy;formation_slot(s,i,fx,fy);auto&a=s.agents[i];float dx=a.x-fx,dy=a.y-fy;float d=sqrtf(dx*dx+dy*dy);avg+=d;if(d>mx)mx=d;}avg/=s.agents.size();}

static void draw_convergence_overlay(float avg,float mx){float norm=fminf(avg/3.f,1.f);float w=0.6f*(1.f-norm);float y=0.9f;
 glBegin(GL_QUADS);glColor3f(0.8f*(1.f-norm),0.8f*norm,0.2f);
 glVertex2f(-0.95f,y);glVertex2f(-0.95f+w,y);glVertex2f(-0.95f+w,y-0.05f);glVertex2f(-0.95f,y-0.05f);glEnd();}

bool swarm_render_init(int w,int h){if(SDL_Init(SDL_INIT_VIDEO)!=0)return false;SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);window=SDL_CreateWindow("Sentinel Swarm",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,SDL_WINDOW_OPENGL);if(!window)return false;gl_ctx=SDL_GL_CreateContext(window);if(!gl_ctx)return false;glClearColor(.05f,.05f,.08f,1);glPointSize(8);glLineWidth(1.2f);return true;}

void swarm_render_draw(const SwarmState&s){glClear(GL_COLOR_BUFFER_BIT);float avg,mx;compute_convergence(s,avg,mx);
 draw_convergence_overlay(avg,mx);

 glBegin(GL_LINE_STRIP);glColor3f(.4f,.4f,.4f);for(int i=0;i<s.agents.size();i++){float fx,fy;formation_slot(s,i,fx,fy);glVertex2f(nx(fx),ny(fy));}glEnd();

 glBegin(GL_LINES);glColor3f(1.f,.3f,.3f);for(int i=0;i<s.agents.size();i++){float fx,fy;formation_slot(s,i,fx,fy);auto&a=s.agents[i];glVertex2f(nx(a.x),ny(a.y));glVertex2f(nx(fx),ny(fy));}glEnd();

 glColor3f(.25f,.35f,.45f);for(auto&a:s.agents)draw_circle(a.x,a.y,NEIGHBOR_RADIUS);

 glBegin(GL_LINES);glColor3f(.2f,.5f,.7f);for(uint32_t i=0;i<s.agents.size();i++){auto nb=find_neighbors(s,i,NEIGHBOR_RADIUS);auto&a=s.agents[i];for(uint32_t j:nb.indices){if(j<=i)continue;auto&b=s.agents[j];glVertex2f(nx(a.x),ny(a.y));glVertex2f(nx(b.x),ny(b.y));}}glEnd();

 glBegin(GL_POINTS);for(uint32_t i=0;i<s.agents.size();i++){auto nb=find_neighbors(s,i,NEIGHBOR_RADIUS);color_from_density(nb.indices.size());auto&a=s.agents[i];glVertex2f(nx(a.x),ny(a.y));}glEnd();SDL_GL_SwapWindow(window);}

void swarm_render_shutdown(){if(gl_ctx)SDL_GL_DeleteContext(gl_ctx);if(window)SDL_DestroyWindow(window);SDL_Quit();}
