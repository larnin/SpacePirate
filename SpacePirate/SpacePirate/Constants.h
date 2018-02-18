#pragma once

#include <Nazara/Core/Flags.hpp>

namespace Constants
{
	constexpr unsigned int pixelsByUnit = 32;
}

/*
CollisionId : Utiliser lors des callbacks
CollisionGroup : Les colliders dans le m�me groupe n'ont pas de collision entre eux (except� pour le groupe z�ro)
CategoryMask : Ce collider appartient � telles cat�gories (chaque bit �tant une cat�gorie)
CollisionMask : Ce collider entre en collisions avec les colliders de telles cat�gories (chaque bit �tant une cat�gorie aussi)
*/

namespace CollisionId
{
	enum CollisionId
	{
		Ground,
		Water,
		Player,
	};
}

namespace CollisionGroup
{
	enum CollisionGroup
	{
		Zero = 0,
		Ground,
		Water,
		Player,
	};
}

enum class CategoryMask
{
	Ground,
	Water,
	Player,

	Max = Player,
};

template<>
struct Nz::EnumAsFlags<CategoryMask>
{
	static constexpr CategoryMask max = CategoryMask::Max;
};

using CategoryMaskFlags = Nz::Flags<CategoryMask>;