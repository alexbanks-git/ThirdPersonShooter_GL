#include <memory>
#include <map>
#include "spawn.hpp"
#include "asset_loader.hpp"

Entity& Spawn::spawn_button(glm::vec2 position, GLfloat width, GLfloat height, glm::vec4 color)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(glm::vec3(position, 0.0f));
	entity->add_component<Button>();
	entity->get_component<Button>()->set_properties(position, width, height);
	entity->get_component<Button>()->set_color(color);
	EntityManager::add_ui_element(std::move(entity));
	return *EntityManager::get_last_ui_element();
}

Entity& Spawn::spawn_point_light(glm::vec3 position, GLfloat radius, GLfloat intensity, glm::vec4 color)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Light>();
	entity->get_component<Light>()->set_properties(radius, intensity, color);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last_light();
}

Entity& Spawn::spawn_directional_light(glm::vec3 dir, GLfloat intens, glm::vec4 col)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(glm::vec3(0.0f, 4.0f, -5.0f));
	entity->add_component<Light>();
	entity->get_component<Light>()->set_properties(dir, intens, col);
	entity->get_component<Light>()->create_depth_map(1280, 1280);
	entity->get_component<Light>()->create_frame_buffer_object();
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last_light();
}

Entity& Spawn::spawn_weapon(glm::vec3 position, std::string model_path, Entity* parent, glm::mat4* offset)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Model>();
	entity->get_component<Model>()->create("Models/" + model_path);
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
	Model* model = entity->get_component<Model>();
	model->create("Models/" + model_path);

	entity->add_component<PhysicsBody>();
		entity->get_component<PhysicsBody>()->create(
		"Box", 0.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x));

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
	model->create("Models/default_sphere.fbx");

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
	model->create("Models/default_cube.fbx");

	PhysicsBody* body = entity->get_component<PhysicsBody>();
	GLint mass;
	if (is_static)
		mass = 0.0f;
	else
		mass = 1.0f;

	body->create("Box", mass, glm::vec3(
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
	model->create("Models/ground_plane.fbx");

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
	
	
	entity->get_component<Model>()->create("Models/cop_zombie.fbx", true);
	
	
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();
	
	
	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_idle.fbx", shoot_anim);
	
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_run.fbx", run_anim);
	animator->add_animation(run_anim);
	
	Animation attack_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_attack.fbx", attack_anim);
	animator->add_animation(attack_anim);
	
	
	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x),
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
	entity->get_component<MonsterController>()->set_tracking_distance(50.0f);

	entity->get_component<Model>()->create("Models/cop_zombie.fbx", true);
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_idle.fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/zombie_walk.fbx", run_anim);
	animator->add_animation(run_anim);

	Animation attack_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_attack.fbx", attack_anim);
	animator->add_animation(attack_anim);

	entity->get_component<PhysicsBody>()->create(
		"Capsule", 1.0f,
		glm::vec3(model->get_max_bounds().z, model->get_max_bounds().y, model->get_max_bounds().x),
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
	entity->get_component<MonsterController>()->set_tracking_distance(50.0f);

	entity->get_component<Model>()->create("Models/cop_zombie.fbx", true);
	Model* model = entity->get_component<Model>();

	entity->add_component<PhysicsBody>();

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_idle.fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cop_zombie_run.fbx", run_anim);
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
	entity->get_component<Model>()->create("Models/survivor.fbx", true);

	entity->add_component<PhysicsBody>();
	Model* model = entity->get_component<Model>();

	Animation idle_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/survivor_idle.fbx", idle_anim);
	animator->add_animation(idle_anim);

	Animation run_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/survivor_run.fbx", run_anim);
	animator->add_animation(run_anim);

	Animation jump_anim = Animation(&model->skeleton);
	jump_anim.set_root_bone("Hips");
	jump_anim.constrain_axes(glm::vec3(0, 1, 0));
	AssetLoader::load_animation("Models/survivor_jump.fbx", jump_anim);
	animator->add_animation(jump_anim);

	Animation aiming_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/Rifle Aiming Idle.fbx", aiming_anim);
	animator->add_animation(aiming_anim);

	Animation shoot_anim = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/Firing Rifle (1).fbx", shoot_anim);
	animator->add_animation(shoot_anim);

	Animation roll_anim = Animation(&model->skeleton);
	roll_anim.set_root_bone("Hips");
	roll_anim.constrain_axes(glm::vec3(1, 0, 1));

	AssetLoader::load_animation("Models/roll_forward.fbx", roll_anim);
	animator->add_animation(roll_anim);

	Animation crouch_idle = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/crouch_idle.fbx", crouch_idle);
	animator->add_animation(crouch_idle);

	Animation crouch_walk = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/crouch_walk.fbx", crouch_walk);
	animator->add_animation(crouch_walk);

	Animation cover_idle = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cover_idle.fbx", cover_idle);
	animator->add_animation(cover_idle);

	Animation cover_left = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cover_left.fbx", cover_left);
	animator->add_animation(cover_left);

	Animation cover_right = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cover_right.fbx", cover_right);
	animator->add_animation(cover_right);

	Animation cover_idle_opposite = Animation(&model->skeleton);
	AssetLoader::load_animation("Models/cover_idle_opposite.fbx", cover_idle_opposite);
	animator->add_animation(cover_idle_opposite);

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

Entity& Spawn::spawn_image(glm::vec3 position, GLfloat width, GLfloat height, std::string path)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Image>();
	entity->get_component<Image>()->set_size(width, height);
	entity->get_component<Image>()->set_image_path(path);
	EntityManager::add_ui_element(std::move(entity));
	return *EntityManager::get_last_ui_element();
}

Entity& Spawn::spawn_bullet(glm::vec3 position, glm::vec3 velocity, glm::vec3 end, std::string path)
{
	std::unique_ptr<Entity> entity(new Entity());
	entity->transform.global_translate(position);
	entity->add_component<Image>();
	entity->get_component<Image>()->set_size(1.0f, 0.05f);
	entity->get_component<Image>()->material = 2;
	entity->add_component<BulletController>();
	entity->get_component<BulletController>()->set_velocity(velocity);
	entity->get_component<BulletController>()->set_endpoint(end);
	entity->get_component<Image>()->set_image_path(path);
	EntityManager::add_entity(std::move(entity));
	return *EntityManager::get_last();
}

