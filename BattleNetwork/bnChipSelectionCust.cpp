#include "bnChipSelectionCust.h"
#include "bnTextureResourceManager.h"
#include "bnShaderResourceManager.h"

ChipSelectionCust::ChipSelectionCust(int cap) : 
  greyscale(*SHADERS.GetShader(ShaderType::GREYSCALE)) 
{
  chipCap = cap;
  queue = new Bucket[chipCap];
  selectQueue = new Bucket*[chipCap];

  chipCount = selectCount = cursorPos = 0;

  custSprite = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_SELECT_MENU));
  custSprite.setScale(2.f, 2.f);
  custSprite.setPosition(-custSprite.getTextureRect().width*2.f, 0);

  icon.setTexture(*TEXTURES.GetTexture(CHIP_ICONS));
  icon.setScale(sf::Vector2f(2.f, 2.f));

  cursorSmall = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_CURSOR_SMALL));
  cursorSmall.setScale(sf::Vector2f(2.f, 2.f));

  cursorBig = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_CURSOR_BIG));
  cursorBig.setScale(sf::Vector2f(2.f, 2.f));
  cursorBig.setPosition(sf::Vector2f(2.f*92.f, 2.f*111.f));

  sf::Texture* card = TEXTURES.GetTexture(TextureType::CHIP_CARDS);
  chipCard.setTexture(*card);
  chipCard.setScale(2.f, 2.f);
  chipCard.setPosition(2.f*16.f, 48.f);

  sf::Texture* nodata = TEXTURES.GetTexture(TextureType::CHIP_NODATA);
  chipNoData.setTexture(*nodata);
  chipNoData.setScale(2.f, 2.f);
  chipNoData.setPosition(2.f*16.f, 48.f);

  sf::Texture* senddata = TEXTURES.GetTexture(TextureType::CHIP_SENDDATA);
  chipSendData.setTexture(*senddata);
  chipSendData.setScale(2.f, 2.f);
  chipSendData.setPosition(2.f*16.f, 48.f);

  sf::Font* font = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  label.setFont(*font);

  ChipLibrary::GetInstance().LoadLibrary();
}


ChipSelectionCust::~ChipSelectionCust() {
  ClearChips();

  for (int i = 0; i < chipCount; i++) {
    selectQueue[i] = nullptr;
    delete queue[i].data;
  }

  if (chipCount > 0) {
    delete[] queue;
    delete[] selectQueue;
  }

  chipCount = 0;
}

bool ChipSelectionCust::CursorRight() {
  if (++cursorPos > 5) {
    cursorPos = 5;
    return false;
  }

  return true;
}

bool ChipSelectionCust::CursorLeft() {
  if (--cursorPos < 0) {
    cursorPos = 0;
    return false;
  }

  return true;
}

bool ChipSelectionCust::CursorAction() {
  if (cursorPos == 5) {
    // End chip select
    areChipsReady = true;
  } else {
    // Does chip exist 
    if (cursorPos < chipCount) {
      // Queue this chip if not selected
      if (queue[cursorPos].state == 1) {
        selectQueue[selectCount++] = &queue[cursorPos];
        queue[cursorPos].state = 2;

        // Check chip code. If other chips are not compatible, set their bucket state flag to 0
        char code = queue[cursorPos].data->GetCode();

        for (int i = 0; i < chipCount; i++) {
          if (i == cursorPos || (queue[i].state == 0 && queue[i].data->GetCode() - 1 != code) || queue[i].state == 2) continue;
          char otherCode = queue[i].data->GetCode();
          bool isSameChip = (queue[i].data->GetShortName() == queue[cursorPos].data->GetShortName());
          if (code == '=' || otherCode == '=' || otherCode == code || isSameChip ) { queue[i].state = 1; }
          else { queue[i].state = 0; }
        }

        return true;
      }
    }
  }
  return false;
}

bool ChipSelectionCust::CursorCancel() {
  // Unqueue all chips buckets
  if (selectCount <= 0) {
    selectCount = 0;
    return false;// nothing happened
  }

  selectQueue[selectCount-1]->state = 1;

  selectCount--;

  if (selectCount == 0) {
    // Everything is selectable again
    for (int i = 0; i < chipCount; i++) {
      queue[i].state = 1;
    }

    return true;
  }

  /*
    Compatible chip states are built upon adding chips from the last available chip states. 
    The only way to "revert" to the last compatible chip states is to step through the already selected 
    chip queue and build up the state again. 
  */


  for(int i = 0; i < selectCount; i++) {
    char code = selectQueue[i]->data->GetCode();

    for (int j = 0; j < chipCount; j++) {
      if (i > 0) {
        if (queue[j].state == 0 && code != queue[j].data->GetCode() - 1) continue; // already checked and not a PA sequence
      }

      if (queue[j].state == 2) continue; // skip  

      char otherCode = queue[j].data->GetCode();

      bool isSameChip = (queue[j].data->GetShortName() == selectQueue[i]->data->GetShortName());

      if (code == '=' || otherCode == '=' || otherCode == code || isSameChip) { queue[j].state = 1; }
      else { queue[j].state = 0; }
    }
  }

  return true;
}

bool ChipSelectionCust::IsOutOfView() {
  float bounds = -custSprite.getTextureRect().width*2.f;

  if (custSprite.getPosition().x <= bounds) {
    custSprite.setPosition(bounds, custSprite.getPosition().y);
  }

  return (custSprite.getPosition().x == bounds);
}

bool ChipSelectionCust::IsInView() {
  float bounds = 0;

  if (custSprite.getPosition().x >= bounds) {
    custSprite.setPosition(bounds, custSprite.getPosition().y);
  }

  return (custSprite.getPosition().x == bounds);
}

void ChipSelectionCust::Move(sf::Vector2f delta) {
  custSprite.setPosition(custSprite.getPosition() + delta);
}

void ChipSelectionCust::GetNextChips() {
  ClearChips();

  int perTurn = 3; // Limit how many new chips we get per turn
  for (int i = chipCount; i < chipCap; i++) {
    queue[i].data = ChipLibrary::GetInstance().Next();
    queue[i].state = 1;

    if (!queue[i].data) {
      // nullptr is end of list
      return;
    }
    chipCount++;
    perTurn--;

    if (perTurn == 0) return;
  }
}

void ChipSelectionCust::Draw() {
  ENGINE.Draw(custSprite, false);

  if (IsInView()) {
    cursorSmall.setPosition(2.f*(7.0f + (cursorPos*16.0f)), 2.f*103.f); // TODO: Make this relative to cust instead of screen

    for (int i = 0; i < chipCount; i++) {
      icon.setPosition(2.f*(9.0f + (i*16.0f)), 2.f*105.f);
      sf::IntRect iconSubFrame = TEXTURES.GetIconRectFromID(queue[i].data->GetIconID());
      icon.setTextureRect(iconSubFrame);
      icon.SetShader(nullptr);

      if (queue[i].state == 0) {
        icon.SetShader(&greyscale);
        ENGINE.Draw(&icon);
      } else if (queue[i].state == 1) {
        ENGINE.Draw(icon, false);
      }
    }


    icon.SetShader(nullptr);

    for (int i = 0; i < selectCount; i++) {
      icon.setPosition(2 * 97.f, 2.f*(25.0f + (i*16.0f)));
      sf::IntRect iconSubFrame = TEXTURES.GetIconRectFromID((*selectQueue[i]).data->GetIconID());
      icon.setTextureRect(iconSubFrame);
      ENGINE.Draw(icon, false);
    }


    if (cursorPos < 5) {
      // Draw the selected chip info
      label.setFillColor(sf::Color::White);

      if (cursorPos < chipCount) {
        // Draw the selected chip card
        sf::IntRect cardSubFrame = TEXTURES.GetCardRectFromID(queue[cursorPos].data->GetID());
        chipCard.setTextureRect(cardSubFrame);

        chipCard.SetShader(nullptr);

        if (!queue[cursorPos].state) {
          chipCard.SetShader(&greyscale);
          ENGINE.Draw((LayeredDrawable*)&chipCard);
        } else {
          ENGINE.Draw(chipCard, false);
        }

        label.setPosition(2.f*16.f, 16.f);
        label.setString(queue[cursorPos].data->GetShortName());
        ENGINE.Draw(label, false);

        // the order here is very important:
        if (queue[cursorPos].data->GetDamage() > 0) {
          label.setString(std::to_string(queue[cursorPos].data->GetDamage()));
          label.setOrigin(label.getLocalBounds().width*2.f, 0);
          label.setPosition(2.f*(84.f), 143.f);
          ENGINE.Draw(label, false);
        }

        label.setPosition(2.f*16.f, 143.f);
        label.setOrigin(0, 0);
        label.setString(std::string() + queue[cursorPos].data->GetCode());
        label.setFillColor(sf::Color(225, 180, 0));
        ENGINE.Draw(label, false);
      }
      else {
        ENGINE.Draw(chipNoData, false);
      }

      // Draw the small cursor
      ENGINE.Draw(cursorSmall, false);
    }
    else {
      ENGINE.Draw(chipSendData, false);
      ENGINE.Draw(cursorBig, false);
    }
  }
}

Chip** ChipSelectionCust::GetChips() {
  selectedChips = (Chip**)malloc(sizeof(Chip*)*selectCount);

  for (int i = 0; i < selectCount; i++) {
    selectedChips[i] = (*(selectQueue[i])).data;
  }

  return selectedChips;
}

void ChipSelectionCust::ClearChips() {
  for (int i = 0; i < selectCount; i++) {
    if (*(selectedChips+i) != nullptr) {
      delete selectedChips[i];
    }

    (*(selectQueue[i])).data = nullptr;
    selectedChips[i] = nullptr;
  }

  if (selectCount > 0)
    delete[] selectedChips;

  // Restructure queue
  for (int i = 0; i < chipCount; i++) {
    queue[i].state = 1;
    int next = i;
    while (!queue[i].data && next + 1 < chipCount) {
      queue[i].data = queue[next + 1].data;
      queue[next + 1].data = nullptr;
      next++;
    }
  }

  chipCount = chipCount - selectCount;
  selectCount = 0;
}

const int ChipSelectionCust::GetChipCount() {
  return selectCount;
}

void ChipSelectionCust::ResetState() {
  cursorPos = 0;
  areChipsReady = false;
}

bool ChipSelectionCust::AreChipsReady() {
  return areChipsReady;
}
