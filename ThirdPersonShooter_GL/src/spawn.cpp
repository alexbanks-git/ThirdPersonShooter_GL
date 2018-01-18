#include <memory>
#include "spawn.hpp"

Entity& Spawn::spawn_sphere(glm::vec3 position, bool is_static)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<PhysicsBody>();
	Model* model = entity->get_component<Model>();
	model->create("default_sphere.fbx");

	PhysicsBody* body = entity->get_component<PhysicsBody>();
	GLint mass;
	if (is_static)
		mass = 0.0f;
	else
		mass = 1.0f;

	body->create("Sphere", 1.0f, glm::vec3(
		model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x));
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_cube(glm::vec3 position, bool is_static)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<PhysicsBody>();
	Model* model = entity->get_component<Model>();
	model->create("default_cube.fbx");

	PhysicsBody* body = entity->get_component<PhysicsBody>();
	GLint mass;
	if (is_static)
		mass = 0.0f;
	else
		mass = 1.0f;

	body->create("Box", 1.0f, glm::vec3(
		model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x));
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_plane(GLfloat y_position)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(glm::vec3(0.0f, y_position, 0.0f));
	entity->add_component<Model>();
	entity->add_component<PhysicsBody>();
	Model* model = entity->get_component<Model>();
	model->create("ground_plane.fbx");
	
	PhysicsBody* body = entity->get_component<PhysicsBody>();
	body->create("StaticPlane", 0.0f, glm::vec3(
		model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x));
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_camera(glm::vec3 position, GLfloat field_of_view, GLfloat near, GLfloat far)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Camera>();
	entity->get_component<Camera>()->set_clipping_planes(near, far);
	entity->get_component<Camera>()->set_field_of_view(field_of_view);
	entity->add_component<CameraController>();
	entity->get_component<CameraController>()->set_target(&entity->transform);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_basic_enemy(glm::vec3 position, Entity* target)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<AnimationController>();
	entity->add_component<EnemyController>();
	entity->get_component<EnemyController>()->set_target(target);
	entity->add_component<PhysicsBody>();
	std::vector<std::string> anim_paths;
	anim_paths.push_back("Firing Rifle (1).fbx");
	anim_paths.push_back("survivor_run.fbx");
	Model* model = entity->get_component<Model>();
	model->create(
		"survivor.fbx", anim_paths, entity->get_component<AnimationController>());
	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f, glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x));
	entity->get_component<PhysicsBody>()->set_kinematic(true);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_player(glm::vec3 position, Camera* camera)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<AnimationController>();
	std::vector<std::string> anim_paths;
	anim_paths.push_back("survivor_idle.fbx");
	anim_paths.push_back("survivor_run.fbx");
	anim_paths.push_back("survivor_jump.fbx");
	anim_paths.push_back("Rifle Aiming Idle.fbx");
	anim_paths.push_back("Firing Rifle (1).fbx");
	entity->get_component<Model>()->create(
		"survivor.fbx", anim_paths, entity->get_component<AnimationController>());

	entity->add_component<PhysicsBody>();
	Model model = *entity->get_component<Model>();
	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f, glm::vec3(model.get_max_bounds().z, model.get_max_bounds().y, model.get_max_bounds().x));

	entity->add_component<PlayerController>();
	entity->get_component<PlayerController>()->set_camera(camera);
	entity->get_component<PhysicsBody>()->set_kinematic(true);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_image(glm::vec3 position, std::string path)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Image>();
	entity->get_component<Image>()->set_size(3.0f, 3.0f);
	entity->get_component<Image>()->set_image_path(path);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_bullet(glm::vec3 position, glm::vec3 velocity, glm::vec3 end, std::string path)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Image>();
	entity->get_component<Image>()->set_size(0.1f, 0.1f);
	entity->get_component<Image>()->material = 2;
	entity->add_component<BulletController>();
	entity->get_component<BulletController>()->set_velocity(velocity);
	entity->get_component<BulletController>()->set_endpoint(end);
	entity->get_component<Image>()->set_image_path(path);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

