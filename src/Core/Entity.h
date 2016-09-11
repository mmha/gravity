#pragma once

#include "Attribute.h"
#include "internal/Hash.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>

#define GE_DECLARE_ATTRIBUTE_ACCESSOR(attribType, abbr) \
	struct INTERNAL_ACCESSOR_##abbr { \
		using type = attribType; \
		uint32_t hashcode; \
	}; \
	inline namespace literals { \
		constexpr INTERNAL_ACCESSOR_##abbr operator "" _##abbr(const char *str, size_t len) { \
			return {::ge::Core::crc32(str, len)}; \
		} \
	} \

namespace ge::Core
{
	class EntityBase
	{
		public:
		virtual void update(float dt) = 0;
		inline virtual void onMessage([[maybe_unused]] uint64_t code, [[maybe_unused]] void *data) {};
		virtual ~EntityBase() = default;
	};

	template<typename... Behaviours>
	class Entity final : public EntityBase, Behaviours...
	{
		Entity(Behaviours &&... behaviours)
		: Behaviours{behaviours}...
		{
			int{ (Behaviours::setParent(this),0)... };
		}

		~Entity() = default;

		void update(float dt) override
		{
			int {(Behaviours::update(dt),0)...};

			for(auto &c : children)
			{
				c.update(dt);
			}
		}

		template<typename T>
		typename T::type &operator[](T accessor)
		{
			return static_cast<T &>(*attributes[accessor.hashcode]);
		}

		template<typename T>
		const typename T::type &operator[](T accessor) const
		{
			return static_cast<T &>(*attributes[accessor.hashcode]);
		}

		std::unordered_map<uint32_t, std::unique_ptr<AttributeBase>> attributes;
		std::vector<Entity> children;
	};

	inline namespace literals
	{
		constexpr uint32_t operator"" _msg(const char *str, size_t len)
		{
			return crc32(str, len);
		}
	}
}
