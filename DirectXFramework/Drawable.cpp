#include "Drawable.h"

std::vector<std::unique_ptr<Bindable>> Drawable::staticBinds_;

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;