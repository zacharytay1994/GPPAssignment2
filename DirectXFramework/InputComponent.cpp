#include "InputComponent.h"
#include "Entity.h"

InputComponent::InputComponent(std::shared_ptr<Input> input, Entity * entity)
	:
	Component(entity),
	input_(input)
{
}