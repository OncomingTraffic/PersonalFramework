#include "TestScene.h"
#include "Core/Sprite.h"
#include "Core/Label.h"
#include "Core/GameServices.h"
#include "Cubes.h"
#include "Player.h"
#include "Platform.h"
#include "Core/ObjLoader.h"
#include "Core/FileReader.h"
#include <typeinfo>
#include <memory>
#include "../BaseObject.h"
#include "../RenderingManager.h"
#include "../src/Core/MeshRenderer.h"
#include "../MaterialDefault.h"
#include "../PhysicsManager.h"
#include "../src/Core/LightManager.h"
#include "../RigidBody.h"
#include "../src/Core/PhysicsObject.h"
#include "../src/Core/Camera.h"
#include "../CameraControl.h"
#include "../src/Core/PointLight.h"
#include "../PlayerControl.h"


TestScene::TestScene() : Scene("TestScene")
{
	//sprite = nullptr;
	//label = nullptr;
	//label2 = nullptr;
	//cubes = nullptr;
	player = nullptr;
	//platform = nullptr;
	m_Down = false;
	
}
TestScene::~TestScene()
{
	// Remove all game objects
	// Cleanup unused resources
	// Remove managers

	Cleanup();
}

float TestScene::Load()
{
	m_RenderingManager = std::make_shared<RenderingManager>(this);
	m_PhysicsManager = std::make_shared<PhysicsManager>();
	m_LightManager = std::make_shared<LightManager>();

	m_RenderingManager->SetBruteForceRendering(true);

	m_RenderingManager->AddShader(GameServices::GetShaderManager()->GetShader(std::string("Default")));
	m_PhysicsManager->Initialize();

	//player = std::make_shared<Player>();
	////player->SetScale(glm::vec3(1.f / 15.f, 1.f / 15.f, 1.f / 15.f));
	//
	//std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();
	//
	//std::shared_ptr<Mesh> mesh = GameServices::GetResourceManager()->LoadResource<Mesh>("Models/sphere.obj");
	//meshRenderer->SetMesh(mesh);
	//std::shared_ptr<MaterialDefault> m = std::make_shared<MaterialDefault>();
	//m->Initialize();
	//meshRenderer->SetMaterial(m);
	//player->AddComponent(meshRenderer);


	//std::shared_ptr<RigidBody> rigidbody = std::make_shared<RigidBody>(m_PhysicsManager.get());
	//rigidbody->SetCollisionShape(std::make_shared<btSphereShape>(1.f));
	//rigidbody->SetMass(1.f);
	//
	//player->AddComponent(rigidbody);
	//rigidbody->GetRigidBody()->setRestitution(1.f);


	sphere = std::make_shared<PhysicsObject>();
	sphere->SetParentScene(this);
	sphere->SetPhysicsManager(m_PhysicsManager.get());
	sphere->SetRigidbody(1.f, std::make_shared<btSphereShape>(1.0f), btTransform(btQuaternion::getIdentity(), btVector3(0, 10.0f, 0)));
	//sphere->SetRigidbody(1.f, std::make_shared<btCapsuleShape>(1.0f, 2.0f), btTransform(btQuaternion::getIdentity(), btVector3(0, 10.0f, 0)));
	sphere->Initialize();
	sphere->GetCollisionObject()->setRestitution(0.5f);
	//sphere->GetRigidbody()->setAngularVelocity(btVector3(0, 0, 1));
	sphere->GetRigidbody()->applyImpulse(btVector3(-1, 0, 0), btVector3(0, 1, 0));
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Friction: %f", sphere->GetRigidbody()->getFriction());
	sphere->GetRigidbody()->setFriction(0.25f);
	sphere->GetRigidbody()->setRollingFriction(1.0f);

	
	std::shared_ptr<MeshRenderer> meshRenderer = std::make_shared<MeshRenderer>();	
	std::shared_ptr<Mesh> mesh = GameServices::GetResourceManager()->LoadResource<Mesh>("Models/TallBox.obj");
	meshRenderer->SetMesh(mesh);
	std::shared_ptr<MaterialDefault> m = std::make_shared<MaterialDefault>();
	m->Initialize();
	meshRenderer->SetMaterial(m);
	sphere->AddComponent(meshRenderer);
	sphere->AddComponent(std::make_shared<PointLight>(glm::vec4(0.0f, 1.0f, 0.0f, 1)));
	sphere->AddComponent(std::make_shared<PlayerControl>());

	platform = std::make_shared<PhysicsObject>();
	platform->SetParentScene(this);
	platform->SetPhysicsManager(m_PhysicsManager.get());

	// The local scaling in the btStaticPlaneShape seems to increase the "height" of the plane, causing collisions to happen above where the plane should be
	// It is best to create the plane with the normal being up (0, 1, 0) to match up with the mesh, then rotating it after
	//platform->SetRigidbody(0.f, std::make_shared<btStaticPlaneShape>(btVector3(0.f, 1, 0.f), 0.f), btTransform(btQuaternion(0, 0, glm::radians(-15.f)), btVector3(0, -10.0f, 0)));
	platform->SetRigidbody(0.f, std::make_shared<btStaticPlaneShape>(btVector3(0.f, 1.f, 0.f), 0.f), btTransform(btQuaternion(glm::radians(90.f), 0, 0), btVector3(0, -10.0f, 0)));
	platform->Initialize();
	platform->GetCollisionObject()->setRestitution(1.0f);


	std::shared_ptr<MeshRenderer> meshRenderer2 = std::make_shared<MeshRenderer>();

	std::shared_ptr<Mesh> mesh2 = GameServices::GetResourceManager()->LoadResource<Mesh>("Models/Plane.obj");
	meshRenderer2->SetMesh(mesh2);
	std::shared_ptr<MaterialDefault> m2 = std::make_shared<MaterialDefault>();
	m2->Initialize();
	meshRenderer2->SetMaterial(m2);
	platform->AddComponent(meshRenderer2);
	meshRenderer2->SetBoundingBoxDimensions(glm::vec3(-100, -10, -100), glm::vec3(100, 10, 100));


	cameraHolder = std::make_unique<GameObject>();
	cameraHolder->SetParentScene(this);
	cameraHolder->Initialize();
	cam = new Camera(std::string("Default"));
	cameraHolder->AddComponent(std::shared_ptr<Camera>(cam));
	//cameraHolder->SetPosition(glm::vec3(0, -9.9f, -30));
	cameraHolder->SetPosition(glm::vec3(0, 0.f, -30));
	cameraHolder->AddComponent(std::make_shared<CameraControl>(cam));

	//glUniform3f(light, 2, 5, -1);
	//glUniform3f(color, 1.0f, 0, 0.f);
	//glUniform3f(light2, -2, 4, 1);
	//glUniform3f(color2, 0.0f, 0.0f, 1.0f);
	std::shared_ptr<GameObject> obj = CreateGameObject(glm::vec3(2, 5, -1));
	obj->AddComponent(std::make_shared<PointLight>(glm::vec4(1.0f, 0, 0.f, 1), glm::vec3(-0.9f, 0.1f, 0.01f)));

	obj = CreateGameObject(glm::vec3(-2, 4, 1));
	obj->AddComponent(std::make_shared<PointLight>(glm::vec4(0.0f, 0.0f, 1.0f, 1)));


	

	//rb = rigidbody.get();


	//player->SetShader(GameServices::GetShaderManager()->AddShader(std::string("Shaders/Lighting")));


	music = GameServices::GetResourceManager()->LoadResource<Mix_Music>("Audio/cloudy-mountain-peaks5.mp3"); // NOTE - in Audacity, exporting as 48000, average 224kbps seems optimal
	Mix_VolumeMusic(32);
	//Mix_PlayMusic(music.get(), -1);
	
	//GameServices::GetCameraManager()->GetActiveCamera()->SetPosition(glm::vec3(0, 10, -5));
	//GameServices::GetCameraManager()->GetActiveCamera()->LookAtTarget(sphere->GetPosition());
	//GameServices::GetCameraManager()->GetActiveCamera()->FollowTarget(sphere.get());

	//std::string data = LoadFile("Text.txt");
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "(FileReader) File contents: \"%s\"", data.c_str());
	//DisplayTextFile("Text.txt");

	//std::shared_ptr<TTF_Font> font(TTF_OpenFont("lazy.ttf", 12), [&](TTF_Font* font) // [&] will receive argument by reference, in this case the argument is 'aFilePath'
	//{
	//	// Custom deleter lambda
	//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Closing TTF_Font: (%s)", "lazy.ttf");
	//	TTF_CloseFont(font);
	//});


	return 1.0f;
}

void TestScene::Cleanup()
{
	platform->Cleanup();
	platform.reset();
	sphere->Cleanup();
	sphere.reset();
	cameraHolder->Cleanup();
	cameraHolder.reset();

	if (player != nullptr)
	{
		if (!player.unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "More than one reference to Player");
		}
		player.reset();
	}

	for (unsigned int i = 0; i < m_BaseObjects.size(); ++i)
	{
		m_BaseObjects.at(i)->Cleanup();
		if (!m_BaseObjects.at(i).unique())
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Object referenced by another shared_ptr and was not deleted");
		}
	}

	m_BaseObjects.clear();

	//m_RenderingManager.reset();

	Mix_HaltMusic();
	music.reset();
	GameServices::GetResourceManager()->UnloadResource<Mix_Music>(std::string("Audio/cloudy-mountain-peaks5.mp3"));
	GameServices::GetAudioManager()->RemoveAll();
}


void TestScene::Update(float aDelta)
{
	//GameServices::GetCameraManager()->GetActiveCamera()->Update(aDelta);
	//cam->Update(aDelta);

	const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

	if (KeyStates[SDL_SCANCODE_P])
	{
		btVector3 scale = sphere->GetCollisionObject()->getCollisionShape()->getLocalScaling();

		scale *= 1 + 0.1f * aDelta;

		sphere->GetCollisionObject()->getCollisionShape()->setLocalScaling(scale);
	}

	if (KeyStates[SDL_SCANCODE_RCTRL])
	{
		if (m_Down == false)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Creating Object");
			m_Down = true;
			std::shared_ptr<PhysicsObject> object = std::make_shared<PhysicsObject>();
			object->SetParentScene(this);
			object->SetPhysicsManager(m_PhysicsManager.get());
			object->SetRigidbody(1.f, std::make_shared<btSphereShape>(1.0f), btTransform(btQuaternion::getIdentity(), cameraHolder->GetBTPosition()));
			object->Initialize();
			object->GetCollisionObject()->setRestitution(0.5f);

			std::shared_ptr<MeshRenderer> renderer = std::make_shared<MeshRenderer>();
			std::shared_ptr<Mesh> mesh = GameServices::GetResourceManager()->LoadResource<Mesh>("Models/UnitSphere.obj");
			renderer->SetMesh(mesh);
			std::shared_ptr<MaterialDefault> m = std::make_shared<MaterialDefault>();
			m->Initialize();
			renderer->SetMaterial(m);
			object->AddComponent(renderer);

			AddBaseObject(object);
		}
	}
	else
	{
		m_Down = false;
	}

	//player->Update(aDelta);

	Scene::Update(aDelta);

	//m_PhysicsManager->Update(aDelta);

	//player->UpdateRBPos(rb);
}

void TestScene::Draw()
{
	m_RenderingManager->Draw();
	//sprite->Draw();
	//label->Draw();
	//label2->Draw();
	////cubes->Draw();
	//player->Draw();
	//platform->Draw();
}