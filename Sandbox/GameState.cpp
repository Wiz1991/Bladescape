#include "GameState.h"
#include "States.h"
#include "CommandIDs.h"
#include "ResourceIDs.h"
#include <xyginext/util/Vector.hpp>
#include "ShapeUtils.hpp"
#include "MessageIDs.h"
#include "xyginext/graphics/SpriteSheet.hpp"

//components
#include <xyginext/ecs/components/Camera.hpp>
#include <xyginext/ecs/components/Drawable.hpp>
#include <xyginext/ecs/components/Sprite.hpp>
#include <xyginext/ecs/components/Transform.hpp>
#include <xyginext/ecs/components/CommandTarget.hpp>
#include <xyginext/ecs/components/BroadPhaseComponent.hpp>

//systems
#include <xyginext/ecs/systems/RenderSystem.hpp>
#include <xyginext/ecs/systems/SpriteSystem.hpp>
#include <xyginext/ecs/systems/CommandSystem.hpp>
#include "BallSystem.h"
#include <xyginext/ecs/systems/DynamicTreeSystem.hpp>
#include "CollisionSystem.h"

//SFML
#include <SFML/Window/Event.hpp>

//REST

GameState::GameState(xy::StateStack& stack, xy::State::Context context)
	: xy::State(stack, context)
	, mGameScene(context.appInstance.getMessageBus())
{
	loadResources();
	initialiseScene();

	mGameScene.getActiveCamera().getComponent<xy::Camera>().setView(context.defaultView.getSize());
	mGameScene.getActiveCamera().getComponent<xy::Camera>().setViewport(context.defaultView.getViewport());

	context.appInstance.setMouseCursorVisible(false);
	context.appInstance.getRenderWindow()->setMouseCursorGrabbed(true);
}

bool GameState::handleEvent(const sf::Event& evt)
{
	if (evt.type == sf::Event::MouseMoved) {
		auto mouseWorldPos = xy::App::getRenderWindow()->mapPixelToCoords({ evt.mouseMove.x,evt.mouseMove.y });
		xy::Command cmd;
		cmd.targetFlags = CommandID::Paddle;
		cmd.action = [mouseWorldPos](xy::Entity entity, float) {
			float posX = mouseWorldPos.x;
			auto spriteWidth = entity.getComponent<xy::Sprite>().getTextureBounds().width / 2;
			posX = std::max(spriteWidth, mouseWorldPos.x);
			posX = std::min(posX, xy::DefaultSceneSize.x - spriteWidth);

			auto& tx = entity.getComponent<xy::Transform>();
			auto currentPos = tx.getPosition();
			currentPos.x = posX;
			tx.setPosition(currentPos);
		};
		mGameScene.getSystem<xy::CommandSystem>().sendCommand(cmd);
	}
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Escape)
		requestStackPush(States::PAUSE);

	if (evt.type == sf::Event::MouseButtonReleased && evt.mouseButton.button == sf::Mouse::Button::Left) {
		xy::Command cmd;
		cmd.targetFlags = CommandID::Paddle;
		cmd.action = [&](xy::Entity entity, float)
		{
			auto& paddle = entity.getComponent<Paddle>();
			if (paddle.ball.isValid())
			{
				paddle.ball.getComponent<Ball>().state = Ball::State::Active;
				paddle.ball.getComponent<Collider>().dynamic = true;
				auto ballBounds = paddle.ball.getComponent<xy::Sprite>().getTextureBounds();
				paddle.ball.getComponent<xy::Transform>().setPosition(entity.getComponent<xy::Transform>().getPosition() + sf::Vector2f(0.f, -ballBounds.height / 2.f));
				entity.getComponent<xy::Transform>().removeChild(paddle.ball.getComponent<xy::Transform>());
				paddle.ball = {};
			}
		};
		mGameScene.getSystem<xy::CommandSystem>().sendCommand(cmd);
	}

	mGameScene.forwardEvent(evt);
	return true;
}

void GameState::handleMessage(const xy::Message& msg)
{
	if (msg.id == MessageID::BallMessage) {
		const auto& data = msg.getData<BallEvent>();
		if (data.action == BallEvent::Despawned)
			spawnBall();
	}
	mGameScene.forwardMessage(msg);
}

bool GameState::update(float dT)
{
	mGameScene.update(dT);
	return true;
}

void GameState::draw()
{
	auto window = getContext().appInstance.getRenderWindow();

	window->draw(mGameScene);
}

void GameState::initialiseScene()
{
	auto& messageBus = getContext().appInstance.getMessageBus();
	mGameScene.addSystem<BallSystem>(messageBus);
	mGameScene.addSystem<xy::DynamicTreeSystem>(messageBus);
	mGameScene.addSystem<CollisionSystem>(messageBus);
	mGameScene.addSystem<xy::CommandSystem>(messageBus);
	mGameScene.addSystem<xy::SpriteSystem>(messageBus);
	mGameScene.addSystem<xy::RenderSystem>(messageBus);

	auto entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(xy::DefaultSceneSize.x / 2.f, xy::DefaultSceneSize.y - 40.f);
	entity.addComponent<xy::Sprite>()= SpriteID::sprites[SpriteID::Paddle];
	entity.addComponent<xy::Drawable>();
	entity.addComponent<xy::CommandTarget>().ID = CommandID::Paddle;
	entity.addComponent<Paddle>();

	auto paddleBounds = entity.getComponent<xy::Sprite>().getTextureBounds();
	entity.getComponent<xy::Transform>().setOrigin(paddleBounds.width / 2.f, paddleBounds.height / 2.f);
	entity.addComponent<xy::BroadphaseComponent>(paddleBounds);
	entity.addComponent<Collider>();

	spawnBall();

	sf::FloatRect wallBounds = { 0.f, 0.f, xy::DefaultSceneSize.x, 20.f };
	entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>();
	entity.addComponent<xy::BroadphaseComponent>(wallBounds);
	entity.addComponent<Collider>();
	Shape::setRectangle(entity.addComponent<xy::Drawable>(), { wallBounds.width, wallBounds.height });

	wallBounds.width = 20.f;
	wallBounds.height = xy::DefaultSceneSize.y;
	entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(0.f, 20.f);
	entity.addComponent<xy::BroadphaseComponent>(wallBounds);
	entity.addComponent<Collider>();
	Shape::setRectangle(entity.addComponent<xy::Drawable>(), { wallBounds.width, wallBounds.height });

	entity = mGameScene.createEntity();
	entity.addComponent<xy::Transform>().setPosition(xy::DefaultSceneSize.x - wallBounds.width, 20.f);
	entity.addComponent<xy::BroadphaseComponent>(wallBounds);
	entity.addComponent<Collider>();
	Shape::setRectangle(entity.addComponent<xy::Drawable>(), { wallBounds.width, wallBounds.height });



	static const sf::FloatRect BlockSize = SpriteID::sprites[SpriteID::Block].getTextureBounds();
	auto addBlock = [&](sf::Vector2f position, sf::Color color) {
		auto entity = mGameScene.createEntity();
		entity.addComponent<xy::Transform>();
		entity.getComponent<xy::Transform>().setPosition(position);
		entity.addComponent<xy::Sprite>() = SpriteID::sprites[SpriteID::Block];
		entity.addComponent<xy::BroadphaseComponent>(BlockSize);
		entity.getComponent<xy::Sprite>().setColour(color);
		entity.addComponent<xy::CommandTarget>().ID = CommandID::Block;
		entity.addComponent<xy::Drawable>();
		entity.addComponent <Collider>().callback = [&](xy::Entity ent, xy::Entity other, Manifold manifold) {
			if (other.hasComponent<Ball>())
			auto* msg = getContext().appInstance.getMessageBus().post<BlockEvent>(BlockEvent::Destroyed);
			mGameScene.destroyEntity(ent);
		
		};
	};
	static const std::array<sf::Color, 6u> colours =
	{
		sf::Color::Red, sf::Color::Magenta, sf::Color::Yellow,
		sf::Color::Green, sf::Color::Cyan, sf::Color::Blue
	};


	static const std::size_t BlockXCount = 12;
	static const std::size_t BlockYCount = 10;

	sf::Vector2f offset = xy::DefaultSceneSize;
	offset.x -= BlockXCount * BlockSize.width;
	offset.x /= 2.f;

	offset.y -= BlockYCount * BlockSize.height;
	offset.y /= 2.f;

	for (auto y = 0u; y < BlockYCount; ++y)
	{
		for (auto x = 0u; x < BlockXCount; ++x)
		{
			sf::Vector2f position(x * BlockSize.width, y * BlockSize.height);
			auto colour = colours[y % colours.size()];

			addBlock(offset + position, colour);
		}
	}

}

void GameState::loadResources()
{
	xy::SpriteSheet spriteSheet;
	spriteSheet.loadFromFile("assets/sprites/sprites.spt",mResources);
	
	SpriteID::sprites[SpriteID::Paddle] = spriteSheet.getSprite("Paddle");
	SpriteID::sprites[SpriteID::Ball] = spriteSheet.getSprite("Ball");
	SpriteID::sprites[SpriteID::Block] = spriteSheet.getSprite("Block");
}

xy::StateID GameState::stateID() const
{
	return States::GAME;
}

void GameState::spawnBall()
{
	xy::Command cmd;
	cmd.targetFlags = CommandID::Paddle;
	cmd.action = [&](xy::Entity entity, float)
	{
		auto& paddle = entity.getComponent<Paddle>();
		paddle.ball = mGameScene.createEntity();
		paddle.ball.addComponent<xy::Transform>();
		paddle.ball.addComponent<xy::Sprite>()= SpriteID::sprites[SpriteID::Ball];
		paddle.ball.addComponent<xy::Drawable>();
		paddle.ball.addComponent<Ball>();

		auto ballBounds = paddle.ball.getComponent<xy::Sprite>().getTextureBounds();
		auto paddleBounds = entity.getComponent<xy::Sprite>().getTextureBounds();
		paddle.ball.getComponent<xy::Transform>().setOrigin(ballBounds.width / 2.f, ballBounds.height / 2.f);
		paddle.ball.getComponent<xy::Transform>().setPosition(paddleBounds.width / 2.f, -ballBounds.height / 2.f);
		paddle.ball.addComponent<xy::BroadphaseComponent>(ballBounds);
		paddle.ball.addComponent<Collider>().callback =
			[](xy::Entity e, xy::Entity other, Manifold man)
		{
			//if we hit the paddle change the velocity angle
			if (other.hasComponent<Paddle>())
			{
				auto newVel = e.getComponent<xy::Transform>().getPosition() - other.getComponent<xy::Transform>().getPosition();
				e.getComponent<Ball>().velocity = xy::Util::Vector::normalise(newVel);
			}
			else
			{
				//reflect the ball's velocity around the collision normal
				auto vel = e.getComponent<Ball>().velocity;
				vel = xy::Util::Vector::reflect(vel, man.normal);
				e.getComponent<Ball>().velocity = vel;
			}
		};

		entity.getComponent<xy::Transform>().addChild(paddle.ball.getComponent<xy::Transform>());
	};
	mGameScene.getSystem<xy::CommandSystem>().sendCommand(cmd);
}