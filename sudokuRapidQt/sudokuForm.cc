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

#include <fstream>
#include <QMessageBox>
#include <QFileDialog>

#include "sudokuForm.h"
#include "sudokuScene.h"
#include "sudokuCell.h"



SudokuForm::SudokuForm( QWidget *  parent ) :
    isEnabled( true ), scene( new SudokuScene )
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
    if ( ! isEnabled )
        return;
    try
    {
        setCellValue( cell, value );
    }
    catch ( SudokuRapid::Unsolvable &  /*e*/ )
    {
        QMessageBox::critical( this, QObject::tr( "Error" ),
                QObject::tr( "Value cannot be set, try again" ) );
    }
}


void  SudokuForm::restartBoard( void )
{
    restart();
    makeAdditionalCleanup();
    enable();
}


void  SudokuForm::randomBoard( void )
{
    if ( ! isEnabled )
        return;
    try
    {
        makeRandom();
    }
    catch ( SudokuRapid::Unsolvable &  /*e*/ )
    {
        enable( false );
        QMessageBox::critical( this, QObject::tr( "Error" ),
                QObject::tr( "Sorry: badly randomized" ) );
    }
}


void  SudokuForm::solveBoard( void )
{
    if ( ! isEnabled )
        return;
    try
    {
        solve();
    }
    catch ( SudokuRapid::Unsolvable &  e )
    {
        enable( false );
        scene->getCell( e.cell )->setError( true );
        QMessageBox::critical( this, QObject::tr( "Error" ),
                QObject::tr( "Board is not solvable" ) );
    }
}


void  SudokuForm::openFile( void )
{
    QString         fileName( QFileDialog::getOpenFileName(
                                this, tr( "Open File" ) ) );
    std::ifstream   file( fileName.toLatin1().constData() );
    file.exceptions( std::ios::failbit );
    restartBoard();
    char    number( 0 );
    bool    unsolvableCaught( false );
    for ( int i( 0 ); i < 9; ++i )
    {
        for ( int j( 0 ); j < 9; ++j )
        {
            file >> number;
            if ( ! isdigit( number ) )
                continue;
            int     cell( i * 9 + j );
            int     value( number - '0' );
            if ( unsolvableCaught )
            {
                if ( ! cellState[ cell ].value )
                    scene->getCell( cell )->setValueReadRest( value );
            }
            else
            {
                try
                {
                    setCellValue( cell, value );
                }
                catch ( SudokuRapid::Unsolvable &  /*e*/ )
                {
                    enable( false );
                    if ( ! cellState[ cell ].value )
                        scene->getCell( cell )->setValueReadRest( value );
                    scene->getCell( cell )->setError( true );
                    unsolvableCaught = true;
                }
            }
        }
    }
    if ( unsolvableCaught )
        QMessageBox::critical( this, QObject::tr( "Error" ),
                               QObject::tr( "Cannot set value in red box" ) );
    file.close();
}


void  SudokuForm::makeAdditionalCleanup( void )
{
    for ( int  i( 0 ); i < 81; ++i )
    {
        scene->getCell( i )->setValueReadRest( 0 );
        scene->getCell( i )->setError( false );
    }
    scene->invalidate();
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


void  SudokuForm::enable( bool  enabled )
{
    isEnabled = enabled;
    randomButton->setEnabled( enabled );
    solveButton->setEnabled( enabled );
    scene->enable( enabled );
}

