/*

  This file acts as a bottleneck.
  It includes everything an interface needs in the correct order.

 */

//
// Everything needed from ../lib/ needed by tools.
// Streamers must include libraries specific to their needs.
//
#include "../lib/string.h"

//
// Everything an interface needs
//
#include "streamers/streamer.h"

#include "kinds_and_types.h"
#include "tokens.h"
#include "syntax.h"

#include "lexer.h"
#include "parser.h"

#include "delexer.h"
#include "deparser.h"

#include "languages/languages.h"