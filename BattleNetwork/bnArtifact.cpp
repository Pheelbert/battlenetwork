#include "bnArtifact.h"
#include <random>
#include <time.h>

#include "bnArtifact.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnTextureResourceManager.h"

Artifact::Artifact(void) {
  SetLayer(1);
  texture = nullptr;
  animation = FrameAnimation();
}

Artifact::~Artifact(void) {
}