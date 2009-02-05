/*
 * =============================================================================
 *
 *       Filename:  sudokuCell.cc
 *
 *    Description:  SudokuCell implementation
 *
 *        Version:  1.0
 *        Created:  04.02.2009 14:17:50
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha), alexey.radkov@gmail.com
 *        Company:  StarSoft Development Labs
 *
 * =============================================================================
 */

#include <QFont>
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include "sudokuCell.h"


namespace
{
    QFont   font( "Sans", 18 );
    QColor  brushInitColor( Qt::white );
    QColor  penColor( 128, 128, 64 );
    QColor  penColorFocused( 96, 96, 64 );
}


SudokuCell::SudokuCell( qreal  x, qreal  y, qreal  width, qreal  height,
                        int  number, QGraphicsItem *  parent ) :
    QGraphicsRectItem( x, y, width, height, parent ),
    number( number ), maturity( 0 ), value( 0 ), isHovered( false )
{
    setFlag( QGraphicsItem::ItemIsFocusable );
    setAcceptHoverEvents( true );
}


void  SudokuCell::paint( QPainter *  painter,
                         const QStyleOptionGraphicsItem *  /*option*/,
                         QWidget *  /*widget*/ )
{
    QPen    pen( getColor() );
    if ( value )
        pen.setColor( penColor );
    if( hasFocus() )
        pen.setColor( penColorFocused );
    painter->setBrush( QBrush( getColor() ) );
    painter->setFont( font );
    painter->setPen( pen );
    painter->drawRect( rect() );
    QString     text;
    switch ( value )
    {
        case 1 : case 2 : case 3 :
        case 4 : case 5 : case 6 :
        case 7 : case 8 : case 9 :
            text = QString::number( value );
            painter->drawText( rect().adjusted( 8, -6, 8, -6 ).bottomLeft(),
                               text );
            break;
        default:
            break;
    }
}


void  SudokuCell::hoverEnterEvent( QGraphicsSceneHoverEvent *  /*event*/ )
{
    isHovered = true;
    update();
}


void  SudokuCell::hoverLeaveEvent( QGraphicsSceneHoverEvent *  /*event*/ )
{
    isHovered = false;
    update();
}


void  SudokuCell::focusInEvent( QFocusEvent *  /*event*/ )
{
    update();
}


void  SudokuCell::focusOutEvent( QFocusEvent *  /*event*/ )
{
    update();
}


QColor  SudokuCell::getColor( void ) const
{
    QColor  color( brushInitColor );
    if ( value )
        color.setRgb( 128, 192, 255 );
    else
        if ( maturity )
            color.setRgb( 255, 255 / 9 * ( 9 - maturity ),
                               255 / 9 * ( 9 - maturity ) );
    if ( isHovered )
        color = color.lighter( 110 );
    return color;
}

