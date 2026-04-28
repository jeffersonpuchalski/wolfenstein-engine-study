#pragma once

template<typename TEntity>
class IComponentPool
{
public:
	virtual ~IComponentPool() = default;
	virtual void remove(TEntity e) = 0;
};
