// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class Cog;

// Conditional policy for comparing names of cogs
struct NameCondition
{
  // Comparison operator (checks the names of the cogs against the stored name)
  bool operator()(const Cog& cog);
  bool operator()(const Cog* cog);

  // What we're comparing against
  String Name;
};

// Conditional policy that checks for root cogs in a space.
struct RootCondition
{
  bool operator()(const Cog& cog);
  bool operator()(const Cog* cog);
};

// Conditional policy for comparing names of root cogs.
struct RootNameCondition : public RootCondition, public NameCondition
{
  bool operator()(const Cog& cog);
  bool operator()(const Cog* cog);
};

/// Range to visit all objects in a Hierarchy.
class HierarchyRange
{
public:
  typedef Cog value_type;
  typedef Cog& ref_type;
  typedef Cog& reference;
  typedef ref_type FrontResult;
  // All in entire tree (including those above)
  static HierarchyRange EntireTree(Cog* object);
  // All in sub tree (not including the object)
  static HierarchyRange SubTree(Cog* object);
  HierarchyRange();
  // Range functions
  bool Empty();
  void PopFront();
  ref_type Front();

private:
  HierarchyRange(Cog* object);
  static Cog* GetRoot(Cog* object);
  static Cog* MostLeft(Cog* object);
  Cog* mCurrent;
  Cog* mStarting;
};

} // namespace Zero
