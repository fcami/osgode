/*!
 * @file ODEObject.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2014 by Rocco Martino                            *
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
#include <osgODE/ODEObject>
#include <osgODE/World>
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
unsigned int    ODEObject::s_last_id = 0 ;
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ODEObject::ODEObject(void):
    m_ID        ( 0 ),
    m_actor     ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ODEObject::ODEObject(const ODEObject& other, const osg::CopyOp& copyop):
    osg::Object             ( other, copyop ),
    m_update_callback       ( other.m_update_callback ),
    m_post_update_callback  ( other.m_post_update_callback ),
    m_ID                    ( 0 ),
    m_user_object           ( other.m_user_object.get() ),
    m_actor_bound           ( other.m_actor_bound ),
    m_actor                 ( other.m_actor )
{
    if( copyop.getCopyFlags() & osg::CopyOp::DEEP_COPY_CALLBACKS ) {
        if( other.m_update_callback.valid() ) {
            m_update_callback = osg::clone( other.m_update_callback.get(), copyop ) ;
        }

        if( other.m_post_update_callback.valid() ) {
            m_post_update_callback = osg::clone( other.m_post_update_callback.get(), copyop ) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ODEObject::~ODEObject(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::setActor( bool actor )
{
    if( m_actor != actor ) {
        m_actor = actor ;

        if( m_world.valid() ) {
            m_world->setDirtyActors(true) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
osgODE::ODEObject::setWorldInternal(World* world)
{
    m_world = world ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::setIDInternal(unsigned int id)
{
    m_ID = id ;
    s_last_id = osg::maximum(s_last_id, id) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
ODEObject::generateID(void)
{
    m_ID = ++s_last_id ;

    return m_ID ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::update(ooReal step_size)
{
    (void) step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::postUpdate(ooReal step_size)
{
    (void) step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
ODEObject::addToWorldInternal(World* world)
{
    PS_ASSERT1( world != NULL ) ;

    if( m_actor ) {
        world->setDirtyActors( true ) ;
    }


    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
ODEObject::removeFromWorldInternal(World* world)
{
    PS_ASSERT1( world != NULL ) ;

    if( m_actor ) {
        world->setDirtyActors( true ) ;
    }

    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::accept(osg::NodeVisitor& nv)
{
    (void) nv ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const osg::BoundingSphere&
ODEObject::getBound(void) const
{
    return m_bounding_sphere ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::callUpdateCallbackInternal(void)
{
    if( m_update_callback.valid() ) {
        (*m_update_callback)(this) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::callPostUpdateCallbackInternal(void)
{
    if( m_post_update_callback.valid() ) {
        (*m_post_update_callback)(this) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ODEObject::updateTransformInternal(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#define AS_NULL( NAME ) NAME* ODEObject::as##NAME(void) { return NULL ; }

AS_NULL( BypassJoint ) ;
AS_NULL( CharacterBase) ;
AS_NULL( Collidable) ;
AS_NULL( Container) ;
AS_NULL( FixedJoint) ;
AS_NULL( HingeJoint) ;
AS_NULL( Joint ) ;
AS_NULL( RigidBody ) ;
AS_NULL( SliderJoint ) ;
AS_NULL( Space ) ;
AS_NULL( Transformable ) ;
AS_NULL( UniversalJoint ) ;
AS_NULL( World ) ;

#undef AS_NULL
/* ....................................................................... */
/* ======================================================================= */
