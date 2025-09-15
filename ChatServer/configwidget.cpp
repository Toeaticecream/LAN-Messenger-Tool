#include "configwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

ConfigWidget::ConfigWidget(QWidget *parent)
    : QWidget(parent)
{
    config.loadConfig();
    configButton = new QPushButton("配置", this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(configButton);
    setLayout(mainLayout);



    // 创建非模态配置窗口
    configDialog = new QDialog(this);
    configDialog->setWindowTitle("配置窗口");
    configDialog->resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(configDialog);

    // --- 颜色选择 ---
    senderBgColorDialog = new QColorDialog(Qt::blue, configDialog);
    senderTextColorDialog = new QColorDialog(Qt::white, configDialog);
    receiverBgColorDialog = new QColorDialog(Qt::lightGray, configDialog);
    receiverTextColorDialog = new QColorDialog(Qt::black, configDialog);

    senderBgButton = new QPushButton("发送方背景颜色");
    senderTextButton = new QPushButton("发送方文字颜色");
    receiverBgButton = new QPushButton("接收方背景颜色");
    receiverTextButton = new QPushButton("接收方文字颜色");

    layout->addWidget(new QLabel("发送方背景颜色"));
    layout->addWidget(senderBgButton);
    layout->addWidget(new QLabel("发送方文字颜色"));
    layout->addWidget(senderTextButton);
    layout->addWidget(new QLabel("接收方背景颜色"));
    layout->addWidget(receiverBgButton);
    layout->addWidget(new QLabel("接收方文字颜色"));
    layout->addWidget(receiverTextButton);

    // 初始颜色（按钮背景色）
    senderBgButton->setStyleSheet(QString("background-color: %1").arg(config.getSenderBgColor().name()));
    senderTextButton->setStyleSheet(QString("background-color: %1").arg(config.getSenderTextColor().name()));
    receiverBgButton->setStyleSheet(QString("background-color: %1").arg(config.getReceiverBgColor().name()));
    receiverTextButton->setStyleSheet(QString("background-color: %1").arg(config.getReceiverTextColor().name()));


    messageBubbleMaxCountLineEdit = new QLineEdit(configDialog);
    messageBubbleMaxCountLineEdit->setPlaceholderText("请输入消息最大数量");
    messageBubbleMaxCountLineEdit->setText(QString::number(config.getMessageBubbleMaxCount()));

    fontLabel = new QLabel("字体样式",this);
    fontSizeComboBox = new QComboBox(this);
    fontSizeComboBox->setEditable(true);
    fontLabel->setStyleSheet(QString("font-size: %1pt;").arg(config.getFontSize()));
    fontSizeComboBox->addItems({"12", "14", "16", "18", "20", "24", "28", "32", "40", "64", "128"});
    fontSizeComboBox->setEditText(QString::number(config.getFontSize()));



    // --- IP 和 Port ---
    ipLineEdit = new QLineEdit(configDialog);
    portLineEdit = new QLineEdit(configDialog);
    ipLineEdit->setPlaceholderText("请输入IP地址");
    ipLineEdit->setText(config.getIp());
    portLineEdit->setPlaceholderText("请输入端口");
    portLineEdit->setText(config.getPort());

    layout->addWidget(new QLabel("IP 地址"));
    layout->addWidget(ipLineEdit);
    layout->addWidget(new QLabel("端口"));
    layout->addWidget(portLineEdit);
    layout->addWidget(new QLabel("消息最大数量"));
    layout->addWidget(messageBubbleMaxCountLineEdit);
    layout->addWidget(fontLabel);
    layout->addWidget(fontSizeComboBox);

    // --- 保存按钮 ---
    saveConfigButton = new QPushButton("保存", configDialog);
    layout->addWidget(saveConfigButton);

    connect(configButton, &QPushButton::clicked, this, &ConfigWidget::onClickedConfigButton);
    connect(saveConfigButton, &QPushButton::clicked, this, &ConfigWidget::onSaveConfigButton);

    connect(senderBgButton, &QPushButton::clicked, this, &ConfigWidget::onChooseSenderBgColor);
    connect(senderTextButton, &QPushButton::clicked, this, &ConfigWidget::onChooseSenderTextColor);
    connect(receiverBgButton, &QPushButton::clicked, this, &ConfigWidget::onChooseReceiverBgColor);
    connect(receiverTextButton, &QPushButton::clicked, this, &ConfigWidget::onChooseReceiverTextColor);
    connect(fontSizeComboBox, &QComboBox::currentTextChanged,this,&ConfigWidget::onChoosefontSize);

}

void ConfigWidget::onClickedConfigButton()
{
    configDialog->show();
    configDialog->raise();   // 确保窗口在最前面
    configDialog->activateWindow();
}

void ConfigWidget::onSaveConfigButton()
{
    QString ip = ipLineEdit->text();
    QString port = portLineEdit->text();
    quint32 messageBubbleMaxCount = messageBubbleMaxCountLineEdit->text().toUInt();

    config.setIp(ip);
    config.setPort(port);
    config.setMessageBubbleMaxCount(messageBubbleMaxCount);

    emit configMessageBubble(config.getSenderBgColor(),
                             config.getSenderTextColor(),
                             config.getReceiverBgColor(),
                             config.getReceiverTextColor(),
                             config.getMessageBubbleMaxCount(),
                             config.getFontSize());

    config.saveConfig();
    configDialog->close();
}

void ConfigWidget::onChooseSenderBgColor()
{
    QColor color = QColorDialog::getColor(config.getSenderBgColor(), this, "选择发送方背景颜色");
    if (color.isValid())
    {
        config.setSenderBgColor(color);
        senderBgButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        emit configMessageBubble(config.getSenderBgColor(),
                                 config.getSenderTextColor(),
                                 config.getReceiverBgColor(),
                                 config.getReceiverTextColor(),
                                 config.getMessageBubbleMaxCount(),
                                 config.getFontSize());
    }
}

void ConfigWidget::onChooseSenderTextColor()
{
    QColor color = QColorDialog::getColor(config.getSenderTextColor(), this, "选择发送方文字颜色");
    if (color.isValid())
    {
        config.setSenderTextColor(color);
        senderTextButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        emit configMessageBubble(config.getSenderBgColor(),
                                 config.getSenderTextColor(),
                                 config.getReceiverBgColor(),
                                 config.getReceiverTextColor(),
                                 config.getMessageBubbleMaxCount(),
                                 config.getFontSize());
    }
}

void ConfigWidget::onChooseReceiverBgColor()
{
    QColor color = QColorDialog::getColor(config.getReceiverBgColor(), this, "选择接收方背景颜色");
    if (color.isValid())
    {
        config.setReceiverBgColor(color);
        receiverBgButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        emit configMessageBubble(config.getSenderBgColor(),
                                 config.getSenderTextColor(),
                                 config.getReceiverBgColor(),
                                 config.getReceiverTextColor(),
                                 config.getMessageBubbleMaxCount(),
                                 config.getFontSize());
    }
}

void ConfigWidget::onChooseReceiverTextColor()
{
    QColor color = QColorDialog::getColor(config.getReceiverTextColor(), this, "选择接收方文字颜色");
    if (color.isValid())
    {
        config.setReceiverTextColor(color);
        receiverTextButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        emit configMessageBubble(config.getSenderBgColor(),
                                 config.getSenderTextColor(),
                                 config.getReceiverBgColor(),
                                 config.getReceiverTextColor(),
                                 config.getMessageBubbleMaxCount(),
                                 config.getFontSize());
    }
}

void ConfigWidget::onChoosefontSize(const QString &text)
{
    bool ok;
    int size = text.toInt(&ok);
    if (ok)
    {
        fontLabel->setStyleSheet(QString("font-size: %1pt;").arg(size));

        config.setFontSize(size);
        emit configMessageBubble(config.getSenderBgColor(),
                                 config.getSenderTextColor(),
                                 config.getReceiverBgColor(),
                                 config.getReceiverTextColor(),
                                 config.getMessageBubbleMaxCount(),
                                 config.getFontSize());
    }
}



const QString ConfigWidget::getIp()
{
    return config.getIp();
}
const QString ConfigWidget::getPort()
{
    return config.getPort();
}


const QColor ConfigWidget::getSenderBgColor() { return config.getSenderBgColor(); }
const QColor ConfigWidget::getSenderTextColor() { return config.getSenderTextColor(); }
const QColor ConfigWidget::getReceiverBgColor() { return config.getReceiverBgColor(); }
const QColor ConfigWidget::getReceiverTextColor() { return config.getReceiverTextColor(); }

const quint32 ConfigWidget::getMessageBubbleMaxCount() { return config.getMessageBubbleMaxCount(); }
const quint16 ConfigWidget::getFontSize(){ return config.getFontSize(); }
