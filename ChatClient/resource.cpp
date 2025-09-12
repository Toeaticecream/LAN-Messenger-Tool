#include "resource.h"

const QString StyleSheet=R"(
        /* 整体窗口 */
        QWidget {
            background-color: #2b2b2b;
            color: #ffffff;
            font-size: 12pt;
        }

        /* 状态标签 */
        QLabel#stateLabel {
            color: #00ffff;
            font-weight: bold;
        }

        /* 输入框和消息显示框 */
        QLineEdit, QTextEdit {
            background-color: #3c3f41;
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px;
        }

        /* 按钮 */
        QPushButton {
            background-color: #555555;
            color: #ffffff;
            border: 1px solid #888888;
            border-radius: 4px;
            padding: 6px 12px;
        }

        QPushButton:hover {
            background-color: #666666;
        }

        QPushButton:pressed {
            background-color: #444444;
        }

        QPushButton:disabled {
            background-color: #333333;
            color: #777777;
        }

        /* 可切换按钮选中状态（监听按钮按下状态） */
        QPushButton:checked {
            background-color: #00aaff;  /* 选中颜色，高亮明显 */
            color: #ffffff;
            border: 1px solid #00ffff;
        }

        /* 可切换按钮选中且按下 */
        QPushButton:checked:pressed {
            background-color: #0088cc;
            border: 1px solid #00ffff;
        }

        /* 滚动条美化 */
        QScrollBar:vertical {
            background: #2b2b2b;
            width: 12px;
            margin: 0px 0px 0px 0px;
        }

        QScrollBar::handle:vertical {
            background: #555555;
            min-height: 20px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical:hover {
            background: #777777;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            background: none;
        }

        QScrollBar:horizontal {
            background: #2b2b2b;
            height: 12px;
            margin: 0px 0px 0px 0px;
        }

        QScrollBar::handle:horizontal {
            background: #555555;
            min-width: 20px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #777777;
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            background: none;
        }
    )";
