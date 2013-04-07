#include <osgODE/Manager>
#include <osgODE/Space>
#include <osgODE/TriMesh>
#include <osgODE/Sphere>
#include <osgODE/DynamicParticleSystem>
#include <osgODE/DynamicParticleGeode>
#include <osgODE/Notify>

#include <osgODEUtil/CreateTriMeshFromNode>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>



#include <osgParticle/Particle>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#if 0
#include <pViewer/NodeMasks>
#endif




#ifndef OSGODE_DATA_PATH
#define OSGODE_DATA_PATH "../../data/export"
#endif




int
main(int argc, char** argv)
{
    osgDB::Registry::instance()->getDataFilePathList().push_back( OSGODE_DATA_PATH ) ;




    osg::ref_ptr<osg::Group>    root = new osg::Group() ;




    /*
     * [1] Create the manager and the space
     */
    osgODE::Manager*    manager = new osgODE::Manager() ;
    {
        root->addChild( manager ) ;

        manager->setup(false, true, 1.0/100.0) ;

        manager->setWorld( new osgODE::Space() ) ;
    }




    /*
     * [2] Create a floor
     */
    {
        osg::Node*  plane = osgDB::readNodeFile( "floor.osgb" ) ;

        PS_ASSERT1( plane != NULL ) ;



        // build the motionless trimesh
        osgODE::TriMesh*    trim = osgODEUtil::createTriMeshFromNode(plane) ;

        trim->setKinematic(true) ;

        trim->setPosition( osg::Z_AXIS * -0.5 ) ;


        // add the trimesh to the space
        manager->getWorld()->addObject(trim) ;

        // add the graphic node to the simulation
        trim->getMatrixTransform()->addChild(plane) ;


#if 0
        plane->setNodeMask( pViewer::NodeMasks::ALBEDO |
                            pViewer::NodeMasks::MATERIAL |
                            pViewer::NodeMasks::NORMAL
        ) ;
#endif
    }




    /*
     * [3] Create the particle effect
     */
    {
        // create the particle system
        osgODE::DynamicParticleSystem   *ps = new  osgODE::DynamicParticleSystem() ;

        const char* texture = argc > 1 ? argv[1] : "" ;

        ps->setDefaultAttributes(texture, true, false) ;

        // set the world to use
        ps->setWorld( manager->getWorld() ) ;


        //
        // Set the particle body template
        //
        osgODE::Collidable* body_template = new osgODE::Sphere() ;

        body_template->setSize( osg::Vec3(0.2, 0.2, 0.2) ) ;

        // tell the particles to not collide each other
        body_template->setCategoryBits(1) ;
        body_template->setCollideBits(~1) ;

        body_template->setMaxAngularSpeed( 0.0 ) ;
        body_template->setGyroscopicMode( false ) ;

        ps->setBodyTemplate( body_template ) ;



        // cretate the emitter
        osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter() ;

        emitter->setParticleSystem(ps) ;



        osgParticle::RandomRateCounter *rrc =
            static_cast<osgParticle::RandomRateCounter *>( emitter->getCounter() ) ;

        rrc->setRateRange(20, 30);


        // the updater
        osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater() ;
        psu->addParticleSystem( ps ) ;



        // the particle template
        osgParticle::Particle   particle_template ;
        particle_template.setLifeTime(3) ;

        ps->setDefaultParticleTemplate( particle_template ) ;



        // The DynamicParticleGeode communicates the world-to-local
        // matrix to the particle systems. This is required because the
        // DynamicParticleSystem needs to transform bodies properties
        // from world frame to drawable local frame

        osg::Geode *geode = new osgODE::DynamicParticleGeode() ;
        geode->addDrawable(ps) ;



        root->addChild( geode ) ;
        root->addChild( emitter ) ;
        root->addChild( psu ) ;


#if 0
        geode->setNodeMask( pViewer::NodeMasks::TRANSPARENCY ) ;
#endif
    }


#if 0
    root->getOrCreateStateSet()->getOrCreateUniform("uMaterial", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(1.0, 0.8, 0.5, 1.0) ) ;
    root->getOrCreateStateSet()->getOrCreateUniform("uColor", osg::Uniform::FLOAT_VEC4)->set( osg::Vec4(0.8, 0.8, 0.8, 1.0) ) ;
#endif



    /*
     * [4] write out the graph
     */
    osgDB::writeNodeFile(*root, "output.osgb") ;




    // bye
    return 0 ;
}