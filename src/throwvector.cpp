/***************************************************************************
                          throwvector.cpp  -  description
                             -------------------
    begin                : Thu Jan 17 2002
    copyright            : (C) 2002 by Samuele Catuzzi
    email                : samuele_catuzzi@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "throwvector.h"
#include <math.h>
#include <iostream>

const double ThrowVector::Pi=3.14159265358979323846;

ThrowVector::ThrowVector(QCanvas * canvas, Ball * whiteb):QCanvasLine(canvas){

 setPen( QPen( QColor(red), 1, DashLine )  );
 length = 310; //length of cue trajectory
 setZ(7);
 alpha= 0 ;
 resolution=50;
 angle=0;
 count=0;
 speed=0;
 xstrt_pos=0;
 ystrt_pos=0;
 anim_speed=3;
 white_ball=whiteb;
 setAnimated(true );
}

ThrowVector::~ThrowVector(){
}
/** No descriptions */
void ThrowVector::setdirection(int beta){
 int xfinal_pos,yfinal_pos;
 xstrt_pos=(int)rint(white_ball->xcenter());
 ystrt_pos=(int)rint(white_ball->ycenter());

 angle=beta;
 alpha=(Pi/resolution)*angle;

 xfinal_pos=(int)( rint(length*cos(alpha)) ) + xstrt_pos ;
 yfinal_pos=(int)( rint(length*sin(alpha)) ) + ystrt_pos ;

 setPoints(xstrt_pos,ystrt_pos,xfinal_pos,yfinal_pos);
}
/** No descriptions */
void ThrowVector::setResolution(int newres){
 if (resolution > 0)
    {
     resolution=newres;
     angle=0;
     alpha=0;
    }
}
/** No descriptions */
int ThrowVector::getResolution(){
 return resolution;
}
/** No descriptions */
double ThrowVector::getalpha(){

 angle=angle%(resolution*2);

 return alpha;
}

void ThrowVector::advance(int phase){

 xstrt_pos=(int)rint(white_ball->xcenter());
 ystrt_pos=(int)rint(white_ball->ycenter());

 if ( ( xstrt_pos != 0) && ( ystrt_pos != 0 ) && ( phase == 1 ) && visible() )
    {
     int xfinal_pos,yfinal_pos;
     int xst_pos,yst_pos;

     speed=(speed + 1)%anim_speed;// animation speed
     if (speed == 0)
         count=(count + 1)%8; //to shift a dash line

     len=length - count;

     xst_pos= (int)(rint( xstrt_pos + count*cos(alpha) ));
     yst_pos= (int)(rint( ystrt_pos + count*sin(alpha) ));

     // ***billiard edges limit:
     // 40<= xfinal_pos <=703
     // 37<= yfinal_pos <=358
     // ..minus 5 pixel of distance from border

     xfinal_pos=(int)(rint( length*cos(alpha) + xstrt_pos  )) ;
     yfinal_pos=(int)(rint( length*sin(alpha) + ystrt_pos  )) ;

     if ( ( xst_pos >= 45 ) && ( xst_pos <= 698 ) && ( yst_pos >= 42 ) && ( yst_pos <= 353 ) ) {

          if ( xfinal_pos > 698  ) {
               if ( cos(alpha) != 0.0 )
                    len= (int)(rint( (698 - xst_pos)/cos(alpha) )) ;
               else
                    len= (int)(rint( (698 - xst_pos)/0.00001 )) ;
               xfinal_pos=(int)(rint( (len*cos(alpha) + xst_pos)  )) ;
               yfinal_pos=(int)(rint( (len*sin(alpha) + yst_pos)  )) ;
          } else if ( xfinal_pos < 45 ) {
               if ( cos(alpha) != 0.0 )
                    len= (int)(rint( (45 - xst_pos )/cos(alpha) )) ;
               else
                    len= (int)(rint( (45 - xst_pos )/0.00001 )) ;
               xfinal_pos=(int)(rint( (len*cos(alpha) + xst_pos)  )) ;
               yfinal_pos=(int)(rint( (len*sin(alpha) + yst_pos)  )) ;
          }

          if ( yfinal_pos > 353 ) {
               if ( sin(alpha) != 0.0 )
                    len= (int)(rint( (353 - yst_pos)/sin(alpha) )) ;
               else
                    len= (int)(rint( (353 - yst_pos)/0.00001 )) ;
               xfinal_pos=(int)(rint( (len*cos(alpha) + xst_pos)  )) ;
               yfinal_pos=(int)(rint( (len*sin(alpha) + yst_pos)  )) ;
          } else if ( yfinal_pos < 42 ) {
               if ( sin(alpha) != 0.0 )
                    len= (int)(rint( (42 - yst_pos )/sin(alpha) )) ;
               else
                    len= (int)(rint( (42 - yst_pos )/0.00001 )) ;
               xfinal_pos=(int)(rint( (len*cos(alpha) + xst_pos)  )) ;
               yfinal_pos=(int)(rint( (len*sin(alpha) + yst_pos)  )) ;
          }

     } else {
          if ( (( xfinal_pos < 45  )&&( xfinal_pos < xst_pos)) ||
               (( xfinal_pos > 698 )&&( xfinal_pos > xst_pos)) ||
               (( yfinal_pos < 42  )&&( yfinal_pos < yst_pos)) ||
               (( yfinal_pos > 353 )&&( yfinal_pos > yst_pos)) ) {
               // length = 20 pixel ;
               xfinal_pos=(int)(rint( 20*cos(alpha) + xstrt_pos  )) ;
               yfinal_pos=(int)(rint( 20*sin(alpha) + ystrt_pos  )) ;
          }
     }

     setPoints( xst_pos,yst_pos,xfinal_pos,yfinal_pos );
    }

}


/** set correct animation speed

setfromCanvasPeriod( int msec)
msec depend of value chosed when setAdvancePeriod(int msec)
on the widget parent is called
 */
void ThrowVector::setfromCanvasPeriod(int msec){
     anim_speed= (int)rint(90/msec);
}
