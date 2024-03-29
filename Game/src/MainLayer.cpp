#include "MainLayer.h"

MainLayer::MainLayer(TowerDelivery::Application* game)
	:Layer("Example"), useDebugCamera(false), m_Game(game)
{
	//load config
	INIReader reader("assets/settings.ini");
	window_width = reader.GetInteger("window", "window_width", 1280);
	window_height = reader.GetInteger("window", "window_height", 768);
	fullscreen_width = reader.GetInteger("window", "fullscreen_width", 1920);
	fullscreen_height = reader.GetInteger("window", "fullscreen_height", 1080);
	refresh_rate = reader.GetInteger("window", "refresh_rate", 60);
	fullscreen = reader.GetBoolean("window", "fullscreen", false);
	showFPS = reader.GetBoolean("window", "showFPS", false);
	showHUD = reader.GetBoolean("window", "showHUD", true);
	exposure = (float)reader.GetReal("rendering", "exposure", 1.0f);

	game->GetWindow().SetRefreshRate(refresh_rate);

	//setup bullet world
	btDefaultCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_configuration);
	btDbvtBroadphase* broadphase = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration));
	dynamicsWorld->setGravity(btVector3(0, -20, 0));

	//setup shaders
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	shader.reset(new TowerDelivery::Shader("assets/shader/main.vert", "assets/shader/main.frag"));
	shaderLight.reset(new TowerDelivery::Shader("assets/shader/main.vert", "assets/shader/light_source.frag"));
	shaderBlur.reset(new TowerDelivery::Shader("assets/shader/blur.vert", "assets/shader/blur.frag"));
	shaderFinal.reset(new TowerDelivery::Shader("assets/shader/final.vert", "assets/shader/final.frag"));
	shaderParticle.reset(new TowerDelivery::Shader("assets/shader/particle.vert", "assets/shader/particle.frag"));
	shaderPBR.reset(new TowerDelivery::Shader("assets/shader/pbr.vert", "assets/shader/pbr.frag"));

	stbi_set_flip_vertically_on_load(true);

	//setup character
	characterController = new TowerDelivery::CharacterController(0.84f, 1.7f, 60.0f, btVector3(spawnPos.x, spawnPos.y, spawnPos.z), dynamicsWorld.get());
	characterModel = new TowerDelivery::Model("assets/models/character/character.obj");

	//setup cameras
	playerCamera = new TowerDelivery::PlayerCamera(characterController);
	camera = new TowerDelivery::Camera(glm::vec3(-25.0f, 16.0f, -20.0f));
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.5f, 100.0f);

	//setup model for point lights
	lightModel = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(1.0f, 1.0f, 1.0f));

	//load textures

	//floor
	tex_diff_floor = TowerDelivery::loadTexture("assets/textures/floor/albedo.png");

	//dynamic cubes
	tex_diff_cube = TowerDelivery::loadTexture("assets/textures/dynamic_cube/albedo.png");
	tex_spec_cube = TowerDelivery::loadTexture("assets/textures/dynamic_cube/metallic.png");

	//particle
	tex_particle = TowerDelivery::loadTexture("assets/textures/box_particle.png");

	//container
	c_albedo_black = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_black.png");
	c_albedo_blue = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_blue.png");
	c_albedo_green = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_green.png");
	c_albedo_grey = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_grey.png");
	c_albedo_orange = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_orange.png");
	c_albedo_red = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_red.png");
	c_albedo_white = TowerDelivery::loadTexture("assets/textures/chipped-paint/albedo_white.png");
	c_normal = TowerDelivery::loadTexture("assets/textures/chipped-paint/normal.png");
	c_metallic = TowerDelivery::loadTexture("assets/textures/chipped-paint/metallic.png");
	c_roughness = TowerDelivery::loadTexture("assets/textures/chipped-paint/roughness.png");
	c_ao = TowerDelivery::loadTexture("assets/textures/chipped-paint/ao.png");

	//character
	ch_basecolor = TowerDelivery::loadTexture("assets/textures/robot1/basecolor.png");
	ch_normal = TowerDelivery::loadTexture("assets/textures/robot1/normal.png");
	ch_metallic = TowerDelivery::loadTexture("assets/textures/robot1/metallic.png");
	ch_roughness = TowerDelivery::loadTexture("assets/textures/robot1/roughness.png");
	ch_ao = TowerDelivery::loadTexture("assets/textures/robot1/AO.png");

	//create area for lose condition
	loseArea = new TowerDelivery::DetectionArea(glm::vec3(10.0f, -15.0f, 9.0f), 100.0f, 14.6f, 100.0f);
	TowerDelivery::VertexArray* gl_loseArea = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(60.0f, 1.5f, 60.0f));
	TowerDelivery::GameObject* m_loseArea = new TowerDelivery::GameObject(gl_loseArea, &tex_diff_floor);
	m_loseArea->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -1.0f, 10.0f)));
	m_gameObjects.push_back(m_loseArea);

	//setup check points
	{
		particleSystem = new TowerDelivery::ParticleSystem();

		cp_models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(15.47, 10.0, 2.32));
		cp_models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-4.2, 19.0, 15.48));
		cp_models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(-0.62, 40.0, 8.39));
		cp_models[3] = glm::translate(glm::mat4(1.0f), glm::vec3(15.09, 65.34, 10.96));

		cp_areas[0] = new TowerDelivery::DetectionArea(glm::vec3(15.47, 11.97, 2.32), 2.0f, 4.0f, 2.0f);
		cp_areas[1] = new TowerDelivery::DetectionArea(glm::vec3(-4.2, 20.43, 15.48), 2.0f, 4.0f, 2.0f);
		cp_areas[2] = new TowerDelivery::DetectionArea(glm::vec3(-0.62, 41.98, 8.39), 2.0f, 4.0f, 2.0f);
		cp_areas[3] = new TowerDelivery::DetectionArea(glm::vec3(15.09, 67.34, 10.96), 2.0f, 4.0f, 2.0f);

		cp_spawnPos[0] = glm::vec3(15.47, 13.0, 2.32);
		cp_spawnPos[1] = glm::vec3(-4.2, 21.0, 15.48);
		cp_spawnPos[2] = glm::vec3(-0.62, 43.0, 8.39);
		cp_spawnPos[3] = glm::vec3(15.09, 68.34, 10.96);

		cp_reached[0] = false;
		cp_reached[1] = false;
		cp_reached[2] = false;
		cp_reached[3] = false;
	}

	//create containers
	{
		containerType1 = new TowerDelivery::Model("assets/models/container/container1.obj");
		containerType2 = new TowerDelivery::Model("assets/models/container/container2.obj");
		containerType3 = new TowerDelivery::Model("assets/models/container/container3.obj");
		containerType4 = new TowerDelivery::Model("assets/models/container/container4.obj");

		container1 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(1.56f, 2.3277f, -4.9344f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container1);
		container2 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(9.9759f, 3.1237f, -0.34716f), glm::vec3(0.0f, 90.0f, 0.0f), c_albedo_orange, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container2);
		container3 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(1.4744f, 9.715f, 6.6981f), glm::vec3(90.0f, -90.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container3);
		container4 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(15.355f, 3.0915f, -0.34916f), glm::vec3(0.0f, -90.0f, 0.0f), c_albedo_white, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container4);
		container5 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(-1.2851f, 3.2039f, 3.549f), glm::vec3(180.0f, 0.0f, 180.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container5);
		container6 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(-2.5231f, 3.0915f, 9.3131f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_grey, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container6);
		container7 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(14.552f, 3.0915f, 9.3131f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container7);
		container8 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(0.20066f, 3.0915f, 16.913f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container8);
		container9 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(15.641f, 7.6725f, 14.969f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container9);
		container10 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(24.482f, 2.3121f, 13.681f), glm::vec3(0.0f, 90.0f, 0.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container10);
		container11 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(14.831f, 8.4459f, 2.2828f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container11);
		container12 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(14.438f, 9.4187f, 7.2223f), glm::vec3(180.0f, 0.0f, 180.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container12);
		container13 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(-4.017f, 12.448f, 9.9435f), glm::vec3(0.0f, 0.0f, 90.0f), c_albedo_grey, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container13);
		container14 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(-4.017f, 12.532f, 15.544f), glm::vec3(180.0f, 0.0f, -90.0f), c_albedo_orange, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container14);
		container15 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(4.079f, 9.5254f, 15.104f), glm::vec3(180.0f, 0.0f, 180.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container15);
		container16 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(-1.2851f, 18.155f, 1.9854f), glm::vec3(-180.0f, 0.0f, 270.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container16);
		container17 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(23.123f, 10.424f, 13.207f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_grey, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container17);
		container18 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(20.052f, 20.157f, 5.4737f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container18);
		container19 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(14.705f, 20.294f, 9.296f), glm::vec3(0.0f, 0.0f, 90.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container19);
		container20 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(-2.1687f, 20.3f, 9.9149f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container20);
		container21 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(23.594f, 25.013f, 12.318f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container21);
		container22 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(-1.0128f, 25.879f, -0.7676f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container22);
		container23 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(15.721f, 24.031f, 16.981f), glm::vec3(180.0f, 0.0f, 90.0f), c_albedo_orange, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container23);
		container24 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(2.4552f, 30.662f, 13.595f), glm::vec3(0.0f, 0.0f, 90.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container24);
		container25 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(23.014f, 36.418f, 4.9557f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_white, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container25);
		container26 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(13.629f, 29.921f, 4.8622f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container26);
		container27 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(14.711f, 29.958f, 9.8735f), glm::vec3(0.0f, 0.0f, 180.0f), c_albedo_orange, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container27);
		container28 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(2.7215f, 30.229f, 3.251f), glm::vec3(0.0f, 90.0f, 180.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container28);
		container29 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(3.101f, 36.488f, 8.5148f), glm::vec3(0.0f, 180.0f, 0.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container29);
		container30 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(1.1828f, 40.501f, 15.193f), glm::vec3(180.0f, 0.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container30);
		container31 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(14.953f, 37.972f, 15.927f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_white, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container31);
		container32 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(23.736f, 41.255f, 14.263f), glm::vec3(0.0f, 0.0f, -90.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container32);
		container33 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(19.317f, 46.439f, 6.3541f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container33);
		container34 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(14.813f, 46.87f, 13.254f), glm::vec3(0.0f, 0.0f, 180.0f), c_albedo_orange, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container34);
		container35 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(15.731f, 46.909f, 18.26f), glm::vec3(0.0f, 0.0f, 0.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container35);
		container36 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(4.6018f, 44.732f, 17.959f), glm::vec3(0.0f, -90.0f, 0.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container36);
		container37 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model4, containerType4, glm::vec3(25.834f, 52.95f, 14.536f), glm::vec3(0.0f, 90.0f, 0.0f), c_albedo_grey, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container37);
		container38 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(20.434f, 53.717f, 8.6974f), glm::vec3(0.0f, 90.0f, -90.0f), c_albedo_black, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container38);
		container39 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model3, containerType3, glm::vec3(20.583f, 56.473f, 14.082f), glm::vec3(0.0f, -90.0f, 90.0f), c_albedo_green, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container39);
		container40 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(20.333f, 52.1f, 22.482f), glm::vec3(0.0f, 90.0f, 0.0f), c_albedo_red, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container40);
		container41 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(15.321f, 51.217f, 19.786f), glm::vec3(0.0f, 90.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container41);
		container42 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model2, containerType2, glm::vec3(15.131f, 59.445f, 10.946f), glm::vec3(0.0f, 0.0f, 90.0f), c_albedo_white, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container42);
		container43 = new ContainerObject(shaderPBR.get(), dynamicsWorld.get(), model1, containerType1, glm::vec3(13.629f, 29.921f, -0.10338f), glm::vec3(0.0f, 180.0f, 0.0f), c_albedo_blue, c_normal, c_metallic, c_roughness, c_ao);
		m_containers.push_back(container43);
	}

	//create dynamic cubes

	dc_positions.push_back(glm::vec3(6.84, 13.0, 15.35));
	dc_sizes.push_back(glm::vec3(2.0f, 2.0f, 2.0f));

	dc_positions.push_back(glm::vec3(1.48, 15.0, 7.99));
	dc_sizes.push_back(glm::vec3(4.0f, 4.0f, 4.0f));

	dc_positions.push_back(glm::vec3(-5.15, 30.0, -0.73));
	dc_sizes.push_back(glm::vec3(2.0f, 2.0f, 2.0f));

	dc_positions.push_back(glm::vec3(17.01, 37.0, 0.62));
	dc_sizes.push_back(glm::vec3(3.0f, 6.0f, 3.0f));

	dc_positions.push_back(glm::vec3(15.92, 36.0, 7.68));
	dc_sizes.push_back(glm::vec3(4.0f, 4.0f, 4.0f));

	dc_positions.push_back(glm::vec3(11.01, 35.0, 1.91));
	dc_sizes.push_back(glm::vec3(2.0f, 2.0f, 2.0f));

	dc_positions.push_back(glm::vec3(-1.16, 46.02, 15.16));
	dc_sizes.push_back(glm::vec3(2.0f, 2.0f, 2.0f));

	dc_positions.push_back(glm::vec3(20.27, 62.61, 8.69));
	dc_sizes.push_back(glm::vec3(2.0f, 2.0f, 2.0f));

	GenerateDynamicCubes();

	// configure (floating point) framebuffers
	// ---------------------------------------
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// create depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)window_width, (GLsizei)window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	if (fullscreen) {
		game->GetWindow().SetFullscreen(fullscreen);
		//unsigned int* size = game->GetWindow().GetScreenSize();
		//TowerDelivery::WindowResizeEvent event(size[0], size[1]);
		TowerDelivery::WindowResizeEvent event(fullscreen_width, fullscreen_height);
		OnWindowResizeEvent(event);
	}
	else {
		game->GetWindow().SetSize(window_width, window_height);
	}

	//setup text
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	text = new TowerDelivery::TextRenderer(window_width, window_height);
	text->Load("assets/fonts/Montserrat-Regular.ttf", 72);

	//shader configuration
	shader->Bind();
	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);
	shader->setFloat("material.shininess", 0.5f);

	shaderBlur->Bind();
	shaderBlur->setInt("image", 0);

	shaderFinal->Bind();
	shaderFinal->setInt("scene", 0);
	shaderFinal->setInt("bloomBlur", 1);

	shaderParticle->Bind();
	shaderParticle->setInt("myTextureSampler", 0);

	shaderPBR->Bind();
	shaderPBR->setInt("albedoMap", 0);
	shaderPBR->setInt("normalMap", 1);
	shaderPBR->setInt("metallicMap", 2);
	shaderPBR->setInt("roughnessMap", 3);
	shaderPBR->setInt("aoMap", 4);

	glClearColor(0.072, 0.144, 0.304, 1.0f);

	//set point lights
	light_positions.push_back(glm::vec3(16.81, 20.0, 3.72));
	light_positions.push_back(glm::vec3(3.0, 15.0, 2.75));
	light_positions.push_back(glm::vec3(12.4, 18.0, 13.54));
	light_positions.push_back(glm::vec3(2.44, 26.0, 10.48));
	light_positions.push_back(glm::vec3(3.81, 36.0, 5.39));
	light_positions.push_back(glm::vec3(23.44, 35.0, 11.14));
	light_positions.push_back(glm::vec3(7.02, 49.18, 9.84));
	light_positions.push_back(glm::vec3(17.49, 60.0, 15.18));
	light_positions.push_back(glm::vec3(23.67, 62.44, 14.08));
	light_positions.push_back(glm::vec3(15.26, 75.0, 10.76));

	for (unsigned int i = 0; i < light_positions.size(); i++) {
		shaderPBR->Bind();
		shaderPBR->setVec3("lightPositions[" + std::to_string(i) + "]", light_positions[i].x, light_positions[i].y, light_positions[i].z);
		shaderPBR->setVec3("lightColors[" + std::to_string(i) + "]", 1.0f * 45.0f, 1.0f * 45.0f, 1.0f * 45.0f);

		shader->Bind();
		shader->setVec3("pointLights[" + std::to_string(i) + "].position", light_positions[i].x, light_positions[i].y, light_positions[i].z);
		shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.1f, 0.1f, 0.1f);
		shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", 1.0f, 1.0f, 1.0f);
		shader->setVec3("pointLights[" + std::to_string(i) + "].specular", 2.0f, 2.0f, 2.0f);
		shader->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
		shader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
		shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
	}

	//set main shader lights
	shader->Bind();

	shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("dirLight.diffuse", 0.01f, 0.01f, 0.01f);
	shader->setVec3("dirLight.specular", 0.05f, 0.05f, 0.05f);
}

void MainLayer::OnAttach()
{
}

void MainLayer::OnDetach()
{
}

void MainLayer::OnUpdate(TowerDelivery::Timestep ts) {
	characterController->OnUpdate(ts);

	//check lose condition
	if (loseArea->Contains(characterController) && !lost) {
		lives--;

		if (lives == 0) {
			lost = true;
			TD_TRACE("You lost the game!");
		}
		else {
			TD_TRACE("{0} lives left!", lives);

			glm::vec3 respawnPos = spawnPos;

			for (int i = 3; i >= 0; i--) {
				if (cp_reached[i] == true) {
					respawnPos = cp_spawnPos[i];
					break;
				}
			}

			characterController->SetPosition(respawnPos);

			for each (btRigidBody * body in dc_bodies) {
				delete body->getMotionState();
				delete body->getCollisionShape();
				dynamicsWorld->removeRigidBody(body);
				delete body;
			}
			dc_bodies.clear();
			dc_gameObjects.clear();
			GenerateDynamicCubes();
		}
	}

	//update world and character
#ifdef TD_DEBUG
	dynamicsWorld->stepSimulation(1.0f / 60.0f);
#endif

#ifdef TD_RELEASE
	dynamicsWorld->stepSimulation(ts, 1);
#endif

	for (unsigned int i = 0; i < 4; i++) {
		if (cp_reached[i] == false && cp_areas[i]->Contains(characterController)) {
			cp_reached[i] = true;
			TD_TRACE("You have collected Package {0}", i);
			packagesCollected++;
		}
	}

	//handle camera zoom
	playerCamera->SetZoom(10.0f);

	btVector3 t_playerPos = glm2bt(characterController->GetPosition());
	btVector3 t_cameraPos = glm2bt(playerCamera->GetPosition());

	btCollisionWorld::ClosestRayResultCallback res(t_playerPos, t_cameraPos);
	dynamicsWorld->rayTest(t_playerPos, t_cameraPos, res);

	if (res.hasHit()) {
		btVector3 hitposition = res.m_hitPointWorld;
		float hitDistance = t_playerPos.distance(hitposition);
		playerCamera->SetZoom(hitDistance - 0.1f);
	}

	//prepare for rendering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. render scene into floating point framebuffer
	// -----------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Bind();
	shader->setMat4("projection", projectionMatrix);

	shaderLight->Bind();
	shaderLight->setMat4("projection", projectionMatrix);

	// initialize static shader uniforms before rendering
	// --------------------------------------------------
	shaderPBR->Bind();
	shaderPBR->setMat4("projection", projectionMatrix);

	shaderParticle->Bind();
	shaderParticle->setMat4("projection", projectionMatrix);

	if (useDebugCamera) {
		shader->Bind();
		shader->setVec3("viewPos", camera->Position);
		shader->setMat4("view", camera->GetViewMatrix());

		shaderPBR->Bind();
		shaderPBR->setVec3("camPos", camera->Position);
		shaderPBR->setMat4("view", camera->GetViewMatrix());

		shaderLight->Bind();
		shaderLight->setMat4("view", camera->GetViewMatrix());

		shaderParticle->Bind();
		shaderParticle->setMat4("view", camera->GetViewMatrix());

		camera->OnUpdate(ts);
	}
	else {
		shader->Bind();
		shader->setVec3("viewPos", playerCamera->GetPosition());
		shader->setMat4("view", playerCamera->GetViewMatrix());

		shaderPBR->Bind();
		shaderPBR->setVec3("camPos", playerCamera->GetPosition());
		shaderPBR->setMat4("view", playerCamera->GetViewMatrix());

		shaderLight->Bind();
		shaderLight->setMat4("view", playerCamera->GetViewMatrix());

		shaderParticle->Bind();
		shaderParticle->setMat4("view", playerCamera->GetViewMatrix());

		playerCamera->OnUpdate(ts);
	}

	//prepare drawing objects
	glm::mat4 model = glm::mat4(1.0f);

	//draw character
	shaderPBR->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ch_basecolor);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ch_normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ch_metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ch_roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ch_ao);

	shaderPBR->setMat4("model", characterController->GetModelMatrix());

	characterModel->Draw(*shaderPBR);

	//draw game objects
	shader->Bind();
	for each (TowerDelivery::GameObject * gameObject in m_gameObjects)
	{
		gameObject->OnUpdate();
		gameObject->Draw(shader.get());
	}

	for each (TowerDelivery::GameObject * dynamicCube in dc_gameObjects)
	{
		dynamicCube->OnUpdate();
		dynamicCube->Draw(shader.get());
	}

	for each (ContainerObject * container in m_containers)
	{
		container->Draw();
	}

	//draw checkpoints (particle systems)
	shaderParticle->Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_particle);
	particleSystem->OnUpdate(ts, playerCamera->GetPosition());

	for (unsigned int i = 0; i < 4; i++) {
		if (cp_reached[i] == false) {
			shaderParticle->setMat4("model", cp_models[i]);
			particleSystem->Draw();
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	//draw point light as cube
	shaderLight->Bind();

	for (unsigned int i = 0; i < light_positions.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(light_positions[i].x, light_positions[i].y, light_positions[i].z));
		model = glm::scale(model, glm::vec3(0.5f));
		shaderLight->setMat4("model", model);
		shaderLight->setVec3("lightColor", glm::vec3(1.0f * 45.0f, 1.0f * 45.0f, 1.0f * 45.0f));
		lightModel->draw();
	}

	//render HUD

	if (showHUD) {
		text->RenderText("Lives left: " + std::to_string(lives), 30.0f, 40.0f, 0.4f);
		text->RenderText("Packages collected: " + std::to_string(packagesCollected) + "/" + std::to_string(packages), 30.0f, 80.0f, 0.4f);

		if (showFPS) {
			int fps = 1.0f / ts;
			text->RenderText("FPS " + std::to_string(fps), 30.0f, 120.0f, 0.4);
		}

		if (lost) {
			text->RenderText("You Lost!", window_width / 2.0f - 380.0f, window_height / 2.0f - 50.0f, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (packagesCollected == packages) {
			text->RenderText("You Won!", window_width / 2.0f - 380.0f, window_height / 2.0f - 50.0f, 2.0f);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. blur bright fragments with two-pass Gaussian Blur
	// --------------------------------------------------
	glActiveTexture(GL_TEXTURE0);
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	shaderBlur->Bind();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		shaderBlur->setBool("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		renderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
	// --------------------------------------------------------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderFinal->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[horizontal]);
	shaderFinal->setBool("bloom", bloom);
	shaderFinal->setFloat("exposure", exposure);

	renderQuad();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MainLayer::OnEvent(TowerDelivery::Event& event) {
	//TD_TRACE("{0}", event);
	TowerDelivery::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<TowerDelivery::KeyPressedEvent>(TD_BIND_EVENT_FN(MainLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<TowerDelivery::WindowResizeEvent>(TD_BIND_EVENT_FN(MainLayer::OnWindowResizeEvent));
}

bool MainLayer::OnKeyPressedEvent(TowerDelivery::KeyPressedEvent& event) {
	if (event.GetKeyCode() == TD_KEY_F2) {
		useDebugCamera = !useDebugCamera;
	}
	if (event.GetKeyCode() == TD_KEY_F3) {
		showHUD = !showHUD;
	}
	if (event.GetKeyCode() == TD_KEY_SPACE) {
		//characterController->Jump();
	}

	return true;
}

bool MainLayer::OnWindowResizeEvent(TowerDelivery::WindowResizeEvent& event) {
	window_width = event.GetWidth();
	window_height = event.GetHeight();

	//TD_WARN("triggered resize event: {0}, {1}", window_width, window_height);

	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height, 0.5f, 100.0f);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window_width, window_height);

	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)window_width, (GLsizei)window_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	return false;
}

void MainLayer::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void MainLayer::GenerateDynamicCubes()
{
	for (unsigned int i = 0; i < dc_positions.size(); i++)
	{
		btCollisionShape* boxShape = new btBoxShape(glm2bt(dc_sizes[i] / 2.0f));

		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1000.f);

		bool isDynamic = true;

		btVector3 localInertia(0, 0, 0);

		boxShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(glm2bt(dc_positions[i]));

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
		btRigidBody* bt_dynamicCube = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(bt_dynamicCube);

		dc_bodies.push_back(bt_dynamicCube);

		TowerDelivery::VertexArray* gl_dynamicCube = new TowerDelivery::VertexArray(TowerDelivery::VertexArray::createCubeVertexArray(dc_sizes[i].x, dc_sizes[i].y, dc_sizes[i].z));
		TowerDelivery::GameObject* m_dynamicCube = new TowerDelivery::GameObject(gl_dynamicCube, &tex_diff_cube, bt_dynamicCube, &tex_spec_cube);
		//m_gameObjects.push_back(m_dynamicCube);
		dc_gameObjects.push_back(m_dynamicCube);
	}
}