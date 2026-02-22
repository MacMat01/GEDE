#pragma once
class IPickupEffect
{
public:
    virtual ~IPickupEffect() {}

	virtual void run() = 0;	                             // Starts the motion along the trajectory
	virtual void update(float delta_time) = 0;           // Steps the motion along the trajectory
	virtual bool isRunning() = 0;                        // True if motion is ongoing
	virtual bool isFinished() = 0;                       // False if motion has run its course
};

