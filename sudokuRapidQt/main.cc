/*
 * =============================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02.02.2009 17:06:19
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#include <QtGui>
#include <stdlib.h>
#include <fcntl.h>

#include "sudokuForm.h"


unsigned long  getRandom( void )
{
#   ifdef __linux__
        unsigned long   seed( 0 );

        int     fd( open( "/dev/urandom", O_RDONLY ) );
        if ( fd == -1 )
        {
            perror( "open" );
            return 0;
        }
        if ( read( fd, &seed, sizeof( seed ) ) < 0 )
        {
            perror( "read" );
            seed = 0;
        }
        if ( close( fd ) )
            perror( "close" );

        return seed;
#   else
        return ( unsigned long )time( 0 );
#   endif
}


int  main( int  argc, char **  argv)
{
    srand( getRandom() );

    QApplication    app( argc, argv );
    SudokuForm      form;

    form.show();

    return app.exec();
}

