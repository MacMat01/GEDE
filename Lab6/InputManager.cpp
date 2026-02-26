#include "pch.h"
#include "InputManager.h"

InputManager::InputManager()
{
    joystick_ = nullptr;
    player_axis_x_ = 0;
    player_axis_y_ = 0;
    camera_x_ = 0;
    camera_y_ = 0;

    // Initialize the joystick subsystem
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    // Check for joystick
    if (SDL_NumJoysticks() > 0) {
        // Open joystick
        joystick_ = SDL_JoystickOpen(0);

        if (joystick_) {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickNameForIndex(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick_));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick_));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick_));
        }
        else {
            printf("Couldn't open Joystick 0\n");
        }
    }
}

InputManager::~InputManager()
{
    // Close if opened
    if (SDL_JoystickGetAttached(joystick_)) {
        SDL_JoystickClose(joystick_);
    }
}

Ogre::Vector2 InputManager::getCameraMovement() const
{
    Ogre::Real x = Ogre::Math::Abs(camera_x_) > JOYSTICK_DEAD_ZONE ? camera_x_ / 32767.0 : 0;
    Ogre::Real y = Ogre::Math::Abs(camera_y_) > JOYSTICK_DEAD_ZONE ? camera_y_ / 32767.0 : 0;

    return Ogre::Vector2(x, y);
}

Ogre::Vector2 InputManager::getCharacterMovement() const
{
    Ogre::Real x = Ogre::Math::Abs(player_axis_x_) > JOYSTICK_DEAD_ZONE ? player_axis_x_ / 32767.0 : 0.0;
    Ogre::Real y = Ogre::Math::Abs(player_axis_y_) > JOYSTICK_DEAD_ZONE ? player_axis_y_ / 32767.0 : 0.0;

    return Ogre::Vector2(x, y);
}

void InputManager::update()
{
    player_axis_x_ = SDL_JoystickGetAxis(joystick_, 0);
    player_axis_y_ = SDL_JoystickGetAxis(joystick_, 1);
    camera_x_ = SDL_JoystickGetAxis(joystick_, 2);
    camera_y_ = SDL_JoystickGetAxis(joystick_, 3);

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_JOYAXISMOTION)
        {
            if (event.jaxis.which == 0)
            {
                if (event.jaxis.axis == 0)
                {
                    const auto value = event.jaxis.value;
                    player_axis_x_ = value > player_axis_x_ ? value : player_axis_x_;
                }
                if (event.jaxis.axis == 1)
                {
                    const auto value = event.jaxis.value;
                    player_axis_y_ = value > player_axis_y_ ? value : player_axis_y_;
                }
                if (event.jaxis.axis == 2)
                {
                    const auto value = event.jaxis.value;
                    camera_x_ = value > camera_x_ ? value : camera_x_;
                }
                if (event.jaxis.axis == 3)
                {
                    const auto value = event.jaxis.value;
                    camera_y_ = value > camera_y_ ? value : camera_y_;
                }
            }
        }
    }
}
