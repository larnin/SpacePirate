#pragma once

#include <Nazara/Core/Flags.hpp>

namespace Constants
{
	constexpr unsigned int pixelsByUnit = 32;
}

/*
CollisionId : Utiliser lors des callbacks
CollisionGroup : Les colliders dans le même groupe n'ont pas de collision entre eux (excepté pour le groupe zéro)
CategoryMask : Ce collider appartient à telles catégories (chaque bit étant une catégorie)
CollisionMask : Ce collider entre en collisions avec les colliders de telles catégories (chaque bit étant une catégorie aussi)
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