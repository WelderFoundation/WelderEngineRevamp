// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIWidget, builder, type)
{
  ZeroBindDocumented();
  ZeroBindComponent();
  ZeroBindSetup(SetupMode::DefaultSerialization);
}

void TBUIWidget::Serialize(Serializer& stream)
{
}

void TBUIWidget::Initialize(CogInitializer& initializer)
{
}
} // namespace Zero