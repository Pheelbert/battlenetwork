#include "bnChipUseListener.h"
#include "bnChipUsePublisher.h"

void ChipUseListener::Subscribe(ChipUsePublisher& publisher) {
  publisher.AddListener(this);
}