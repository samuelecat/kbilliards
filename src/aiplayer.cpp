/***************************************************************************
 *   Copyright (C) 2004 by Samuele Catuzzi                                 *
 *   samuele_catuzzi@yahoo.it                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "aiplayer.h"
#include <iostream>
#include <sstream>


#include <qnamespace.h>
#include <qpen.h>
#include <qcolor.h>

using namespace std;

AIplayer::AIplayer(Kbilliards *kb, QCanvas * canvas):QObject( (QObject *)kb, "aiplayer" )
{
     kbill=kb;
     error_factor=1.4 ; //absolute error parameter to add when calculate trajectory
     show_trajectory=false;

     line1  = new QCanvasLine(canvas);
     line2  = new QCanvasLine(canvas);
     line3  = new QCanvasLine(canvas);
}


AIplayer::~AIplayer()
{
     delete (line1);
     delete (line2);
     delete (line3);
}


/*!
    \fn AIplayer::stroke()
 */
void AIplayer::stroke()
{
     int min=16;
     double nearest, tmp;
     Ball *nextBallseq=0;
     Ball *it;
     QList<Ball> valid_balls,clone ;

     if ( kbill->gamestatus == 0 ) return;

     clone=kbill->ingameballs ;
     clone.remove(kbill->whiteball);

     if ( clone.isEmpty() ) return;

     // determine ball to hit
     switch ( kbill->actualgameoptions.game ) {
          case 1:
          case 2:
               //first choice: select minor ball number
               for(it=clone.first(); it != 0; it=clone.next()) {
                    if ( it->ballnumber < min ){
                         min=it->ballnumber;
                         nextBallseq=it;
                    }
               }
               clone.remove(nextBallseq);
               valid_balls.prepend(nextBallseq);
               //cout<<"nextBallseq="<<nextBallseq->ballnumber<<endl;

               // then select from nearest ball to far one
               while ( ! clone.isEmpty() ) {
                    nearest=1000000; //very big value
                    for(it=clone.first(); it != 0; it=clone.next()) {
                         tmp=(it->vect_Pos - kbill->whiteball->vect_Pos).abs();
                         if ( tmp < nearest ){
                              nearest=tmp;
                              nextBallseq=it;
                         }
                    }
                    clone.remove(nextBallseq);
                    valid_balls.append(nextBallseq);
               }
               break;

          default:
               cout<<"actualgameoptions.game = "<<kbill->actualgameoptions.game<<endl;
               return;
     }

     //determine best directions and power
     long double power1,power2,v1,v2,cos ;
     double power=0;

     samuele::Vector trajectory,to_hole,temp,tr;
     bool found;
     samuele::Vector v_HOLE;
     samuele::Vector reflex ;

     found=false;

     for(it=valid_balls.first(); (it != 0)&&(!found); it=valid_balls.next()) {

          nearest=1000000; //very big value

          for(int i=1;i<7;i++) {
               switch(i){
                    case 1:
                         //try hole 1
                         //cout<<"TL HOLE"<<endl;
                         v_HOLE.set(TL_HOLE,0);
                         break;
                    case 2:
                         //try hole 2
                         //cout<<"TM HOLE"<<endl;
                         v_HOLE.set(TM_HOLE,0);
                         break;
                    case 3:
                         //try hole 3
                         //cout<<"TR HOLE"<<endl;
                         v_HOLE.set(TR_HOLE,0);
                         break;
                    case 4:
                         //try hole 4
                         //cout<<"BL HOLE"<<endl;
                         v_HOLE.set(BL_HOLE,0);
                         break;
                    case 5:
                         //try hole 5
                         //cout<<"BM HOLE"<<endl;
                         v_HOLE.set(BM_HOLE,0);
                         break;
                    case 6:
                         //try hole 6
                         //cout<<"BR HOLE"<<endl;
                         v_HOLE.set(BR_HOLE,0);
                         break;
               };

               temp = v_HOLE - (it->vect_Pos) ;
               if ( isPathFree(it,it,temp) ) {
                    temp = it->vect_Pos - ((temp.unit())*(Ball_Physics::diameter)) ;
                    tr = temp - kbill->whiteball->vect_Pos ;
                    if ( tr.abs() < ((it->vect_Pos - kbill->whiteball->vect_Pos).abs() - (Ball_Physics::diameter)*0.5) )
                         if ( isPathFree(kbill->whiteball,it,tr) && ( temp.abs() < nearest ) ) {
                              to_hole=v_HOLE - (it->vect_Pos);

                              //determine approximate power

                              power1 = sqrtl( 2*(kbill->actualconfig.decel)* tr.abs() )  ;

                              power = power1 ;

                              cos=tr.unit()*to_hole.unit();
                              v2=sqrtl( 2*(kbill->actualconfig.decel)*to_hole.abs() ) ;

                              if ( cos != 0 ) {
                                   power2 = v2/cos ;

                                   power = power1 + power2 ;

                                   if ( power2 != v2 ) {

                                        reflex = tr.unit()*power2 - to_hole.unit()*v2 ;
                                        v1 = reflex.x*reflex.x + reflex.y*reflex.y ;

                                        reflex = reflex.unit()*(v1/(2*kbill->actualconfig.decel));

                                        reflex = reflex*2 ; //approximate

                                        //v1 = v2*(sqrt(1 - cos*cos)/cos) ;
                                        //v1 = sqrt(power2*power2 - v2*v2) ;
                                        //v1 = power2 - v2 ;

                                        //reflex = reflex.unit()*((v1*v1)/(2*kbill->actualconfig.decel));
                                        //reflex = reflex.unit()*((v1/v2)*(to_hole.abs()));
                                   }
                                   else reflex.null() ;

                                   /*
                                   cout<<"kbill->MaxThrowPower="<<(kbill->MaxThrowPower)<<" MaxDist="<<(((kbill->MaxThrowPower)*(kbill->MaxThrowPower))/(2*kbill->actualconfig.decel))<<endl;
                                   cout<<"cos="<<cos<<endl;
                                   cout<<"acos(cos)="<<acos(cos)<<endl;
                                   cout<<"power1="<<power1<<endl;
                                   cout<<"power2="<<power2<<endl;
                                   cout<<"power="<<power<<endl;
                                   cout<<"v1="<<v1<<endl;
                                   cout<<"v2="<<v2<<endl;
                                   cout<<"|reflex|="<<reflex.abs()<<endl;
                                   */

                                   if (( power <= (kbill->MaxThrowPower) )&&( ! goInHole(kbill->whiteball->vect_Pos + tr,reflex) ))
                                        found=true;
                                   //else
                                   //     cout<<"too power or white go in hole, hole("<<v_HOLE.x<<","<<v_HOLE.y<<") "<<endl;

                                   //cout<<endl;

                                   trajectory=tr;
                                   nextBallseq=it;
                              }
                         }
               }

          } //end for(int i=1;i<7;i++)

     } //end for ..valid_balls..

     double curr_error;
     samuele::Vector err_vect,L;

     curr_error = error_factor*rand()/(RAND_MAX) ; // 0 <= curr_error <= error_val
     //cout<<"curr_error="<<curr_error<<" (curr_error - error_factor/2)="<<(curr_error - error_factor/2)<<endl;

     // if no solutions found
     if ( ! found ) {
          //cout<<"AI: no solutions found :-("<<endl ;
          trajectory = valid_balls.first()->vect_Pos - kbill->whiteball->vect_Pos ;

          // add an error
          err_vect.set((trajectory.unit()).y,-((trajectory.unit()).x),0.0);
          err_vect = err_vect*(curr_error - error_factor/2);// -error_val/2 <= curr_error <= +error_val/2
          trajectory= trajectory + err_vect;

          power = kbill->MaxThrowPower ;
          trajectory = trajectory.unit() * power ;
     } else {

          //power=ceil(power);

          if ( power > kbill->MaxThrowPower ) {
               power = kbill->MaxThrowPower;
               //cout<<"too power"<<endl;
          }

          //******DEBUG draw trajectory on the carpet

          drawLine(1,kbill->whiteball->vect_Pos, kbill->whiteball->vect_Pos + trajectory, QColor(255,0,0) );

          drawLine(2,nextBallseq->vect_Pos, nextBallseq->vect_Pos + to_hole, QColor(255,0,0) );

          drawLine(3,kbill->whiteball->vect_Pos + trajectory, kbill->whiteball->vect_Pos + trajectory + reflex, QColor(0,0,255) );

          //******end DEBUG*******


          // add an error

          err_vect.set((trajectory.unit()).y,-((trajectory.unit()).x),0.0);
          err_vect = err_vect*(curr_error - error_factor/2);// -error_val/2 <= curr_error <= +error_val/2
          trajectory= trajectory + err_vect;

          trajectory = trajectory.unit() * power ;
     }

     kbill->Music->OneShot("shot");

     kbill->whiteball->setVelocity(trajectory.x,trajectory.y);

}


/*!
    \fn AIplayer::isPathFree()
 */
bool AIplayer::isPathFree(Ball *from,Ball *exclude,samuele::Vector v)
{

     Ball *k;
     QList<Ball> clone ;
     double diameter;
     samuele::Vector a,j;
     bool ret_value=true;

     a=from->vect_Pos ;
     diameter= Ball_Physics::diameter ;

     clone=kbill->ingameballs;

     //cout<<"-isPathFree-"<<endl;
     for(k=clone.first(); k != 0; k=clone.next()) {
          if (( k != exclude)&&(k->ballnumber != 0)) {

               j=k->vect_Pos;

               if ( ( (fabs(v.y*j.x - v.x*j.y + (v.x*a.y - v.y*a.x))/v.abs() < diameter) &&
                       (0 < (v.x*j.x + v.y*j.y - v.x*a.x - v.y*a.y)) &&
                       ( (v.x*j.x + v.y*j.y - v.x*a.x - v.y*a.y) < (v.x*v.x + v.y*v.y)) ) ||
                       ( (a - j).abs() < diameter ) ||
                       ( (a + v - j).abs() < diameter ) ) {

                    //cout<<" si scontra con la "<<k->ballnumber<<endl;
                    ret_value=false;
               }
          }
     }

     //cout<<endl;

     return ret_value;

}



/*!
    \fn AIplayer::goInHole(samuele::Vector tr)
 */
bool AIplayer::goInHole(samuele::Vector P, samuele::Vector tr)
{
     bool ret_value=false;
     samuele::Vector v_HOLE;
     double hole_diameter ;

     hole_diameter = (Ball_Physics::diameter)*2.0 ;


     if ( ( tr.x == 0 ) && ( tr.y == 0 ) ) return false;

     for(int i=1;i<7;i++) {
          switch(i){
               case 1:
                    v_HOLE.set(TL_HOLE,0);
                    break;
               case 2:
                    v_HOLE.set(TM_HOLE,0);
                    break;
               case 3:
                    v_HOLE.set(TR_HOLE,0);
                    break;
               case 4:
                    v_HOLE.set(BL_HOLE,0);
                    break;
               case 5:
                    v_HOLE.set(BM_HOLE,0);
                    break;
               case 6:
                    v_HOLE.set(BR_HOLE,0);
                    break;
          };

          if ( ( ( fabs(tr.y*v_HOLE.x - tr.x*v_HOLE.y + (tr.x*P.y - tr.y*P.x))/tr.abs() < hole_diameter ) &&
               ( 0 < (tr.x*v_HOLE.x + tr.y*v_HOLE.y - tr.x*P.x - tr.y*P.y) ) &&
               ( (tr.x*v_HOLE.x + tr.y*v_HOLE.y - tr.x*P.x - tr.y*P.y) < (tr.x*tr.x + tr.y*tr.y) ) ) ||
               ( (P - v_HOLE).abs() < hole_diameter ) ||
               ( (P + tr - v_HOLE).abs() < hole_diameter ) ) {

               //cout<<" va in buca "<<endl;
               return true;
          }
     }

     return ret_value;

}

void AIplayer::drawLine(int id,samuele::Vector From,samuele::Vector To, QColor color) {

     QCanvasLine *line;
     int x1,y1,x2,y2 ;

     if ( ! show_trajectory ) {
          hideTrajectories() ;
          return ;
     }

     x1=(int)round(From.x);
     y1=(int)round(From.y);
     x2=(int)round(To.x);
     y2=(int)round(To.y);
     
     switch(id) {
          case 2: line=line2; break;
          case 3: line=line3; break;
          default: line=line1; break;
     }
     
     if ( ( x2 > 0 )&&( y2 > 0 ) ) {
          line->setPen( QPen( color, 1, Qt::DotLine ) );
          line->setPoints(x1,y1,x2,y2);
          line->setZ(1);
          line->show();
     } else {
          line->hide();
     }
}



/*!
    \fn AIplayer::slotShowTrajectory(bool flag)
 */
void AIplayer::slotShowTrajectory(bool flag)
{
     show_trajectory = flag;
     if ( !show_trajectory )
           hideTrajectories() ;
}


/*!
    \fn AIplayer::hideTrajectories()
 */
void AIplayer::hideTrajectories()
{
     line1->hide();
     line2->hide();
     line3->hide();
}

#include "aiplayer.moc"

