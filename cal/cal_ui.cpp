#include "cal_ui.hpp"

namespace vastina {

#define CreateButton(button, text, member) \
    button = new QPushButton(text, this);  \
    connect(button, &QPushButton::clicked, this, member);

Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    // 创建垂直布局
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);

    // 创建显示结果的文本框
    display = new QLineEdit();
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    verticalLayout->addWidget(display);

    // 创建水平布局
    QList<QHBoxLayout *> horizontalLayouts;
    horizontalLayouts.resize(4);
    for (auto &&horizontalLayout : horizontalLayouts)
        horizontalLayout = new QHBoxLayout(this);

    buttons.resize(16);
    // 创建数字按钮
    for (int i = 0; i < 10; i++) {
        buttons[i] = new QPushButton(QString::number(i), this);
        connect(buttons[i], &QPushButton::clicked, this, &Calculator::digitClicked);
        horizontalLayouts[3 - (i / 3 + (i % 3 > 0))]->addWidget(buttons[i]);
    }
    // 创建操作符按钮
    CreateButton(buttons[14], tr("C"), &Calculator::clear)
        horizontalLayouts[3]
            ->addWidget(buttons[14]);
    CreateButton(buttons[15], tr("="), &Calculator::assignClicked);
    horizontalLayouts[3]->addWidget(buttons[15]);
    CreateButton(buttons[10], tr("+"), &Calculator::additiveClicked);
    horizontalLayouts[3]->addWidget(buttons[10]);
    CreateButton(buttons[11], tr("-"), &Calculator::subtractiveClicked)
    horizontalLayouts[2]->addWidget(buttons[11]);
    CreateButton(buttons[12], tr("*"), &Calculator::multiplicativeClicked);
    horizontalLayouts[1]->addWidget(buttons[12]);
    CreateButton(buttons[13], tr("/"), &Calculator::divideClicked);
    horizontalLayouts[0]->addWidget(buttons[13]);

    for (auto &&horizontalLayout : horizontalLayouts)
        verticalLayout->addLayout(horizontalLayout);

    // 设置窗口大小和标题
    // setFixedSize(200, 200);
    setWindowTitle(tr("calculator"));
    res_ = new QString();
}

Calculator::~Calculator() {}

void Calculator::digitClicked() {
    updater([this]() {
        QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
        int digitValue = clickedButton->text().toInt();
        if (display->text() == "0" && digitValue == 0)
            return;
        display->setText(display->text() + QString::number(digitValue));
    });
}

void Calculator::additiveClicked() {
    updater([this]() { display->setText(display->text() + "+"); });
}

void Calculator::subtractiveClicked() {
    updater([this]() { display->setText(display->text() + "-"); });
}

void Calculator::multiplicativeClicked() {
    updater([this]() { display->setText(display->text() + "*"); });
}

void Calculator::divideClicked() {
    updater([this]() { display->setText(display->text() + "/"); });
}

void Calculator::clear() {
    display->clear();
}

void Calculator::assignClicked() {
    updater([this]() {
        docalculate(display->text());
        display->setText(*res_);
        res_playing_ = true;
    });
}

void Calculator::updater(const std::function<void()> &update) {
    if (res_playing_) {
        display->clear();
        res_playing_ = false;
    }
    update();
}

void Calculator::docalculate(const QString &tocal) {
    *res_ = tocal + "--tocal";
}

// QPushButton* Calculator::createButton(const QString &text, const std::function<void()>& member)
//     {
//         QPushButton *button = new QPushButton(text, this);
//         connect(button, &QPushButton::clicked, this, member);
//         return button;
//     }

}; // namespace vastina