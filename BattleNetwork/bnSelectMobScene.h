#pragma once

enum class SelectedNavi {
  STARMAN,
  MEGAMAN,
  SIZE // denotes length of list and if we've reached the end
};


class SelectMobScene
{
public:
  static int Run(SelectedNavi navi);
};

