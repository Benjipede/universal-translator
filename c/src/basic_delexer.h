void delex_whitespace(Writer *writer, TokenWhitespace *token)
 {
     for(s64 count = token->line_feed_count; count > 0; --count)
     {
         writer->put(writer, '\n');
     }
     for(s64 count = token->space_count; count > 0; --count)
     {
         writer->put(writer, ' ');
     }
}