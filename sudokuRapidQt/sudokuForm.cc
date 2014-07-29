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
#include <QString>
#include <QFileDialog>

#include "sudokuForm.h"
#include "sudokuScene.h"
#include "sudokuCell.h"
#include "sudokuRapidCommon.h"



SudokuForm::SudokuForm( QWidget *  parent ) :
    isEnabled( true ), scene( new SudokuScene )
{
    Q_UNUSED( parent );
    setupUi( this );

    graphicsView->setScene( scene );
    connect( scene, SIGNAL( valueSet( int, int ) ),
                    SLOT( updateBoard( int, int ) ) );
    connect( scene, SIGNAL( wantHint( int ) ), SLOT( showHint( int ) ) );
    connect( newButton, SIGNAL( clicked() ), SLOT( restartBoard() ) );
    connect( openButton, SIGNAL( clicked() ), SLOT( openFile() ) );
    connect( randomButton, SIGNAL( clicked() ), SLOT( randomBoard() ) );
    connect( solveButton, SIGNAL( clicked() ), SLOT( solveBoard() ) );
}


SudokuForm::~SudokuForm()
{
    delete scene;
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
        return;
    }
    scene->setCellValue( cell, value );
}


void  SudokuForm::showHint( int  cell )
{
    SudokuRapid::CellValues     values;
    getFreeValues( cell, values );
    scene->setCellHint( cell, values );
}


void  SudokuForm::restartBoard( void )
{
    scene->cleanupCells();
    restart();
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
    catch ( SudokuRapid::Unsolvable &  e )
    {
        enable( false );
        scene->setCellError( e.cell );
        showHint( e.cell );
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
        scene->setCellError( e.cell );
        showHint( e.cell );
    }
}


void  SudokuForm::openFile( void )
{
    QString         fileName( QFileDialog::getOpenFileName(
                                this, tr( "Open File" ) ) );
    if ( fileName.isNull() )
        return;
    restartBoard();
    std::ifstream   file( fileName.toLatin1().constData() );
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
            scene->setCellValue( cell, value );
            if ( ! unsolvableCaught )
            {
                try
                {
                    setCellValue( cell, value );
                }
                catch ( SudokuRapid::Unsolvable &  e )
                {
                    enable( false );
                    scene->setCellError( e.cell );
                    unsolvableCaught = true;
                }
            }
        }
    }
    file.close();
}


void  SudokuForm::publish( void )
{
    for ( int  i( 0 ); i < 81; ++i )
    {
        scene->setCellValue( i, cellState[ i ].value, true );
        scene->setCellMaturity( i, getMaturity( i ) );
    }
    scene->invalidate();
}


void  SudokuForm::publish( int  cell )
{
    scene->setCellMaturity( cell, getMaturity( cell ) );
    scene->invalidate();
}


void  SudokuForm::publish( int cell, const CellList &  affectedCells )
{
    scene->setCellValue( cell, cellState[ cell ].value, true );
    for ( CellList::const_iterator  k( affectedCells.begin() );
                k != affectedCells.end(); ++k )
    {
        scene->setCellMaturity( *k, getMaturity( *k ) );
    }
    scene->invalidate();
}


void  SudokuForm::enable( bool  enabled )
{
    if ( isEnabled == enabled )
        return;
    randomButton->setEnabled( enabled );
    solveButton->setEnabled( enabled );
    graphicsView->setEnabled( enabled );
    scene->enable( enabled );
    isEnabled = enabled;
}

