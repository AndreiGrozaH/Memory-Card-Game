#include "mainwindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include <algorithm>
#include <random>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), firstCard(nullptr), secondCard(nullptr), pairsCount(0) {

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: darkgreen");

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(1, 10);
    connect(slider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    mainLayout->addWidget(slider);

    gridLayout = new QGridLayout();
    mainLayout->addLayout(gridLayout);
}

MainWindow::~MainWindow() {
    for (auto card : cards) {
        delete card;
    }
}

void MainWindow::onSliderValueChanged(int value) {
    pairsCount = value;
    initializeGame();
}

void MainWindow::initializeGame() {
    for (auto card : cards) {
        gridLayout->removeWidget(card);
        delete card;
    }
    cards.clear();
    images.clear();
    //colors.clear();
    firstCard = nullptr;
    secondCard = nullptr;

    setupCards();
}

void MainWindow::setupCards() {
    int totalCards = pairsCount * 2;
    for (int i = 1; i <= pairsCount; ++i) {
        //QColor color = QColor::fromRgb(QRandomGenerator::global()->generate());
        QString imagePath = QString(":/new/prefix1/image%1.png").arg(i);
        //colors.push_back(color);
        //colors.push_back(color);
        images.push_back(imagePath);
        images.push_back(imagePath);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(images.begin(), images.end(), g);

    int gridSize = std::ceil(std::sqrt(totalCards));
    for (int i = 0; i < totalCards; ++i) {
        QPushButton *card = new QPushButton(this);
        card->setFixedSize(75, 75);
        card->setIcon(QIcon(":/images/back.png"));
        card->setIconSize(QSize(75, 75));
        card->setStyleSheet("background-color: gray;");
        connect(card, &QPushButton::clicked, this, &MainWindow::onCardClicked);
        cards.push_back(card);
        gridLayout->addWidget(card, i / gridSize, i % gridSize);
    }
}

void MainWindow::onCardClicked() {
    QPushButton *clickedCard = qobject_cast<QPushButton*>(sender());
    if (!clickedCard || clickedCard == firstCard) return;

    int index = std::distance(cards.begin(), std::find(cards.begin(), cards.end(), clickedCard));
    //QColor color = colors[index];
    QString imagePath = images[index];
    clickedCard->setIcon(QIcon(imagePath));
    clickedCard->setIconSize(QSize(75, 75));
    //clickedCard->setStyleSheet(QString("background-color: %1;").arg(color.name()));

    if (!firstCard) {
        firstCard = clickedCard;
    } else if (!secondCard) {
        secondCard = clickedCard;
        checkForMatch();
    }
}

void MainWindow::checkForMatch() {
    if (!firstCard || !secondCard) return;

    int firstIndex = std::distance(cards.begin(), std::find(cards.begin(), cards.end(), firstCard));
    int secondIndex = std::distance(cards.begin(), std::find(cards.begin(), cards.end(), secondCard));

    if (images[firstIndex] == images[secondIndex]) {
        firstCard->setDisabled(true);
        secondCard->setDisabled(true);
        firstCard = nullptr;
        secondCard = nullptr;
        if (std::all_of(cards.begin(), cards.end(), [](QPushButton *card) { return !card->isEnabled(); })) {
            QMessageBox::information(this, "Congrats!", "You won!");
        }
    } else {
        QTimer::singleShot(500, [this]() {
            //firstCard->setStyleSheet("background-color: gray;");
            //secondCard->setStyleSheet("background-color: gray;");
            firstCard->setIcon(QIcon(":/images/back.png"));
            secondCard->setIcon(QIcon(":/images/back.png"));
            firstCard->setIconSize(QSize(75, 75));
            secondCard->setIconSize(QSize(75, 75));
            firstCard = nullptr;
            secondCard = nullptr;
        });
    }

}
