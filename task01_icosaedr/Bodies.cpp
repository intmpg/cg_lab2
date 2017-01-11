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


const GLfloat RADIUS_SPHERE = 0.75;
const double ICOSAHEDRON_SIDE = 4 * RADIUS_SPHERE / sqrt(10 + 2 * sqrt(5));
const double ALPHA = acos((1 - ICOSAHEDRON_SIDE*ICOSAHEDRON_SIDE / 2 / RADIUS_SPHERE / RADIUS_SPHERE)); // первый угол поворота по тэта 
const double k = M_PI / 180;

const Vertex ICOSAHEDRON_VERTICIES[] =
{
	{0, 0, RADIUS_SPHERE },
	{ RADIUS_SPHERE*sin(ALPHA)*sin(0), RADIUS_SPHERE*sin(ALPHA)*cos(0), RADIUS_SPHERE*cos(ALPHA) },
	{ RADIUS_SPHERE*sin(ALPHA)*sin(72 * k) , RADIUS_SPHERE*sin(ALPHA)*cos(72 * k) , RADIUS_SPHERE*cos(ALPHA) },
	{ RADIUS_SPHERE*sin(ALPHA)*sin(2 * 72 * k) , RADIUS_SPHERE*sin(ALPHA)*cos(2 * 72 * k) , RADIUS_SPHERE*cos(ALPHA) },
	{ RADIUS_SPHERE*sin(ALPHA)*sin(3 * 72 * k) , RADIUS_SPHERE*sin(ALPHA)*cos(3 * 72 * k) , RADIUS_SPHERE*cos(ALPHA) },
	{ RADIUS_SPHERE*sin(ALPHA)*sin(4 * 72 * k) , RADIUS_SPHERE*sin(ALPHA)*cos(4 * 72 * k) , RADIUS_SPHERE*cos(ALPHA) },

	{ RADIUS_SPHERE*sin(M_PI - ALPHA)*sin(-36 * k), RADIUS_SPHERE*sin(M_PI - ALPHA)*cos(-36 * k), RADIUS_SPHERE*cos(M_PI - ALPHA) },
	{ RADIUS_SPHERE*sin(M_PI - ALPHA)*sin(36 * k), RADIUS_SPHERE*sin(M_PI - ALPHA)*cos(36 * k), RADIUS_SPHERE*cos(M_PI - ALPHA) },
	{ RADIUS_SPHERE*sin(M_PI - ALPHA)*sin((36 + 72)* k), RADIUS_SPHERE*sin(M_PI - ALPHA)*cos((36 + 72) * k), RADIUS_SPHERE*cos(M_PI - ALPHA) },
	{ RADIUS_SPHERE*sin(M_PI - ALPHA)*sin((36 + 2*72)* k), RADIUS_SPHERE*sin(M_PI - ALPHA)*cos((36 + 2*72) * k), RADIUS_SPHERE*cos(M_PI - ALPHA) },
	{ RADIUS_SPHERE*sin(M_PI - ALPHA)*sin((36 + 3 * 72)* k), RADIUS_SPHERE*sin(M_PI - ALPHA)*cos((36 + 3 * 72) * k), RADIUS_SPHERE*cos(M_PI - ALPHA) },

	{0, 0, -RADIUS_SPHERE}
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

CIcosaedr::CIcosaedr()
	: m_alpha(1)
{
}

void CIcosaedr::Update(float deltaTime)
{
    (void)deltaTime;
}

void CIcosaedr::Draw() const
{
	if (m_alpha < 0.99f)
	{
		glFrontFace(GL_CW);
		OutputFaces();
		glFrontFace(GL_CCW);
	}
	OutputFaces();
}

void CIcosaedr::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

void CIcosaedr::OutputFaces() const
{
	glBegin(GL_TRIANGLES);
	
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
	glEnd();
}


