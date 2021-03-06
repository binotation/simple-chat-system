#include "dynstring.h"
#include <stdlib.h>
#include <string.h>

void dynstring_init( DynString *dstr, unsigned int size )
{
    dstr->str = malloc( sizeof(char) * size );
    dstr->str[ dstr->length = 0 ] = '\0';
    dstr->size = size;
}

void dynstring_nfrom( DynString *dstr, const char *str, unsigned int length, unsigned int size )
{
    dynstring_init( dstr, size );
    memcpy( dstr->str, str, sizeof(char) * length );
    dstr->str[ dstr->length = length ] = '\0';
}

void dynstring_destroy( DynString *dstr )
{
    free( dstr->str );
}

void dynstring_clear( DynString *dstr )
{
    dstr->str[ dstr->length = 0 ] = '\0';
}

void dynstring_npush( DynString *dstr, const char *str, unsigned int length )
{
    if( dstr->length + length + 1 > dstr->size )
    {
        dstr->size <<= 1;
        while( dstr->length + length + 1 > dstr->size ) dstr->size <<= 1;
        dstr->str = realloc( dstr->str, sizeof(char) * dstr->size );
    }

    memcpy( dstr->str + dstr->length, str, sizeof(char) * length);
    dstr->str[ dstr->length += length ] = '\0';
}

void dynstring_pushc( DynString *dstr, char c )
{
    if( dstr->length + 2 > dstr->size )
    {
        dstr->str = realloc( dstr->str, sizeof(char) * ( dstr->size <<= 1 ));
    }
    dstr->str[ dstr->length++ ] = c;
    dstr->str[ dstr->length ] = '\0';
}

char dynstring_popc( DynString *dstr )
{
    if( dstr->length == 0 ) return '\0';
    char c = dstr->str[ --dstr->length ];
    dstr->str[ dstr->length ] = '\0';
    return c;
}

enum ReadlineResult dynstring_readline( DynString *dstr, FILE *stream )
{
    dstr->str[ dstr->length = 0 ] = '\0';
    char *s;
    unsigned int count = 0;				// Number of iterations
    unsigned int num;					// Number of chars read in the current iteration
    unsigned int shift = dstr->size;	// Number of chars to read next

    do
    {
        s = fgets( dstr->str + dstr->length, shift, stream );
        if( s == NULL ) // If error or eof reached while no chars have been read
        {
            if( feof( stream ) != 0 && count == 0 ) return READLINE_EOF;
            else if( feof( stream ) != 0 && count > 0 ) return READLINE_SUCCESS;
            else return READLINE_ERROR;
        }
        else
        {
            count++;
            dstr->length += num = strlen( dstr->str + dstr->length );

            // If buffer maxed out, i.e. more to read
            if( num == shift - 1 && dstr->str[ dstr->length - 1 ] != '\n' )
            {
                dstr->str = realloc( dstr->str, sizeof(char) * ( dstr->size <<= 1 ));
                shift = dstr->size / 2 + 1; // +1 because we overwrite '\0' in the next iteration
            }
            else if( dstr->str[ dstr->length - 1 ] != '\n' )
            {
                return READLINE_SUCCESS;
            }
            else
            {
                dstr->str[ --dstr->length ] = '\0';
                return READLINE_SUCCESS;
            }
        }
    } while(1);
}
