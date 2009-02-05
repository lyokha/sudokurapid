/*
 * =============================================================================
 *
 *       Filename:  sudokuRapid.h
 *
 *    Description:  Sudoku Solver Lib header file
 *
 *        Version:  1.0
 *        Created:  29.01.2009 20:24:43
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#ifndef SUDOKURAPID_H
#define SUDOKURAPID_H


#include <set>
#include <list>
#include <stack>
#include <stdexcept>


namespace SudokuRapid {

struct Unsolvable : std::exception
{
    enum Type
    {
        VALUE_SET,
        ALL_BITS_SET,
        INSUF_STCAPACITY,
        ZERO_VALUE,
        FATAL
    };

    Unsolvable( Type  type, int  cell, long  state, int value ) :
        type( type ), cell( cell ), state( state ), value( value )
    {}

    ~Unsolvable( void ) throw()
    {}

    const char *  what() const throw()
    {
        switch ( type )
        {
            case VALUE_SET :
                return "Cannot set value.";
            case ALL_BITS_SET :
                return "All bits set.";
            case INSUF_STCAPACITY :
                return "Insufficient state capacity.";
            case ZERO_VALUE :
                return "Attempt to asign zero value to a cell.";
            case FATAL :
                return "The board is not solvable.";
            default :
                return "Never should get here. ";
        }
    }

    Type            type;
    int             cell;
    long            state;
    int             value;
};


class  BasicBoard
{
    friend class SetCellBits;
    friend class GetCellState;

    protected:
        typedef std::list< int >            CellList;

        typedef std::set< int >             CellSet;

        struct CellState
        {
            long    state;
            int     value;
        };

        struct BoardState
        {
            CellState   cellState[ 81 ];
            CellSet     availableCells;
            int         cell;
            long        state;
        };

        typedef std::stack< BoardState >    BoardStack;

    public:
        BasicBoard( void );

        virtual ~BasicBoard( void ) {}

        void  restart( void );

        void  readFromFile( const char *  fileName );

        void  makeRandom( void );

        int   setCellValue( int  cell, int  value );

        void  solve( void );

        int   getMaturity( int  cell ) const;

        unsigned int  getNumberOfRetries( void ) const
        {
            return nmbOfRetries;
        }

    protected:
        virtual void   publish( void )
        {}

        virtual void   publish( int  /*cell*/ )
        {}

        virtual void   publish( int  /*cell*/,
                                const CellList &  /*affectedCells*/ )
        {}

    private:
        void  getAffectedCells( int  cell, CellList &  affectedCells ) const;

        int   getFirstMatureCell( void ) const;

        void  saveToStack( int  cell, long  state );

        bool  restoreFromStack( int &  cell, long &  state );

        template< typename  GetGrpCellsFun >
        void  chkGrpCellState( int  cell, const CellList &  affectedCells );

        void  applyHeuristics( int  cell, const CellList &  affectedCells );

        int   commitSingletons( void );

    protected:
        CellState       cellState[ 81 ];

    private:
        CellSet         availableCells;

        BoardStack      boardStateStack;

        unsigned int    nmbOfRetries;

};

};  // namespace SudokuRapid


#endif  // SUDOKURAPID_H

