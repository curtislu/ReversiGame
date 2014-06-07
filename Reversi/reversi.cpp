#include <QtGui>
#include "reversi.h"

Reversi::Reversi( QWidget *parent ) :
    QWidget( parent )
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = 0;

    board[3][3] = 1;
    board[3][4] = -1;
    board[4][3] = -1;
    board[4][4] = 1;

    depth = 4;
    myTurn = true;
    winCount = 0;

    playerScore = 0;
    opponentScore = 0;

    setFixedSize( 384, 404 );
    boardTable = createBoard();
    message = new QLabel( tr("It is your turn") );
    resetButton = new QPushButton( "Restart" );
    resetButton->setMaximumWidth( 80 );
    connect( resetButton, SIGNAL( clicked() ), this, SLOT( restart() ) );
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget( boardTable, 0, 0, 1, 2 );
    mainLayout->addWidget( message, 1, 0 );
    mainLayout->addWidget( resetButton, 1, 1 );
    setLayout( mainLayout );
    setWindowTitle(tr("Reversi"));
    updateBoard();

    gameFlowTimer = new QTimer( this );
    connect( gameFlowTimer, SIGNAL( timeout() ), this, SLOT( correctTheGameFlow() ) );
    gameFlowTimer->start( 2000 );
}

Reversi::~Reversi()
{

}

void Reversi::restart()
{
    if( myTurn ){
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = 0;

        board[3][3] = 1;
        board[3][4] = -1;
        board[4][3] = -1;
        board[4][4] = 1;

        playerScore = 0;
        opponentScore = 0;

        message->setText( "The game is reset. Start playing." );
        updateBoard();
        gameFlowTimer->start( 2000 );
    }
}

void Reversi::calcFinalScores()
{
    for( int row = 0; row <= 7; row++ ){
        for( int col = 0; col <= 7; col++ ){
            if( board[row][col] == 1 )
                playerScore++;
            else if(  board[row][col] == -1  )
                    opponentScore++;
        }
    }
}

QTableWidget* Reversi::createBoard()
{
    QTableWidget* boardTable = new QTableWidget( 8, 8 );
    boardTable->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    boardTable->verticalHeader()->setResizeMode( QHeaderView::Stretch );
    boardTable->horizontalHeader()->hide();
    boardTable->verticalHeader()->hide();
    connect( boardTable, SIGNAL( cellClicked ( int, int ) ), this, SLOT( play( int, int ) ) );

    return boardTable;
}

void Reversi::updateBoard()
{
    boardTable->setRowCount(0);
    for( int row = 0; row < 8; row++ ){
        boardTable->insertRow( row );
        for( int col = 0; col < 8; col++ ){
            QTableWidgetItem *cell = new QTableWidgetItem();
            if( board[row][col] == 1 ){
                QImage *image = new QImage( "white-stone.gif" );
                QBrush *brush = new QBrush( *image );
                cell->setBackground( *brush );
            }
            else if( board[row][col] == -1 ){
                QImage *image = new QImage( "black-stone.gif" );
                QBrush *brush = new QBrush( *image );
                cell->setBackground( *brush );
            }
            cell->setFlags( cell->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable );
            boardTable->setItem( row, col, cell );
        }
    }
}

bool Reversi::thereIsMoveFor( int c )
{
    bool exists = false;
    for( int row = 0; row < 8; row++ ){
        for( int col = 0; col < 8; col++ ){
            if( isMoveAllowed( c, board, row, col ) )
            {
                exists = true;
                break;
            }
        }
    }

    return exists;
}

void Reversi::gameOver()
{
    calcFinalScores();
    QString lastMessage = "Game over! ";
    if( playerScore > opponentScore )
        lastMessage += "You Won!!!!    ";
    else if( playerScore < opponentScore )
        lastMessage += "You Lost :(    ";
    else
        lastMessage += "You Draw    ";
    lastMessage += "White score: %1, Black score: %2";
    message->setText( lastMessage.arg( playerScore ).arg( opponentScore ) );
    gameFlowTimer->stop();
}

void Reversi::correctTheGameFlow()
{
    if( !thereIsMoveFor( 1 ) && !thereIsMoveFor( -1 )  )
    {
        gameOver();
        return;
    }

    if( myTurn ){
        if( !thereIsMoveFor( 1 ) ){
            myTurn = false;
            AI_Play();
        }
    }
    else {
        if( !thereIsMoveFor( -1 ) ){
                message->setText( "No move for the opponent. Your turn!!!" );
                myTurn = true;
        }
    }
}

void Reversi::play( int row, int col )
{
    if( myTurn && isMoveAllowed( WHITE, board, row, col ) ){
        message->setText( "" );
        myTurn = false;
        flip( WHITE, row, col, board );
        updateBoard();
        QTimer::singleShot( 500, this, SLOT( AI_Play() ) );
    }
    else {
        message->setText( "The move is not allowed" );
    }
}

void Reversi::AI_Play()
{
    cellInfo AI_move = getTotalScore( board, BLACK, depth );
    flip( BLACK, AI_move.row, AI_move.col, board );
    updateBoard();
    myTurn = true;
}

int Reversi::showCountDir(int color, int row, int col, int stepRow, int stepCol, int array[8][8])
{
    if(stepRow == 0 && stepCol == 0)
        return 0;

    if(array[row + stepRow][col + stepCol] != -color)
        return 0;

    int count = 0;

    while(row <= 8 && row >= -1 && col <= 8 && col >= -1)
    {
        row = row + stepRow;
        col = col + stepCol;
        if((row < 0) || (row > 7) || (col < 0) || (col > 7))
        {
            count = 0;
            break;
        }
        else
        {
            if (array[row][col] == -color)
            {
                count++;
            }
            else
            {
                if (array[row][col] == 0)
                {
                    count = 0;
                    break;
                }
                else
                {
                    break;
                }
            }
        }

    }

    return count;
}

int Reversi::countFlips(int color, int row, int col, int array[8][8])
{
    int count = 0;
    if(isMoveAllowed(color, array, row, col))
    {
        for (int stepRow = -1; stepRow <= 1; stepRow++)
            for (int stepCol = -1; stepCol <= 1; stepCol++)
                count = count + showCountDir(color, row, col, stepRow, stepCol, array);
    }

    return (color * count);

}

void Reversi::flipDir(int color, int row, int col, int stepRow, int stepCol, int array[8][8])
{
    if (stepRow == 0 && stepCol == 0)
        return;
    if (array[row + stepRow][col + stepCol] != -color)
        return;

    int copy[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            copy[i][j] = array[i][j];

    copy[row][col] = color;

    while(row <= 8 && row >= -1 && col <= 8 && col >= -1)
    {
        row = row + stepRow;
        col = col + stepCol;
        if((row < 0) || (row > 7) || (col < 0) || (col > 7))
        {
            break;
        }
        else
        {
            if (array[row][col] == -color)
            {
                copy[row][col] = color;
            }
            else
            {
                if (array[row][col] == 0)
                {
                    break;
                }
                else
                {
                    for (int i = 0; i < 8; i++)
                        for (int j = 0; j < 8; j++)
                            array[i][j] = copy[i][j];
                    break;
                }
            }
        }

    }



}

void Reversi::flip(int color, int row, int col, int array[8][8])
{
    if(isMoveAllowed(color, array, row, col))
    {
        for (int stepRow = -1; stepRow <= 1; stepRow++)
            for (int stepCol = -1; stepCol <= 1; stepCol++)
                flipDir(color, row, col, stepRow, stepCol, array);
    }
}

bool Reversi::isMoveAllowed(int playerColor, int currentPosition[ROW][COLUMN], int row, int col)
{
    //Check if current position is empty.
    if (currentPosition[row][col] != 0)
        return false;

    // Check all board positions for a valid move.
    int i,j;
    for(i = 0; i < ROW; i++)
        for(j = 0; j < COLUMN; j++)
        {

            //Check to see if at least one can be flipped.
            int stepRow, stepCol;
            for(stepRow = -1; stepRow <= 1; stepRow++)
                for(stepCol = -1; stepCol <= 1; stepCol++)
                    if(!(stepRow == 0 && stepCol ==0) && canBeFlipped(playerColor,currentPosition,row,col,stepRow,stepCol))
                        return true;
            //Nothing can be flipped.
        }
    return false;
}


bool Reversi::canBeFlipped(int colorPlayer, int currentPosition[ROW][COLUMN], int row, int col, int nextRow, int nextCol)
{
    int stepRow = row + nextRow, stepCol = col + nextCol;
    while(stepRow >= 0 && stepRow < 8 && stepCol >= 0 && stepCol < 8 && (currentPosition[stepRow][stepCol] == -colorPlayer))
    {
        stepRow += nextRow;
        stepCol += nextCol;
    }
    // If we ran off the board, only moved one space or didn't land on
    // a disc of the same color, return false.
        if (stepRow < 0 || stepRow > 7 || stepCol < 0 || stepCol > 7 || (stepRow - nextRow == row && stepCol - nextCol == col) || currentPosition[stepRow][stepCol] != colorPlayer)
            return false;
    // Otherwise, return true;
        return true;
}

//bestMove& getBestMove(int color, int array[8][8])
//{
//    int maxScore = 0;
//    int maxRow = 0;
//    int maxCol = 0;
//    int k;

//    int count = 0;
//    for (int curRow = 0; curRow < 8; curRow++)      //current row and column.
//        for (int curCol = 0; curCol < 8; curCol++)
//        {
//           k = countFlips(color, curRow, curCol, array);
//           if (k > maxScore)
//           {
//               maxScore = k;
//               maxRow = curRow;
//               maxCol = curCol;
//           }
//        }
//    bestMove *bm = new bestMove(maxRow, maxCol, maxScore*color);
//    return *bm;
//}


Reversi::cellInfo& Reversi::getTotalScore(int array[8][8], int color, int depth)
{
    cellInfo *min;
    if (color == -1)
        min = new cellInfo(-1,-1, 1000);
    else
        min = new cellInfo(-1,-1, -1000);

    if (depth == 0)
    {
        min->score = 0;
        return *min;
    }

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (isMoveAllowed(color, array, r, c))
            {
                int flips = countFlips(color, r, c, array);


                int copy[8][8];
                copyBoard(array, copy);
                flip(color, r, c, copy);
                int subTotal = getTotalScore(copy, -color, (depth - 1)).score;
                if(color == -1)
                {
                    if (flips + subTotal < min->score)
                    {
                        min->row = r;
                        min->col = c;
                        min->score = flips + subTotal;
                    }
                }
                else
                {
                    if (flips + subTotal > min->score)
                    {
                        min->row = r;
                        min->col = c;
                        min->score = flips + subTotal;
                    }
                }
            }
        }
    }
    return *min;
}



void Reversi::copyBoard(int arrayStat[8][8], int arrayCopy[8][8])
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            arrayCopy[i][j] = arrayStat[i][j];
}




















