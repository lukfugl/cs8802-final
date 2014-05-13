//
//  VizRenderer.m
//  cs8802 final viz
//
//  Created by Jacob Fugal on 5/7/14.
//  Copyright (c) 2014 Jacob Fugal. All rights reserved.
//

#import "VizRenderer.h"
#include "Data.h"

#include <OpenGL/gl.h>
#include <math.h>

@implementation VizRenderer

-(void) render: (int) iteration
{
    glPushMatrix();
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (-200, 200, -150, 150, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    [self renderRealStatic];
    [self renderRealDynamic: iteration];
    [self renderEstimatedStatic: iteration];
    [self renderEstimatedDynamic: iteration];

    glFlush();
    glPopMatrix();
}

-(void) renderRealStatic
{
    glColor4f(0.5f, 1.0f, 0.5f, 0.5f);
    renderBoxZone(-92, -82,   45,  55);
    renderBoxZone( 92,  82,   45,  55);
    renderBoxZone( -5,   5, -105, -95);

    glColor4f(0.5f, 0.5f, 1.0f, 0.5f);
    renderBoxZone( -5, 5, -5, 5);

    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    renderObstacle(-60,   70);
    renderObstacle( 50,   60);
    renderObstacle(  0,   40);
    renderObstacle(-40,    0);
    renderObstacle( 40,    0);
    renderObstacle(100,  -20);
    renderObstacle( 90,  -30);
    renderObstacle(  0,  -50);
    renderObstacle( 60,  -70);
    renderObstacle(-40, -100);
}

-(void) renderRealDynamic: (int) iteration
{
    double x, y, h;
    realBond(iteration, &x, &y, &h);
    
    glColor4f(1.0f, 0.85f, 0.35f, 1.0f);
    renderBond(x, y, h);

    for (int i = 0; i < 4; i++) {
        realGuard(i, iteration, &x, &y);
        renderGuard(x, y, 1.0, 0.35, 0.35, 1.0);
    }
}

-(void) renderEstimatedStatic: (int) iteration
{
    glColor4f(0.8f, 0.8f, 0.8f, 0.6f);
    
    double x, y;
    for (int i = 0; i < 10; i++) {
        estimatedObstacle(i, iteration, &x, &y);
        renderObstacle(x, y);
    }
}

-(void) renderEstimatedDynamic: (int) iteration
{
    double x, y, h;
    estimatedBond(iteration, &x, &y, &h);
    
    glColor4f(0.85f, 0.35f, 1.0f, 0.7f);
    renderBond(x, y, h);
    
    for (int i = 0; i < 4; i++) {
        estimatedGuard(i, iteration, &x, &y);
        renderGuard(x, y, 0.35, 0.85, 1.0, 0.7);
    }
}

static void renderBoxZone(double minx, double maxx, double miny, double maxy)
{
    glBegin(GL_POLYGON);
    glVertex2f(minx, miny);
    glVertex2f(minx, maxy);
    glVertex2f(maxx, maxy);
    glVertex2f(maxx, miny);
    glEnd();
}

static void renderObstacle(double x, double y)
{
    renderCircle(x, y, 10);
}

static void renderBond(double x, double y, double heading)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(heading, 0, 0, 1);
    glBegin(GL_TRIANGLES);
    {
        glVertex2f(-2, -1);
        glVertex2f(-2, 1);
        glVertex2f( 3, 0);
    }
    glEnd();
    glPopMatrix();
}

static void renderGuard(double x, double y, double r, double g, double b, double a) {
    glColor4f(r, g, b, a * 0.6);
    renderGuardSight(x, y);
    
    glColor4f(r, g, b, a);
    renderGuardBody(x, y);
}

static void renderGuardSight(double x, double y)
{
    renderCircle(x, y, 15);
}

static void renderGuardBody(double x, double y)
{
    renderCircle(x, y, 2);
}

static void renderCircle(double x, double y, double radius)
{
    int sides = 72;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; i++) {
        double t = 2 * M_PI * i / sides;
        glVertex2f(cos(t) * radius, sin(t) * radius);
    }
    glEnd();
    glPopMatrix();
}

@end
