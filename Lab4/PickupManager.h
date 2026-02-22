#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "PickupObject.h"
#include <iostream>
#include <random>

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


        static void Update(Ogre::Real delta_time, const Uint8* state);


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
};

