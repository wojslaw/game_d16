#pragma once
#include <assert.h>
#include <vector>
#include <string>
#include <array>

#include "../interface.hpp"
#include "../../d16.h"
#include "rollmod.hpp"
#include "stattype.hpp"


void
minigame_combat(FILE * f);



void
display_table_item_base(FILE * f);

void
display_table_item_type(FILE * f);
