/*!
 * @file Joint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2015 by Rocco Martino                            *
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
#include <osgODE/Joint>
#include <osgODE/World>
#include <osgODE/StaticWorld>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Joint::Joint(void):
    m_ensure_two_bodies( false )
{
    _initParams() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint::Joint(const Joint& other, const osg::CopyOp& copyop):
    Transformable(other, copyop),
    m_ensure_two_bodies( other.m_ensure_two_bodies ),
    m_functions( other.m_functions ),
    m_anchor1(other.m_anchor1),
    m_anchor2(other.m_anchor2),
    m_axis1(other.m_axis1),
    m_axis2(other.m_axis2),
    m_axis3(other.m_axis3),
    m_param_map(other.m_param_map)
{
    m_ODE_joint = other.cloneODEJoint(StaticWorld::instance()->getODEWorld()) ;

    dJointSetData(m_ODE_joint, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint::~Joint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::postUpdate(ooReal step_size)
{
    // update local data

    readAxis1Implementation(m_axis1) ;
    readAxis2Implementation(m_axis2) ;
    readAxis3Implementation(m_axis3) ;

    readAnchor1Implementation(m_anchor1) ;
    readAnchor2Implementation(m_anchor2) ;


    this->Transformable::postUpdate( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Joint::addToWorldInternal(World* world)
{
    PS_DBG2("oo::Joint::addToWorldInternal(%p, world=%p)", this, world) ;



    PS_ASSERT1( world != NULL ) ;



    if(world != getWorld()) {


        //
        // clone the joint in the new world
        //
        {
            // Set the world right here because the *MotorJoint classes
            // check it in their finalize method
            setWorldInternal(world) ;

            dJointID    j = cloneODEJoint( world->getODEWorld() ) ;

            dJointSetData(j, this) ;


            dJointDestroy(m_ODE_joint) ;

            m_ODE_joint = j ;
        }



        // call the finalize to give effect to the joint
        finalize() ;



        setJointFeedback(m_feedback.get()) ;



        return Transformable::addToWorldInternal( world ) ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Joint::removeFromWorldInternal(World* world)
{
    PS_DBG2("oo::Joint::removeFromWorldInternal(%p, world=%p)", this, world) ;



    PS_ASSERT1( world != NULL ) ;


    if( world == getWorld()  ||  ! getWorld() ) {


        //
        // clone the joint in the static world
        //
        {
            dJointID    j = cloneODEJoint(StaticWorld::instance()->getODEWorld()) ;


            dJointSetData(j, this) ;

            dJointDestroy(m_ODE_joint) ;

            m_ODE_joint = j ;
        }




        setJointFeedback(m_feedback.get()) ;



        return Transformable::removeFromWorldInternal( world ) ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::finalize(void)
{
    PS_DBG2("oo::Joint::finalize(%p)", this) ;



    dBodyID b1 = NULL ;
    dBodyID b2 = NULL ;



    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;
    }



    if(    ( ! m_ensure_two_bodies )     ||     ( b1 && b2 )    ) {

        dJointAttach(m_ODE_joint, b1, b2) ;


        _restoreParams() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::setBody1(osgODE::RigidBody* body)
{
    PS_DBG2("oo::Joint::setBody1(%p, body=%p)", this, body) ;

    if( m_body1.valid() ) {
        m_body1->removeJointInternal(this) ;
    }

    m_body1 = body ;

    if(body) {
        body->addJointInternal(this) ;
    }

    checkBodiesInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::setBody2(osgODE::RigidBody* body)
{
    PS_DBG2("oo::Joint::setBody2(%p, body=%p)", this, body) ;

    if( m_body2.valid() ) {
        m_body2->removeJointInternal(this) ;
    }

    m_body2 = body ;

    if(body) {
        body->addJointInternal(this) ;
    }

    checkBodiesInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint*
Joint::asJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::_initParams(void)
{
    setAxis1( osg::X_AXIS ) ;
    setAxis2( osg::Y_AXIS ) ;
    setAxis3( osg::Z_AXIS ) ;

    setAnchor1( osg::Vec3(0,0,0) ) ;
    setAnchor2( osg::Vec3(0,0,0) ) ;



#define SET_DEFAULT_PARAMS( X ) \
{ \
    setParam( dParamLoStop##X           , -FLT_MAX ) ; \
    setParam( dParamHiStop##X           ,  FLT_MAX ) ; \
    setParam( dParamVel##X              ,  0.0 ) ; \
    setParam( dParamLoVel##X            ,  FLT_MAX ) ; \
    setParam( dParamHiVel##X            ,  FLT_MAX ) ; \
    setParam( dParamFMax##X             ,  0.0 ) ; \
    setParam( dParamFudgeFactor##X      ,  0.0 ) ; \
    setParam( dParamBounce##X           ,  0.0 ) ; \
    setParam( dParamCFM##X              ,  0.0 ) ; \
    setParam( dParamStopERP##X          ,  1.0 ) ; \
    setParam( dParamStopCFM##X          ,  0.0 ) ; \
    setParam( dParamSuspensionERP##X    ,  1.0 ) ; \
    setParam( dParamSuspensionCFM##X    ,  0.0 ) ; \
    setParam( dParamERP##X              ,  1.0 ) ; \
}


    SET_DEFAULT_PARAMS(1) ;
    SET_DEFAULT_PARAMS(2) ;
    SET_DEFAULT_PARAMS(3) ;
    SET_DEFAULT_PARAMS(4) ;
    SET_DEFAULT_PARAMS(5) ;
    SET_DEFAULT_PARAMS(6) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::_restoreParams(void)
{
    setAxis1( m_axis1 ) ;
    setAxis2( m_axis2 ) ;
    setAxis3( m_axis3 ) ;

    setAnchor1(m_anchor1) ;
    setAnchor2(m_anchor2) ;


    for( ParamMap::iterator itr = m_param_map.begin();
         itr != m_param_map.end();
         itr++ ) {

        setParamImplementation( itr->first, itr->second ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
