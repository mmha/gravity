#include "DrawableBehaviour.h"
#include "Renderer.h"

using namespace ge;
using namespace ge::gl4;

DrawableBehaviour::DrawableBehaviour(Mesh &&mesh, Renderer &renderer)
:	renderer_(renderer)
,	mesh_(std::move(mesh))
{

}

void DrawableBehaviour::onUpdate(float dt)
{
	// Simple rendering is time invariant
	(void) dt;
	mesh_.draw();
}

void ge::gl4::DrawableBehaviour::setParent(Core::EntityBase *)
{
}
