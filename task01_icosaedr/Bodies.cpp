#include "stdafx.h"
#include "Bodies.h"
#include <stdint.h>

namespace
{
	
typedef glm::vec3 Vertex;
const glm::vec3 RED = { 1.f, 0.f, 0.f };
const glm::vec3 GREEN = { 0.f, 1.f, 0.f };
const glm::vec3 BLUE = { 0.f, 0.f, 1.f };
const glm::vec3 MAGENTA = { 0.6f, 0.f, 0.3f };
const glm::vec3 CYAN = { 0.f, 0.5f, 0.7f };
const glm::vec3 GREY = { 0.9f, 0.92f, 0.95f };
const glm::vec3 GOLD = { 0.98f, 0.85f, 0.42f };

const glm::vec3 colors[] = { RED , GREEN, BLUE, MAGENTA, CYAN, GREY, GOLD };

enum class Colors
{
	Red = 0,
	Green,
	Blue,
	Magenta,
	Cyan,
	Grey,
	Gold,

	NumColors
};


const GLfloat SPHERE_RADIUS = 1;
const double ICOSAHEDRON_SIDE = 4 * SPHERE_RADIUS / sqrt(10 + 2 * sqrt(5));
const double ANGLE = acos((1 - ICOSAHEDRON_SIDE*ICOSAHEDRON_SIDE / 2 / SPHERE_RADIUS / SPHERE_RADIUS)); // первый угол поворота по тэта 
const double ANGLE_TO_RAD = M_PI / 180;

const Vertex ICOSAHEDRON_VERTICIES[] =
{
	{0, 0, SPHERE_RADIUS },
	{ SPHERE_RADIUS*sin(ANGLE)*sin(0), SPHERE_RADIUS*sin(ANGLE)*cos(0), SPHERE_RADIUS*cos(ANGLE) },
	{ SPHERE_RADIUS*sin(ANGLE)*sin(72 * ANGLE_TO_RAD) , SPHERE_RADIUS*sin(ANGLE)*cos(72 * ANGLE_TO_RAD) , SPHERE_RADIUS*cos(ANGLE) },
	{ SPHERE_RADIUS*sin(ANGLE)*sin(2 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*sin(ANGLE)*cos(2 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*cos(ANGLE) },
	{ SPHERE_RADIUS*sin(ANGLE)*sin(3 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*sin(ANGLE)*cos(3 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*cos(ANGLE) },
	{ SPHERE_RADIUS*sin(ANGLE)*sin(4 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*sin(ANGLE)*cos(4 * 72 * ANGLE_TO_RAD) , SPHERE_RADIUS*cos(ANGLE) },

	{ SPHERE_RADIUS*sin(M_PI - ANGLE)*sin(-36 * ANGLE_TO_RAD), SPHERE_RADIUS*sin(M_PI - ANGLE)*cos(-36 * ANGLE_TO_RAD), SPHERE_RADIUS*cos(M_PI - ANGLE) },
	{ SPHERE_RADIUS*sin(M_PI - ANGLE)*sin(36 * ANGLE_TO_RAD), SPHERE_RADIUS*sin(M_PI - ANGLE)*cos(36 * ANGLE_TO_RAD), SPHERE_RADIUS*cos(M_PI - ANGLE) },
	{ SPHERE_RADIUS*sin(M_PI - ANGLE)*sin((36 + 72)* ANGLE_TO_RAD), SPHERE_RADIUS*sin(M_PI - ANGLE)*cos((36 + 72) * ANGLE_TO_RAD), SPHERE_RADIUS*cos(M_PI - ANGLE) },
	{ SPHERE_RADIUS*sin(M_PI - ANGLE)*sin((36 + 2*72)* ANGLE_TO_RAD), SPHERE_RADIUS*sin(M_PI - ANGLE)*cos((36 + 2*72) * ANGLE_TO_RAD), SPHERE_RADIUS*cos(M_PI - ANGLE) },
	{ SPHERE_RADIUS*sin(M_PI - ANGLE)*sin((36 + 3 * 72)* ANGLE_TO_RAD), SPHERE_RADIUS*sin(M_PI - ANGLE)*cos((36 + 3 * 72) * ANGLE_TO_RAD), SPHERE_RADIUS*cos(M_PI - ANGLE) },

	{0, 0, -SPHERE_RADIUS}
};


struct STriangleFace
{
	uint16_t vertexIndex1;
	uint16_t vertexIndex2;
	uint16_t vertexIndex3;
	uint16_t colorIndex;
};


const STriangleFace ICOSAHEDRON_FACES[] = 
{

	{ 0, 2, 1, static_cast<uint16_t>(Colors::Red) },
	{ 0, 3, 2, static_cast<uint16_t>(Colors::Green) },
	{ 0, 4, 3, static_cast<uint16_t>(Colors::Blue) },
	{ 0, 5, 4, static_cast<uint16_t>(Colors::Cyan) },
	{ 0, 1, 5, static_cast<uint16_t>(Colors::Magenta) },
	{ 6, 1, 7, static_cast<uint16_t>(Colors::Grey) },
	{ 7, 1, 2, static_cast<uint16_t>(Colors::Cyan) },
	{ 7, 2, 8, static_cast<uint16_t>(Colors::Green) },
	{ 8, 2, 3, static_cast<uint16_t>(Colors::Blue) },
	{ 8, 3, 9, static_cast<uint16_t>(Colors::Cyan) },
	{ 9, 3, 4, static_cast<uint16_t>(Colors::Magenta) },
	{ 9, 4, 10, static_cast<uint16_t>(Colors::Grey) },
	{ 10, 4, 5, static_cast<uint16_t>(Colors::Red) },
	{ 10, 5, 6, static_cast<uint16_t>(Colors::Green) },
	{ 6, 5, 1, static_cast<uint16_t>(Colors::Blue) },
	{ 7, 11, 6, static_cast<uint16_t>(Colors::Cyan) },
	{ 8, 11, 7, static_cast<uint16_t>(Colors::Magenta) },
	{ 9, 11, 8, static_cast<uint16_t>(Colors::Grey) },
	{ 10, 11, 9, static_cast<uint16_t>(Colors::Red) },
	{ 6, 11, 10, static_cast<uint16_t>(Colors::Gold) },

};

}

CIcosahedron::CIcosahedron()
	: m_alpha(0)
{
}

void CIcosahedron::Update(float deltaTime)
{
    (void)deltaTime;
}

void CIcosahedron::Draw() const
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBegin(GL_TRIANGLES);
	OutputFaces();
	glEnd();		  

}

void CIcosahedron::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

void CIcosahedron::OutputFaces() const
{
	
	for (const STriangleFace &face : ICOSAHEDRON_FACES)
	{
		const Vertex &v1 = ICOSAHEDRON_VERTICIES[face.vertexIndex1];
		const Vertex &v2 = ICOSAHEDRON_VERTICIES[face.vertexIndex2];
		const Vertex &v3 = ICOSAHEDRON_VERTICIES[face.vertexIndex3];
		glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
		glm::vec3 color = colors[face.colorIndex];

		glColor4f(color.x, color.y, color.z, m_alpha);
		glNormal3fv(glm::value_ptr(normal));
		glVertex3fv(glm::value_ptr(v1));
		glVertex3fv(glm::value_ptr(v2));
		glVertex3fv(glm::value_ptr(v3));
	}	 

}


