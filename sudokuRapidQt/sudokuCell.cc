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
#include <QFontMetrics>
#include <QColor>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>

#include "sudokuCell.h"


namespace
{
    QColor  brushInitColor( Qt::white );
    QColor  brushErrorColor( Qt::red );
    QColor  brushValueSetColor( 128, 192, 255 );
    QColor  penValueSetColor( 96, 96, 64 );
    QColor  penColorFocused( 80, 80, 56 );
    QColor  penErrorColor( Qt::black );
    QColor  penHintHoverColor( 192, 128, 128 );
}


SudokuCell::SudokuCell( qreal  x, qreal  y, qreal  width, qreal  height,
                        int  number, int  smallerFontSize,
                        QGraphicsItem *  parent ) :
    QGraphicsRectItem( x, y, width, height, parent ),
    number( number ), maturity( 0 ), valueAssigned( 0 ), valueDeduced( 0 ),
    smallerFontSize( smallerFontSize ), hintPos( 0 ),
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
    isHintVisible = true;
}


void  SudokuCell::paint( QPainter *  painter,
                         const QStyleOptionGraphicsItem *  /*option*/,
                         QWidget *  /*widget*/ )
{
    painter->setPen( QPen( getPenColor() ) );
    painter->setBrush( QBrush( getBrushColor() ) );
    painter->drawRect( rect() );
    QFont           font( scene()->font() );
    painter->setFont( font );
    if ( ! isSet() )
    {
        if ( isHintVisible )
        {
            font.setPixelSize( smallerFontSize );
            painter->setFont( font );
            for ( SudokuRapid::CellValues::const_iterator  k(
                        hintValues.begin() ); k != hintValues.end(); ++k )
            {
                painter->setPen( QPen( getPenColor() ) );
                if ( hintPos == *k )
                    painter->setPen( QPen( penHintHoverColor ) );
                drawNumber( painter, *k, *k );
            }
        }
    }
    else
    {
        switch ( valueAssigned )
        {
            case 1 : case 2 : case 3 :
            case 4 : case 5 : case 6 :
            case 7 : case 8 : case 9 :
                drawNumber( painter, valueAssigned );
                break;
            default:
                break;
        }
        if ( valueDeduced == valueAssigned )
            return;
        int     pos( 0 );
        switch ( valueDeduced )
        {
            case 1 : case 2 : case 3 :
            case 4 : case 5 : case 6 :
            case 7 : case 8 : case 9 :
                if ( valueAssigned )
                {
                    font.setPixelSize( smallerFontSize );
                    painter->setFont( font );
                    pos = 1;
                }
                drawNumber( painter, valueDeduced, pos );
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
    hintPos = 0;
    update();
}


void  SudokuCell::hoverMoveEvent( QGraphicsSceneHoverEvent *  event )
{
    if ( ! hasFocus() || ! isHintVisible )
        return;
    int     lastHintPos( hintPos );
    hintPos = getHintPos( event->pos() - rect().topLeft() );
    if ( hintPos == lastHintPos )
        return;
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
    if ( ! isEnabled() )
        color.setHsvF( color.hueF(), color.saturationF() * 0.8,
                                     color.valueF() * 0.8 );
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


int  SudokuCell::getHintPos( const QPointF &  pos ) const
{
    int     x( pos.x() / ( rect().width() / 3 ) );
    int     y( pos.y() / ( rect().height() / 3 ) );
    return y * 3 + x + 1;
}


int  SudokuCell::getValueAssociated( const QPointF &  pos ) const
{
    if ( ! hasFocus() || ! isHintVisible )
        return 0;
    return getHintPos( pos );
}


void  SudokuCell::drawNumber( QPainter *  painter, int  nmb, int  pos )
{
    QPointF     adjust( 1, 1 );
    if ( pos > 0 )
    {
        adjust.rx() += ( ( pos - 1 ) % 3 - 1 ) * rect().width() / 3;
        adjust.ry() += ( ( pos - 1 ) / 3 - 1 ) * rect().height() / 3;
    }
    QRectF      drawRect( rect().translated( adjust ) );
    painter->drawText( drawRect, Qt::AlignCenter, QString::number( nmb ) );
}

