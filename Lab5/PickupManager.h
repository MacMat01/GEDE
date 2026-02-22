#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "PickupObject.h"
#include <iostream>
#include <random>
#include <chrono>

using namespace std;
using namespace Ogre;
using namespace OgreBites;

class PickupManager : public Singleton<PickupManager>
{
    public:
        PickupManager(PickupManager const&) = delete;
        void operator=(PickupManager const&) = delete;
        /**
        Initialize the Pickup Manager.
        Return true upon success.
        */
        static bool initialize(SceneManager* sceneManager, SceneNode* playerNode);

        /**
        Destroy the Pickup Manager.
        */
        static void destroy();


        /** Override standard Singleton retrieval. */
        static PickupManager& getSingleton();


        /// @copydoc Singleton::getSingleton()
        static PickupManager* getSingletonPtr();

        static void addPickupObject(const char* mesh_file_name);
        static void addPickupObject(const char* mesh_file_name, Vector3 position);


        static void Update(Ogre::Real delta_time, const Uint8* state);

        static void createPickupTower();


    protected:
        static SceneManager* scene_manager_;
        static SceneNode* player_node_;
        static std::list<IPickupObject*> pickup_objects;

        /** Class default constructor */
        PickupManager();

        /** Class destructor */
        ~PickupManager();

        /** Initialize the pickup manager instance. */
        bool _initialize(SceneManager* sceneManager, SceneNode* playerNode);

        /** Destory the pickup manager instance. */
        void _destroy();

        // Tower members
        static bool create_pickup_tower;
        static int tower_current_layers;
        static int tower_max_layers;
        static int tower_width;
        static int tower_length;
        static float tower_current_height;
        static float tower_spawn_timer;
        static float tower_spawn_period;
        static double tower_instantiation_milliseconds;

        static void _create_pickup_tower_layer();
        static void _destroy_pickup_tower();
        static void _initialize_tower_parameters();
};

