/*
 * =============================================================================
 *
 *       Filename:  sudokuForm.h
 *
 *    Description:  SudokuForm header file
 *
 *        Version:  1.0
 *        Created:  02.02.2009 17:13:33
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#ifndef SUDOKUFORM_H
#define SUDOKUFORM_H


#include "ui_sudokuForm.h"
#include "sudokuRapid.h"

class SudokuScene;


class SudokuForm : public QWidget,
                   private SudokuRapid::BasicBoard,
                   private Ui::SudokuForm
{
    Q_OBJECT

    public:
        SudokuForm( QWidget *  parent = 0 );

    public slots:
        void  updateBoard( int  cell, int  value );

        void  showHint( int  cell );

        void  restartBoard( void );

        void  randomBoard( void );

        void  solveBoard( void );

        void  openFile( void );

    private:
        void  publish( void );

        void  publish( int  cell );

        void  publish( int  cell, const CellList &  affectedCells );

        void  makeAdditionalCleanup( void );

        void  enable( bool  enabled =  true );

    private:
        bool            isEnabled;
        SudokuScene *   scene;
};


#endif

