#include "physics_debug_drawer.hpp"

PhysicsDebugDrawer::PhysicsDebugDrawer()
{
	debug_mode = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	if (debug_mode > 0)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glm::vec3 start = glm::vec3();
		start.x = from.getX();
		start.y = from.getY();
		start.z = from.getZ();

		glm::vec3 end = glm::vec3();
		end.x = to.getX();
		end.y = to.getY();
		end.z = to.getZ();

		vertex_data.push_back(start);
		vertex_data.push_back(end);

		glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(glm::vec3), &vertex_data[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		glDrawArrays(GL_LINES, vertex_data.size()-2, 2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	debug_mode = debugMode;
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	btVector3 to = pointOnB + normalOnB*distance;
	const btVector3&from = pointOnB;

	drawLine(from, to, color);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString) {}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	std::cout << warningString << std::endl;
}

void PhysicsDebugDrawer::clearLines()
{
	vertex_data.clear();
}