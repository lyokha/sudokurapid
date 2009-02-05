/*
 * =============================================================================
 *
 *       Filename:  sudokuForm.cc
 *
 *    Description:  SudokuForm implementation
 *
 *        Version:  1.0
 *        Created:  02.02.2009 17:45:22
 *       Revision:  none
 *
 *         Author:  Alexey Radkov (lyokha)
 *        Company:  
 *
 * =============================================================================
 */

#include <QMessageBox>
#include <QFileDialog>

#include "sudokuForm.h"
#include "sudokuScene.h"
#include "sudokuCell.h"



SudokuForm::SudokuForm( QWidget *  parent ) : scene( new SudokuScene )
{
    Q_UNUSED( parent );
    setupUi( this );

    graphicsView->setScene( scene );
    connect( scene, SIGNAL( valueSet( int, int ) ),
                    SLOT( updateBoard( int, int ) ) );
    connect( newButton, SIGNAL( clicked() ), SLOT( restartBoard() ) );
    connect( openButton, SIGNAL( clicked() ), SLOT( openFile() ) );
    connect( randomButton, SIGNAL( clicked() ), SLOT( randomBoard() ) );
    connect( solveButton, SIGNAL( clicked() ), SLOT( solveBoard() ) );
}


void  SudokuForm::updateBoard( int  cell, int  value )
{
    try
    {
        setCellValue( cell, value );
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        QMessageBox::critical( this, QObject::tr( "Value cannot be set" ),
                QObject::tr( e.what() ) );
    }
}


void  SudokuForm::restartBoard( void )
{
    restart();
}


void  SudokuForm::randomBoard( void )
{
    try
    {
        makeRandom();
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        QMessageBox::critical( this, QObject::tr( "Error when randomizing" ),
                QObject::tr( e.what() ) );
    }
}


void  SudokuForm::solveBoard( void )
{
    try
    {
        solve();
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        QMessageBox::critical( this, QObject::tr( "Board is not solvable" ),
                QObject::tr( e.what() ) );
    }
}


void  SudokuForm::openFile( void )
{
    QString     fileName( QFileDialog::getOpenFileName( this,
                            tr( "Open file" ) ) );
    restart();
    try
    {
        readFromFile( fileName.toLatin1().constData() );
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        QMessageBox::critical( this, QObject::tr( "Board is not solvable" ),
                QObject::tr( e.what() ) );
    }
}


void  SudokuForm::publish( void )
{
    for ( int  i( 0 ); i < 81; ++i )
    {
        scene->getCell( i )->setValue( cellState[ i ].value );
        scene->getCell( i )->setMaturity( getMaturity( i ) );
    }
    scene->invalidate();
}


void  SudokuForm::publish( int  cell )
{
    scene->getCell( cell )->setMaturity( getMaturity( cell ) );
    scene->invalidate();
}


void  SudokuForm::publish( int cell, const CellList &  affectedCells )
{
    scene->getCell( cell )->setValue( cellState[ cell ].value );
    for ( CellList::const_iterator  k( affectedCells.begin() );
                k != affectedCells.end(); ++k )
    {
        scene->getCell( *k )->setMaturity( getMaturity( *k ) );
    }
    scene->invalidate();
}

