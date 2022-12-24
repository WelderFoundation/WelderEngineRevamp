// MIT Licensed (see LICENSE.md).
#pragma once

#include "Common/CommonStandard.hpp"
#include "Platform/PlatformStandard.hpp"
#include "Serialization/SerializationStandard.hpp"
#include "Meta/MetaStandard.hpp"
#include "Support/SupportStandard.hpp"

namespace Zero
{
// Forward declarations
class Cog;
class Event;
class Component;
class Serializer;
class Level;
class Space;
class GameSession;
class GameWidget;
class Transform;
class ContentLibrary;
class ResourcePackage;
class ResourceLibrary;
class ProjectSettings;
class Hierarchy;
class KeyboardEvent;
class UpdateEvent;
class ActionSpace;
class SavingEvent;
class DocumentResource;

class ContentLibrary;
class ContentItem;
class ContentComponent;
class BuildOptions;
class ContentComposition;

// Engine library
class ZeroNoImportExport EngineLibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(EngineLibrary, "ZeroEngine");

  static bool Initialize();
  static void Shutdown();

private:
};

} // namespace Zero

#include "Rectangle.hpp"
#include "Resource/Resource.hpp"
#include "EngineBindingExtensions.hpp"
#include "EngineObject.hpp"
#include "EngineContainers.hpp"
#include "EngineMath.hpp"
#include "CogId.hpp"
#include "HierarchyRange.hpp"
#include "Cog.hpp"
#include "Component.hpp"
#include "ComponentMeta.hpp"
#include "CogMetaComposition.hpp"
#include "CogMeta.hpp"
#include "Space.hpp"
#include "DocumentResource.hpp"
#include "Scripting/ZilchResource.hpp"
#include "Resource/ResourceLibrary.hpp"
#include "JobSystem.hpp"
#include "EngineEvents.hpp"
#include "System.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include "ThreadDispatch.hpp"
#include "Game.hpp"
#include "Factory.hpp"
#include "ArchetypeRebuilder.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Resource/ResourcePropertyOperations.hpp"
#include "ErrorContext.hpp"
#include "Tracker.hpp"
#include "Hierarchy.hpp"
#include "TransformSupport.hpp"
#include "Transform.hpp"
#include "Action/Action.hpp"
#include "Action/ActionSystem.hpp"
#include "Action/ActionEase.hpp"
#include "Action/MetaAction.hpp"
#include "Action/BasicActions.hpp"
#include "Action/ActionGenerator.hpp"
#include "EditorSupport.hpp"
#include "CogSerialization.hpp"
#include "ObjectLoader.hpp"
#include "ObjectSaver.hpp"
#include "CogInitializer.hpp"
#include "CogPath.hpp"
#include "ObjectLink.hpp"
#include "Platform/Input/Keyboard.hpp"
#include "Environment.hpp"
#include "SystemObjectManager.hpp"
#include "Platform/OsWindow.hpp"
#include "Platform/OsShell.hpp"
#include "Resource/ResourceManager.hpp"
#include "Archetype.hpp"
#include "Platform/Input/Mouse.hpp"
#include "Level.hpp"
#include "Operation.hpp"
#include "Resource/ResourceListOperation.hpp"
#include "MetaOperations.hpp"
#include "CogRestoreState.hpp"
#include "CogOperations.hpp"
#include "Animation/AnimationNode.hpp"
#include "Animation/PropertyTrack.hpp"
#include "Animation/AnimationGraph.hpp"
#include "Animation/AnimationGraphEvents.hpp"
#include "Animation/Animation.hpp"
#include "CogSelection.hpp"
#include "Configuration.hpp"
#include "LauncherConfiguration.hpp"
#include "DebugDraw.hpp"
#include "Scripting/ScriptEvents.hpp"
#include "TextResource.hpp"
#include "EditorSettings.hpp"
#include "Documentation.hpp"
#include "ShortcutsDoc.hpp"
#include "ObjectStore.hpp"
#include "ResourceTable.hpp"
#include "SampleCurve.hpp"
#include "Noise.hpp"
#include "HeightMap.hpp"
#include "HeightMapSource.hpp"
#include "Gradient.hpp"
#include "ColorGradient.hpp"
#include "Area.hpp"
#include "Tweakables.hpp"
#include "SceneGraph.hpp"
#include "ProxyObject.hpp"
#include "Project.hpp"
#include "RaycastProvider.hpp"
#include "SimpleResourceFactory.hpp"
#include "Platform/Input/GamePadSystem.hpp"
#include "Platform/Input/JoystickSystem.hpp"
#include "EventDirectoryWatcher.hpp"
#include "CogRange.hpp"
#include "CogHelpers.hpp"
#include "ComponentHierarchy.hpp"
#include "DataSource.hpp"
#include "EngineLibraryExtensions.hpp"
#include "Scripting/ZilchManager.hpp"
#include "Spline.hpp"
#include "HierarchySpline.hpp"
#include "AsyncProcess.hpp"
#include "CopyOnWrite.hpp"

// Co-dependent libraries
//#include "Systems/Content/ContentStandard.hpp"

#include "Content/ContentItem.hpp"
#include "Content/ContentLibrary.hpp"
#include "Content/BuildOptions.hpp"
#include "Content/ContentSystem.hpp"
#include "Content/ContentUtility.hpp"
#include "Content/ContentComposition.hpp"
#include "Content/DataContent.hpp"
#include "Content/TagsContent.hpp"
#include "Content/BaseBuilders.hpp"
#include "Content/VectorContent.hpp"
#include "Content/ArchetypeProcessor.hpp"
#include "Content/SupportComponents.hpp"
#include "Content/RichAnimation.hpp"
#include "Content/BinaryContent.hpp"
#include "Content/ResourceLoaderTemplates.hpp"
