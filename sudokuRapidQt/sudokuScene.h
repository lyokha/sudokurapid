/*
 * =============================================================================
 *
 *       Filename:  sudokuScene.h
 *
 *    Description:  SudokuScene header file
 *
 *        Version:  1.0
 *        Created:  04.02.2009 14:12:32
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#ifndef SUDOKUSCENE_H
#define SUDOKUSCENE_H


#include <QGraphicsScene>

class SudokuCell;


class SudokuScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        SudokuScene( void );

        void  mousePressEvent( QGraphicsSceneMouseEvent *  event );

        void  keyPressEvent( QKeyEvent *  event );

        SudokuCell *  getCell( int  number );

    private:
        QGraphicsLineItem *     lh[ 10 ];
        QGraphicsLineItem *     lv[ 10 ];
        SudokuCell *            cell[ 81 ];

    signals:
        void    valueSet( int  cell, int  value );
};


#endif

