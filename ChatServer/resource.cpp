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

        /* QScrollArea 外层样式 */
        QScrollArea {
            background-color: #3c3f41;
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 4px;
            padding: 4px;
        }

        QWidget[ContainerWidget="true"] {
            background-color: transparent;
        }

        /* QScrollArea 外层样式 */
        QScrollArea[MessageBubble="true"] {
            background: transparent;
            border: none;
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

        /* 滑块两侧轨道 */
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: #2b2b2b;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            background: none;
            border: none;
        }

        QScrollBar::handle:horizontal {
            background: #606060;
            min-width: 20px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #505050;
        }

        /* 滑块两侧轨道 */
        QScrollBar::add-page:horizontal,   /* 滑块右侧 */
        QScrollBar::sub-page:horizontal {  /* 滑块左侧 */
            background: #777777;           /* 轨道颜色 */
        }

        /* 隐藏滚动条两端按钮 */
        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            background: none;
            border: none;
        }
    )";
