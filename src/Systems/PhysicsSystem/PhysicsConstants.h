//#pragma once
//
//#include "btbulletCollisionCommon.h"
//#include "btbulletDynamicsCommon.h"
//
///**
// * Define which group of objects an object can collide with
// **/
//enum CollisionLayers {
//    COL_NOTHING = 0,           //!< Collide with nothing
//    COL_DEFAULT = 1<<0,        //!< Collide with everything
//    COL_RAYCAST = 1<<1,        //!< Able to be hit by a raycast
//    COL_VISION_AFFECTED = 1<<2 //!< Can collide with the frustum in frustum culling
//};
//
///**
// * Make it easy to choose which collision layer(s) a game object goes into
// **/
//enum ObjectType {
//	NOTHING                     = COL_NOTHING,												//!< Don't collide with anything ever
//	WORLD                       = COL_DEFAULT|COL_RAYCAST,									//!< Collide with just about everything
//	VISION_AFFECTED_COLLISION   = COL_DEFAULT|COL_RAYCAST|COL_VISION_AFFECTED,				//!< Have vision affected objects collide with everything
//	VISION_AFFECTED_NOCOLLISION = COL_RAYCAST|COL_VISION_AFFECTED,							//!< Have vision affected objecst not collide with anything
//	PLAYER                      = COL_DEFAULT,												//!< Collide with everything in the default layer
//	FRUSTUM                     = COL_VISION_AFFECTED,										//!< Only collide with vision affected objects
//    ALL                         = COL_NOTHING|COL_DEFAULT|COL_RAYCAST|COL_VISION_AFFECTED	//!< Collide with everything ever
//};
//
///**
// * Have custom callbacks on detecting overlaps
// **/
//struct CustomFilterCallback : public btOverlapFilterCallback
//{
//	/**
//	 * @return true when pairs need collision
//	 **/
//	virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const
//	{
//		return (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
//	}
//};

//virtual btDynamicsWorld* getWorld();

//btDynamicsWorld* world;                    //!< The world the physics simulation exists in
	//btDispatcher* dispatcher;                  //!< Event handling
	//btCollisionConfiguration* collisionConfig; //!< Collision Handling
	//btBroadphaseInterface* broadphase;         //!< Handles the Broad Phase of Collision Detections, aka determine pairs of objects that could possibly be colliding
	//btConstraintSolver* solver;                //!< Solves Constraints
	//btGhostPairCallback* ghostPairCallback;	   //!< Needs to be separate so it can be deallocated.
	//btOverlapFilterCallback* filterCallback;   //!< Needs to be separate so it can be deallocated.
	
	//float pStepSize;
	//float pAccumulator;

	//collisionConfig		= new btDefaultCollisionConfiguration();
 //   dispatcher			= new btCollisionDispatcher(collisionConfig);
 //   broadphase			= new btDbvtBroadphase();
 //   solver				= new btSequentialImpulseConstraintSolver();
 //   world				= new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfig);
	//ghostPairCallback	= new btGhostPairCallback();
	//filterCallback		= new CustomFilterCallback(); //Set up custom collision filter

	////Set up custom collision filter
	//world->getPairCache()->setOverlapFilterCallback(filterCallback);

	//broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback);
	//world->setGravity(btVector3(0.0f,-9.81f,0.0f));

	//pStepSize = 1.0f / 60.0f; //Update at 60 fps
	//pAccumulator = 0.0f;

	/*delete world;
    delete solver;
    delete collisionConfig;
    delete dispatcher;
	delete ghostPairCallback;
    delete filterCallback;
    delete broadphase;*/

	/*pAccumulator += dt;

	 if(pAccumulator >= pStepSize)
	 {
		 world->stepSimulation(pStepSize);
		 pAccumulator-= pStepSize;
	 }
*/