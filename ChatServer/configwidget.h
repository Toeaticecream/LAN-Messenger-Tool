#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QColorDialog>
#include <QLineEdit>
#include <QComboBox>
#include "config.h"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget *parent = nullptr);


    const QString getIp();
    const QString getPort();

    const QColor getSenderBgColor();
    const QColor getSenderTextColor();
    const QColor getReceiverBgColor();
    const QColor getReceiverTextColor();

    const quint32 getMessageBubbleMaxCount();
    const quint16 getFontSize();

private:
    Config config;
    QPushButton *configButton;
    QPushButton *saveConfigButton;
    QDialog *configDialog;

    QPushButton *senderBgButton;
    QPushButton *senderTextButton;
    QPushButton *receiverBgButton;
    QPushButton *receiverTextButton;

    QColorDialog *senderBgColorDialog;
    QColorDialog *senderTextColorDialog;
    QColorDialog *receiverBgColorDialog;
    QColorDialog *receiverTextColorDialog;

    QLineEdit* messageBubbleMaxCountLineEdit;

    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;

    QLabel* fontLabel;
    QComboBox* fontSizeComboBox;

private:


private slots:
    void onClickedConfigButton();
    void onSaveConfigButton();

    void onChooseSenderBgColor();
    void onChooseSenderTextColor();
    void onChooseReceiverBgColor();
    void onChooseReceiverTextColor();
    void onChoosefontSize(const QString &text);

signals:
    void configMessageBubble(const QColor &senderBgColor, const QColor &senderTextColor, const QColor &receiverBgColor, const QColor &receiverTextColor, const quint32 &messageBubbleMaxCount, const quint16 &fontSize);

};

#endif // CONFIGWIDGET_H
