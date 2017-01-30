 void delex_whitespace(Writer *writer, Token token)
 {
     for(s64 count = token.newline_count; count > 0; --count)
     {
         writer->put(writer, '\n');
     }
     for(s64 count = token.space_count; count > 0; --count)
     {
         writer->put(writer, ' ');
     }
}