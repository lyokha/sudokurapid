/*
 * =============================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  Sudoku Solver Console
 *
 *        Version:  1.0
 *        Created:  29.01.2009 22:46:21
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */


#include <iostream>
#include <fstream>
#include <bitset>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "../sudokuRapid.h"


class SudokuConsole : public SudokuRapid::BasicBoard
{
    public:
        void  print( void ) const
        {
            for ( int  i( 0 ); i < 9; ++i )
            {
                for ( int  j( 0 ); j < 9; ++j )
                {
                    if ( cellState[ i * 9 + j ].value )
                        std::cout << " " << cellState[ i * 9 + j ].value;
                    else
                        if ( cellState[ i * 9 + j ].state )
                            std::cout << " -";
                        else
                            std::cout << " *";
                }
                std::cout << std::endl;
            }
        }

        void  readFromFile( const char *  fileName )
        {
            std::ifstream   file( fileName );
            file.exceptions( std::ios::failbit );
            char    number( 0 );
            for ( int i( 0 ); i < 9; ++i )
            {
                for ( int j( 0 ); j < 9; ++j )
                {
                    file >> number;
                    if ( ! isdigit( number ) )
                        continue;
                    int     cell( i * 9 + j );
                    int     value( number - '0' );
                    try
                    {
                        setCellValue( cell, value );
                    }
                    catch ( SudokuRapid::Unsolvable &  e )
                    {
                        file.close();
                        throw;
                    }
                }
            }
            file.close();
        }
};


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


int  main( int  argc, char **  argv )
{
    int             returnValue( 0 );
    SudokuConsole   board;

    srand( getRandom() );

    try
    {
        if ( argc == 2 )
        {
            board.readFromFile( argv[ 1 ] );
        }
        else
        {
            board.makeRandom();
        }
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        std::cerr << "Unsolvable init: " << e.what() << " Cell " << e.cell <<
                     ", state " << std::bitset< 9 >( e.state ).to_string() <<
                     ", value " << e.value << std::endl;
        std::cerr << "Curent board:" << std::endl;
        board.print();
        return 1;
    }
    catch ( std::ios_base::failure &  e )
    {
        std::cerr << "Bad file: " << e.what() << std::endl;
        return 3;
    }
    catch( ... )
    {
        std::cerr << "Unspecified error occured" << std::endl;
        return 3;
    }

    std::cout << "Original board setup:" << std::endl;
    board.print();

    try
    {
        board.solve();
        std::cout << "Solved board:" << std::endl;
        board.print();
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        std::cerr << "Unsolvable state: " << e.what() << std::endl <<
                     "Current board:" << std::endl;
        board.print();
        returnValue = 2;
    }
    catch( ... )
    {
        std::cerr << "Unspecified error occured" << std::endl;
        returnValue = 3;
    }

    std::cout << "Number or retries: " << board.getNumberOfRetries() <<
                                          std::endl;

    return returnValue;
}

