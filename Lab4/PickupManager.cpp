#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
SceneManager* PickupManager::scene_manager_;
SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects;

PickupManager::PickupManager(){ }

PickupManager::~PickupManager(){ }

bool PickupManager::initialize(SceneManager* sceneManager, SceneNode* playerNode)
{
    if (msSingleton == NULL)
    {
        msSingleton = OGRE_NEW PickupManager;
        if (false == msSingleton->_initialize(sceneManager, playerNode))
        {
            OGRE_DELETE msSingleton;
            msSingleton = NULL;
            return false;
        }
    }

    return true;
}

void PickupManager::destroy()
{
    if (msSingleton != NULL)
    {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = NULL;
    }
}

PickupManager& PickupManager::getSingleton()
{
	assert(msSingleton);
	return (*msSingleton);
}

PickupManager* PickupManager::getSingletonPtr()
{
	return msSingleton;
}

/** This function spawns a new pickup object at a random position near the player */
void PickupManager::addPickupObject(const char* mesh_file_name)
{
    random_device rd;                           // obtain a random number from hardware
    mt19937 gen(rd());                          // seed the generator
    uniform_real_distribution<> distr(-50, 50); // define the range

    // Create a new pickup Object in a random position near the player
    Ogre::Vector3 playerPosition = player_node_->getPosition();
    Real randomXOffset = distr(gen); 
    Real randomZOffset = distr(gen); 
    Ogre::Vector3 newSpawnPosition = Ogre::Vector3(playerPosition.x + randomXOffset, -3.0f, playerPosition.z + randomZOffset);
    PickupObject* pickupObject = new PickupObject(scene_manager_, mesh_file_name, newSpawnPosition, Vector3(1, 1, 1));
    pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();


    // Set the color of the object
    String MaterialName = pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    MaterialPtr pMaterial = MaterialManager::getSingleton().getByName(MaterialName);
    
    float r = 255.0f / 255;
    float g = 215.0f / 255;
    float b = 0.0f / 255;

    ColourValue* color = new ColourValue(r, g, b, 1.0F);
    
    pMaterial->setDiffuse(*color);
    pMaterial->setAmbient(*color);
    pMaterial->setSpecular(*color);


    // Insert the new Pickup Object in the list of managed objects
    pickup_objects.push_back(pickupObject);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        bool erased = false;
        IPickupObject* pickupObject = *i;

        pickupObject->update(delta_time);

        // TODO: Check for collision with a game object that has not yet been picked up
        // Basically write the if clause.
			const Ogre::Vector3 playerPos = player_node_ != nullptr ? player_node_->getPosition() : Ogre::Vector3::ZERO;
			const Ogre::Vector3 pickupPos = pickupObject->getSceneNode() != nullptr ? pickupObject->getSceneNode()->getPosition() : Ogre::Vector3::ZERO;
			const Ogre::Real pickupRadius = 5.0f;

			if (playerPos.distance(pickupPos) <= pickupRadius)
			{
				// BONUS
				// TODO: Make the scene node of the cube a child of the player's scene node, and center it on the player
				if (pickupObject->getSceneNode() != nullptr && player_node_ != nullptr)
				{
					scene_manager_->getRootSceneNode()->removeChild(pickupObject->getSceneNode());
					player_node_->addChild(pickupObject->getSceneNode());
					pickupObject->getSceneNode()->setPosition(Ogre::Vector3::ZERO);
				}

				pickupObject->runPickupEffect();
			}

        if (pickupObject->isPickedUp())
        {
            // If the effect is finished we can dispose of the object
            if (pickupObject->getPickupEffect()->isFinished())
            {
                // TODO: If the effect is finished then delete the object, this involves 2 parts.
				
				// 1) Destroy the scene node
                // hint: https://ogrecave.github.io/ogre/api/1.12/class_ogre_1_1_scene_manager.html#aea3103164ed0f27baeb67a3ae2fe429b
                
				// 2) Erase from PickupManager
				if (pickupObject->getSceneNode() != nullptr && scene_manager_ != nullptr)
				{
					scene_manager_->destroySceneNode(pickupObject->getSceneNode());
				}

				OGRE_DELETE pickupObject;
				i = pickup_objects.erase(i);
				erased = true;
				addPickupObject("MyOgreCube.mesh");
            }
        }

        // Don't increase the counter if we have deleted an item, otherwise it throws an error
        if (!erased) ++i;
    }
}


bool PickupManager::_initialize(SceneManager* sceneManager, SceneNode* playerNode)
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here initialization stuff if needed
    scene_manager_ = sceneManager;
    player_node_ = playerNode;

    return true;
}

void PickupManager::_destroy()
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here destruction stuff if needed
    OGRE_DELETE scene_manager_;
    scene_manager_ = NULL;
    OGRE_DELETE player_node_;
    player_node_ = NULL;
    OGRE_DELETE &pickup_objects;
}
