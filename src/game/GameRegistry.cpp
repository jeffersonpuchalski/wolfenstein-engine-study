#include <game/GameRegistry.hpp>


void GameRegistry::addAsset(Component component)
{
    registry.addComponent(component.getId(), component);
}

void GameRegistry::deleteAsset(Component component)
{
    registry.deleteComponent(component.getId(), component);
}
