#ifndef REVERSI_H
#define REVERSI_H

#include <QWidget>


QT_BEGIN_NAMESPACE
class QLabel;
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE

class Reversi : public QWidget
{
    Q_OBJECT

    public:
        Reversi(QWidget *parent = 0);
        ~Reversi();
        QTableWidget* createBoard();
        void updateBoard();

    private slots:
            void play( int row, int col );
            void AI_Play();

    private:
        struct cellInfo{
            cellInfo(int r, int c, int s){
                row = r; col = c; score = s;
            }
            int row, col, score;
        };
        enum Size { ROW = 8, COLUMN = 8 };
        enum Color { WHITE = 1, BLACK = -1};

        int showCountDir(int color, int row, int col, int stepRow, int stepCol, int array[8][8]);
        int countFlips(int color, int row, int col, int array[8][8]);
        void flipDir(int color, int row, int col, int stepRow, int stepCol, int array[8][8]);
        void flip(int color, int row, int col, int array[8][8]);
        bool isMoveAllowed(int playerColor,int currentPosition[ROW][COLUMN],int row, int col);
        bool canBeFlipped(int colorPlayer,int currentPosition[ROW][COLUMN], int row, int col, int nextRow, int nextCol);
        void copyBoard(int arrayStat[8][8], int arrayCopy[8][8]);
        //bestMove& getBestMove(int player, int array[8][8]);
        cellInfo& getTotalScore(int array[8][8], int color, int depth);

        int board[ROW][COLUMN];
        QTableWidget *boardTable;
        int depth;
        bool myTurn;
};

#endif // REVERSI_H















