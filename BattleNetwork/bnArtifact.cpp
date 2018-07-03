#include "bnArtifact.h"
#include <random>
#include <time.h>

#include "bnArtifact.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnTextureResourceManager.h"

Artifact::Artifact(Field* _field, Team _team) {
  this->SetField(_field);
  this->SetTeam(_team);
}

Artifact::Artifact(void) {
  SetLayer(0);
  texture = nullptr;
}

Artifact::~Artifact(void) {
}