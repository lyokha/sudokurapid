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

        void  setValue( int  val )
        {
            value = val;
        }

        void  setValueReadRest( int  val )
        {
            valueReadRest = val;
        }

        void  setMaturity( int  val )
        {
            maturity = val;
        }

        int  getValue( void ) const
        {
            return value;
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
        QColor  getColor( void ) const;

    private:
    int     number;
    int     maturity;
    int     value;
    int     valueReadRest;
    bool    isHovered;
    bool    isError;
};


#endif

