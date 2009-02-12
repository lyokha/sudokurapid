/*
 * =============================================================================
 *
 *       Filename:  sudokuCell.h
 *
 *    Description:  SudokuCell header file
 *
 *        Version:  1.0
 *        Created:  04.02.2009 14:09:07
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H


#include <QGraphicsRectItem>

#include "sudokuRapidCommon.h"


class SudokuCell : public QGraphicsRectItem
{
    public:
        SudokuCell( qreal  x, qreal  y, qreal  width, qreal  height,
                    int  number, QGraphicsItem *  parent = 0 );

        void  paint( QPainter *  painter,
                     const QStyleOptionGraphicsItem *  option,
                     QWidget *  widget = 0 );

        void  hoverEnterEvent( QGraphicsSceneHoverEvent *  event );

        void  hoverLeaveEvent( QGraphicsSceneHoverEvent *  event );

        void  focusInEvent( QFocusEvent *  event );

        void  focusOutEvent( QFocusEvent *  event );

        void  showHint( SudokuRapid::CellValues &  values );

        void  setValue( int  value, bool  deduced = false );

        void  setMaturity( int  value )
        {
            maturity = value;
        }

        int   getValueAssigned( void ) const
        {
            return valueAssigned;
        }

        int   getValueDeduced( void ) const
        {
            return valueDeduced;
        }

        bool  isSet( void ) const
        {
            return valueAssigned || valueDeduced;
        }

        int   getNumber( void ) const
        {
            return number;
        }

        void  setError( bool  error = true )
        {
            isError = error;
        }

    private:
        QColor  getBrushColor( void ) const;

        QColor  getPenColor( void ) const;

    private:
    int     number;
    int     maturity;
    int     valueAssigned;
    int     valueDeduced;
    bool    isHovered;
    bool    isError;
    bool    isHintVisible;

    SudokuRapid::CellValues     hintValues;
};


#endif

