#include "box.hpp"

ChatBox::ChatBox(QWidget *parent) : QWidget(parent)
{
    // 设置布局为垂直布局
    layout = new QVBoxLayout(this);
    setLayout(layout);

    // 创建聊天列表控件
    chatListWidget = new QListWidget(this);
    layout->addWidget(chatListWidget);

    // 创建输入框和发送按钮
    inputLineEdit = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    // 连接发送按钮的点击事件到槽函数
    connect(sendButton, &QPushButton::clicked, this, &ChatBox::sendMessage);

    // 将输入框和发送按钮添加到布局中
    layout->addWidget(inputLineEdit);
    layout->addWidget(sendButton);

    // 设置滚动条策略为总是显示
    chatListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // 设置不可编辑
    chatListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 设置选择模式为不可选中
    chatListWidget->setSelectionMode(QAbstractItemView::NoSelection);
}

void ChatBox::addMessage(const QString& message)
{
    // 创建新的 QListWidgetItem
    auto item = new QListWidgetItem(message);
    item->setTextAlignment(Qt::AlignLeft); // 文字左对齐

    // 添加到聊天框
    chatListWidget->addItem(item);

    // 滚动到底部
    chatListWidget->scrollToBottom();
}

void ChatBox::sendMessage()
{
    QString message = inputLineEdit->text();
    if (!message.isEmpty())
    {
        addMessage(message);
        inputLineEdit->clear();
    }
}
