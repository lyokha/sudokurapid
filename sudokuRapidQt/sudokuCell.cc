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
    QFont   fontValue( "Sans", 18 );
    QFont   fontExtraValue( "Sans", 8 );
    QFont   fontHintValue( "Sans", 8 );
    QColor  brushInitColor( Qt::white );
    QColor  brushErrorColor( Qt::red );
    QColor  brushValueSetColor( 128, 192, 255 );
    QColor  penValueSetColor( 128, 128, 64 );
    QColor  penColorFocused( 96, 96, 64 );
    QColor  penErrorColor( Qt::black );
}


SudokuCell::SudokuCell( qreal  x, qreal  y, qreal  width, qreal  height,
                        int  number, QGraphicsItem *  parent ) :
    QGraphicsRectItem( x, y, width, height, parent ),
    number( number ), maturity( 0 ), valueAssigned( 0 ), valueDeduced( 0 ),
    isHovered( false ), isError( false ), isHintVisible( false )
{
    setFlag( QGraphicsItem::ItemIsFocusable );
    setAcceptHoverEvents( true );
}


void  SudokuCell::setValue( int  value, bool  deduced )
{
    if ( deduced )
        valueDeduced = value;
    else
        valueAssigned = value;
}


void  SudokuCell::showHint( SudokuRapid::CellValues &  values )
{
    hintValues = values;
    isHintVisible =true;
}


void  SudokuCell::paint( QPainter *  painter,
                         const QStyleOptionGraphicsItem *  /*option*/,
                         QWidget *  /*widget*/ )
{
    painter->setPen( QPen( getPenColor() ) );
    painter->setBrush( QBrush( getBrushColor() ) );
    painter->drawRect( rect() );
    QString     text;
    QRectF      textRect( rect() );
    if ( ! isSet() )
    {
        if ( isHintVisible )
        {
            painter->setFont( fontHintValue );
            for ( SudokuRapid::CellValues::const_iterator  k(
                        hintValues.begin() ); k != hintValues.end(); ++k )
            {
                text = QString::number( *k );
                QRectF  textRectAdjusted( textRect.adjusted(
                                                    ( *k - 1 ) % 3 * 9 + 2,
                                                    ( *k - 1 ) / 3 * 9 + 9,
                                                    ( *k - 1 ) % 3 * 9 + 2,
                                                    ( *k - 1 ) / 3 * 9 + 9 ) );
                painter->drawText( textRectAdjusted.topLeft(), text );
            }
        }
    }
    else
    {
        textRect = rect().adjusted( 7, -5, 7, -5 );
        painter->setFont( fontValue );
        switch ( valueAssigned )
        {
            case 1 : case 2 : case 3 :
            case 4 : case 5 : case 6 :
            case 7 : case 8 : case 9 :
                text = QString::number( valueAssigned );
                painter->drawText( textRect.bottomLeft(), text );
                break;
            default:
                break;
        }
        if ( valueDeduced == valueAssigned )
            return;
        if ( valueAssigned )
        {
            textRect = rect().adjusted( 2, -17, 2, -17 );
            painter->setFont( fontExtraValue );
        }
        switch ( valueDeduced )
        {
            case 1 : case 2 : case 3 :
            case 4 : case 5 : case 6 :
            case 7 : case 8 : case 9 :
                text = QString::number( valueDeduced );
                painter->drawText( textRect.bottomLeft(), text );
                break;
            default:
                break;
        }
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
    isHintVisible = false;
    update();
}


QColor  SudokuCell::getBrushColor( void ) const
{
    QColor  color( brushInitColor );
    if ( isError )
        color = brushErrorColor;
    else
        if ( valueAssigned )
            color = brushValueSetColor;
        else
            if ( valueDeduced )
                color = brushValueSetColor.lighter( 130 );
            else
                if ( maturity )
                    color.setRgb( 255, 170 / 9 * ( 9 - maturity ) + 85,
                                       170 / 9 * ( 9 - maturity ) + 85 );
    if ( isHovered )
        color = color.lighter( 110 );
    return color;
}


QColor  SudokuCell::getPenColor( void ) const
{
    QColor  color( getBrushColor() );
    if ( hasFocus() )
        color = penColorFocused;
    else
        if ( isError )
            color = penErrorColor;
        else
            if ( valueAssigned )
                color = penValueSetColor;
            else
                if ( valueDeduced )
                    color = penValueSetColor.lighter( 130 );
    if ( isHovered )
        color = color.lighter( 110 );
    return color;
}

