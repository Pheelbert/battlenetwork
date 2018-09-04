#pragma once

// Register these navis
#include "bnPlayer.h"
#include "bnStarman.h"

/***********************************************************************
************    Register your custom navis here    *********************
************************************************************************/

void QueuNaviRegistration() {
  /*********************************************************************
  **********            Register megaman            ********************
  **********************************************************************/
  auto megamanInfo = NAVIS.AddSpot();  // Create navi info object
  megamanInfo->SetNaviClass<Player>(); // Deffer loading of class type
  megamanInfo->SetSpecialDescription("Star of the series. Well rounded stats."); // Set property
  megamanInfo->SetBattleAnimationPath("resources/navis/megaman/megaman.animation");
  megamanInfo->SetOverworldAnimationPath("resources/navis/megaman/megaman.animation");

  NAVIS.Register(megamanInfo);       // Add to roster

                                     // Register Starman
  auto starmanInfo = NAVIS.AddSpot();
  starmanInfo->SetNaviClass<Starman>();
  starmanInfo->SetSpecialDescription("Projectile chips turn into arrows w/ rapid fire");
  starmanInfo->SetBattleAnimationPath("resources/navis/starman/starman.animation");
  starmanInfo->SetOverworldAnimationPath("resources/navis/starman/starman.animation");

  NAVIS.Register(starmanInfo);
}