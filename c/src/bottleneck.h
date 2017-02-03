/*

  This file acts as a bottleneck.
  It includes everything an interface needs in the correct order.

 */

//
// Everything needed from ../lib/ needed by streamers and tools
//
#include "../lib/unicode.h"

//
// Everything an interface needs
//
#include "streamers/reader.h"
#include "streamers/writer.h"

#include "tokens.h"
#include "syntax.h"

#include "lexer.h"
#include "parser.h"

#include "delexer.h"
#include "deparser.h"

#include "languages/languages.h"