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
#include <QFont>
#include <QFontMetrics>
#include "sudokuRapidCommon.h"

class SudokuCell;


class SudokuScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        explicit SudokuScene( const QString &  fontFamily = "Sans" );

        void  mousePressEvent( QGraphicsSceneMouseEvent *  event );

        void  keyPressEvent( QKeyEvent *  event );

        void  enable( bool  enabled = true );

        void  setCellValue( int  nmb, int  value, bool  deduced = false );

        void  setCellMaturity( int  nmb, int  maturity );

        void  setCellError( int  nmb, bool error = true );

        void  setCellHint( int  nmb, SudokuRapid::CellValues &  values );

        void  cleanupCells( void );

    private:
        QGraphicsLineItem *     lh[ 10 ];
        QGraphicsLineItem *     lv[ 10 ];
        SudokuCell *            cell[ 81 ];

    signals:
        void    valueSet( int  cell, int  value );

        void    wantHint( int  cell );
};


#endif

