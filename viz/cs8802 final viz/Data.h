//
//  Data.h
//  cs8802 final viz
//
//  Created by Jacob Fugal on 5/7/14.
//  Copyright (c) 2014 Jacob Fugal. All rights reserved.
//

#ifndef cs8802_final_viz_Data_h
#define cs8802_final_viz_Data_h

void realBond(int iteration, double *x, double *y, double *h);
void realGuard(int index, int iteration, double *x, double *y);
void estimatedObstacle(int number, int iteration, double *x, double *y);
void estimatedBond(int iteration, double *x, double *y, double *h);
void estimatedGuard(int number, int iteration, double *x, double *y);

#endif
