#include "draw.h"
#include "graphics3d.h"
#include <math.h>


void draw_circle_2D(Vec2D point,
                   float radius,
                   Uint32  segments,
                   Vec3D color,
                   float alpha)
{
  Vec3D pos,pos2;
  GraphicsView view;
  
  graphics_get_view(&view);
  
  opengl_get_gl_coordinate(
    point.x,
    point.y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos.x,
    &pos.y,
    &pos.z
  );
  opengl_get_gl_coordinate(
    point.x + radius,
    point.y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos2.x,
    &pos2.y,
    &pos2.z
  );
  
  draw_circle_3D(pos,
                     fabs(pos2.x - pos.x),
                     segments,
                     color,
                     alpha);
}

void draw_circle_3D(Vec3D point,
                        float radius,
                        Uint32  segments,
                        Vec3D color,
                        float alpha)
{
  Vec3D p1,p2;
  float step;
  int i;
  if (segments < 3)return;
  step = 2 * M_PI / segments;
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  glPushMatrix();

  glTranslatef(point.x,point.y,point.z);
  
  glBegin( GL_LINES );
  
  for (i = 0; i < segments;i++)
  {
    vec3d_set_angle_by_radians(&p1,i * step);
    vec3d_set_angle_by_radians(&p2,(i + 1) * step);
    p1.x *= radius;
    p1.y *= radius;
    p2.x *= radius;
    p2.y *= radius;
    glVertex3f(p1.x,p1.y,0);
    glVertex3f(p2.x,p2.y,0);
  }
  
  glEnd( );
  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  
}

void draw_dot_3D(Vec3D point,
                     float radius,
                     Vec3D color,
                     float alpha)
{
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  glPushMatrix();
  glEnable(GL_POINT_SMOOTH);
  glPointSize(radius);
  glBegin( GL_POINTS );
  glVertex3f(point.x,point.y,point.z);
  glEnd( );
  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
}

void draw_line_2D(Vec2D p1,
                      Vec2D p2,
                      float radius,
                      Vec3D color,
                      float alpha)
{
  Vec3D pos,pos2;
  GraphicsView view;
    
  graphics_get_view(&view);
  
  opengl_get_gl_coordinate(
    p1.x,
    p1.y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos.x,
    &pos.y,
    &pos.z
  );
  
  opengl_get_gl_coordinate(
    p2.x,
    p2.y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &pos2.x,
    &pos2.y,
    &pos2.z
  );
  
  draw_line_3D(pos,
                   pos2,
                   radius,
                   color,
                   alpha);
}

void draw_line_3D(Vec3D p1,
                      Vec3D p2,
                      float radius,
                      Vec3D color,
                      float alpha)
{
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  glPushMatrix();
  glDepthFunc(GL_LEQUAL);
  
  glLineWidth(1 + radius);

  glBegin( GL_LINES );
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glEnd( );

  glLineWidth(1);

  glDepthFunc(GL_LESS);

  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
}

void draw_tri_3D(Vec3D p1,
                     Vec3D p2,
                     Vec3D p3,
                     Vec3D color,
                     float alpha)
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  glPushMatrix();
  glBegin( GL_TRIANGLES );
  glVertex3f(p1.x,p1.y,p1.z);
  glVertex3f(p2.x,p2.y,p2.z);
  glVertex3f(p3.x,p3.y,p3.z);
  glEnd( );
  
  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}


void draw_rect_3D_solid(RectFloat rect, Uint32 radius, Orientation ori)
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(ori.color.x,ori.color.y,ori.color.z,ori.alpha);
  glPushMatrix();
  glLineWidth(1 + radius);
  glTranslatef(ori.position.x,ori.position.y,ori.position.z);
  glRotatef(ori.rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(ori.rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(ori.rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(ori.scale.x,ori.scale.y,ori.scale.z);
  
  
  glBegin( GL_QUADS );
  
  glVertex3f(rect.x,rect.y,0);
  glVertex3f(rect.x + rect.w,rect.y,0);
  
  glVertex3f(rect.x + rect.w,rect.y,0);
  glVertex3f(rect.x + rect.w,rect.y + rect.h,0);
  
  glVertex3f(rect.x + rect.w,rect.y + rect.h,0);
  glVertex3f(rect.x,rect.y + rect.h,0);
  
  glVertex3f(rect.x,rect.y + rect.h,0);
  glVertex3f(rect.x,rect.y,0);
  glEnd( );
  
  glPopMatrix();
  glLineWidth(1);
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}

void draw_rect_3D(RectFloat rect, Uint32 radius, Orientation ori)
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(ori.color.x,ori.color.y,ori.color.z,ori.alpha);
  glPushMatrix();
  glLineWidth(1 + radius);
  glTranslatef(ori.position.x,ori.position.y,ori.position.z);
  glRotatef(ori.rotation.x, 1.0f, 0.0f, 0.0f);
  glRotatef(ori.rotation.y, 0.0f, 1.0f, 0.0f);
  glRotatef(ori.rotation.z, 0.0f, 0.0f, 1.0f);
  glScalef(ori.scale.x,ori.scale.y,ori.scale.z);
  
  
  glBegin( GL_LINES );
  glDepthFunc(GL_LEQUAL);
  
  glVertex3f(rect.x,rect.y,0);
  glVertex3f(rect.x + rect.w,rect.y,0);
  
  glVertex3f(rect.x + rect.w,rect.y,0);
  glVertex3f(rect.x + rect.w,rect.y + rect.h,0);

  glVertex3f(rect.x + rect.w,rect.y + rect.h,0);
  glVertex3f(rect.x,rect.y + rect.h,0);

  glVertex3f(rect.x,rect.y + rect.h,0);
  glVertex3f(rect.x,rect.y,0);
  glEnd( );
  
  glPopMatrix();
  glLineWidth(1);
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}

void draw_get_2d_position(Vec2D *out,Vec3D in)
{
  GraphicsView view;
  int x,y;
  if (!out)return;
  graphics_get_view(&view);
  opengl_get_screen_coordinate(
    in.x,
    in.y,
    in.z,
    view.modelView,
    view.projection,
    view.viewPort,
    &x,
    &y
  );
  out->x = x;
  out->y = y;
}

void draw_get_3d_position(Vec3D *out,Vec2D in)
{
  GraphicsView view;
  if (!out)return;
  
  graphics_get_view(&view);
  opengl_get_gl_coordinate(
    in.x,
    in.y,
    0.99f,
    view.modelView,
    view.projection,
    view.viewPort,
    &out->x,
    &out->y,
    &out->z
  );
}

void draw_rect(RectFloat rect,Vec3D color,float alpha)
{
    Vec3D pos,pos2;
  GraphicsView view;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  
  graphics_get_view(&view);
  opengl_get_gl_coordinate(
      rect.x,
      rect.y,
      0.99f,
      view.modelView,
      view.projection,
      view.viewPort,
      &pos.x,
      &pos.y,
      &pos.z
  );

  opengl_get_gl_coordinate(
      rect.x + rect.w,
      rect.y + rect.h,
      0.99f,
      view.modelView,
      view.projection,
      view.viewPort,
      &pos2.x,
      &pos2.y,
      &pos2.z
  );

  glPushMatrix();
  
  glBegin( GL_LINES );
  glVertex3f(pos.x,pos.y,pos.z);
  glVertex3f(pos2.x,pos.y,pos.z);
  
  glVertex3f(pos2.x,pos.y,pos.z);
  glVertex3f(pos2.x,pos2.y,pos.z);
  
  glVertex3f(pos2.x,pos2.y,pos.z);
  glVertex3f(pos.x,pos2.y,pos.z);

  glVertex3f(pos.x,pos2.y,pos.z);
  glVertex3f(pos.x,pos.y,pos.z);
  glEnd( );

  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);

}

void draw_solid_rect(RectFloat rect,Vec3D color,float alpha)
{
  Vec3D pos,pos2;
  GraphicsView view;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glColor4f(color.x,color.y,color.z,alpha);
  
  graphics_get_view(&view);
  opengl_get_gl_coordinate(
      rect.x,
      rect.y,
      0.99f,
      view.modelView,
      view.projection,
      view.viewPort,
      &pos.x,
      &pos.y,
      &pos.z
  );

  opengl_get_gl_coordinate(
      rect.x + rect.w,
      rect.y + rect.h,
      0.99f,
      view.modelView,
      view.projection,
      view.viewPort,
      &pos2.x,
      &pos2.y,
      &pos2.z
  );

  glPushMatrix();
  
  glBegin( GL_QUADS );
  glVertex3f(pos.x,pos.y,pos.z);
  glVertex3f(pos2.x,pos.y,pos.z);
  glVertex3f(pos2.x,pos2.y,pos.z);
  glVertex3f(pos.x,pos2.y,pos.z);
  glEnd( );

  glPopMatrix();
  glColor4f(1,1,1,1);
  glDisable(GL_BLEND);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}
