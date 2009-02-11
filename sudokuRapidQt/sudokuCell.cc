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
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
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
    QFont   fontReadRest( "Sans", 12 );
    QColor  brushInitColor( Qt::white );
    QColor  brushErrorColor( Qt::red );
    QColor  penColor( 128, 128, 64 );
    QColor  penColorFocused( 96, 96, 64 );
    QColor  penErrorColor( Qt::black );
}


SudokuCell::SudokuCell( qreal  x, qreal  y, qreal  width, qreal  height,
                        int  number, QGraphicsItem *  parent ) :
    QGraphicsRectItem( x, y, width, height, parent ),
    number( number ), maturity( 0 ), value( 0 ), valueReadRest( 0 ),
    isHovered( false ), isError( false )
{
    setFlag( QGraphicsItem::ItemIsFocusable );
    setAcceptHoverEvents( true );
}


void  SudokuCell::paint( QPainter *  painter,
                         const QStyleOptionGraphicsItem *  /*option*/,
                         QWidget *  /*widget*/ )
{
    QPen    pen( getColor() );
    int     valueAny( value );
    if ( ! valueAny )
        valueAny = valueReadRest;
    if ( valueAny )
        pen.setColor( penColor );
    if( hasFocus() )
        pen.setColor( penColorFocused );
    if ( isError )
        pen.setColor( penErrorColor );
    painter->setPen( pen );
    painter->setBrush( QBrush( getColor() ) );
    if ( valueReadRest )
        painter->setFont( fontReadRest );
    else
        painter->setFont( font );
    painter->drawRect( rect() );
    if ( ! valueAny )
        return;
    QString     text;
    QRectF      textRect( rect().adjusted( 7, -5, 7, -5 ) );
    if ( valueReadRest )
        textRect = rect().adjusted( 2, -12, 2, -12 );
    switch ( valueAny )
    {
        case 1 : case 2 : case 3 :
        case 4 : case 5 : case 6 :
        case 7 : case 8 : case 9 :
            text = QString::number( valueAny );
            painter->drawText( textRect.bottomLeft(), text );
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
            color.setRgb( 255, 170 / 9 * ( 9 - maturity ) + 85,
                               170 / 9 * ( 9 - maturity ) + 85 );
    if ( isError )
        color = brushErrorColor;
    if ( isHovered )
        color = color.lighter( 110 );
    return color;
}

