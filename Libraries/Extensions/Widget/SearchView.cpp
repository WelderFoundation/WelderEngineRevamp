// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

namespace SearchViewUi
{
const cstr cLocation = "EditorUi/SearchView";
Tweakable(Vec4, BackgroundColor, Vec4(1, 1, 1, 1), cLocation);
Tweakable(Vec4, PrimaryColor, Vec4(1, 1, 1, 1), cLocation);
Tweakable(Vec4, SecondaryColor, Vec4(1, 1, 1, 1), cLocation);
Tweakable(Vec4, HoverColor, Vec4(1, 1, 1, 1), cLocation);
Tweakable(Vec4, InvalidTextColor, Vec4(1, 1, 1, 1), cLocation);
Tweakable(Vec4, InvalidTipBorder, Vec4(1, 1, 1, 1), cLocation);
Tweakable(float, RowSize, Pixels(20), cLocation);
} // namespace SearchViewUi

namespace Events
{
DefineEvent(SearchCanceled);
DefineEvent(SearchPreview);
DefineEvent(SearchCompleted);
DefineEvent(AlternateSearchCompleted);
} // namespace Events

ZilchDefineType(SearchViewEvent, builder, type)
{
}

ZilchDefineType(AlternateSearchCompletedEvent, builder, type)
{
}

class TagProvider : public SearchProvider
{
public:
  TagProvider() : SearchProvider("Tag")
  {
  }

  String GetElementType(SearchViewResult& element) override
  {
    const String tag = "Tag";
    return tag;
  }

  bool OnMatch(SearchView* searchView, SearchViewResult& element) override
  {
    // Tags don't have the provider type in the TagChain (unlike Libraries).
    searchView->AddTag(element.Name);
    return false;
  }
};

static TagProvider sTagProviderInterface;

struct SortByPriority
{
  bool operator()(const SearchViewResult& left, const SearchViewResult& right)
  {
    if (left.Priority == right.Priority)
      return left.Name < right.Name;
    else
      return left.Priority > right.Priority;
  }
};

bool SearchProvider::FilterResult(SearchViewResult& result)
{
  if (mFilter)
    return mFilter->FilterResult(result);
  return true;
}

SearchData::~SearchData()
{
  ClearSearchProviders();
}

void SearchData::Search()
{
  forRange (SearchProvider* provider, SearchProviders.All())
  {
    provider->Search(*this);
  }
}

void SearchData::AddAvailableTagsToResults()
{
  forRange (StringParam tag, AvailableTags.All())
  {
    int priority = PartialMatch(SearchString.All(), tag.All(), CaseInsensitiveCompare);
    if (priority != cNoMatch && !ActiveTags.Contains(tag))
    {
      SearchViewResult& results = Results.PushBack();
      results.Data = (void*)tag.Hash();
      results.Interface = &sTagProviderInterface;
      results.Name = tag;
      results.Priority = priority + SearchViewResultPriority::TagBegin;
    }
  }
}

void SearchData::Sort()
{
  Zero::Sort(Results.All(), SortByPriority());
}

void SearchData::ClearSearchProviders()
{
  DeleteObjectsInContainer(SearchProviders);
}

ZilchDefineType(SearchViewElement, builder, type)
{
}

SearchViewElement::SearchViewElement(Composite* parent) : Composite(parent)
{
  static const String className = "ListBox";
  mDefSet = mDefSet->GetDefinitionSet(className);

  mBackground = CreateAttached<Element>(cWhiteSquare);
  mName = new Text(this, cText);
  mType = new Text(this, cText);

  ConnectThisTo(this, Events::LeftMouseUp, OnMouseUp);
  ConnectThisTo(this, Events::MouseEnter, OnMouseMove);
}

void SearchViewElement::OnMouseUp(MouseEvent* event)
{
  mView->SetSelection(mIndex);
  mView->Selected();
  event->Handled = true;
}

void SearchViewElement::OnMouseMove(MouseEvent* event)
{
  mView->SetSelection(mIndex);
}

void SearchViewElement::Setup(SearchView* view, uint index, bool selected, SearchViewResult& element)
{
  mView = view;

  mIndex = index;
  mName->SetText(element.Name);
  String typeString = element.Interface->GetElementType(element);

  mType->SetText(typeString);

  Vec3 indent = Pixels(4, 0, 0);
  Vec3 offset = Pixels(0, 2, 0);

  mName->SetTranslation(indent + offset);

  Vec2 textSize = mType->GetMinSize();
  mType->SetTranslation(Vec3(GetSize().x - textSize.x, 0, 0) - indent + offset);

  mBackground->SetSize(GetSize());
  if (selected)
    mBackground->SetColor(SearchViewUi::HoverColor);
  else
  {
    if (index % 2)
      mBackground->SetColor(SearchViewUi::PrimaryColor);
    else
      mBackground->SetColor(SearchViewUi::SecondaryColor);
  }

  if (element.mStatus.Failed())
    mName->SetColor(SearchViewUi::InvalidTextColor);
  else
    mName->SetColor(Vec4(1, 1, 1, 1));
}

void SearchViewElement::UpdateTransform()
{
  mName->SetSize(mSize);
  mType->SetSize(mSize);
  Composite::UpdateTransform();
}

const float cPreviewSize = 130.0f;

ZilchDefineType(SearchView, builder, type)
{
}

SearchView::SearchView(Composite* parent) : Composite(parent), mEventTerminated(false)
{
  static const String className = "ListBox";
  mDefSet = mDefSet->GetDefinitionSet(className);
  this->SetLayout(CreateStackLayout(LayoutDirection::TopToBottom, Pixels(0, 4), Thickness(0, 1, 0, 0)));

  mSelectedIndex = 0;

  mBackground = CreateAttached<Element>(cWhiteSquare);
  mBackground->SetVisible(true);

  Composite* searchRow = new Composite(this);
  searchRow->SetLayout(CreateFillLayout(Thickness(1, 0, 1, 0)));
  searchRow->SetSizing(SizeAxis::X, SizePolicy::Flex, 1.0f);
  searchRow->SetSizing(SizeAxis::Y, SizePolicy::Fixed, 16.0f);
  {
    mSearchBar = new TagChainTextBox(searchRow);
    mSearchBar->SetSizing(SizePolicy::Flex, 1.0f);
  }

  mSearch = &mSearchBar->mSearch;
  ConnectThisTo(mSearchBar, Events::SearchDataModified, OnSearchDataModified);
  ConnectThisTo(mSearchBar, Events::KeyPreview, OnSearchKeyPreview);

  mArea = new ScrollArea(this);
  mArea->SetSizing(SizeAxis::Y, SizePolicy::Flex, 20);
  mArea->SetClientSize(Pixels(10, 20));

  ConnectThisTo(mSearchBar, Events::TextEnter, OnEnter);
  ConnectThisTo(mSearchBar, Events::KeyDown, OnKeyPressed);
  ConnectThisTo(mSearchBar, Events::KeyRepeated, OnKeyPressed);
  ConnectThisTo(mArea, Events::ScrollUpdated, OnScrollUpdated);
}

SearchView::~SearchView()
{
  mToolTip.SafeDestroy();
}

void SearchView::UpdateTransform()
{
  mSearchBar->MarkAsNeedsUpdate();
  mBackground->SetSize(GetSize());
  mBackground->SetColor(SearchViewUi::BackgroundColor);

  PositionToolTip();

  Composite::UpdateTransform();
}

WidgetRect SearchView::GetToolTipRect()
{
  // Top left of the current row
  Vec2 topLeft = GetScreenRect().TopLeft();

  // Move it to the selected row
  topLeft += Vec2(0, (mSelectedIndex + 1) * SearchViewUi::RowSize);

  // Move it by the scroll area
  topLeft.y += mArea->GetClientOffset().y;

  // Size of each row
  Vec2 size = Vec2(mSize.x, SearchViewUi::RowSize);

  return WidgetRect::PointAndSize(topLeft, size);
}

void SearchView::PositionToolTip()
{
  ToolTip* toolTip = mToolTip;
  if (toolTip == nullptr)
    return;

  // Position the tooltip
  ToolTipPlacement placement;

  WidgetRect rect = GetToolTipRect();
  Vec2 topLeft = rect.TopLeft();

  placement.SetScreenRect(rect);
  placement.SetPriority(IndicatorSide::Right, IndicatorSide::Left, IndicatorSide::Top, IndicatorSide::Bottom);

  toolTip->SetArrowTipTranslation(placement);

  // y starts at the bottom of the screen
  float lowerY = GetScreenRect().TopLeft().y;
  float upperY = GetScreenRect().BottomRight().y;

  // only show the tooltip when it is within the scroll area
  if (topLeft.y <= upperY && topLeft.y >= lowerY)
  {
    toolTip->SetVisible(true);
    ResolveVerticalToolTipOverlap();
  }
  else
  {
    toolTip->SetVisible(false);
  }
}

void SearchView::PositionAlternateToolTip()
{
  ToolTip* alternateTip = mAlternateToolTip;
  if (alternateTip == nullptr)
    return;

  ToolTipPlacement placement;
  WidgetRect searchRect = mSearchBar->GetScreenRect();
  // Push out the searchbox tooltip to be aligned with the list-item tooltip.
  searchRect.X += 1.0f;

  placement.SetScreenRect(searchRect);
  placement.SetPriority(IndicatorSide::Right, IndicatorSide::Left, IndicatorSide::Top, IndicatorSide::Bottom);

  alternateTip->SetArrowTipTranslation(placement);

  ResolveVerticalToolTipOverlap();
}

void SearchView::ResolveVerticalToolTipOverlap()
{
  ToolTip* toolTip = mToolTip;
  ToolTip* alternateTip = mAlternateToolTip;

  // No need to resolve overlap if both of the tooltips aren't active.
  if (toolTip == nullptr || alternateTip == nullptr)
    return;

  WidgetRect itemRect = toolTip->GetScreenRect();
  WidgetRect searchRect = alternateTip->GetScreenRect();

  // If the top of of the itemTip is above the bottom of the alternateTip,
  // (by at least one pixel) then there's overlap.
  float overlap = searchRect.Bottom() - itemRect.Top();
  if (overlap < 1.0f)
    return;

  // Place the item tooltip next to its associated UI element.
  itemRect = GetToolTipRect();
  // Resolve the vertical overlap, and also allow for a 2 pixel gap between the
  // tooltips.
  overlap = Math::Ceil(overlap) + 2.0f;
  itemRect.Y += overlap;

  ToolTipPlacement placeItem;
  placeItem.SetScreenRect(itemRect);

  placeItem.SetPriority(IndicatorSide::Right, IndicatorSide::Left, IndicatorSide::Top, IndicatorSide::Bottom);

  toolTip->SetArrowTipTranslation(placeItem);
  // Keep the tooltip arrow next to the UI element associated with the tooltip.
  toolTip->TranslateArrowOffset(Vec2(0, -overlap));
}

void SearchView::OnSearchDataModified(Event* e)
{
  ClearToolTips();

  mSelectedIndex = 0;
  BuildResults();
}

void SearchView::OnSearchKeyPreview(KeyboardEvent* e)
{
  if (e->Key == Keys::Escape)
  {
    Canceled();
  }
}

void SearchView::SetSelection(int index)
{
  int newIndex = Math::Clamp(index, 0, int(mSearch->Results.Size() - 1));
  if (mSearch->Results.Size() > 0)
  {
    if (mSelectedIndex != newIndex)
    {
      ClearToolTips();

      mSelectedIndex = newIndex;
      BuildResults();
    }
  }
}

void SearchView::MoveSelection(int change)
{
  ClearToolTips();

  if (mSearch->Results.Size() > 0)
  {
    mSelectedIndex = Math::Clamp(int(mSelectedIndex + change), 0, int(mSearch->Results.Size() - 1));
    mSearchBar->mSearchIndex = mSelectedIndex;
    float location = float(mSelectedIndex) * SearchViewUi::RowSize;
    if (change > 0)
      location = float(mSelectedIndex + 1) * SearchViewUi::RowSize;
    mArea->ScrollAreaToView(Vec2(0, location), Vec2(0, location));
    BuildResults();
  }
}

void SearchView::Selected()
{
  if (mSearch->Results.Size() > 0)
  {
    SearchViewResult& foundElement = mSearch->Results[mSelectedIndex];
    bool searchCompleted = foundElement.Interface->OnMatch(this, foundElement);

    if (searchCompleted)
    {
      ClearToolTips();

      SearchViewEvent e;
      e.Element = &foundElement;
      e.View = this;
      DispatchEvent(Events::SearchCompleted, &e);
      mEventTerminated = true;
    }
  }
}

void SearchView::OnScrollUpdated(Event* e)
{
  BuildResults();
}

// Get the element in this tree right before the root widget
Composite* GetBeforeRoot(Composite* composite)
{
  Composite* parent = composite->GetParent();

  if (parent == NULL)
    return composite;
  if (parent == composite->GetRootWidget())
    return composite;

  return GetBeforeRoot(parent);
}

void SearchView::BuildResults()
{
  Composite* window = GetBeforeRoot(this);

  float allRowHeight = Pixels(SearchViewUi::RowSize) * float(mSearch->Results.Size());

  mArea->SetClientSize(Vec2(Pixels(10), allRowHeight));

  Vec2 areaSize = mArea->GetSize();

  Vec2 size = this->GetSize();
  Vec2 visibleSize = mArea->GetClientVisibleSize();

  Vec3 windowTrans = window->GetTranslation();
  Vec2 windowSize = window->GetSize();

  // Is there any tooltip info for the text in the search box?
  if (!mSearch->SearchString.Empty())
  {
    // Group all search provider previews into one widget for the
    // search TextBox's tooltip.
    Composite* alternatePreviewWidget = new Composite(window->GetParent());
    alternatePreviewWidget->SetLayout(CreateStackLayout());

    bool needsAlternatePreview = false;

    // Add to the search preview widget, if applicable.
    forRange (SearchProvider* provider, mSearch->SearchProviders.All())
      needsAlternatePreview |= provider->AddToAlternatePreview(mSearch, alternatePreviewWidget);

    if (!needsAlternatePreview)
    {
      alternatePreviewWidget->Destroy();
    }
    else
    {
      // Destroy the old search-box tooltip, if it exists.
      mAlternateToolTip.SafeDestroy();

      ToolTip* toolTip = new ToolTip(this);
      toolTip->SetDestroyOnMouseExit(false);
      toolTip->mContentPadding = Thickness(1, 1, 1, 1);

      alternatePreviewWidget->SetSize(Vec2(cPreviewSize));
      toolTip->SetContent(alternatePreviewWidget);
      mAlternateToolTip = toolTip;

      PositionAlternateToolTip();
    }
  }

  if (mSearch->Results.Size() > 0)
  {
    SearchViewResult& result = mSearch->Results[mSelectedIndex];

    Composite* root = window->GetParent();
    Composite* composite = result.Interface->CreatePreview(window->GetParent(), result);
    if (composite)
    {
      // Destroy the old list-item tooltip, if it exists.
      mToolTip.SafeDestroy();

      // Create a new tooltip
      ToolTip* toolTip = new ToolTip(this);
      toolTip->SetDestroyOnMouseExit(false);
      toolTip->mContentPadding = Thickness(1, 1, 1, 1);

      if (result.mStatus.Failed())
        toolTip->SetColorScheme(ToolTipColorScheme::Red);

      bool matchWidth = mAlternateToolTip.IsNotNull();
      // Get the search-box tooltip's content size without padding,
      // as the list-item tooltip sets its own padding.
      float width = (matchWidth) ? mAlternateToolTip->GetPaddedContentSize().x : cPreviewSize;

      // Defer border-display to the parent's (tooltip) border.
      if (MultiLineText* content = Type::DynamicCast<MultiLineText*>(composite))
        content->mBorder->SetVisible(false);

      if (matchWidth)
        toolTip->BestFitTextToMaxWidth(true, width);

      composite->SetSize(Pixels(width, cPreviewSize));
      toolTip->SetContent(composite);
      mToolTip = toolTip;

      PositionToolTip();
    }

    const String tag = "Tag";
    if (result.Interface->GetElementType(result) != tag)
    {
      SearchViewEvent e;
      e.Element = &result;
      e.View = this;
      DispatchEvent(Events::SearchPreview, &e);
    }
  }

  Vec4 areaRect = mArea->GetClientArea();

  forRange (SearchViewElement* element, mElements.All())
  {
    element->SetActive(false);
    element->Destroy();
  }

  mElements.Clear();

  uint firstVisible = uint(areaRect.y / Pixels(SearchViewUi::RowSize));

  uint numberOfVisibleElements = uint(areaRect.w / Pixels(SearchViewUi::RowSize));

  Composite* area = mArea->GetClientWidget();

  uint start = firstVisible;
  uint end = Math::Min(start + numberOfVisibleElements, (uint)mSearch->Results.Size());
  for (uint i = start; i < end; ++i)
  {
    SearchViewResult& element = mSearch->Results[i];
    SearchViewElement* elementview = new SearchViewElement(area);
    elementview->SetTranslation(Pixels(0, SearchViewUi::RowSize, 0) * float(i));
    elementview->SetSize(Vec2(visibleSize.x, Pixels(SearchViewUi::RowSize)));
    elementview->Setup(this, i, mSelectedIndex == i, element);
    mElements.PushBack(elementview);
  }

  window->MoveToFront();

  if (ToolTip* toolTip = mToolTip)
    toolTip->MoveToFront();

  GetParent()->MarkAsNeedsUpdate();
}

bool SearchView::TakeFocusOverride()
{
  mSearchBar->TakeFocus();
  return true;
}

bool CheckTags(HashSet<String>& testTags, BoundType* type)
{
  forRange (CogComponentMeta* metaComponent, type->HasAll<CogComponentMeta>())
  {
    if (CheckTags(testTags, metaComponent->mTags))
      return true;
  }

  if (MetaScriptTagAttribute* tagAttribute = type->HasInherited<MetaScriptTagAttribute>())
  {
    if (CheckTags(testTags, tagAttribute->mTagSet))
      return true;
  }

  return false;
}

bool CheckAndAddTags(SearchData& search, BoundType* type)
{
  if (CheckTags(search.ActiveTags, type))
  {
    forRange (CogComponentMeta* metaComponent, type->HasAll<CogComponentMeta>())
    {
      forRange (String& tag, metaComponent->mTags.All())
        search.AvailableTags.Insert(tag);
    }

    if (MetaScriptTagAttribute* tagAttribute = type->HasInherited<MetaScriptTagAttribute>())
    {
      forRange (String& tag, tagAttribute->mTagSet.All())
        search.AvailableTags.Insert(tag);
    }

    return true;
  }
  else
  {
    return false;
  }
}

bool CheckAndAddSingleTag(SearchData& search, StringParam tag)
{
  bool valid = search.ActiveTags.Empty() || search.ActiveTags.Contains(tag);
  if (search.ActiveTags.Empty())
    search.AvailableTags.Insert(tag);
  return valid;
}

void SearchView::AddTag(StringParam tag, bool removeable)
{
  mSearchBar->AddTag(tag, removeable);
}

void SearchView::AddHiddenTag(StringParam tag)
{
  mSearch->ActiveTags.Insert(tag);
}

void SearchView::AddMetaType(BoundType* boundType)
{
  mSearch->ActiveMeta.PushBack(boundType);
}

void SearchView::Search(StringParam text)
{
  mSelectedIndex = 0;
  mSearch->SearchString = text;
  mSearch->Results.Clear();

  // Collect all search results
  forRange (SearchProvider* provider, mSearch->SearchProviders.All())
  {
    provider->Search(*mSearch);
  }

  // Add Available tags to search results
  mSearch->AddAvailableTagsToResults();
  mSearch->Sort();

  // Builds results into ui
  BuildResults();
}

void SearchView::OnDestroy()
{
  Canceled();
  Composite::OnDestroy();
}

void SearchView::OnEnter(ObjectEvent* event)
{
  Selected();
}

void SearchView::OnKeyPressed(KeyboardEvent* event)
{
  if (event->Key == Keys::Down)
  {
    MoveSelection(1);
  }
  else if (event->Key == Keys::Up)
  {
    MoveSelection(-1);
  }
  else if (event->Key == Keys::Space)
  {
    if (mSearchBar->GetText().Empty())
      Canceled();
  }
  else if (event->Key == Keys::Enter && event->GetModifierPressed())
  {
    forRange (SearchProvider* provider, mSearch->SearchProviders.All())
      provider->AttemptAlternateSearchCompleted();
  }
}

void SearchView::ClearToolTips()
{
  // Destroy the old list-item tooltip, if it exists.
  mToolTip.SafeDestroy();
  // Destroy the old search-box tooltip, if it exists.
  mAlternateToolTip.SafeDestroy();
}

void SearchView::Canceled()
{
  ClearToolTips();

  if (!mEventTerminated)
  {
    mEventTerminated = true;
    SearchViewEvent searchEvent;
    searchEvent.Element = nullptr;
    searchEvent.View = this;

    DispatchEvent(Events::SearchCanceled, &searchEvent);
  }
}

Composite* CreateTextPreview(Composite* parent, StringParam text)
{
  if (text.Empty())
    return nullptr;

  MultiLineText* desc = new MultiLineText(parent);
  desc->mBackground->SetVisible(false);
  desc->SetText(text);
  return desc;
};

} // namespace Zero
