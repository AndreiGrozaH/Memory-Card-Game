#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QGridLayout>
#include <vector>

//         QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSliderValueChanged(int value);
    void onCardClicked();

private:
    QSlider *slider;
    QGridLayout *gridLayout;
    std::vector<QPushButton*> cards;
    std::vector<QString> images;
    //std::vector<QColor> colors;
    QPushButton *firstCard;
    QPushButton *secondCard;
    int pairsCount;

    void initializeGame();
    void setupCards();
    void checkForMatch();
};

#endif // MAINWINDOW_H
