/*
 * =============================================================================
 *
 *       Filename:  sudokuRapid.cc
 *
 *    Description:  Sudoku Solver Lib implementation file
 *
 *        Version:  1.0
 *        Created:  09.02.2008 15:01:54 MSK
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */


#include <iostream>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <string>
#include <string.h>

#include "sudokuRapid.h"


namespace SudokuRapid {

namespace
{
    int     cells[] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,
                         9, 10, 11, 12, 13, 14, 15, 16, 17,
                        18, 19, 20, 21, 22, 23, 24, 25, 26,
                        27, 28, 29, 30, 31, 32, 33, 34, 35,
                        36, 37, 38, 39, 40, 41, 42, 43, 44,
                        45, 46, 47, 48, 49, 50, 51, 52, 53,
                        54, 55, 56, 57, 58, 59, 60, 61, 62,
                        63, 64, 65, 66, 67, 68, 69, 70, 71,
                        72, 73, 74, 75, 76, 77, 78, 79, 80 };
}



struct GetRowCells : public std::binary_function< int, int, bool >
{
    bool operator() ( int  cell, int  refCell ) const
    {
        return ( cell / 9 == refCell / 9 );
    }
};


struct GetColCells : public std::binary_function< int, int, bool >
{
    bool operator() ( int  cell, int  refCell ) const
    {
        return ( cell % 9 == refCell % 9 );
    }
};


struct GetBoxCells : public std::binary_function< int, int, bool >
{
    bool operator() ( int  cell, int  refCell ) const
    {
        int     box( cell / 9 / 3 * 3 + cell % 9 / 3 ),
                refBox( refCell / 9 / 3 * 3 + refCell % 9 / 3 );
        return ( box == refBox );
    }
};


struct GetAffectedCells : public std::binary_function< int, int, bool >
{
    bool operator() ( int cell, int  refCell ) const
    {
        int     row( cell / 9 ), refRow( refCell / 9 );
        int     col( cell % 9 ), refCol( refCell % 9 );
        int     box( row / 3 * 3 + col / 3 ),
                refBox( refRow / 3 * 3 + refCol / 3 );
        if ( row == refRow && col == refCol )
            return false;
        return ( refRow == row || refCol == col || refBox == box );
    }
};


struct GetCommonBits : public std::binary_function< long, long, long >
{
    long operator() ( int  nmb1, int  nmb2 ) const
    {
        return ( nmb1 & nmb2 );
    }
};

struct SetCellBits : public std::binary_function< int, long, void >
{
    SetCellBits( BasicBoard *  board ) : board( board ) {}
    void operator() ( int  cell, long  state ) const
    {
        board->cellState[ cell ].state |= state;
    }
    BasicBoard *    board;
};

struct GetCellState : public std::unary_function< int, long >
{
    GetCellState( BasicBoard *  board ) : board( board ) {}
    long operator() ( int  cell ) const
    {
        return board->cellState[ cell ].state;
    }
    BasicBoard *    board;
};


static inline int  countSetBits9( long  nmb )
{
    nmb =  ( nmb & 0x49 ) + ( ( nmb >> 1 ) & 0x49 ) + ( ( nmb >> 2 ) & 0x49 );
    return ( nmb & 0x07 ) + ( ( nmb >> 3 ) & 0x07 ) + ( ( nmb >> 6 ) & 0x07 );
}


static inline long  getNthNotSetBit( long  nmb, int  n )
{
    for ( int  i( 0 ); i < n; ++i )
        nmb |= nmb + 1;
    return ( ~ nmb & ( nmb + 1 ) );
}


static inline int  bitToValue( long  state )
{
    int     result( 0 );
    do
    {
        if ( state == 1 )
            return result + 1;
        state >>= 1;
        ++result;
    } while ( result < 9 );
    return 0;
}


inline const char *  getStringWithNSpaces( size_t  n )
{
    static char     buf[ 128 ];
    char *          bufp( buf );
    for ( size_t  i( 0 ); i < n; ++i )
        *bufp++ = ' ';
    *bufp = 0;
    return buf;
}



BasicBoard::BasicBoard( void ) : availableCells( cells, cells + 81 ),
                                 nmbOfRetries( 0 )
{
    memset( cellState, 0, sizeof( CellState ) * 81 );
}


void  BasicBoard::restart( void )
{
    availableCells.clear();
    availableCells.insert( cells, cells + 81 );
    memset( cellState, 0, sizeof( CellState ) * 81 );
    while ( ! boardStateStack.empty() )
    {
        boardStateStack.pop();
    }
    nmbOfRetries = 0;
    publish();
}


void  BasicBoard::getAffectedCells( int  cell, CellList &  affectedCells ) const
{
    std::remove_copy_if( availableCells.begin(), availableCells.end(),
                         std::back_inserter( affectedCells ),
                         std::not1(
                             std::bind2nd( GetAffectedCells(), cell ) ) );
}


int  BasicBoard::getFirstMatureCell( void ) const
{
    int     result( -1 );
    int     maxSetBits( 0 );
    for ( std::set< int >::const_iterator  k( availableCells.begin() );
          k != availableCells.end(); ++k )
    {
        if ( maxSetBits >= 8 )
            return result;
        if ( result != -1 )
        {
            int     state( countSetBits9( cellState[ *k ].state ) );
            if ( state > maxSetBits )
            {
                maxSetBits = state;
                result = *k;
            }
        }
        else
        {
            maxSetBits = countSetBits9( cellState[ result ].state );
            result = *k;
        }
    }
    return result;
};


int  BasicBoard::setCellValue( int  cell, int  value )
{
    if ( value == 0 )
    {
        throw Unsolvable( Unsolvable::ZERO_VALUE,
                          cell, cellState[ cell ].state, value );
    }
    long    state( 1 << ( value - 1 ) );
    if ( cellState[ cell ].state & state )
    {
        throw Unsolvable( Unsolvable::VALUE_SET,
                          cell, cellState[ cell ].state, value );
    }
    cellState[ cell ].value = value;
    availableCells.erase( cell );
    CellList    affectedCells;
    getAffectedCells( cell, affectedCells );
    std::for_each( affectedCells.begin(), affectedCells.end(),
                   std::bind2nd( SetCellBits( this ), state ) );
    publish( cell, affectedCells );
    return commitSingletons();
}


void  BasicBoard::makeRandom( void )
{
    do
    {
        if ( availableCells.empty() )
            break;
        int     cell( rand() % ( *availableCells.rbegin() + 1 ) );
        cell = *availableCells.lower_bound( cell );
        int     nmbOfAvailBits( 9 - countSetBits9( cellState[ cell ].state ) );
        if ( nmbOfAvailBits == 0 )
        {
            throw Unsolvable( Unsolvable::ALL_BITS_SET, cell,
                              cellState[ cell ].state,
                              cellState[ cell ].value );
        }
        int     state( getNthNotSetBit( cellState[ cell ].state,
                       rand() % nmbOfAvailBits ) );
        if ( setCellValue( cell, bitToValue( state ) ) )
            break;
    } while ( true );
}


int  BasicBoard::getMaturity( int  cell ) const
{
    return countSetBits9( cellState[ cell ].state );
}


void  BasicBoard::saveToStack( int  cell, long  state )
{
#   ifdef DEBUG
        size_t  bssize( boardStateStack.size() );
        std::cout << getStringWithNSpaces( bssize ) << "> Cell " << cell <<
                     ", state " <<
                     std::bitset< 9 >( cellState[ cell].state ).to_string() <<
                     " => " << std::bitset< 9 >( state ).to_string() <<
                     ", stack size " << bssize << std::endl;
#   endif
    boardStateStack.push( BoardState() );
    memcpy( boardStateStack.top().cellState, cellState,
            sizeof( CellState ) * 81 );
    boardStateStack.top().availableCells = availableCells;
    boardStateStack.top().cell = cell;
    boardStateStack.top().state = state;
}


bool  BasicBoard::restoreFromStack( int &  cell, long &  state )
{
    if ( boardStateStack.empty() )
        return false;
    cell = boardStateStack.top().cell;
    state = boardStateStack.top().state;
#   ifdef DEBUG
        size_t  bssize( boardStateStack.size() );
        std::cout << getStringWithNSpaces( bssize - 1 ) << "< Cell " << cell <<
                     ", state " << std::bitset< 9 >( state ).to_string() <<
                     " <= " <<
                     std::bitset< 9 >( cellState[ cell].state ).to_string() <<
                     ", stack size " << bssize - 1 << std::endl;
#   endif
    memcpy( cellState, boardStateStack.top().cellState,
            sizeof( CellState ) * 81 );
    availableCells = boardStateStack.top().availableCells;
    boardStateStack.pop();
    publish();
    return true;
}


template< typename GetGrpCellsFun >
void  BasicBoard::chkGrpCellState( int  cell, const CellList &  affectedCells )
{
    CellList            grpCells;
    std::list< long >   grpStates;
    std::remove_copy_if( affectedCells.begin(), affectedCells.end(),
                         std::back_inserter( grpCells ),
                         std::not1( std::bind2nd( GetGrpCellsFun(), cell ) ) );
    std::transform( grpCells.begin(), grpCells.end(),
                    std::back_inserter( grpStates ), GetCellState( this ) );
    long    grpMatesState( std::accumulate( grpStates.begin(), grpStates.end(),
                                       0x1FF, GetCommonBits() ) );
    if ( grpStates.empty() )
        grpMatesState = 0;
    int     grpSize( grpCells.size() + 1 );
    long    grpState( grpMatesState & cellState[ cell ].state );
    if ( grpSize > countSetBits9( ~ grpState ) )
    {
        throw Unsolvable( Unsolvable::INSUF_STCAPACITY, cell,
                          cellState[ cell ].state,
                          cellState[ cell ].value );
    }
    int   state( ( grpMatesState ^ cellState[ cell ].state ) & grpMatesState );
    if ( state )
    {
        cellState[ cell ].state = ~ state & 0x1FF;
        publish( cell );
    }
}


inline void  BasicBoard::applyHeuristics( int  cell,
                                          const CellList &  affectedCells )
{
    chkGrpCellState< GetRowCells >( cell, affectedCells );
    chkGrpCellState< GetColCells >( cell, affectedCells );
    chkGrpCellState< GetBoxCells >( cell, affectedCells );
}


int  BasicBoard::commitSingletons( void )
{
    int     result( 0 );
    do
    {
        bool    changeMade( false );
        for ( std::set< int >::iterator  k( availableCells.begin() );
              k != availableCells.end(); )
        {
            int     cell( *k );
            CellList    affectedCells;
            getAffectedCells( cell, affectedCells );
            applyHeuristics( cell, affectedCells );
            if( cellState[ cell ].state == 0x1FF )
            {
                throw Unsolvable( Unsolvable::ALL_BITS_SET, cell,
                                  cellState[ cell ].state,
                                  cellState[ cell ].value );
            }

            if ( countSetBits9( cellState[ cell ].state ) == 8 )
            {
                int     state( ~ cellState[ cell ].state & 0x1FF );
                cellState[ cell ].value = bitToValue( state );
                availableCells.erase( k++ );
                std::for_each( affectedCells.begin(), affectedCells.end(),
                               std::bind2nd( SetCellBits( this ), state ) );
                publish( cell, affectedCells );
                ++result;
                changeMade = true;
            }
            else
                ++k;
        }

        if ( ! changeMade )
            break;
    } while ( true );
    return result;
}


void  BasicBoard::solve( void )
{
    int     cell( 0 );
    long    curState( 0 );
    long    curOverallState( 0 );

    while ( ! availableCells.empty() )
    {
        bool    badWayCaught( false );
        do
        {
            if ( badWayCaught )
            {
                do
                {
                    if ( ! restoreFromStack( cell, curOverallState ) )
                    {
                        throw Unsolvable( Unsolvable::FATAL, cell,
                                          curOverallState, 0 );
                    }
                    if ( curOverallState != 0x1FF )
                        break;
                } while ( true );
                badWayCaught = false;
            }
            else
            {
                cell = getFirstMatureCell();
                curOverallState = cellState[ cell ].state;
                availableCells.erase( cell );
            }
            try
            {
                do
                {
                    int  nmbOfAvailBits( 9 - countSetBits9( curOverallState ) );
                    if ( nmbOfAvailBits == 0 )
                    {
                        throw Unsolvable( Unsolvable::ALL_BITS_SET, cell,
                                          cellState[ cell ].state,
                                          bitToValue( curState ) );
                    }
                    curState = getNthNotSetBit( curOverallState,
                                                rand() % nmbOfAvailBits );
                    curOverallState |= curState;
                    saveToStack( cell, curOverallState );
                    cellState[ cell ].value = bitToValue( curState );
                    CellList    affectedCells;
                    getAffectedCells( cell, affectedCells );
                    std::for_each( affectedCells.begin(), affectedCells.end(),
                                   std::bind2nd(
                                       SetCellBits( this ), curState ) );
                    publish( cell, affectedCells );
                    try
                    {
                        commitSingletons();
                        break;
                    }
                    catch ( Unsolvable &  e )
                    {
                        restoreFromStack( cell, curOverallState );
                        ++nmbOfRetries;
                    }
                } while ( true );
            }
            catch ( Unsolvable &  e )
            {
                badWayCaught = true;
            }
        } while ( badWayCaught );
    }
}

};  // namespace SudokuRapid

