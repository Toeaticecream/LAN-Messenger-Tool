#include "config.h"

#include <QFile>
#include <QSettings>
#include "resource.h"

Config::Config()
    :m_senderBgColor(QColor(0, 180, 255)),m_senderTextColor(Qt::white),m_receiverBgColor(QColor(240, 240, 240)),m_receiverTextColor(Qt::black)
    ,m_messageBubbleMaxCount(256),m_fontSize(14)
    ,m_ip("0.0.0.0"),m_port("8080")
{

}

void Config::loadConfig()
{
    QFile file(CONFIG_PATH);
    if (!file.exists())
    {
        QMessageBox::information(nullptr, "提示", QString("配置文件不存在，使用默认配置，请修改 %1 权限").arg(CONFIG_PATH));
        saveConfig();
    }

    QFile file2(CONFIG_PATH);
    if (file2.exists())
    {
        if(!file2.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(nullptr, "提示", QString("配置文件不可读，使用默认配置，请修改 %1 权限").arg(CONFIG_PATH));
            saveConfig();
        }
        file2.close();
    }

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);

    settings.beginGroup("General");
    m_senderBgColor = QColor(settings.value("senderBgColor", m_senderBgColor.name()).toString());
    m_senderTextColor = QColor(settings.value("senderTextColor", m_senderTextColor.name()).toString());
    m_receiverBgColor = QColor(settings.value("receiverBgColor", m_receiverBgColor.name()).toString());
    m_receiverTextColor = QColor(settings.value("receiverTextColor", m_receiverTextColor.name()).toString());
    m_messageBubbleMaxCount = settings.value("messageBubbleMaxCount", m_messageBubbleMaxCount).toUInt();
    m_fontSize = settings.value("fontSize", m_fontSize).toUInt();

    m_ip = settings.value("ip", m_ip).toString();
    m_port = settings.value("port", m_port).toString();
    settings.endGroup();
}

void Config::saveConfig()
{
    QFile file(CONFIG_PATH);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(nullptr, "错误", "配置文件无法创建或写入，可能权限不足");
        return;
    }
    file.close();

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("senderBgColor", m_senderBgColor.name());
    settings.setValue("senderTextColor", m_senderTextColor.name());
    settings.setValue("receiverBgColor", m_receiverBgColor.name());
    settings.setValue("receiverTextColor", m_receiverTextColor.name());
    settings.setValue("messageBubbleMaxCount", QString::number(m_messageBubbleMaxCount));
    settings.setValue("fontSize", QString::number(m_fontSize));

    settings.setValue("ip", m_ip);
    settings.setValue("port", m_port);
    settings.endGroup();

    settings.sync(); // 强制保存
}

void Config::setSenderBgColor(const QColor &color)
{
    m_senderBgColor = color;
}

void Config::setSenderTextColor(const QColor &color)
{
    m_senderTextColor = color;
}

void Config::setReceiverBgColor(const QColor &color)
{
    m_receiverBgColor = color;
}

void Config::setReceiverTextColor(const QColor &color)
{
    m_receiverTextColor = color;
}

void Config::setMessageBubbleMaxCount(const quint32 &messageBubbleMaxCount)
{
    m_messageBubbleMaxCount = messageBubbleMaxCount;
}

void Config::setFontSize(const quint16 &fontSize)
{
    m_fontSize = fontSize;
}



void Config::setIp(const QString &ip)
{
    m_ip = ip;
}

void Config::setPort(const QString &port)
{
    m_port = port;
}
