#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>

class BSTree;
class TreeWidget;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QLabel;
class QCheckBox;
class QSlider;
class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    BSTree* m_tree;
    TreeWidget* m_treeWidget;
    QLineEdit* m_valueEdit;
    QLineEdit* m_prefixEdit;
    QTextEdit* m_output;
    
    // Элементы управления для демо-режима
    QCheckBox* m_demoModeCheck;
    QPushButton* m_prevStepBtn;
    QPushButton* m_nextStepBtn;
    QPushButton* m_playBtn;
    QSlider* m_stepSlider;
    QLabel* m_stepLabel;
    
    bool m_isPlaying = false;
    int m_playTimerId = -1;

    void refresh();
    void log(const QString& text);
    void setupDemoControls(QVBoxLayout* layout);
    void updateDemoControls();
    void goToNextStep();
    void goToPrevStep();
    void togglePlayPause();
    void stopTimer();
    void timerEvent(QTimerEvent* event) override;
};

#endif