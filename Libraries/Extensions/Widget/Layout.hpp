// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

namespace DockMode
{
typedef uint Type;
static const cstr EnumName = "DockMode";
enum Enum
{
  DockNone = 0,
  DockLeft = (1 << 1),
  DockTop = (1 << 2),
  DockRight = (1 << 3),
  DockBottom = (1 << 4),
  DockFill = (1 << 5),
};
enum
{
  Size = 6
};
static const cstr Names[] = {"DockNone", "DockLeft", "DockTop", "DockRight", "DockBottom", "DockFill", NULL};
static const uint Values[] = {DockNone, DockLeft, DockTop, DockRight, DockBottom, DockFill};
} // namespace DockMode

// DeclareEnum6(DockMode, DockNone, DockLeft, DockTop, DockRight, DockBottom,
// DockFill);

DeclareEnum9(DockArea, Top, Bottom, Left, Right, Center, Count, TopTool, BotTool, Floating);

// Direction of layout needs to match
// Dock area
DeclareEnum4(LayoutDirection, TopToBottom, BottomToTop, LeftToRight, RightToLeft);

// Get the axis related to that dock area
uint GetAxis(DockArea::Enum dockArea);

// Get the sign for that dock area, is the area
// in the positive or negative direction on its axis?
// What is the sign towards that area
int GetSign(DockArea::Enum dockArea);

DeclareEnum2(LimitMode, Limited, Unlimited);

struct LayoutArea
{
  ZilchDeclareType(LayoutArea, TypeCopyMode::ValueType);

  LimitMode::Enum HorizLimit = LimitMode::Unlimited;
  LimitMode::Enum VerticalLimit = LimitMode::Unlimited;

  Vec2 Size = Vec2::cZero;
  Vec3 Offset = Vec3::cZero;
};

class Composite;
class Widget;
struct Thickness;

struct LayoutResult
{
  Widget* PlacedWidget = nullptr;
  Vec3 Translation = Vec3::cZero;
  Vec2 Size = Vec2::cZero;
};

LayoutResult AspectLayout(Vec2 aspect, Vec2 size);

/// Layout class used to layout widgets on a composite widget.
class Layout
{
public:
  ZilchDeclareType(Layout, TypeCopyMode::ReferenceType);

  Layout(Thickness padding = Thickness::cZero);

  virtual Vec2 Measure(Composite* widget, LayoutArea data) = 0;
  virtual Vec2 DoLayout(Composite* widget, LayoutArea data) = 0;
  virtual ~Layout()
  {
  }

  Thickness Padding;
  bool mDebug;
};

class FillLayout : public Layout
{
public:
  ZilchDeclareType(FillLayout, TypeCopyMode::ReferenceType);

  FillLayout(Thickness padding = Thickness::cZero);
  Vec2 Measure(Composite* widget, LayoutArea data) override;
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;
};

class StackLayout : public Layout
{
public:
  ZilchDeclareType(StackLayout, TypeCopyMode::ReferenceType);

  StackLayout(Thickness padding = Thickness::cZero);
  StackLayout(LayoutDirection::Enum direction, Vec2 spacing = Vec2::cZero, Thickness padding = Thickness::cZero);

  Vec2 Measure(Composite* widget, LayoutArea data) override;
  float ComputeFlexRatio(float fixedSize, float totalFlex, float flexMinSize, float totalSize);
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;

  static StackLayout* CreateRowLayout();

  LayoutDirection::Enum Direction;
  Vec2 Spacing;
};

class EdgeDockLayout : public Layout
{
public:
  ZilchDeclareType(EdgeDockLayout, TypeCopyMode::ReferenceType);

  Vec2 Measure(Composite* widget, LayoutArea data) override;
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;
};

class DockLayout : public Layout
{
public:
  ZilchDeclareType(DockLayout, TypeCopyMode::ReferenceType);

  DockLayout(Thickness padding = Thickness::cZero);

  Vec2 Measure(Composite* widget, LayoutArea data) override;
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;
};

class RatioLayout : public Layout
{
public:
  ZilchDeclareType(RatioLayout, TypeCopyMode::ReferenceType);

  RatioLayout(Thickness padding = Thickness::cZero);

  Vec2 Measure(Composite* widget, LayoutArea data) override;
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;

  DockArea::Enum mAlignment;
};

class GridLayout : public Layout
{
public:
  ZilchDeclareType(GridLayout, TypeCopyMode::ReferenceType);

  GridLayout(Thickness padding = Thickness::cZero);

  struct TilePlacement
  {
    Widget* Widget;
    Vec2 Size;
    Vec2 Position;
    bool Overlaps(const TilePlacement& rhs);
  };

  Vec2 Measure(Composite* widget, LayoutArea data) override;
  Vec2 DoLayout(Composite* widget, LayoutArea data) override;

  float CellSize;

private:
  void PlaceTile(TilePlacement& placement);
  int FindOverlappingTile(TilePlacement& placement);
  void CellClamp(Vec2& vec);

  Vec2 mSize;
  Array<TilePlacement> mPlacedTiles;
};

Layout* CreateStackLayout(LayoutDirection::Enum direction,
                          Vec2Param spacing = Vec2::cZero,
                          const Thickness& padding = Thickness::cZero);
Layout* CreateStackLayout();
Layout* CreateRowLayout();
Layout* CreateFillLayout(Thickness padding = Thickness::cZero);

Layout* CreateDockLayout();
Layout* CreateDockLayout(const Thickness& padding);
Layout* CreateEdgeDockLayout();
Layout* CreateRatioLayout();

void PlaceWithLayout(LayoutResult& result, Widget* widget);

LayoutResult RemoveThickness(Thickness thickness, Vec2Param outerSize, Vec3Param offset = Vec3::cZero);

} // namespace Zero
