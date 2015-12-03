#ifndef GLDWINDOWCOMBOBOX_H
#define GLDWINDOWCOMBOBOX_H

#include <QDesktopWidget>
#include <QBoxLayout>
#include <QSizeGrip>
#include <QPointer>
#include <QLineEdit>
#include <QStyle>
#include <QAbstractSpinBox>
#include <QDialog>
#include "GLDWAbstractspinbox.h"

class QStyleOptionSpinBox;
class GLDWindowComboBox;
class GLDComboBoxPopup;

class GLDWIDGETSHARED_EXPORT GLDWindowComboBox : public GLDWAbstractSpinBox
{
    Q_OBJECT
    Q_PROPERTY(bool hasSelectedText READ hasSelectedText)
public:
    enum ButtonTypes { None, ComboBoxArrow, Ellipsis };

    explicit GLDWindowComboBox(QWidget *parent = 0);
    ~GLDWindowComboBox();

    //设置下拉窗体对象
    QWidget *popupWidget() const;
    virtual void setPopupWidget(QWidget *popup);

    //包含下拉窗体的外部控件
    inline GLDComboBoxPopup *comboBoxPopup() { return m_pComboxPopup; }

    bool isEditable() const;
    void setEditable(bool editable);

    QString editText() const;
    void setEditText(const QString &text);

    inline GLDPlainTextEdit *getLineEdit() { return lineEdit(); }

    //设置是否显示下拉窗体
    bool framePopup() const;
    void setFramePopup(bool enable);

    inline bool hidePopupOnEdit() const { return m_hidePopupOnEdit; }
    inline void setHidePopupOnEdit(const bool value) { m_hidePopupOnEdit = value; }

    inline bool showPopupOnEnter() const { return m_showPopupOnEnter; }
    inline void setShowPopupOnEnter(const bool value) { m_showPopupOnEnter = value; }

    inline bool hideEditOnExit() const { return m_hideEditOnExit; }
    inline void setHideEditOnExit(bool value) { m_hideEditOnExit = value;}

    /**
     * @brief 下拉窗体显示的时间, 默认为150毫秒, 时间越小, 显示越快
     *        若要设置此时间, 请在setPopupWidget之后设置
     * @param time
     */
    void setPopupScrollTime(int time);
    int popupScrollTime();

    void setResizeEnable(bool value);
    void setPopupSize(QSize value);
    void setPopupHeight(int value);

    void hidePopup(); //只隐藏，不关闭，焦点在editor上面

    void setButtonTypes(ButtonTypes type);
    ButtonTypes buttonType();

    bool hasSelectedText();

    inline void setAnimationPopup(bool animateEnabled = true) { m_animationPopup = animateEnabled; }
    inline bool animationPopup() const { return m_animationPopup; }
public:
    void showPopup();
    void closePopup();

Q_SIGNALS:
    void onManualColsePopup();
    void keyPressType(Qt::Key key, bool &handled);
    void onHide();
    void shouldShowPopup(bool &value);

    void selectionChanged();
    void cursorPositionChanged();

public slots:
    void cut();
    void copy();
    void paste();
    void deleteSelectedText();

private Q_SLOTS:
    void doPopupHide();
    void doSelectionChanged();
    void doCursorPositionChanged();

protected:
    void init();
    QStyle::SubControl newHoverControl(const QPoint &pos);
    virtual void updateEditFieldGeometry();
    void updateArrow(QStyle::StateFlag state);
    void syncPopupWidget();
    void initComboBoxPopup(QWidget *cw = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void focusInEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void initStyleOption(QStyleOptionSpinBox *option) const;

    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *);

    void setPopupFixedWidth(int width);

protected:
    ButtonTypes m_buttonType; //edit中的按钮样式

private:
    GLDComboBoxPopup *m_pComboxPopup;
    bool m_hasHadFocus;
    bool m_framePopuped;
    bool m_hidePopupOnEdit; //弹出窗体后点击edit时隐藏下拉窗体
    bool m_showPopupOnEnter; //进入编辑框时直接弹出窗体
    bool m_hideEditOnExit; //失去焦点退出编辑
    QStyle::StateFlag m_arrowState;
    bool m_editorNeedFocus;//进入编辑框并弹出窗体时设置焦点在editor上    
    bool m_animationPopup;
#ifdef QT_KEYPAD_NAVIGATION
    bool focusOnButton;
#endif
    friend class GLDComboBoxPopup;
};

class GLDWIDGETSHARED_EXPORT GLDComboBoxPopup : public QDialog
{
    Q_OBJECT
public:
    explicit GLDComboBoxPopup(QWidget *parent = 0, QWidget *cw = 0);
    QWidget *popupWidget() const { return const_cast<GLDComboBoxPopup*>(this)->verifyPopupInstance(); }

    void setpopupWidget(QWidget *cw);
    void setSizeGripEnabled(bool enabled);
    void setPopupSize(QSize value);
    void setPopupHeight(int value);
    int popuHeight();
    inline void setAtBottom(bool value) { m_isAtBottom = value; }
    QSize sizeHint() const;
    inline void setScrollTime(int time) { m_scrollTime = time; }
    inline int scrollTime() { return m_scrollTime; }
    inline void setAnimationPopup(bool animateEnabled = true) { m_animationPopup = animateEnabled; }
    inline bool animationPopup() const { return m_animationPopup; }

Q_SIGNALS:
    void onHide();
    void shouldShowPopup(bool &value);
public Q_SLOTS:
    void setEditorFocus();
    void show();
    int exec();
protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);
    bool eventFilter(QObject *object, QEvent *event);
    void positionFramePopup();
private:
    QWidget *verifyPopupInstance();
    void setEdgeCursor();
    void alignResizer(const QRect &rect);
    void initPopupSize();
private:
    QPointer<QWidget> m_popupWidget;
    QPoint m_curPos;
    QSizeGrip *m_resizer;
    bool m_sizeGripEnabled;
    //暂时不用，m_popupWidget没有设置eventFilter
    bool m_cursorChanged;
    bool m_canDrag;
    bool m_isAtBottom; //resizer是否位于下面
    QSize m_popupSize;
    int m_scrollTime; //ComboBoxPopup滚动出来所需要的时间,0为没有滚动效果
    bool m_animationPopup;
    friend class GLDWindowComboBox;
};

#endif // GLDWINDOWCOMBOBOX_H
