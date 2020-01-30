#include "ECS.h"

void Entity::addGroup(Group group)
{
	m_groupBitSet[group] = true;
	m_manager.addToGroup(this, group);
}

