#include <memory>
#include "spawn.hpp"
#include "asset_loader.hpp"

Entity& Spawn::spawn_weapon(glm::vec3 position, std::string model_path, Entity* parent, glm::mat4* offset)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->get_component<Model>()->create(model_path);
	entity->add_component<PlayerWeapon>();
	entity->get_component<PlayerWeapon>()->attach_to_entity(parent, offset);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_model(glm::vec3 position, std::string model_path)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->get_component<Model>()->create(model_path);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

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

Entity& Spawn::spawn_text(glm::vec3 position, std::string text)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<TextComponent>();
	entity->get_component<TextComponent>()->set_text(text);
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
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_runner(glm::vec3 position, Entity* target)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<AnimationController>();
	AnimationController* animator = entity->get_component<AnimationController>();

	entity->add_component<MonsterController>();
	entity->get_component<MonsterController>()->set_target(target);

	entity->get_component<MonsterController>()->set_max_speed(1.0f);
	entity->get_component<MonsterController>()->set_tracking_distance(2.0f);
	
	entity->get_component<Model>()->create("cop_zombie.fbx", true);
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();
	
	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_idle.fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_run.fbx", run_anim);
	animator->add_animation(run_anim);

	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y - 0.5f, model->get_max_bounds().x),
		glm::vec3(0, -1, 0));

	entity->get_component<PhysicsBody>()->set_kinematic(true);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_walker(glm::vec3 position, Entity* target)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<AnimationController>();
	AnimationController* animator = entity->get_component<AnimationController>();

	entity->add_component<MonsterController>();
	entity->get_component<MonsterController>()->set_target(target);

	entity->get_component<MonsterController>()->set_max_speed(0.5f);
	entity->get_component<MonsterController>()->set_tracking_distance(2.0f);

	entity->get_component<Model>()->create("cop_zombie.fbx", true);
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_idle.fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_run.fbx", run_anim);
	animator->add_animation(run_anim);

	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y - 0.5f, model->get_max_bounds().x),
		glm::vec3(0, -1, 0));

	entity->get_component<PhysicsBody>()->set_kinematic(true);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

Entity& Spawn::spawn_crawler(glm::vec3 position, Entity* target)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->add_component<AnimationController>();
	AnimationController* animator = entity->get_component<AnimationController>();

	entity->add_component<MonsterController>();
	entity->get_component<MonsterController>()->set_target(target);

	entity->get_component<MonsterController>()->set_max_speed(1.5f);
	entity->get_component<MonsterController>()->set_tracking_distance(3.0f);

	entity->get_component<Model>()->create("cop_zombie.fbx", true);
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_idle.fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("cop_zombie_run.fbx", run_anim);
	animator->add_animation(run_anim);

	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y - 0.5f, model->get_max_bounds().x),
		glm::vec3(0, -1, 0));

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
	AnimationController* animator = entity->get_component<AnimationController>();
	entity->get_component<Model>()->create("survivor.fbx", true);

	entity->add_component<PhysicsBody>();
	Model* model = entity->get_component<Model>();

	Animation idle_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("survivor_idle.fbx", idle_anim);
	animator->add_animation(idle_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("survivor_run.fbx", run_anim);
	animator->add_animation(run_anim);

	Animation jump_anim = Animation(&model->skeleton);
	jump_anim.set_root_bone("Hips");
	jump_anim.constrain_axes(glm::vec3(0, 1, 0));
	AssetLoader::load_animation("survivor_jump.fbx", jump_anim);
	animator->add_animation(jump_anim);

	Animation aiming_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Rifle Aiming Idle.fbx", aiming_anim);
	animator->add_animation(aiming_anim);

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Firing Rifle (1).fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation roll_anim = Animation(&model->skeleton);
	roll_anim.set_root_bone("Hips");
	roll_anim.constrain_axes(glm::vec3(1, 0, 1));
	AssetLoader::load_animation("Dive Roll.fbx", roll_anim);
	animator->add_animation(roll_anim);

	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y - 0.5f, model->get_max_bounds().x),
		glm::vec3(0, -1, 0));

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

