#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>

namespace zt
{
	template<typename T>
	using Vector2 = glm::vec<2, T, glm::defaultp>;
	using Vector2f = glm::vec<2, float, glm::defaultp>;
	using Vector2d = glm::vec<2, double, glm::defaultp>;
	using Vector2i = glm::vec<2, int, glm::defaultp>;
	using Vector2ui = glm::vec<2, unsigned int, glm::defaultp>;

	template<typename T>
	using Vector3 = glm::vec<3, T, glm::defaultp>;
	using Vector3f = glm::vec<3, float, glm::defaultp>;
	using Vector3d = glm::vec<3, double, glm::defaultp>;
	using Vector3i = glm::vec<3, int, glm::defaultp>;
	using Vector3ui = glm::vec<3, unsigned int, glm::defaultp>;

	template<typename T>
	using Vector4 = glm::vec<4, T, glm::defaultp>;
	using Vector4f = glm::vec<4, float, glm::defaultp>;
	using Vector4d = glm::vec<4, double, glm::defaultp>;
	using Vector4i = glm::vec<4, int, glm::defaultp>;
	using Vector4ui = glm::vec<4, unsigned int, glm::defaultp>;

	using Matrix4f = glm::mat4;

}