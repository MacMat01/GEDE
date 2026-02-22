#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
SceneManager* PickupManager::scene_manager_;
SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects;

bool PickupManager::create_pickup_tower = false;
int PickupManager::tower_current_layers = 0;
int PickupManager::tower_max_layers = 5;
int PickupManager::tower_width = 5;
int PickupManager::tower_length = 5;
float PickupManager::tower_current_height = 0.0f;
float PickupManager::tower_spawn_timer = 0.0f;
float PickupManager::tower_spawn_period = 1.0f;
double PickupManager::tower_instantiation_milliseconds = 0.0;

PickupManager::PickupManager() {}

PickupManager::~PickupManager() {}

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
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> distr(-50, 50);

    // Create a new pickup Object in a random position near the player
    Ogre::Vector3 playerPosition = player_node_->getPosition();
    Real randomXOffset = distr(gen);
    Real randomZOffset = distr(gen);
    Ogre::Vector3 newSpawnPosition = Ogre::Vector3(playerPosition.x + randomXOffset, -3.0f, playerPosition.z + randomZOffset);
    PickupObject* pickupObject = new PickupObject(scene_manager_, mesh_file_name, newSpawnPosition, Vector3(1, 1, 1));

    // Set the color of the object
    String MaterialName = pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    MaterialPtr pMaterial = MaterialManager::getSingleton().getByName(MaterialName);

    ColourValue color(1.0f, 215.0f / 255.0f, 0.0f, 1.0f);

    pMaterial->setDiffuse(color);
    pMaterial->setAmbient(color);
    pMaterial->setSpecular(color);

    // Insert the new Pickup Object in the list of managed objects
    pickup_objects.push_back(pickupObject);
}

void PickupManager::addPickupObject(const char* mesh_file_name, Vector3 position)
{
    const auto start = std::chrono::high_resolution_clock::now();

    // instantiation WITH memory allocation testing feature
    PickupObject* pickupObject = new PickupObject(2, scene_manager_, mesh_file_name, position, Vector3(1, 1, 1));

    const auto end = std::chrono::high_resolution_clock::now();
    const double duration = std::chrono::duration<double, std::milli>(end - start).count();
    tower_instantiation_milliseconds += duration;

    pickup_objects.push_back(pickupObject);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    if(create_pickup_tower && (tower_current_layers < tower_max_layers))
    {
        tower_spawn_timer += delta_time;
        if (tower_spawn_timer >= tower_spawn_period)
        {
            _create_pickup_tower_layer();
            tower_spawn_timer = 0.0f;
        }
    }
    else if (create_pickup_tower && (tower_current_layers >= tower_max_layers))
    {
        cout << "Time to instantiate pickup objects in the tower: " << tower_instantiation_milliseconds << " (ms)" << endl;
        _destroy_pickup_tower();
        _initialize_tower_parameters();
        createPickupTower();
    }

    // Update all the managed pickup objects, and delete them if they finished the effect
    const Ogre::Vector3 playerPos = player_node_ != nullptr ? player_node_->getPosition() : Ogre::Vector3::ZERO;
    const Ogre::Real pickupRadiusSq = 5.0f * 5.0f;

    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        bool erased = false;
        IPickupObject* pickupObject = *i;

        pickupObject->update(delta_time);

        const Ogre::Vector3 pickupPos = pickupObject->getSceneNode() != nullptr ? pickupObject->getSceneNode()->getPosition() : Ogre::Vector3::ZERO;

        if (playerPos.squaredDistance(pickupPos) <= pickupRadiusSq)
        {
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

    _initialize_tower_parameters();

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
    OGRE_DELETE& pickup_objects;
}

void PickupManager::createPickupTower()
{
    create_pickup_tower = true;
}

void PickupManager::_initialize_tower_parameters()
{
    create_pickup_tower = false;
    tower_spawn_period = 1.0f;
    tower_spawn_timer = 0.0f;
    tower_width = 5;
    tower_length = 5;
    tower_current_height = 0.0f;
    tower_max_layers = 5;
    tower_current_layers = 0;
    tower_instantiation_milliseconds = 0;
}

void PickupManager::_create_pickup_tower_layer()
{
    Vector3 towerSpawnPosition = Vector3(10.0f, tower_current_height, 10.0f);
    float offset = 2.0f;

    for (int i = 0; i < tower_width; i++)
    {
        for (int j = 0; j < tower_length; j++)
        {
            Vector3 brickSpawnPosition = Vector3(towerSpawnPosition.x + (i * offset), towerSpawnPosition.y, towerSpawnPosition.z + (j * offset));
            addPickupObject("MyOgreCube.mesh", brickSpawnPosition);
        }
    }

    tower_current_height += offset;
    tower_current_layers++;
}

void PickupManager::_destroy_pickup_tower()
{
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        IPickupObject* pickupObject = *i;
        pickupObject->getSceneNode()->detachAllObjects();
        scene_manager_->destroyEntity(pickupObject->getEntity());
        scene_manager_->destroySceneNode(pickupObject->getSceneNode());

        delete pickupObject;
        pickupObject = nullptr;

        i = pickup_objects.erase(i);
    }
}
