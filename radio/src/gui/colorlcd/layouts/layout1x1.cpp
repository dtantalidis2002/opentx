/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "trims.h"

#define HAS_TOPBAR()      (persistentData->options[0].value.boolValue == true)
#define HAS_FM()          (persistentData->options[1].value.boolValue == true)
#define HAS_SLIDERS()     (persistentData->options[2].value.boolValue == true)
#define HAS_TRIMS()       (persistentData->options[3].value.boolValue == true)
#define IS_MIRRORED()     (persistentData->options[4].value.boolValue == true)

constexpr coord_t border = 10;

const uint8_t LBM_LAYOUT_1x1[] = {
#include "mask_layout1x1.lbm"
};

const ZoneOption OPTIONS_LAYOUT_1x1[] = {
  { STR_TOP_BAR, ZoneOption::Bool },
  { STR_FLIGHT_MODE, ZoneOption::Bool },
  { STR_SLIDERS, ZoneOption::Bool },
  { STR_TRIMS, ZoneOption::Bool },
  { STR_MIRROR, ZoneOption::Bool },
  { nullptr, ZoneOption::Bool }
};

class Layout1x1: public Layout
{
  public:
    Layout1x1(const LayoutFactory * factory, Layout::PersistentData * persistentData):
      Layout(factory, persistentData)
    {
      decorate();
    }

    void create() override
    {
      Layout::create();
      persistentData->options[0].value.boolValue = true;
      persistentData->options[1].value.boolValue = true;
      persistentData->options[2].value.boolValue = true;
      persistentData->options[3].value.boolValue = true;
      persistentData->options[4].value.boolValue = false;
      persistentData->options[5].value.boolValue = false;
      decorate();
    }

    void decorate()
    {
      Layout::decorate(HAS_TOPBAR(), HAS_SLIDERS(), HAS_TRIMS(), HAS_FM());
    }


    unsigned int getZonesCount() const override
    {
      return 1;
    }

    rect_t getZone(unsigned int index) const override
    {
      rect_t zone = {10, 10, LCD_W - 2 * border, LCD_H - 2 * border};

      if (HAS_TOPBAR()) {
        zone.y += MENU_HEADER_HEIGHT;
        zone.h -= MENU_HEADER_HEIGHT;
      }

      if (HAS_FM() || HAS_TRIMS()) {
        zone.h -= TRIM_SQUARE_SIZE;
      }

      if (HAS_SLIDERS()) {
#if NUM_SLIDERS + NUM_POTS > 2
        zone.h -= TRIM_SQUARE_SIZE;
#endif
        zone.w -= 2 * TRIM_SQUARE_SIZE;
        zone.x += TRIM_SQUARE_SIZE;
      }

      if (HAS_TRIMS()) {
        zone.w -= 2 * TRIM_SQUARE_SIZE;
        zone.x += TRIM_SQUARE_SIZE;
      }

      return zone;
    }

    void checkEvents() override
    {
      Layout::checkEvents();
      decorate();
    }
  protected:
    uint8_t value = 0;
};

BaseLayoutFactory<Layout1x1> layout1x1("Layout1x1", "Fullscreen", LBM_LAYOUT_1x1, OPTIONS_LAYOUT_1x1);
