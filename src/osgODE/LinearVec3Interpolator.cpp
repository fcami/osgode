/*!
 * @file LinearVec3Interpolator.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/LinearVec3Interpolator>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
LinearVec3Interpolator::LinearVec3Interpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LinearVec3Interpolator::LinearVec3Interpolator(const LinearVec3Interpolator& other, const osg::CopyOp& copyop):
    Vec3Interpolator(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LinearVec3Interpolator::~LinearVec3Interpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LinearVec3Interpolator::interpolateImplementation(ooReal x, osg::Vec3& result)
{
    unsigned int            size = m_points.size() ;


    switch( size ) {
        /*
         * No points...
         */
        case 0 :
            result = osg::Vec3() ;
            return ;
            break ;


        /* one point */
        case 1:
            result = m_points[0].second ;
            return ;
            break ;


        default:
            break ;
    }





    unsigned int    x0 = findX0(x) ;

    /* calculate f(x) */

    const Point&    p1 = m_points[x0] ;
    const Point&    p2 = m_points[x0+1] ;


    /*
     * f(x) = y1   +   (x - x1)(y2 - y1)
     *                 -----------------
     *                     (x2 - x1)
     */
    result = p1.second + ( (p2.second - p1.second) * (x - p1.first)  / (p2.first - p1.first) ) ;
}
/* ....................................................................... */
/* ======================================================================= */
