//#include "Application.h"
//
//#ifdef __linux
//#include <X11/Xlib.h>
//#endif // __linux
//
//int main()
//{
//#ifdef __linux
//	XInitThreads();
//#endif //__linux
//	Application app;
//	app.run();
//
//	return 0;
//}

#include <SFML/Graphics.hpp>

#include "chipmunk/chipmunk.h"
int main(void) {
	// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
	cpVect gravity = cpv(0, -100);

	// Create an empty space.
	cpSpace* space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);

	// Add a static line segment shape for the ground.
	// We'll make it slightly tilted so the ball will roll off.
	// We attach it to a static body to tell Chipmunk it shouldn't be movable.
	cpShape* ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 5), cpv(20, -5), 0);
	cpShapeSetFriction(ground, 1);
	cpSpaceAddShape(space, ground);

	// Now let's make a ball that falls onto the line and rolls off.
	// First we need to make a cpBody to hold the physical properties of the object.
	// These include the mass, position, velocity, angle, etc. of the object.
	// Then we attach collision shapes to the cpBody to give it a size and shape.

	cpFloat radius = 5;
	cpFloat mass = 1;

	// The moment of inertia is like mass for rotation
	// Use the cpMomentFor*() functions to help you approximate it.
	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

	// The cpSpaceAdd*() functions return the thing that you are adding.
	// It's convenient to create and add an object in one line.
	cpBody* ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPosition(ballBody, cpv(0, 15));

	// Now we create the collision shape for the ball.
	// You can create multiple collision shapes that point to the same body.
	// They will all be attached to the body and move around to follow it.
	cpShape* ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
	cpShapeSetFriction(ballShape, 0.7);

	// Now that it's all set up, we simulate all the objects in the space by
	// stepping forward through time in small increments called steps.
	// It is *highly* recommended to use a fixed size time step.
	sf::CircleShape ball;
	ball.setRadius(5);
	ball.setFillColor(sf::Color::Red);
	sf::RenderWindow window(sf::VideoMode(800, 600), "Chipmunk");
	sf::Event event;
	cpFloat timeStep = 1.0 / 60.0;
	for (cpFloat time = 0; time < 2; time += timeStep) {
		window.clear(sf::Color::White);
		cpVect pos = cpBodyGetPosition(ballBody);
		cpVect vel = cpBodyGetVelocity(ballBody);
		ball.setPosition(sf::Vector2f(pos.x, pos.y));
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		cpSpaceStep(space, timeStep);
		window.draw(ball);
		window.display();
	}
}