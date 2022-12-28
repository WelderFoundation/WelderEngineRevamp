// MIT Licensed (see LICENSE.md).
#pragma once

#include "Common/CommonStandard.hpp"
#include "PlatformStandard.hpp"
#include "Serialization/SerializationStandard.hpp"
#include "Meta/MetaStandard.hpp"
#include "Support/SupportStandard.hpp"
#include "Image/ImageStandard.hpp"

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
#include "Resource.hpp"
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
#include "ZilchResource.hpp"
#include "ResourceLibrary.hpp"
#include "JobSystem.hpp"
#include "EngineEvents.hpp"
#include "System.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include "ThreadDispatch.hpp"
#include "Game.hpp"
#include "Factory.hpp"
#include "ArchetypeRebuilder.hpp"
#include "ResourceSystem.hpp"
#include "ResourcePropertyOperations.hpp"
#include "ErrorContext.hpp"
#include "Tracker.hpp"
#include "Hierarchy.hpp"
#include "TransformSupport.hpp"
#include "Transform.hpp"
#include "Action.hpp"
#include "ActionSystem.hpp"
#include "ActionEase.hpp"
#include "MetaAction.hpp"
#include "BasicActions.hpp"
#include "ActionGenerator.hpp"
#include "EditorSupport.hpp"
#include "CogSerialization.hpp"
#include "ObjectLoader.hpp"
#include "ObjectSaver.hpp"
#include "CogInitializer.hpp"
#include "CogPath.hpp"
#include "ObjectLink.hpp"
#include "Keyboard.hpp"
#include "Environment.hpp"
#include "SystemObjectManager.hpp"
#include "OsWindow.hpp"
#include "OsShell.hpp"
#include "ResourceManager.hpp"
#include "Archetype.hpp"
#include "Mouse.hpp"
#include "Level.hpp"
#include "Operation.hpp"
#include "ResourceListOperation.hpp"
#include "MetaOperations.hpp"
#include "CogRestoreState.hpp"
#include "CogOperations.hpp"
#include "AnimationNode.hpp"
#include "PropertyTrack.hpp"
#include "AnimationGraph.hpp"
#include "AnimationGraphEvents.hpp"
#include "Animation.hpp"
#include "CogSelection.hpp"
#include "Configuration.hpp"
#include "LauncherConfiguration.hpp"
#include "DebugDraw.hpp"
#include "ScriptEvents.hpp"
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
#include "GamePadSystem.hpp"
#include "JoystickSystem.hpp"
#include "EventDirectoryWatcher.hpp"
#include "CogRange.hpp"
#include "CogHelpers.hpp"
#include "ComponentHierarchy.hpp"
#include "DataSource.hpp"
#include "EngineLibraryExtensions.hpp"
#include "ZilchManager.hpp"
#include "Spline.hpp"
#include "HierarchySpline.hpp"
#include "AsyncProcess.hpp"
#include "CopyOnWrite.hpp"

// Co-dependent libraries
#include "Content/ContentStandard.hpp"
