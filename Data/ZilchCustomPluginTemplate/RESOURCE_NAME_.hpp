#pragma once

// For more information on binding and using Zilch APIs, visit: http://zilch.digipen.edu/
// For auto binding specifically, visit: http://zilch.digipen.edu/home/AutomaticBinding.html

// An example component being bound to the engine
class RESOURCE_NAME_ : public ZeroEngine::ZilchComponent
{
public:
  ZilchDeclareType(RESOURCE_NAME_, Zilch::TypeCopyMode::ReferenceType);
  
  RESOURCE_NAME_();
  ~RESOURCE_NAME_();
  
  void Initialize(ZeroEngine::CogInitializer* initializer);
  
  void OnLogicUpdate(ZeroEngine::UpdateEvent* event);
  
  // A method that we want to expose to script
  Zilch::String Speak();
  
  // A field that we want to expose to script
  int mLives;
  
  // A getter/setter that we want to expose to script
  float GetHealth();
  void SetHealth(float value);
  
private:
  float mHealth;
};

// An example of a custom event that we can send
class RESOURCE_NAME_Event : public ZeroEngine::ZilchEvent
{
public:
  ZilchDeclareType(RESOURCE_NAME_Event, Zilch::TypeCopyMode::ReferenceType);
  
  int mLives;
};
