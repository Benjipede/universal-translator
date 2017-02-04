typedef struct Reader
{
    void *data;
    char *source;
    
    u32 (*curr)(struct Reader *);
    
    u32 (*next)(struct Reader *);
    u32 (*moveahead)(struct Reader *, u64);
    
    u32 (*prev)(struct Reader *);
    u32 (*moveback)(struct Reader *, u64);
    
    u32 (*move)(struct Reader *, s64);
    
    u32 (*peekahead)(struct Reader *, u64);
    u32 (*peekback)(struct Reader *, u64);
    u32 (*peek)(struct Reader *, s64);
} Reader;

u32 curr(Reader *reader) {return reader->curr(reader);}

u32 next(Reader *reader) {return reader->next(reader);}
u32 moveahead(Reader *reader, s64 amount) {return reader->moveahead(reader, amount);}

u32 prev(Reader *reader) {return reader->prev(reader);}
u32 moveback(Reader *reader, s64 amount) {return reader->moveback(reader, amount);}

u32 move(Reader *reader, s64 amount) {return reader->move(reader, amount);}

u32 peekahead(Reader *reader, s64 amount) {return reader->peekahead(reader, amount);}
u32 peekback(Reader *reader, s64 amount) {return reader->peekback(reader, amount);}
u32 peek(Reader *reader, s64 amount) {return reader->peek(reader, amount);}



u32 strong_curr(Reader *reader)
{
    return reader->moveahead(reader, 0);
}

u32 strong_next(Reader *reader)
{
    return reader->moveahead(reader, 1);
}
u32 strong_moveahead__next(Reader *reader, u64 count)
{
    --count;
    while(count)
    {
        reader->next(reader);
        --count;
    }
    return reader->next(reader);
}

u32 strong_prev(Reader *reader)
{
    return reader->moveback(reader, 1);
}
u32 strong_moveback__prev(Reader *reader, u64 count)
{
    --count;
    while(count)
    {
        reader->prev(reader);
        --count;
    }
    return reader->prev(reader);
}

u32 strong_move(Reader *reader, s64 count)
{
    if(count < 0)
        return reader->moveback(reader, -count);
    else
        return reader->moveahead(reader, count);
}
u32 strong_moveahead__move(Reader *reader, u64 count)
{
    return reader->move(reader, (s64)count);
}
u32 strong_moveback__move(Reader *reader, u64 count)
{
    return reader->move(reader, -(s64)count);
}

u32 strong_peekahead__move(Reader *reader, u64 count)
{
    u32 result = reader->moveahead(reader, count);
    reader->moveback(reader, count);
    return result;
}
u32 strong_peekback__move(Reader *reader, u64 count)
{
    u32 result = reader->moveback(reader, count);
    reader->moveahead(reader, count);
    return result;
}
u32 strong_peek(Reader *reader, s64 count)
{
    if(count < 0)
        return reader->peekback(reader, -count);
    else
        return reader->peekahead(reader, count);
}
u32 strong_peekahead__peek(Reader *reader, u64 count)
{
    return reader->peek(reader, (s64)count);
}
u32 strong_peekback__peek(Reader *reader, u64 count)
{
    return reader->peek(reader, -(s64)count);
}



void strengthen_reader(Reader *reader)
{
    if(reader->peek)
    {
        if(!reader->peekahead)  reader->peekahead = strong_peekahead__peek;
        if(!reader->peekback)  reader->peekback = strong_peekback__peek;
    }
    
    if(reader->move)
    {   
        if(!reader->moveahead)  reader->moveahead = strong_moveahead__move;
        if(!reader->moveback)  reader->moveback = strong_moveback__move;
    }
    else
    {
        if(reader->next && !reader->moveahead)  reader->moveahead = strong_moveahead__next;
        if(reader->prev && !reader->moveback)  reader->moveback = strong_moveback__prev;
        
        if(reader->moveahead && reader->moveback)
        {
            reader->move = strong_move;
            if(!reader->peekahead)  reader->peekahead = strong_peekahead__move;
            if(!reader->peekback)  reader->peekback = strong_peekback__move;
        }
        
    }
    if(reader->moveahead && !reader->curr)  reader->curr = strong_curr;
    if(reader->moveahead && !reader->next)  reader->next = strong_next;
    if(reader->moveback && !reader->prev)  reader->prev = strong_prev;
    
    if(reader->peekahead && reader->peekback && !reader->peek)  reader->peek = strong_peek;
}