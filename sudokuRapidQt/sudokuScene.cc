/*
 * =============================================================================
 *
 *       Filename:  sudokuScene.cc
 *
 *    Description:  SudokuScene implementation
 *
 *        Version:  1.0
 *        Created:  04.02.2009 14:21:37
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "sudokuScene.h"
#include "sudokuCell.h"


namespace
{
    QColor  minorGridLinesColor( 192, 192, 192 );
}


SudokuScene::SudokuScene( void ) : isEnabled( true )
{
    for ( int  i( 0 ); i < 10; ++i )
    {
        lh[ i ] = addLine( QLineF( 0, i * 30, 270, i * 30 ) );
        lv[ i ] = addLine( QLineF( i * 30, 0, i * 30, 270 ) );
        if ( i % 3 )
        {
            lh[ i ]->setPen( QPen( minorGridLinesColor ) );
            lv[ i ]->setPen( QPen( minorGridLinesColor ) );
        }
        else
        {
            lh[ i ]->setZValue( 1 );
            lv[ i ]->setZValue( 1 );
        }
    }
    for ( int  i( 0 ); i < 81; ++i )
    {
        cell[ i ] = new SudokuCell( i % 9 * 30 + 1, i / 9 * 30 + 1, 28, 28, i );
        addItem( cell[ i ] );
    }
}


SudokuCell *  SudokuScene::getCell( int  number )
{
    return cell[ number ];
}


void  SudokuScene::enable( bool  enabled )
{
    if ( enabled == isEnabled )
        return;
    for ( int  i( 0 ); i < 81; ++i )
    {
        cell[ i ]->setEnabled( enabled );
    }
    isEnabled = enabled;
}


void  SudokuScene::mousePressEvent( QGraphicsSceneMouseEvent *  event )
{
    SudokuCell *  clicked( dynamic_cast< SudokuCell * >(
                           itemAt( event->scenePos() ) ) );
    if ( clicked )
    {
        clicked->setFocus( Qt::MouseFocusReason );
    }
}


void  SudokuScene::keyPressEvent( QKeyEvent *  event )
{
    SudokuCell *    focused( dynamic_cast< SudokuCell * >( focusItem() ) );
    if ( ! focused )
        return;
    int     row( focused->getNumber() / 9 );
    int     col( focused->getNumber() % 9 );
    switch ( event->key() )
    {
        case Qt::Key_1 : case Qt::Key_2 : case Qt::Key_3 :
        case Qt::Key_4 : case Qt::Key_5 : case Qt::Key_6 :
        case Qt::Key_7 : case Qt::Key_8 : case Qt::Key_9 :
            if ( focused->getValue() )
                break;
            emit valueSet( focused->getNumber(), event->key() - Qt::Key_0 );
            break;
        case Qt::Key_Right : case Qt::Key_D :
            cell[ row * 9 + ( col + 1 ) % 9 ]->setFocus();
            break;
        case Qt::Key_Left : case Qt::Key_A :
            cell[ row * 9 + ( col + 8 ) % 9 ]->setFocus();
            break;
        case Qt::Key_Up : case Qt::Key_W :
            cell[ ( row + 8 ) % 9 * 9 + col ]->setFocus();
            break;
        case Qt::Key_Down : case Qt::Key_S :
            cell[ ( row + 1 ) % 9 * 9 + col ]->setFocus();
            break;
        default:
            break;
    }
}

