#pragma once
#include "bnChip.h"

class ChipUsePublisher;

class ChipUseListener {
public:
  ChipUseListener() = default;
  ~ChipUseListener() = default;

  ChipUseListener(const ChipUseListener& rhs) = delete;
  ChipUseListener(ChipUseListener&& rhs) = delete;

  virtual void OnChipUse(Chip& chip) = 0;
  void Subscribe(ChipUsePublisher& publisher);
};