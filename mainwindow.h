#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BinaryTree;
class TreeWidget;
class QLineEdit;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    BinaryTree* m_tree;
    TreeWidget* m_treeWidget;
    QLineEdit* m_valueEdit;
    QLineEdit* m_prefixEdit;
    QTextEdit* m_output;

    void refresh();
    void log(const QString& text);
};

#endif